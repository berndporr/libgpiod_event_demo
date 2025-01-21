cmake_minimum_required(VERSION 3.6)

project (EventDemo
  DESCRIPTION "Demo of event processing on GPIO with libgpiod"
  LANGUAGES C)

add_compile_options(-Wall -Wconversion -Wextra -pedantic)

add_executable (libgpiod_event_demo libgpiod_event_demo.cpp)
