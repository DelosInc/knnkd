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
    float getMaxPriority();
    bool full();
};

template <typename T>
BoundedPriorityQueue<T>::BoundedPriorityQueue(unsigned int bound)
    :bound(bound),
    size(0) {

}

template <typename T>
void BoundedPriorityQueue<T>::swap(unsigned int index1, unsigned int index2) {
    T* temp = queue[index1];
    queue[index1] = queue[index2];
    queue[index2]  = temp;
}

template <typename T>
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

template <typename T>
T BoundedPriorityQueue<T>::extractMin() {
    return queue[0];
}

template <typename T>
float BoundedPriorityQueue<T>::getMaxPriority() {
    return queue[size];
}

template <typename T>
bool BoundedPriorityQueue<T>::full() {
    if(size == bound) {
        return true;
    }
    return false;
}

#endif
