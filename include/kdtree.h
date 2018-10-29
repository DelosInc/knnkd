#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <math.h>
#include <limits>
#include "point.h"
#include "bpq.h"

template <typename T>
class KdTree {
private:
    Point<T> *root;
    const unsigned int dimensionality;
    unsigned int size;
    Point<T>* buildTree(std::vector<Point<T>>&, typename std::vector<Point<T>>::iterator,
                        typename std::vector<Point<T>>::iterator, unsigned int, Point<T> *);
    float distance(const Point<T>*, const Point<T>*);
public:
    KdTree(unsigned int);
    KdTree(std::vector<Point<T>>&, unsigned int);
    Point<T>* insert(Point<T>*);
    Point<T>* search(Point<T>*);
    Point<T>* nnsearch(const Point<T>*);
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
    root = buildTree(points, points.begin(), points.end(), 0, nullptr);
}

template <typename T>
Point<T>* KdTree<T>::insert(Point<T> *p) {
    Point<T> *y = nullptr;
    Point<T> *x = root;
    while(x != nullptr) {
        y = x;
        if((*p)[x->axis] < (*x)[x->axis]) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    p->parent = y;
    if(y == nullptr) {
        root = p;
    }
    else if((*p)[y->axis] < (*y)[y->axis]) {
        y->left = p;
    }
    else {
        y->right = p;
    }
    p->axis = (++y->axis) % dimensionality;
    return p;
}

template <typename T>
Point<T>* KdTree<T>::search(Point<T> *p) {
    Point<T> *x = root;
    while(x != nullptr && x != p) {
        if((*p)[x->axis] < (*x)[x->axis]) {
            x = x->left;
        }
        else {
            x = x->right;
        }
        return x;
    }
    return nullptr;
}

template <typename T>
void KdTree<T>::display() const {
    std::queue<Point<T>*> q;
    Point<T> *current;
    q.push(root);
    while(!q.empty()) {
        current = q.front();
        q.pop();
        if(current->left) {
            q.push(current->left);
        }
        if(current->right) {
            q.push(current->right);
        }
        std::cout<<current<<std::endl;
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
                               unsigned int depth,
                               Point<T> *parent) {
    if(begin == end) {
        return &*begin;
    }
    unsigned int axis = depth % dimensionality;
    if(end - begin > 1) {
        std::sort(begin, end, [axis](const Point<T>& a, const Point<T>& b) -> bool {
            return a[axis] < b[axis];
        });
    }
    typename std::vector<Point<T>>::iterator middle = begin;
    std::advance(middle, (end - begin) / 2);
    Point<T>* x = new Point<T>(*middle);
    x->axis = axis;
    x->parent = parent;
    if((middle - begin) > 0) {
        x->left = buildTree(points, begin, middle, depth + 1, x);
    }
    if((end - middle - 1) > 0) {
        x->right = buildTree(points, middle + 1, end, depth + 1, x);
    }
    return x;
}

template <typename T>
float KdTree<T>::distance(const Point<T> *p1, const Point<T> *p2) {
    unsigned int squaredDistance = 0;
    for(unsigned int i_dist = 0; i_dist < dimensionality; i_dist ++) {
        squaredDistance += ((*p1)[i_dist] - (*p2)[i_dist]) * ((*p1)[i_dist] - (*p2)[i_dist]);
    }
    return sqrt(squaredDistance);
}


template<typename T>
Point<T>* KdTree<T>::nnsearch(const Point<T>* testPoint) {
    Point<T>* currentPoint = root;
    Point<T>* nearestNeighbor = root;
    float bestDistance = std::numeric_limits<float>::infinity();
    do {
        if(distance(currentPoint, nearestNeighbor) < bestDistance && currentPoint != root) {
            bestDistance = distance(currentPoint, nearestNeighbor);
            nearestNeighbor = currentPoint;
        }
        if((*testPoint)[currentPoint->axis] < (*currentPoint)[currentPoint->axis]) {
            if(currentPoint->left) {
                currentPoint = currentPoint->left;
                continue;
            }
        }
        else {
            if(currentPoint->right) {
                currentPoint = currentPoint->right;
                continue;
            }
        }
        if(abs((*currentPoint)[currentPoint->axis] - (*testPoint)[currentPoint->axis] < bestDistance)) {
            if(currentPoint == currentPoint->parent->left) {
                if(currentPoint->parent->right) {
                    currentPoint = currentPoint->parent->right;
                    continue;
                }
            }
            else {
                if(currentPoint->parent->left) {
                    currentPoint = currentPoint->parent->left;
                    continue;
                }
            }
        }
        else {
            currentPoint = currentPoint->parent;
        }
    } while(currentPoint != root);
    return nearestNeighbor;
}

template <typename T>
BoundedPriorityQueue<Point<T>>& KdTree<T>::knnsearch(const Point<T>& testPoint,
                                                     const unsigned int bound) {
    BoundedPriorityQueue<Point<T>*> nearestNeighbors(bound);
    Point<T> *currentPoint = root;
    while(currentPoint) {
        nearestNeighbors.enqueue(currentPoint, distance(currentPoint, testPoint));
        if((*testPoint)[currentPoint->axis] < (*currentPoint)[currentPoint->axis]) {
            if(currentPoint->left != nullptr) {
                currentPoint = currentPoint->left;
            }
        }
        else {
            if(currentPoint->right != nullptr) {
                currentPoint = currentPoint->right;
            }
        }
        if((abs((*currentPoint)[currentPoint->axis] - (*testPoint)[currentPoint->axis]) <
            (*nearestNeighbors.minPriority)[currentPoint->axis]) || !nearestNeighbors.full()) {
            if(currentPoint == currentPoint->parent->left) {
                if(currentPoint->parent->right) {
                    currentPoint = currentPoint->parent->right;
                }
            }
            else {
                if(currentPoint->parent->left) {
                    currentPoint = currentPoint->parent->left;
                }
            }
        }
    }
    return nearestNeighbors;
}

#endif
