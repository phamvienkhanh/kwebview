cmake_minimum_required(VERSION 3.13)


find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(WEBKITGTK REQUIRED webkit2gtk-4.0)

link_directories(${GTK3_LIBRARY_DIRS} ${WEBKITGTK_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

include_directories(${CMAKE_SOURCE_DIR}/include ${GTK3_INCLUDE_DIRS} ${WEBKITGTK_INCLUDE_DIRS})

add_executable(test ./entrypoint.cc)
target_link_libraries(test ${GTK3_LIBRARIES} ${WEBKITGTK_LIBRARIES} -lpthread)