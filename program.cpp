#include "avl_binary_search_tree.h"
#include <iostream>

int main(){

    IMD::avl_binary_search_tree<int> a{};
    a.insert(9);
    a.insert(3);
    a.insert(11);
    a.insert(-2);
    a.insert(0);
    a.insert(1);
    
    a.print_width();

    std::cout << a.size() << std::endl;

    std::cout << a.rebalance_count() << std::endl;

    return 0;
}