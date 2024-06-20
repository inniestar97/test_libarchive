cmake_minimum_required(VERSION 3.0.0...3.5.0)
project(microtar VERSION 0.1.0 LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules (archive REQUIRED IMPORTED_TARGET libarchive)

add_executable(microtar
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${archive_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::archive
)

