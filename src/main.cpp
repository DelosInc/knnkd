#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>
#include "../include/kdtree.h"

int main() {
    std::ifstream csvin("test.csv");
    std::string line, byte;
    std::vector<int> ip;
    std::vector<Point<int>> tbi;
    while(std::getline(csvin, line)) {
        std::stringstream ss(line);
        ss.str(line);
        while(std::getline(ss, byte, '.')) {
            ip.push_back(std::stoi(byte));
        }
        tbi.push_back(*new Point<int>(ip));
        ip.clear();
    }
    KdTree<int> t(tbi, 4);
    t.display();
    std::cout<<std::endl<<t.nnsearch(new Point<int>(std::vector<int>{192, 168, 0, 1}));
    std::cout<<std::endl;
    std::vector<Point<int>> res = t.knnsearch(new Point<int>(std::vector<int>{10, 1, 67, 155}), 2);
    for(auto i : res) {
        std::cout<<&i<<std::endl;
    }
    csvin.close();
    return 0;
}
