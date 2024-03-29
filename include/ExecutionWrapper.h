//
// Created by fabia on 24.08.2023.
//
#include <chrono>
#include <string>
#include <iostream>
#include <input_generators.h>

#include "kd_wrap.h"
#include "octree_wrap.h"
#include "gift_wrapping.h"


#include "CGALSetup.h"
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

#ifndef EXAMPLE_EXECUTIONWRAPPER_H
#define EXAMPLE_EXECUTIONWRAPPER_H

enum ALGORITHM{NoAlgorithm,Quickhull, KdMarch, OctreeMarch, GiftWrapping};

class ExecutionWrapper{
public:
    ALGORITHM AType = NoAlgorithm;
    SHAPE shape = NoShape;
    long seed = 0;
    long size = 0;
    std::string path;
    double dim[3] ={0,0,0};
    std::vector<Kernel::Point_3> data;

    //output data
    long inputSize = 0;
    long outputSize = 0;
    std::chrono::nanoseconds datastructureBuildTime;
    std::chrono::nanoseconds convexBuildTime;

    ExecutionWrapper(int argc, char* argv[]){ readInput(argc,argv);};
    void readInput(int argc, char* argv[]);
    void prepareData();
    std::chrono::nanoseconds executeQuickhull();
    std::chrono::nanoseconds executeKdMarch();
    std::chrono::nanoseconds executeOctreeMarch();
    std::chrono::nanoseconds executeGiftWrapping();

};


#endif //EXAMPLE_EXECUTIONWRAPPER_H
