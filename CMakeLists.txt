cmake_minimum_required(VERSION 3.17)
project(experimental)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Weverything -Wno-reserved-identifier -Wpedantic -Wno-zero-length-array -Wno-c++98-compat-pedantic -Wno-c++98-compat -Wno-conversion -Wno-ctad-maybe-unsupported -Wno-disabled-macro-expansion -Wno-documentation-unknown-command -Wno-double-promotion -Wno-exit-time-destructors -Wno-float-equal -Wno-global-constructors -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-padded -Wno-switch-enum -Wno-undefined-func-template -Wno-unused-template -Wno-vla -Wno-weak-template-vtables -Wno-weak-vtables -Wno-thread-safety-negative")

add_subdirectory(FilterTest)
add_subdirectory(InvalidLock)

add_executable(main main.cpp)
