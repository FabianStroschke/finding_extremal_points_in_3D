//
// Created by fabia on 22.06.2023.
//
#include "gtest/gtest.h"
#include "kd_wrap.h"
#include "input_generators.h"

#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

//typedefs
typedef CGAL::Polyhedron_3<Kernel> Polyhedron_3;

void testSameConvexHullRandomKD(long seed, long nPoints){

    //generate input
    auto input = generateInputVec3(nPoints, seed);

    //build convex hull
    Polyhedron_3 poly;
    CGAL::convex_hull_3(input.begin(), input.end(), poly);

    //build convex mesh
    Mesh mesh;
    KDWrap(input, mesh);

    //check if both hulls have the same points
    for(auto &p: poly.points()){
        EXPECT_NE(std::find(mesh.points().begin(), mesh.points().end(),p),mesh.points().end());
    }

    //check if both mesh is convex
    EXPECT_EQ(CGAL::is_strongly_convex_3(poly),CGAL::is_strongly_convex_3(mesh));
    for(auto f: mesh.faces()){
        auto faceVertices = mesh.vertices_around_face(mesh.halfedge(f)).begin();
        auto v0 = mesh.point(*faceVertices++);
        auto v1 = mesh.point(*faceVertices++);
        auto v2 = mesh.point(*faceVertices);
        auto cross = CGAL::cross_product(Kernel::Vector_3(v1,v0),Kernel::Vector_3(v2,v0));
        long positive = 0;
        long neutral = 0;
        long negative = 0;
        for (auto p : mesh.points()) {
            if(p == v0 or p == v1 or p == v2)continue;
            double dot = Kernel::Vector_3(p,v0)*cross;
            if(dot>0){
                positive++;
            }else if(dot<0){
                negative++;
            }else{
                neutral++;
            }
        }
            EXPECT_TRUE(not(positive > 0 and negative > 0));
    }
}

void testSameConvexHullInputKD(std::string path){
    //generate input
    auto input = readInputVec3(path);

    //build convex hull
    Polyhedron_3 poly;
    CGAL::convex_hull_3(input.begin(), input.end(), poly);

    //build convex mesh
    Mesh mesh;
    KDWrap(input, mesh);


    //check if both hulls have the same points
    for(auto &p: poly.points()){
        EXPECT_NE(std::find(mesh.points().begin(), mesh.points().end(),p),mesh.points().end());
    }

    //check if both mesh is convex
    EXPECT_EQ(CGAL::is_strongly_convex_3(poly),CGAL::is_strongly_convex_3(mesh));
    for(auto f: mesh.faces()){
        auto faceVertices = mesh.vertices_around_face(mesh.halfedge(f)).begin();
        auto v0 = mesh.point(*faceVertices++);
        auto v1 = mesh.point(*faceVertices++);
        auto v2 = mesh.point(*faceVertices);
        auto cross = CGAL::cross_product(Kernel::Vector_3(v1,v0),Kernel::Vector_3(v2,v0));
        long positive = 0;
        long neutral = 0;
        long negative = 0;
        for (auto p : mesh.points()) {
            if(p == v0 or p == v1 or p == v2)continue;
            double dot = Kernel::Vector_3(p,v0)*cross;
            if(dot>0){
                positive++;
            }else if(dot<0){
                negative++;
            }else{
                neutral++;
            }
        }
        EXPECT_TRUE(not(positive > 0 and negative > 0));
    }
}

TEST(CheckConvexHullKD, N10){
    int n = 10;
    testSameConvexHullRandomKD(790, n);
}

TEST(CheckConvexHullKD, N500){
    int n = 500;
    testSameConvexHullRandomKD(1, n);
    testSameConvexHullRandomKD(13, n);
    testSameConvexHullRandomKD(345, n);
    testSameConvexHullRandomKD(7345, n);
    testSameConvexHullRandomKD(98175, n);
    testSameConvexHullRandomKD(123456, n);
    testSameConvexHullRandomKD(9807, n);
    testSameConvexHullRandomKD(468014, n);
}

TEST(CheckConvexHullKD, N5000){
    int n = 5000;
    testSameConvexHullRandomKD(1, n);
    testSameConvexHullRandomKD(13, n);
    testSameConvexHullRandomKD(345, n);
    testSameConvexHullRandomKD(7345, n);
    testSameConvexHullRandomKD(98175, n);
    testSameConvexHullRandomKD(123456, n);
    testSameConvexHullRandomKD(9807, n);
    testSameConvexHullRandomKD(468014, n);
}

TEST(CheckConvexHullKD, N50000){
    int n = 50000;
    testSameConvexHullRandomKD(1, n);
    testSameConvexHullRandomKD(13, n);
    testSameConvexHullRandomKD(345, n);
    testSameConvexHullRandomKD(7345, n);
    testSameConvexHullRandomKD(98175, n);
    testSameConvexHullRandomKD(123456, n);
    testSameConvexHullRandomKD(9807, n);
    testSameConvexHullRandomKD(468014, n);
}

TEST(CheckConvexHullKD, Objects){
    testSameConvexHullInputKD("../inputs/alligator.obj");
    testSameConvexHullInputKD("../inputs/armadillo.obj");
    testSameConvexHullInputKD("../inputs/beast.obj");
    testSameConvexHullInputKD("../inputs/beetle.obj");
    testSameConvexHullInputKD("../inputs/beetle-alt.obj");
    testSameConvexHullInputKD("../inputs/bimba.obj");
    testSameConvexHullInputKD("../inputs/cow.obj");
    testSameConvexHullInputKD("../inputs/fandisk.obj");
    testSameConvexHullInputKD("../inputs/happy.obj");
    testSameConvexHullInputKD("../inputs/homer.obj");
    testSameConvexHullInputKD("../inputs/horse.obj");
    testSameConvexHullInputKD("../inputs/igea.obj");
    testSameConvexHullInputKD("../inputs/lucy.obj");
    testSameConvexHullInputKD("../inputs/orge.obj");
}
