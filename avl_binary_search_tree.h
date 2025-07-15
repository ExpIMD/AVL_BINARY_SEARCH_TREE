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
            
            node(const Key& key, node* parent, node* left, node* right, size_t height = 1) : _key(key), _parent(parent), _left(left), _right(right), _height(height) {}
            
            bool operator==(const node& other){
                return this->_height == 0 && other._height == 0 ? true : this->_key == other._key;
            }
    
            size_t get_height() const noexcept {
                return this->_height;
            }
    
            int get_balance() const noexcept{
                auto left_height = this->_left->get_height();
                auto right_height = this->_right->get_height();
                
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
        using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;

    
    public:
        using key_type = Key;
        using value_type = Key;

        using key_compare = Comparator;
        using value_compare = Comparator;

        using allocator_type = Allocator;

        using size_type = size_t;
        
    private:
        node* _dummy;
        Comparator _cmp;
        NodeAllocator _alc;

        size_type _size;
        size_type _rebalance_count;

    public:
        avl_binary_search_tree(const Comparator& cmp = Comparator(), const Allocator& alc = Allocator()) : _dummy(create_dummy()), _cmp(cmp), _alc(alc), _size(0), _rebalance_count(0) {}

        size_type rebalance_count() const noexcept{
            return this->_rebalance_count;
        }

        size_type size() const noexcept{
            return this->_size;
        }

        node* create_dummy(){
            node* dummy = this->_alc.allocate(1);
            this->_alc.construct(dummy, Key(), dummy, dummy, dummy, 0);
            return dummy;
        }

        node* create_node(const Key& key, node* parent, node* left, node* right, size_t height = 1){
            node* n = this->_alc.allocate(1);
            this->_alc.construct(n, key, parent, left, right, height);
            return n;
        }
        
        void slight_left_rotate(node* a){
            node* b = a->_right;

            a->_right = b->_left;
            if (b->_left != this->_dummy) b->_left->_parent = a;

            b->_left = a;
            b->_parent = a->_parent;

            if (a->_parent->_left == a) a->_parent->_left = b;
            else a->_parent->_right = b;

            a->_parent = b;
            
            a->update_height();
            b->update_height();

            ++this->_rebalance_count;
        }

        void slight_right_rotate(node* a){
            node* b = a->_left;

            a->_left = b->_right;
            if (b->_right != this->_dummy) b->_right->_parent = a;

            b->_right = a;
            b->_parent = a->_parent;

            if (a->_parent->_left == a) a->_parent->_left = b;
            else a->_parent->_right = b;

            a->_parent = b;

            a->update_height();
            b->update_height();

            ++this->_rebalance_count;
        }

        void big_left_rotate(node* a){
            this->slight_right_rotate(a->_right);
            this->slight_left_rotate(a);
        }

        void big_right_rotate(node* a){
            this->slight_left_rotate(a->_left);
            this->slight_right_rotate(a);
        }

        void rebalance(node* node){
            while(node != this->_dummy){
                auto balance = node->get_balance();
                if (balance < -1 || balance > 1){

                    if (balance == -2){
                        auto temp = node->_right->get_balance();
                        if (temp == 1)
                            this->big_left_rotate(node);
                        else
                            this->slight_left_rotate(node);
                    }

                    if (balance == 2){
                        auto temp = node->_left->get_balance();
                        if (temp == -1)
                            this->big_right_rotate(node);
                        else
                            this->slight_right_rotate(node);
                    }
                }

                node->update_height();
                node = node->_parent;
            }
        }

        void insert(const Key& key){            
            node* current = this->_dummy->_parent;
            node* previous = this->_dummy;

            while(current != this->_dummy){
                previous = current;

                if (this->_cmp(key, current->_key))
                    current = current->_left;
                else
                    current = current->_right;
            }

            node* new_node = this->create_node(key, previous, _dummy, _dummy);

            if (previous == this->_dummy){
                previous->_left = new_node;
                previous->_right = new_node;
                previous->_parent = new_node;
            }
            else{
                if (this->_cmp(previous->_key, key)){
                    previous->_right = new_node;
                    if (this->_dummy->_right != previous)
                        this->_dummy->_right = new_node;
                }
                else{
                    previous->_left = new_node;
                    if (this->_dummy->_left != previous)
                        this->_dummy->_left = new_node;
                }
            }
            
            ++this->_size;
            this->rebalance(new_node->_parent);
        }

        void print_width() const noexcept {
			print_width_helper(this->_dummy->_parent, "");
		}

        void print_width_helper(node* node, const std::string& line) const noexcept {
            if (node != this->_dummy) {
                print_width_helper(node->_right, line + " ");
                std::cout << line << node->_key << std::endl;
                print_width_helper(node->_left, line + " ");
            }
		}
    };   
}
#endif

