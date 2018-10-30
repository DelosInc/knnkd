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
    Point<T>* _nnsearch(const Point<T>*, Point<T>*, Point<T>*, float);
    BoundedPriorityQueue<Point<T>*> _knnsearch(const Point<T>*, Point<T>*, BoundedPriorityQueue<Point<T>*>&);
public:
    KdTree(unsigned int);
    KdTree(std::vector<Point<T>>&, unsigned int);
    Point<T>* insert(Point<T>*);
    Point<T>* search(Point<T>*);
    void display() const;
    Point<T>* nnsearch(Point<T>*);
    std::vector<Point<T>> knnsearch(const Point<T>*, unsigned int);
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
    if (!p1 || !p2){
        return std::numeric_limits<float>::infinity();
    }
    for(unsigned int i_dist = 0; i_dist < dimensionality; i_dist ++) {
        squaredDistance += ((*p1)[i_dist] - (*p2)[i_dist]) * ((*p1)[i_dist] - (*p2)[i_dist]);
    }
    return sqrt(squaredDistance);
}

template<typename T>
Point<T>* KdTree<T>::nnsearch(Point<T>* testPoint) {
    Point<T>* x = distance(root->right, testPoint) < distance(root->left, testPoint) ? root->right : root->left;
    return (_nnsearch(testPoint, root, x, std::numeric_limits<float>::infinity()));
}

template <typename T>
Point<T>* KdTree<T>::_nnsearch(const Point<T>* testPoint, Point<T>* currentPoint,
                               Point<T>* nearestNeighbor, float bestDistance) {
    if(!currentPoint) {
        return nullptr;
    }
    if(distance(currentPoint, nearestNeighbor) < bestDistance) {
        bestDistance = distance(currentPoint, nearestNeighbor);
        nearestNeighbor = currentPoint;
    }
    if((*currentPoint)[currentPoint->axis] > (*testPoint)[currentPoint->axis]) {
        _nnsearch(testPoint, currentPoint->left, nearestNeighbor, bestDistance);
    }
    else {
        _nnsearch(testPoint, currentPoint->right, nearestNeighbor, bestDistance);
    }
    if(abs((*currentPoint)[currentPoint->axis] - (*testPoint)[currentPoint->axis] < bestDistance)) {
        if( currentPoint != root && currentPoint == currentPoint->parent->right) {
            nearestNeighbor = _nnsearch(testPoint, currentPoint->left, nearestNeighbor, bestDistance) ;
        }
        else if(currentPoint != root && currentPoint == currentPoint->parent->left) {
            nearestNeighbor = _nnsearch(testPoint, currentPoint->parent->right, nearestNeighbor, bestDistance);
        }
    }
    return nearestNeighbor;
}

template <typename T>
std::vector<Point<T>> KdTree<T>::knnsearch(const Point<T>* testPoint, unsigned int k) {
    BoundedPriorityQueue<Point<T>*> nearestNeighbors(k);
    nearestNeighbors = _knnsearch(testPoint, root, nearestNeighbors);
    std::vector<Point<T>> kNN;
    for(unsigned i = 0; i < k; i++) {
        kNN.push_back(*nearestNeighbors.extractMin());
    }
    return kNN;
}

template <typename T>
BoundedPriorityQueue<Point<T>*> KdTree<T>::_knnsearch(const Point<T>* testPoint, Point<T>* currentPoint, BoundedPriorityQueue<Point<T>*> &nearestNeighbors) {
    if(!currentPoint) {
        return nearestNeighbors;
    }
    nearestNeighbors.enqueue(currentPoint, distance(currentPoint, testPoint));
    if((*currentPoint)[currentPoint->axis] > (*testPoint)[currentPoint->axis]) {
        _knnsearch(testPoint, currentPoint->left, nearestNeighbors);
    }
    else {
        _knnsearch(testPoint, currentPoint->right, nearestNeighbors);
    }
    if(!nearestNeighbors.full() || abs((*testPoint)[currentPoint->axis] -
       (*currentPoint)[currentPoint->axis]) < nearestNeighbors.getMaxPriority()) {
        if(currentPoint != root && currentPoint == currentPoint->parent->left) {
            _knnsearch(testPoint, currentPoint->right, nearestNeighbors);
        }
        else if(currentPoint != root && currentPoint == currentPoint->parent->right) {
            _knnsearch(testPoint, currentPoint->left, nearestNeighbors);
        }
    }
    return nearestNeighbors;
}




#endif
