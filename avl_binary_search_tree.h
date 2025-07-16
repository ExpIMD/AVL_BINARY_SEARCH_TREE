#ifndef __AVL_BINARY_SEARCH_TREE_
#define __AVL_BINARY_SEARCH_TREE_

#include <stdexcept>
#include <stack>
#include <iostream>
#include <stack>
#include <queue>
#include <functional>

namespace IMD{

    template<typename Key, typename Comparator = std::less<Key>, typename Allocator = std::allocator<Key>>
    class avl_binary_search_tree{
    private:
        class node;

    public:
        class iterator;

    private:
        using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;

    
    public:
        using key_type = Key;
        using key_compare = Comparator;
        using value_type = Key;
        using value_compare = Comparator;
        using allocator_type = Allocator;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        
    private:
        node* _dummy;
        Comparator _cmp;
        NodeAllocator _alc;

        size_type _size;
        size_type _rebalance_count;

    public:
        avl_binary_search_tree(const Comparator& cmp = Comparator(), const Allocator& alc = Allocator()) : _dummy(create_dummy()), _cmp(cmp), _alc(alc), _size(0), _rebalance_count(0) {}


        constexpr size_type rebalance_count() const noexcept{
            return this->_rebalance_count;
        }

        constexpr size_type size() const noexcept{
            return this->_size;
        }

        constexpr bool empty() const noexcept{
            return this->size() == 0;
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

            if (a->_parent == this->_dummy) this->_dummy->_parent = b;
            else if (a->_parent->_left == a) a->_parent->_left = b;
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

            if (a->_parent == this->_dummy) this->_dummy->_parent = b;
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

        iterator begin() const noexcept{
            return iterator(this->_dummy->_left, this->_dummy);
        }
        iterator end() const noexcept{
            return iterator(this->_dummy, this->_dummy);
        }

        reverse_iterator rbegin() const noexcept{
            return reverse_iterator(iterator(end));
        }
        reverse_iterator rend() const noexcept{
            return reverse_iterator(iterator(begin()));
        }

		const_iterator cbegin() const noexcept {
			return const_iterator(iterator(begin()));
		}
		const_iterator cend() const noexcept {
			return const_iterator(iterator(end()));
		}

		const_reverse_iterator crbegin() const	noexcept {
			return const_reverse_iterator(iterator(end()));
		}
		const_reverse_iterator crend() const noexcept {
			return const_reverse_iterator(iterator(begin()));
		}

        iterator insert(const Key& key){            
            node* current = this->_dummy->_parent;
            node* previous = this->_dummy;

            while(current != this->_dummy){
                previous = current;

                if (this->_cmp(current->_key, key))
                    current = current->_right;
                else
                    current = current->_left;
            }

            node* new_node = this->create_node(key, previous, this->_dummy, this->_dummy);

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
            return iterator(new_node, this->_dummy);
        }

        void print_width() const noexcept {
			print_width_helper(this->_dummy->_parent, "");
		}


    private:

        void print_width_helper(node* node, const std::string& line) const noexcept {
            if (node != this->_dummy) {
                print_width_helper(node->_right, line + " ");
                std::cout << line << node->_key << std::endl;
                print_width_helper(node->_left, line + " ");
            }
		}

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
                    
                    left_height = this->_left->get_height();
                    right_height = this->_right->get_height();
                    
                    this->_height = 1 + std::max(left_height, right_height);
                }
            };
        public:
            class iterator{
                private:
                    friend class avl_binary_search_tree;

                    node* _node;
                    node* _dummy;
        
                    iterator(node* current, node* dummy) : _node(current), _dummy(dummy) {}
                
                public:
                    using iterator_category = std::bidirectional_iterator_tag;
                    using value_type = avl_binary_search_tree::value_type;
                    using difference_type = avl_binary_search_tree::difference_type;
                    using pointer = avl_binary_search_tree::const_pointer;
                    using reference = avl_binary_search_tree::const_reference;
        
                    reference operator*() const noexcept{
                        return this->_node->_key;
                    }
                    pointer operator->() const noexcept{
                        return &this->_node->_key;
                    }
        
                    iterator& operator++() noexcept{
                        if (this->_node == this->_dummy)
                            this->_node = this->_dummy->_left;
                        else{
                            node* temp = this->_node;
                            if (temp->_right != this->_dummy){
                                temp = temp->_right;
                                while(temp->_left != this->_dummy)
                                    temp = temp->_left;
                                this->_node = temp;
                            }
                            else{
                                if (temp == this->_dummy->_right) this->_node = this->_dummy;
                                else{
                                    while(temp->_parent->_left != temp)
                                        temp = temp->_parent;
                                    this->_node = temp->_parent;
                                }
                            }
                        }
                        return *this;
                    }

                    bool operator!=(const iterator& other) const {
                        return !this->operator==(other);
                    }
                    bool operator==(const iterator& other) const {
                        return this->_node == other._node;
                    }
                };
    };   
}
#endif

