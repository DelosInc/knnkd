#ifndef BPQ_H
#define BPQ_H

#include <iostream>
#include <vector>

template <typename T>
class BoundedPriorityQueue {
private:
    std::vector<std::pair<T, float>> queue;
    int bound;
    int size;
    int maxIndex;
    BoundedPriorityQueue();
    void siftUp(int);
    void siftDown(int);
    int parent(int);
    int left(int);
    int right(int);
    void swap(int, int);
public:
    BoundedPriorityQueue(int);
    void insert(T, float);
    T extractMin();
};

template <typename T>
BoundedPriorityQueue<T>::BoundedPriorityQueue(int bound)
    : bound(bound),
    size(0),
    maxIndex(0) {
    ;
}

template <typename T>
void BoundedPriorityQueue<T>::swap(int index1, int index2) {
    std::pair<T, float> temp(queue[index1]);
    queue[index1] = queue[index2];
    queue[index2] = temp;
}

template <typename T>
int BoundedPriorityQueue<T>::parent(int index) {
    return index / 2;
}

template <typename T>
int BoundedPriorityQueue<T>::left(int index) {
    return 2 * index + 1;
}

template <typename T>
int BoundedPriorityQueue<T>::right(int index) {
    return 2 * index + 2;
}

template <typename T>
void BoundedPriorityQueue<T>::siftUp(int index) {
    if(index >=0 && queue[index].second < queue[parent(index)].second)  {
        swap(index, parent(index));
        siftUp(parent(index));
    }
}

template <typename T>
void BoundedPriorityQueue<T>::siftDown(int index) {
    int l = left(index);
    int r = right(index);
    int smallest = index;
    if(l <= size && queue[index].second > queue[l].second) {
        smallest = l;
    }
    if (r <= size && queue[smallest].second > queue[r].second) {
        smallest = r;
    }
    if(smallest != index) {
        swap(index, smallest);
        siftDown(smallest);
    }
}

template <typename T>
void BoundedPriorityQueue<T>::insert(T element, float priority) {
    if(size > bound) {
        swap(maxIndex, bound);
    }
//    queue[size] = std::make_pair(element, priority);
    std::pair<T, float> temp(element, priority);
    queue[size] = temp;
    siftUp(size);
    if(priority > queue[maxIndex].second) {
        maxIndex = size;
    }
    size++;
}

template <typename T>
T BoundedPriorityQueue<T>::extractMin() {
    std::pair<T, float> min = queue[0];
    queue[0] = queue[size-1];
    siftDown(0);
    return min.first;
}

#endif
