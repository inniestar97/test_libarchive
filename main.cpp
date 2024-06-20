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

        std::cout << archive_entry_pathname(entry) << " (" << sz << " bytes)" << std::endl;

        char* buf = new char(static_cast<size_t>(sz));

        archive_read_data(a, static_cast<void*>(buf), sz);

        for (size_t i = 0; i < sz; i++) {
            std::cout << buf[i];
        }
        std::cout << std::endl;

        delete buf;
        // archive_read_data_skip(a); // We skip the data

    }

    r = archive_read_free(a); // Close the archive
    if (r != ARCHIVE_OK) {
        std::cerr << "Could not free archive: " << archive_error_string(a) << std::endl;
    }
}

int main(int argc, char** argv) {

    list_tar_contents("../test.tar");
    return 0;
}
