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
        avl_binary_search_tree(const avl_binary_search_tree& other) : _dummy(create_dummy()), _cmp(other._cmp), _alc(other._alc), _size(0), _rebalance_count(0) {
            for(const auto& x : other)
                this->insert(x);
        }
        avl_binary_search_tree(const std::initializer_list<Key>& ilist, const Comparator& cmp = Comparator(), const Allocator& alc = Allocator()) : _dummy(create_dummy()), _cmp(cmp), _alc(alc), _size(0), _rebalance_count(0) {
            for(const auto& x : ilist)
                this->insert(x);
        }
        template <class InputIterator>
        avl_binary_search_tree(InputIterator begin, InputIterator end, const Comparator& cmp = Comparator(), const Allocator& alc = Allocator()) : _dummy(create_dummy()), _cmp(cmp), _alc(alc), _size(0), _rebalance_count(0) {
            this->insert(begin, end);
        }

        ~avl_binary_search_tree() noexcept{
            this->clear();
            this->destroy_node(this->_dummy);
        }

        constexpr size_type rebalance_count() const noexcept{
            return this->_rebalance_count;
        }

        constexpr size_type size() const noexcept{
            return this->_size;
        }

        constexpr bool empty() const noexcept{
            return this->size() == 0;
        }

        size_t leaf_count() const noexcept{
            if (this->empty()) return 0;

            size_t count{0};
        
            std::queue<node*> queue{};
            queue.push(this->_dummy->_parent);
        
            while (!queue.empty()) {
                auto node = queue.front();
                queue.pop();

                if (node->_left == this->_dummy && node->_right == this->_dummy)
                    ++count;
        
                if (node->_left != this->_dummy)
                    queue.push(node->_left);
        
                if (node->_right != this->_dummy)
                    queue.push(node->_right);
            }
            return count;
        }

        void swap(avl_binary_search_tree& other) noexcept{
            std::swap(this->_dummy, other._dummy);
            std::swap(this->_size, other._size);
            std::swap(this->_rebalance_count, other._rebalance_count);
            std::swap(this->_alc, other._alc);
            std::swap(this->_cmp, other._cmp);
        }

        void clear() noexcept{
            if (this->empty()) return;

            std::queue<node*> q;
            q.push(this->_dummy->_parent);

            while (!q.empty()) {
                node* current = q.front();
                q.pop();

                if (current->_left != this->_dummy) {
                    q.push(current->_left);
                }
                if (current->_right != this->_dummy) {
                    q.push(current->_right);
                }

                destroy_node(current);
            }

            this->_dummy->_parent = this->_dummy;
            this->_dummy->_left = this->_dummy;
            this->_dummy->_right = this->_dummy;
            this->_size = 0;
            this->_rebalance_count = 0;

        }

        iterator begin() const noexcept{
            return iterator(this->_dummy->_left, this->_dummy);
        }
        iterator end() const noexcept{
            return iterator(this->_dummy, this->_dummy);
        }

        reverse_iterator rbegin() const noexcept{
            return reverse_iterator(iterator(this->end()));
        }
        reverse_iterator rend() const noexcept{
            return reverse_iterator(iterator(this->begin()));
        }

		const_iterator cbegin() const noexcept {
			return const_iterator(iterator(this->begin()));
		}
		const_iterator cend() const noexcept {
			return const_iterator(iterator(this->end()));
		}

		const_reverse_iterator crbegin() const	noexcept {
			return const_reverse_iterator(iterator(this->end()));
		}
		const_reverse_iterator crend() const noexcept {
			return const_reverse_iterator(iterator(this->begin()));
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
                previous->_parent = new_node;
                previous->_left = new_node;
                previous->_right = new_node;
            }
            else{
                if (this->_cmp(previous->_key, key)){
                    previous->_right = new_node;
                    if (this->_dummy->_right == previous)
                        this->_dummy->_right = new_node;
                }
                else{
                    previous->_left = new_node;
                    if (this->_dummy->_left == previous)
                        this->_dummy->_left = new_node;
                }
            }
            
            ++this->_size;
            this->rebalance(new_node->_parent);

            return iterator(new_node, this->_dummy);
        }

        template<typename InputIterator>
        void insert(InputIterator begin, InputIterator end){
            while(begin != end){
                this->insert(*begin);
                ++begin;
            }
        }

        void print_width() const noexcept {
			print_width_helper(this->_dummy->_parent, "");
		}

        void breadth_first_traversal(std::function<void(const Key&)> action) const {
            if (this->empty()) return;
        
            std::queue<node*> queue{};
            queue.push(this->_dummy->_parent);
        
            while (!queue.empty()) {
                auto node = queue.front();
                queue.pop();
        
                action(node->_key);
        
                if (node->_left != this->_dummy)
                    queue.push(node->_left);
        
                if (node->_right != this->_dummy)
                    queue.push(node->_right);
            }
        }        

        void prefix_traversal(std::function<void(const Key&)> action) const {
            if (this->empty()) return;

            std::stack<node*> stack{};
            stack.push(this->_dummy->_parent);

            while(!stack.empty()){
                auto node = stack.top();
                stack.pop();

                action(node->_key);

                if (node->_right != this->_dummy)
                    stack.push(node->_right);
                if (node->_left != this->_dummy)
                    stack.push(node->_left);
            }
        }

        void infix_traversal(std::function<void(const Key&)> action) const {
            if (this->empty()) return;
        
            std::stack<node*> stack;
            node* current = this->_dummy->_parent; 
        
            while (!stack.empty() || current != this->_dummy) {
                while (current != this->_dummy) {
                    stack.push(current);
                    current = current->_left;
                }
        
                current = stack.top();
                stack.pop();
                action(current->_key);
        
                current = current->_right;
            }
        }

        void postfix_traversal(std::function<void(const Key&)> action) const {
            if (this->empty()) return;
        
            std::stack<node*> stack;
            node* current = this->_dummy->_parent;
            node* last_visited = this->_dummy;
        
            while (!stack.empty() || current != this->_dummy) {
                while (current != this->_dummy) {
                    stack.push(current);
                    current = current->_left;
                }
        
                node* peek = stack.top();
        
                if (peek->_right != this->_dummy && peek->_right != last_visited)
                    current = peek->_right;
                else {
                    action(peek->_key);
                    last_visited = peek;
                    stack.pop();
                }
            }
        }

        void zigzag_traversal(std::function<void(const Key&)> action) const {
            if (this->empty()) return;
            
            std::stack<node*> current_level, next_level;
            bool left_to_right {true};
            
            current_level.push(this->_dummy->_parent);
            
            while (!current_level.empty()) {
                auto node = current_level.top();
                current_level.pop();
                
                action(node->_key);
                
                if (left_to_right) {
                    if (node->_left != this->_dummy)
                        next_level.push(node->_left);
                    if (node->_right != this->_dummy)
                        next_level.push(node->_right);
                } else {
                    if (node->_right != this->_dummy)
                        next_level.push(node->_right);
                    if (node->_left != this->_dummy)
                        next_level.push(node->_left);
                }
                
                if (current_level.empty()) {
                    std::swap(current_level, next_level);
                    left_to_right = !left_to_right;
                }
            }
        }

    private:
        node* create_dummy(){
            node* dummy = this->_alc.allocate(1);

            std::allocator_traits<NodeAllocator>::construct(_alc, &(dummy->_parent));
			dummy->_parent = dummy;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(dummy->_left));
			dummy->_left = dummy;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(dummy->_right));
			dummy->_right = dummy;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(dummy->_height));
			dummy->_height = 0;

            return dummy;
        }
        node* create_node(const Key& key, node* parent, node* left, node* right, size_t height = 1){
            node* node = _alc.allocate(1);

			std::allocator_traits<NodeAllocator>::construct(_alc, &(node->_parent));
			node->_parent = parent;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(node->_left));
			node->_left = left;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(node->_right));
			node->_right = right;

			std::allocator_traits<NodeAllocator>::construct(_alc, &(node->_key), key);

			std::allocator_traits<NodeAllocator>::construct(_alc, &(node->_height));
			node->_height = 1;

			return node;
        }
        void destroy_node(node* node){
            std::allocator_traits<NodeAllocator>::destroy(_alc, &(node->_key));
            this->destroy_dummy(node);
        }
        void destroy_dummy(node* node) {
			std::allocator_traits<NodeAllocator>::destroy(_alc, &(node->_parent));
			std::allocator_traits<NodeAllocator>::destroy(_alc, &(node->_left));
			std::allocator_traits<NodeAllocator>::destroy(_alc, &(node->_right));
			std::allocator_traits<NodeAllocator>::destroy(_alc, &(node->_height));
			std::allocator_traits<NodeAllocator>::deallocate(_alc, node, 1);
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
            else if (a->_parent->_left == a) a->_parent->_left = b;
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
                auto balance = node->balance();
                if (balance < -1 || balance > 1){

                    if (balance == -2){
                        auto temp = node->_right->balance();
                        if (temp == 1)
                            this->big_left_rotate(node);
                        else
                            this->slight_left_rotate(node);
                    }

                    if (balance == 2){
                        auto temp = node->_left->balance();
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

        void print_width_helper(node* node, const std::string& line) const noexcept {
            if (node != this->_dummy) {
                print_width_helper(node->_right, line + " ");
                std::cout << line << node->_key << std::endl;
                print_width_helper(node->_left, line + " ");
            }
		}

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
        
                size_t height() const noexcept {
                    return this->_height;
                }
        
                int balance() const noexcept{
                    auto left_height = this->_left->height();
                    auto right_height = this->_right->height();
                    
                    return left_height - right_height;
                }
        
                void update_height() noexcept {
                    size_t left_height{0};
                    size_t right_height{0};
                    
                    left_height = this->_left->height();
                    right_height = this->_right->height();
                    
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
                        if (_node == _dummy) {
                            return *this;
                        }
                    
                        if (this->_node->_right != this->_dummy) {
                            this->_node = this->_node->_right;
                            while (this->_node->_left != this->_dummy)
                                this->_node = this->_node->_left;
                        } else {
                            while (this->_node->_parent != this->_dummy && this->_node == this->_node->_parent->_right)
                                this->_node = this->_node->_parent;
                            this->_node = this->_node->_parent;
                        }
                        return *this;
                    }
                    iterator& operator--() noexcept {
                        if (this->_node == this->_dummy) {
                            this->_node = this->_dummy->_right;
                            return *this;
                        }
                
                        if (this->_node->_left != this->_dummy) {
                            this->_node = this->_node->_left;
                            while (this->_node->_right != this->_dummy)
                                this->_node = this->_node->_right;
                        } else {
                            while (this->_node->_parent != this->_dummy && this->_node == this->_node->_parent->_left)
                                this->_node = this->_node->_parent;
                            this->_node = this->_node->_parent;
                        }
                        return *this;
                    }

                    iterator operator++(int) noexcept{
                        iterator it{ *this };
                        this->operator++();
                        return it;
                    }
                    iterator operator--(int) noexcept {
                        iterator it {*this};
                        this->operator--();
                        return it;
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
