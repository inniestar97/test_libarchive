cmake_minimum_required(VERSION 3.0.0...${CMAKE_VERSION})
project(microtar VERSION 0.1.0 LANGUAGES C CXX)

add_executable(microtar
    main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $ENV{HOMEBREW_PREFIX}/opt/libarchive/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    $ENV{HOMEBREW_PREFIX}/opt/libarchive/lib/libarchive.dylib
)

target_compile_features(${PROJECT_NAME} PUBLIC
    cxx_std_17
)
