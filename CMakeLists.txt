# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (WIP)

#static compile
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# find python libraries
find_package(Python 3.7 EXACT COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python_INCLUDE_DIRS} ${Python_NumPy_INCLUDE_DIRS})

# populate matplotlib repository
include(FetchContent)
FetchContent_Declare(
        matplotlib
        GIT_REPOSITORY https://github.com/lava/matplotlib-cpp.git
        GIT_TAG        f23347fca25219d1c42cbb91608b5556814bf572
)
FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
    FetchContent_Populate(matplotlib)
endif()
include_directories(SYSTEM ${matplotlib_SOURCE_DIR})

# add executable
add_executable(main main.cpp)

# link python and numpy
target_link_libraries(main
        Python::Python
        Python::Module
        Python::NumPy
        )