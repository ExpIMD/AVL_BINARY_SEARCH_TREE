#ifndef __AVL_BINARY_SEARCH_TREE_
#define __AVL_BINARY_SEARCH_TREE_

#include <stdexcept>

namespace IMD{

    template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
    class avl_binary_search_tree{
    private:
        class node{
        public:
            Key _key;
    
            node* _left;
            node* _right;
    
            size_t _height;
    
            
            
            node(const Key& key, node* left = NULL, node* right = NULL, size_t height = 1) : _key(key), _left(left), _right(right), _height(height) {this->update_height();}
            
            bool operator==(const node& other){
                return this->_height == 0 && other._height == 0 ? true : this->_key == other._key;
            }
    
            size_t get_height() const noexcept {
                return this->_height;
            }
    
            size_t get_balance() const noexcept{
                size_t left_height{0};
                size_t right_height{0};
                
                if (this->_left != NULL)
                    left_height = this->_left->get_height();
                if (this->_right != NULL)
                    right_height = this->_right->get_height();
                
                return left_height - right_height;
            }
    
            void update_height() noexcept {
                size_t left_height{0};
                size_t right_height{0};
                
                if (this->_left != NULL)
                    left_height = this->_left->get_height();
                if (this->_right != NULL)
                    right_height = this->_right->get_height();
                
                this->_height = 1 + std::max(left_height, right_height);
            }
        };   
    };   
}
#endif

