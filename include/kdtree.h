#include <iostream>
#include "point.h"

template <typename T>
class KdTree {
private:
    Point<T>*root;
    unsigned int dimensionality;
    unsigned int size;
    Point<T>* _insert(std::vector<Point<T>>&, unsigned int, unsigned int);
    float distance(const Point<T>*, const Point<T>*);
public:
    KdTree(unsigned int);
    KdTree(std::vector<Point<T>>&, unsigned int);
    Point<T>& insert(const Point<T>&);
    Point<T>& remove(const Point<T>&);
    Point<T>& search(const Point<T>&);
    Point<T>& nnsearch(const Point<T>&);
    BoundedPriorityQueue<Point<T>>& knnsearch(const Point<T>&, const unsigned int);
    void display() const;
    unsigned int getDimensionality() const;
    unsigned int getSize() const;
    bool empty() const;
};
