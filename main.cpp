#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <memory>

void list_tar_contents(const char* filename) {
    struct archive* a;
    struct archive_entry* entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_tar(a);

    r = archive_read_open_filename(a, filename, 1024); // Note 10240 is the block size
    if (r != ARCHIVE_OK) {
        std::cerr << "Could not open archive: " << archive_error_string(a) << std::endl;
        return;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {

        la_int64_t sz  = archive_entry_size(entry);
        size_t buffer_sz = static_cast<size_t>(sz);

        std::cout << archive_entry_pathname(entry) << " (" << sz << " bytes)" << std::endl;

        char* buf = new char[1024];

        memset(static_cast<void*>(buf), 0, buffer_sz);

        archive_read_data(a, static_cast<void*>(buf), sz);

        for (size_t i = 0; i < buffer_sz; i++) {
            std::cout << buf[i];
        }
        std::cout << std::endl;

        delete[] buf;
        // archive_read_data_skip(a); // We skip the data

    }

    r = archive_read_free(a); // Close the archive
    if (r != ARCHIVE_OK) {
        std::cerr << "Could not free archive: " << archive_error_string(a) << std::endl;
    }
}

void list_files_in_tar(std::string tar_path) {
    struct archive* a = archive_read_new();
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        std::cerr << "Could not open tar archive" << std::endl;
        return;
    }

    struct archive_entry* entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        std::cout << archive_entry_pathname(entry) << std::endl;
        archive_read_data_skip(a);
    }

    archive_read_free(a);
}


bool read_file_from_tar_with_offset(std::string& tar_path, std::string& target_file, size_t offset, size_t length) {
    struct archive* a = archive_read_new();
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, tar_path.c_str(), 1024 * 10 /*10kb*/) != ARCHIVE_OK) {
        std::cerr << "Could not open tar archive" << std::endl;
        return false;
    }

    struct archive_entry* entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if (std::string(archive_entry_pathname(entry)) == target_file) {
            size_t total_read = 0;
            size_t skipped = 0;
            const void* buff = nullptr;
            size_t size = 0;
            la_int64_t block_offset = 0;

            while (archive_read_data_block(a, &buff, &size, &block_offset) == ARCHIVE_OK) {
                if (skipped + size <= offset) {
                    skipped += size;
                    continue;
                }

                size_t start = (offset > skipped) ? (offset - skipped) : 0;
                size_t end = std::min(size, start + length - total_read);

                std::cout.write(static_cast<const char*>(buff) + start, end - start);

                total_read += end - start;
                skipped += size;

                if (total_read >= length) {
                    break;
                }
            }

            if (total_read < length) {
                std::cerr << "Requested length exceed the file size." << std::endl;
            }

            archive_read_free(a);
            return true;
        }
        archive_read_data_skip(a);
    }

    archive_read_free(a);
    std::cerr << "File not found in tar archive" << std::endl;
    return false;
}


int main(int argc, char** argv) {

    // list_tar_contents("../test.tar");

    std::string tar_path = "../test.tar";
    std::string target_file = "test/test5.txt";
    size_t offset = 5;
    size_t length = 13;

    // list_tar_contents(tar_path.c_str());

    read_file_from_tar_with_offset(tar_path, target_file, offset, length);
    
    return 0;
}
