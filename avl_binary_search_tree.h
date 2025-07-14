#ifndef __AVL_BINARY_SEARCH_TREE_
#define __AVL_BINARY_SEARCH_TREE_

#include <stdexcept>
#include <stack>
#include <iostream>

namespace IMD{

    template<typename Key, typename Comparator = std::less<Key>, typename Allocator = std::allocator<Key>>
    class avl_binary_search_tree{
    private:
        class node{
        public:
            Key _key;
    
            node* _parent;
            node* _left;
            node* _right;
    
            size_t _height;
            
            node(const Key& key, node* parent = NULL, node* left = NULL, node* right = NULL, size_t height = 1) : _key(key), _parent(parent), _left(left), _right(right), _height(height) {this->update_height();}
            
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
    private:
        node* _root;
        Comparator _cmp;
        Allocator _alc;

        size_t _size;
        size_t _rebalance_count;

    public:
        avl_binary_search_tree(const Comparator& cmp = Comparator(), const Allocator& alc = Allocator()) : _root(NULL), _size(0), _rebalance_count(0), _cmp(cmp), _alc(alc) {}

        size_t rebalance_count() const noexcept{
            return this->_rebalance_count;
        }

        size_t size() const noexcept{
            return this->_size;
        }

        void insert(const Key& key){
            node* new_node = new node(key);
            if (this->_root == NULL){
                this->_root = new_node;
                ++this->_size;
                return;
            }

            node* current = _root;
            node* parent = NULL;

            while(current != NULL){
                parent = current;

                if (this->_cmp(key, current->_key))
                    current = current->_left;
                else if (this->_cmp(current->_key, key))
                    current = current->_right;
                else{
                    delete new_node;
                    return;
                }
            }

            if (this->_cmp(key, parent->_key))
                parent->_left = new_node;
            else
                parent->_right = new_node;

            ++this->_size;
            parent->update_height();
        }

        void print_width() const noexcept {
			print_width_helper(this->_root, "");
		}

        void print_width_helper(node* node, const std::string& line) const noexcept {
            if (node != NULL) {
                print_width_helper(node->_right, line + " ");
                std::cout << line << node->_key << std::endl;
                print_width_helper(node->_left, line + " ");
            }
		}
    };   
}
#endif

