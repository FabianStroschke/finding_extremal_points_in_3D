//
// Created by fabia on 18.06.2023.
//
#include "tree_scan_helper.h"
#include <CGAL/intersections.h>

double cosTheta3(Kernel::Vector_3 u, Kernel::Vector_3 v) {
    return u * v / (sqrt(u.squared_length()) * sqrt(v.squared_length()));
}
double orientedAngleBetweenPlanes(Kernel::Plane_3 u, Kernel::Plane_3 v, Kernel::Vector_3 normalisedNormal){
    return atan2(
            (CGAL::cross_product(u.orthogonal_vector(),v.orthogonal_vector()))*normalisedNormal,
            u.orthogonal_vector()*v.orthogonal_vector());
}
int findBoundaryCell(const CGAL::Bbox_3 &bbox, const Kernel::Point_3 &origin, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet,
                     boundarySide side, double minAngle) {

    Kernel::Plane_3 fixToOrigin(fixPointSet.first,fixPointSet.second, origin);
    Kernel::Vector_3 normal(fixPointSet.first,fixPointSet.second); //vector along rotation axis
    normal /= sqrt(normal.squared_length());

    Kernel::Plane_3 fixToCorner[8] = {
            {fixPointSet.first,fixPointSet.second, {bbox.xmin(), bbox.ymin(), bbox.zmin()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmax(), bbox.ymin(), bbox.zmin()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmin(), bbox.ymax(), bbox.zmin()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmax(), bbox.ymax(), bbox.zmin()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmin(), bbox.ymin(), bbox.zmax()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmax(), bbox.ymin(), bbox.zmax()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmin(), bbox.ymax(), bbox.zmax()}},
            {fixPointSet.first,fixPointSet.second, {bbox.xmax(), bbox.ymax(), bbox.zmax()}},
    };

    //oriented angle see: https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    double angle[8];
    for(int i = 0; i < 8; i++) {
        angle[i] = orientedAngleBetweenPlanes(fixToCorner[i],fixToOrigin,normal);
    }

    int index = 0;
    auto bestAngles = std::minmax_element(angle, angle+8);

    switch (side) {
        case BS_LEFT: //"smallest" angle
            index = bestAngles.first - angle;
            if (*bestAngles.first > minAngle and not(*bestAngles.first <= 0 and *bestAngles.second >= 0)){
                index = -1;
            }
            break;
        case BS_RIGHT: //"biggest" angle


            index = bestAngles.second - angle;
            if (*bestAngles.second < minAngle and not(*bestAngles.first <= 0 and *bestAngles.second >= 0)){
                index = -1;
            }
            break;
    }
    return index;
}

Kernel::Point_3 const *findBoundaryPoint(const Octree &tree, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet, boundarySide side){
    Kernel::Point_3 const *res = nullptr;

    auto bbox = tree.bbox(tree.root());
    Kernel::Point_3 origin((bbox.xmax() + bbox.xmin()) / 2, (bbox.ymax() + bbox.ymin()) / 2, (bbox.zmax() + bbox.zmin()) / 2);

    //TODO: extract from findBoundaryCell (?)
    Kernel::Plane_3 fixToOrigin(fixPointSet.first,fixPointSet.second, origin);
    Kernel::Vector_3 normal(fixPointSet.first,fixPointSet.second); //vector along rotation axis
    normal /= sqrt(normal.squared_length());

    double angle = 0;
    switch (side) {
        case BS_LEFT:
            angle = M_PI;
            break;
        case BS_RIGHT:
            angle = -M_PI;
            break;
    }
    std::vector<int> quadrantOrder = {0, 1, 3, 2};

    std::stack<Octree::Node> stack;
    stack.push(tree.root());
    CGAL::Line_3<Kernel> line(fixPointSet.first, fixPointSet.second);


    while (not stack.empty()) {
        auto currentNode = stack.top();
        stack.pop();
        if (currentNode.size() == 0) continue;
        if (currentNode.is_leaf()) {
            for (auto const &p: currentNode) {
                if(p == fixPointSet.first or p == fixPointSet.second) continue;
                double angle2 = orientedAngleBetweenPlanes({fixPointSet.first,fixPointSet.second, p},fixToOrigin,normal);
                switch (side) {
                    case BS_LEFT: //find negative angle with the biggest absolute value
                        if (angle > angle2) {
                            angle = angle2;
                            res = &p;
                        }
                        break;
                    case BS_RIGHT: //find positive angle with the biggest absolute value
                        if (angle < angle2) {
                            angle = angle2;
                            res = &p;
                        }
                        break;
                }

            }
        } else {
            //for index see: https://doc.cgal.org/latest/Orthtree/classCGAL_1_1Orthtree_1_1Node.html#a706069ea795fdf65b289f597ce1eb8fd
            int idx = 0;
            if(not CGAL::intersection(line, tree.bbox(currentNode))){
                idx = findBoundaryCell(tree.bbox(currentNode), origin, fixPointSet, side, angle);
            }
            if(idx >=0) {
                stack.push(currentNode[idx ^ 7]); //flip x,y,z    //opposite corner of idx
                stack.push(currentNode[idx ^ 6]); //flip y,z      //adjacent to opposite corner
                stack.push(currentNode[idx ^ 5]); //flip x,z      //adjacent to opposite corner
                stack.push(currentNode[idx ^ 3]); //flip x,y      //adjacent to opposite corner
                stack.push(currentNode[idx ^ 4]); //flip z        //adjacent to idx
                stack.push(currentNode[idx ^ 2]); //flip y        //adjacent to idx
                stack.push(currentNode[idx ^ 1]); //flip x        //adjacent to idx
                stack.push(currentNode[idx]);
            }
        }
    }
    return res;

}

