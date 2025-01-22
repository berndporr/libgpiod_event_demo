cmake_minimum_required(VERSION 3.1.0)
add_executable (gpio_printer gpio_printer.cpp)
target_link_libraries(gpio_printer gpiorpi_static)
target_include_directories(gpio_printer PRIVATE ..)
