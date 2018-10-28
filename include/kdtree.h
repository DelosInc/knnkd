#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <algorithm>
#include <queue>
#include <math.h>
#include "point.h"
#include "bpq.h"

template <typename T>
class KdTree {
private:
    Point<T> *root;
    const unsigned int dimensionality;
    unsigned int size;
    Point<T>* buildTree(std::vector<Point<T>>&, typename std::vector<Point<T>>::iterator,
                        typename std::vector<Point<T>>::iterator, unsigned int);
    float distance(const Point<T>*, const Point<T>*);
public:
    KdTree(unsigned int);
    KdTree(std::vector<Point<T>>&, unsigned int);
    Point<T>* insert(const Point<T>&);
    Point<T>* search(const Point<T>&);
    Point<T>& nnsearch(const Point<T>&);
    BoundedPriorityQueue<Point<T>>& knnsearch(const Point<T>&, const unsigned int);
    void display() const;
    unsigned int getDimensionality() const;
    unsigned int getSize() const;
    bool empty() const;
};

template <typename T>
KdTree<T>::KdTree(unsigned int dimensionality)
    : root(nullptr),
      dimensionality(dimensionality),
      size(0) {
    ;
}

template <typename T>
KdTree<T>::KdTree(std::vector<Point<T>>& points, unsigned int dimensionality)
    : dimensionality(dimensionality),
      size(points.size()) {
    root = buildTree(points, 0, points.size(), 0);
}

template <typename T>
Point<T>* KdTree<T>::insert(const Point<T>& p) {
    Point<T> *y = nullptr;
    Point<T> *x = root;
    while(x != nullptr) {
        y = x;
        if(p[x->axis] < x[x->axis]) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    if(y == nullptr) {
        root = p;
    }
    else if(p[x->axis] < y[x->axis]) {
        y->left = p;
    }
    else {
        y->right = p;
    }
    p->axis = (++x->axis) % dimensionality;
    return p;
}

template <typename T>
Point<T>* KdTree<T>::search(const Point<T>& p) {
    Point<T> *x = root;
    while(x != nullptr && x != p) {
        if(p[x->axis] < x[x->axis]) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    return x;
    }
}

template <typename T>
void KdTree<T>::display() const {
    std::queue<Point<T>*> q;
    Point<T> *current;
    q.push(root);
    while(!q.empty()) {
        current = q.pop();
        if(current->left) {
            q.push(current->left);
        }
        if(current->right) {
            q.push(current->right);
        }
        std::cout<<current;
    }
}

template <typename T>
unsigned int KdTree<T>::getDimensionality() const {
    return dimensionality;
}

template <typename T>
unsigned int KdTree<T>::getSize() const {
    return size;
}

template <typename T>
bool KdTree<T>::empty() const {
    return !size;
}

template <typename T>
Point<T>* KdTree<T>::buildTree(std::vector<Point<T>>& points,
                               typename std::vector<Point<T>>::iterator begin,
                               typename std::vector<Point<T>>::iterator end,
                               unsigned int depth) {
    if(begin == end) {
        return *begin;
    }
    unsigned int axis = depth % dimensionality;
    if(end - begin > 1) {
        std::sort(begin, end, [axis](const Point<T>& a, const Point<T>& b) -> bool {
            return a[axis] < b[axis];
        });
    }
    typename std::vector<Point<T>>::iterator middle = begin;
    std::advance(middle, (end - begin) / 2);
    Point<T>* x = *middle;
    x->axis = axis;
    if((middle - begin) > 0) {
        x->left = buildTree(begin, middle, depth + 1);
    }
    if((end - middle) > 0) {
        x->right = buildTree(middle + 1, end, depth + 1);
    }
}

template <typename T>
float KdTree<T>::distance(const Point<T> *p1, const Point<T> *p2) {
    int squaredDistance = 0;
    for(int i_dist = 0; i_dist < p1->dimensionality; i_dist ++) {
        squaredDistance += (p1[i_dist] - p2[i_dist]) * (p1[i_dist] - p2[i_dist]);
    }
    return sqrt(squaredDistance);
}

template <typename T>
Point<T>& KdTree<T>::nnsearch(const Point<T>& testPoint) {
    Point<T> &nearestNeighbor = root;
    Point<T> &currentPoint = root;
    float bestDistance = distance(currentPoint, root);
    int dimensionality = testPoint->dimensionality;
    while(currentPoint) {
        if(distance(currentPoint, nearestNeighbor) < bestDistance) {
            nearestNeighbor = currentPoint;
            bestDistance = distance(currentPoint, testPoint);
        }
        if(testPoint[currentPoint->axis] < currentPoint[currentPoint->axis]) {
            currentPoint = currentPoint->left;
        }
        else {
            currentPoint = currentPoint ->right;
        }
        if((abs(currentPoint[currentPoint->axis] - testPoint[currentPoint->axis]) < bestDistance)) {
            if(currentPoint = currentPoint->parent->left) {
                currentPoint = currentPoint->parent->right;
            }
            else {
                currentPoint = currentPoint->parent->left;
            }
        }
    }
    return nearestNeighbor;
}

#endif