Kernel::Point_3 const *
findBoundaryPoint(const Kd_tree &tree, const std::pair<Kernel::Point_3, Kernel::Point_3> &fixPointSet,
                  boundarySide side, Kernel::Point_3 origin) {
    Kernel::Point_3 const *res = nullptr;

    const CGAL::Kd_tree_rectangle<double, Traits::Dimension>& box(tree.bounding_box());

    Kernel::Plane_3 fixToOrigin(fixPointSet.first,fixPointSet.second, origin);
    Kernel::Vector_3 normal(fixPointSet.first,fixPointSet.second); //vector along rotation axis
    normal /= sqrt(normal.squared_length());

    double angle = 0;
    switch (side) {
        case BS_LEFT:
            angle = M_PI;
            break;
        case BS_RIGHT:
            angle = -M_PI;
            break;
    }
    std::vector<int> quadrantOrder = {0, 1, 3, 2};

    typedef std::pair<Kd_tree::Node_const_handle,CGAL::Kd_tree_rectangle<double, Traits::Dimension>> node_bbox_pair;
    std::stack<node_bbox_pair> stack;
    stack.push(node_bbox_pair(tree.root(), tree.bounding_box()));
    CGAL::Line_3<Kernel> line(fixPointSet.first, fixPointSet.second);


    while (not stack.empty()) {
        auto currentPair = stack.top();
        stack.pop();
        if (currentPair.first->is_leaf()) {
            auto node = static_cast<Kd_tree::Leaf_node_const_handle>(currentPair.first);
            for (const auto & p : *node) {
                if(p == fixPointSet.first or p == fixPointSet.second) continue;
                double angle2 = orientedAngleBetweenPlanes({fixPointSet.first,fixPointSet.second, p},fixToOrigin,normal);
                switch (side) {
                    case BS_LEFT: //find negative angle with the biggest absolute value
                        if (angle > angle2) {
                            angle = angle2;
                            res = &p;
                        }
                        break;
                    case BS_RIGHT: //find positive angle with the biggest absolute value
                        if (angle < angle2) {
                            angle = angle2;
                            res = &p;
                        }
                        break;
                }
            }
        } else {
            auto node = static_cast<Kd_tree::Internal_node_const_handle>(currentPair.first);
            CGAL::Bbox_3 bbox(currentPair.second.min_coord(0),currentPair.second.min_coord(1),currentPair.second.min_coord(2),
                              currentPair.second.max_coord(0),currentPair.second.max_coord(1),currentPair.second.max_coord(2));
            int idx = 0;
            if(not CGAL::intersection(line, bbox)){
                idx = findBoundaryCell(bbox, origin, fixPointSet, side, angle);
            }
            if(idx >=0) {
                CGAL::Kd_tree_rectangle<double, Traits::Dimension> bbox_upper(currentPair.second);
                CGAL::Kd_tree_rectangle<double, Traits::Dimension> bbox_lower(currentPair.second);
                node->split_bbox(bbox_lower, bbox_upper);
                //if idx corner is the max value along the cutting dimension, push lower() than upper()

                /** idx is a bit code for a corner, see here: https://doc.cgal.org/latest/Orthtree/classCGAL_1_1Orthtree_1_1Node.html#a706069ea795fdf65b289f597ce1eb8fd
                 *  Example:
                 *      idx = 6 == 110b; cutting_dimension = 1;
                 *      0 == 110b & (1<<1)
                 *      => 0 == 110b & 010b
                 *      => 0 == 010b => false
                 *      corner 6 on the y-max side of the box, therefor bbox_upper should be evaluated first aka pushed last on the stack (upper contains all y-max corners)
                 **/
                if(0==(idx&(1<<node->cutting_dimension()))){
                    stack.push(node_bbox_pair(node->upper(), bbox_upper));
                    stack.push(node_bbox_pair(node->lower(), bbox_lower));
                }else{
                    stack.push(node_bbox_pair(node->lower(), bbox_lower));
                    stack.push(node_bbox_pair(node->upper(), bbox_upper));
                }
            }
        }
    }
    return res;

}
