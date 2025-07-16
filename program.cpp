#include "avl_binary_search_tree.h"
#include <iostream>
#include <vector>

int main(){

    std::vector<int> vec{3, 2, 1, 0, -1, -2, -3};

    IMD::avl_binary_search_tree<int> a{vec.begin(), vec.end()};

    a.zigzag_traversal([](int x){std::cout << x << " ";});

    std::cout << std::endl;

    a.print_width();

    std::cout << std::endl << a.size() << std::endl;

    std::cout << a.rebalance_count() << std::endl;

    return 0;
}