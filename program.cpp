#include "avl_binary_search_tree.h"
#include <iostream>

int main(){




    IMD::avl_binary_search_tree<int> a{std::initializer_list<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

    a.print_width();


    std::cout << std::endl << a.size() << std::endl;

    std::cout << a.rebalance_count() << std::endl;

    return 0;
}