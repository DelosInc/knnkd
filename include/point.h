#ifndef POINT_H
#define POINT_H

#include<iostream>
#include<vector>

template <typename T>
class Point {
private:
    std::vector<T> coordinates;
    unsigned int dimensionality;
    Point();
public:
    Point(std::vector<T>);
    T& operator[](const int&) const;
    const T& operator[](const int&) const;
    unsigned int getDimensionality() const;
};

template <typename T>
Point<T>::Point(std::vector<T> coordinates)
    : dimensionality(coordinates.size()),
      coordinates(coordinates) {
    ;
}

template <typename T>
T& Point<T>::operator[](const int &k) const {
    return coordinates[k];
}

template <typename T>
const T& Point<T>::operator[](const int &k) const {
    return coordinates[k];
}

template <typename T>
unsigned int getDimensionality() const {
    return dimensionality;
}

#endif
