#include "avl_binary_search_tree.h"
#include <iostream>

int main(){

    std::cout << "Hello, world!" << std::endl;


    IMD::avl_binary_search_tree<int> a{};
    a.insert(9);
    a.insert(1);
    a.insert(12);
    a.insert(2);
    a.insert(4);
    a.insert(13);

    a.print_width();

    return 0;
}