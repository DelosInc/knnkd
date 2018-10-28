#include <iostream>
#include <random>
#include "../include/kdtree.h"

int main() {
    Point<int> *x;
    std::vector<Point<int>> tbi;
    std::default_random_engine rng;
    std::uniform_int_distribution<int> d(-10,10);
    for(unsigned int i = 0; i < 10; i++) {
        x = new Point<int>(std::vector<int>{d(rng), d(rng)});
        tbi.push_back(*x);
    }
    KdTree<int> t(tbi, 2);
    x = new Point<int>(std::vector<int>{d(rng), d(rng)});
    t.insert(x);
    t.search(x);
    t.nnsearch(x);
    return 0;
}
