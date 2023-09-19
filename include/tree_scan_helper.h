//
// Created by fabia on 16.06.2023.
//

#ifndef EXAMPLE_TREE_SCAN_HELPER_H
#define EXAMPLE_TREE_SCAN_HELPER_H

#include "CGALSetup.h"
#include <CGAL/Octree.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits.h>

enum CH_LOCATION{INSIDE,BORDER,ISOLATED};

typedef Kernel::Point_3 Point_3;
class Point_3_prop: public Point_3 {
public:
    CH_LOCATION loc = ISOLATED;
    using Point_3::Point_3;
};

struct Construct_coord_iterator {
    typedef  const double* result_type;
    const double* operator()(const Point_3_prop& p) const
    { return static_cast<const double*>(p.cartesian_begin()); }
    const double* operator()(const Point_3_prop& p, int)  const
    { return static_cast<const double*>(p.cartesian_end()); }
};

typedef CGAL::Search_traits<double, Point_3_prop, const double*, Kernel::Construct_cartesian_const_iterator_3 , CGAL::Dimension_tag<3>> Traits;
typedef CGAL::Kd_tree<Traits> Kd_tree;
typedef CGAL::Octree<Kernel, std::vector<Kernel::Point_3>> Octree;



//TODO:change to Clockwise / counter clockwise?
enum boundarySide {
    BS_LEFT, BS_RIGHT
};

double cosTheta3(Kernel::Vector_3 u, Kernel::Vector_3 v);

double orientedAngleBetweenPlanes(Kernel::Plane_3 u, Kernel::Plane_3 v, Kernel::Vector_3 normalisedNormal);

int findBoundaryCell(const CGAL::Bbox_3 &bbox, const Kernel::Point_3 &origin, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet,
                     boundarySide side, double minAngle);

Kernel::Point_3 const *findBoundaryPoint(const Octree &tree, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet, boundarySide side);
Kernel::Point_3 const *
findBoundaryPoint(const Kd_tree &tree, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet,
                  boundarySide side, Kernel::Point_3 origin);


#endif //EXAMPLE_TREE_SCAN_HELPER_H
