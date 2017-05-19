cmake_minimum_required(VERSION 3.7)
project(apo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        change_me.c
        font_prop14x16.c
        font_rom8x16.c
        font_types.h
        julia_set.c
        julia_set.h
        mzapo_parlcd.c
        mzapo_parlcd.h
        mzapo_phys.c
        mzapo_phys.h
        mzapo_regs.h)

add_executable(apo ${SOURCE_FILES})