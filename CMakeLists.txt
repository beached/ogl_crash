# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/ogl_crash
#

cmake_minimum_required(VERSION 3.20)

project("ogl_crash"
        VERSION "0.1.0"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

add_executable(ogl_crash ogl_crash.cpp)

#target_link_libraries(vtest1 PRIVATE benchmark::benchmark benchmark::benchmark_main daw::daw-header-libraries)
