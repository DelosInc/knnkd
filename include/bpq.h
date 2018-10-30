#ifndef BPQ_H
#define BPQ_H

#include <iostream>
#include <vector>

template <typename T>
class BoundedPriorityQueue {
private:
    std::vector< std::pair<T, float> > queue;
    unsigned int bound;
    unsigned int size;
    void swap(unsigned int, unsigned int);
public:
    BoundedPriorityQueue(unsigned int);
    void enqueue(T, float);
    T extractMin();
    unsigned getMaxPriority();
    bool full();
};

BoundedPriorityQueue<T>::BoundedPriorityQueue(unsigned int bound)
    :bound(bound),
    size(0) {

}

void BoundedPriorityQueue<T>::swap(unsigned int index1, unsigned int index2) {
    Point<T>* swap = queue[index1];
    queue[index1] = queue[index2];
    queue[index2]  = swap;
}

void BoundedPriorityQueue<T>::enqueue(T element, float priority) {
    if(!full()) {
        for(unsigned int i = 0; i < size; i++) {
            if(priority < queue[i]) {
                for(int j = i; j < size; j++) {
                    queue[j+1] = queue[j];
                }
                queue[i] = element;
            }
        }
    }
}

T BoundedPriorityQueue<T>::extractMin() {
    return queue[0];
}

float BoundedPriorityQueue<T>::getMaxPriority() {
    return queue[size];
}

bool BoundedPriorityQueue<T>::full() {
    if(size == bound) {
        return true;
    }
    return false;
}

#endif
