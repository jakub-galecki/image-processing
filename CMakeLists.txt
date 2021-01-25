cmake_minimum_required(VERSION 3.17)
project(image_processing C)

set(CMAKE_C_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable  (image_processing
        main.c bmp_img.c bmp_img.h conversions.c conversions.h)
target_link_libraries(image_processing ${GTK3_LIBRARIES} -lm)