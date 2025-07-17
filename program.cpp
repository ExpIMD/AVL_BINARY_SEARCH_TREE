#include "avl_binary_search_tree.h"
#include <iostream>
#include <vector>

int main(){

    std::vector<int> vec{4, 3, 2, 1, 5, 0, -5, -1, -2, -3, -4};

    IMD::avl_binary_search_tree<int> a{vec.begin(), vec.end()};

    a.print_width();

    std::cout << std::endl << a.size() << std::endl;

    std::cout << a.rebalance_count() << std::endl;

    std::cout << a.leaf_count() << std::endl;
    

    return 0;
}