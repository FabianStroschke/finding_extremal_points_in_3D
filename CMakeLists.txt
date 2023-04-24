# Projects Settings
cmake_minimum_required(VERSION 3.1)
project(WIP)

#static compile
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# add include dir
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
#add glm
#add_subdirectory(external/glm)

#add polyscope
add_subdirectory(external/polyscope)

#add CGAL
find_package(CGAL REQUIRED COMPONENTS Core)

# add executables
add_executable(optimizedGrahamScan optimized_graham_scan.cpp src/input_generators.cpp)
add_executable(grahamScan graham_scan.cpp src/input_generators.cpp)
add_executable(quadtreeScan quadtree_scan.cpp src/input_generators.cpp)
add_executable(quickhull quickhull.cpp src/input_generators.cpp)
add_executable(octreeScan octree_scan.cpp src/input_generators.cpp)

#link main
target_link_libraries(optimizedGrahamScan
        polyscope
        )

#link gscan
target_link_libraries(grahamScan CGAL::CGAL CGAL::CGAL_Core)

#link quadscan
target_link_libraries(quadtreeScan CGAL::CGAL CGAL::CGAL_Core)
target_link_libraries(quadtreeScan
        Polyscope
        )

#link quickhull
target_link_libraries(quickhull CGAL::CGAL CGAL::CGAL_Core)
target_link_libraries(quickhull
        Polyscope
        )
target_link_libraries(octreeScan CGAL::CGAL CGAL::CGAL_Core)
target_link_libraries(octreeScan
        Polyscope
        )