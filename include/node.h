#ifndef NODE_H
#define NODE_H

#include<point.h>

template <typename T>
struct Node {
    Point<T> p;
    Node *left, *right;
    unsigned int axis;
    Node(std::vector<T>, const unsigned int);
};

template <typename T>
Node::Node(std::vector<T> coordinates, const unsigned int axis)
    : Point(coordinates),
      axis(axis) {
    ;
}

#endif
