#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

template <typename T>
class Point {
private:
    std::vector<T> coordinates;
    unsigned int dimensionality;
    Point();
public:
    unsigned int axis;
    Point<T> *left, *right;
    Point(std::vector<T>);
    T& operator[](const int&);
    const T& operator[](const int&) const;
    bool operator != (const Point<T>&) const;
    unsigned int getDimensionality() const;
};

template <typename T>
Point<T>::Point(std::vector<T> coordinates)
    : dimensionality(coordinates.size()),
      coordinates(coordinates),
      left(nullptr),
      right(nullptr) {
    ;
}

template <typename T>
T& Point<T>::operator[](const int &k) {
    return coordinates[k];
}

template <typename T>
const T& Point<T>::operator[](const int &k) const {
    return coordinates[k];
}

template <typename T>
bool Point<T>::operator != (const Point<T>& p) const {
    for(unsigned int i = 0; i < coordinates.size(); i++) {
        if(this[i] != p[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
unsigned int Point<T>::getDimensionality() const {
    return dimensionality;
}

#endif
