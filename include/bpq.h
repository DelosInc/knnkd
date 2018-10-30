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
void BoundedPriorityQueue<T>::enqueue(T element, float priority) {
    if(queue.empty()) {
        queue.push_back(std::make_pair(element, priority));
        size++;
        return;
    }
    if(full()) {
        queue.pop_back();
        size--;
    }
    unsigned int i = queue.size();
    while(i--) {
        if(priority > queue[i].second) {
            break;
        }
    }
    queue.insert(queue.begin()+size, std::make_pair(element, priority));
    size++;
}


template <typename T>
T BoundedPriorityQueue<T>::extractMin() {
    T min = queue.front().first;
    queue.erase(queue.begin(), queue.begin()+1);
    size--;
    return min;
}

template <typename T>
float BoundedPriorityQueue<T>::getMaxPriority() {
    return queue[size].second;
}

template <typename T>
bool BoundedPriorityQueue<T>::full() {
    if(size == bound) {
        return true;
    }
    return false;
}

#endif
