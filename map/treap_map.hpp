/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <utility>
namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        struct node{
            value_type key;
            int siz;
            int hei;
            int priority;
            node *lc , *rc;
            node()=default;
            node(const value_type& key_):key(key_),priority(rand()),hei(1),siz(1){}
        };
        typedef pair<node*,node*> pii;
        Compare cmp;
        node *root = nullptr;
        node *head = nullptr;
        size_t num = 0;
        int height(node *r){
            if (!r) return 0;
            return r->hei;
        }
        int size(node *r){
            if (!r) return 0;
            return r->siz;
        }
        void copy(node *&cur, node *other_cur) {
            if (other_cur == nullptr) {
                cur = nullptr;
                return;
            }
            cur = new node;
            cur->key = new value_type(*other_cur->key), cur->color = other_cur->priority,cur->hei= other_cur->hei,cur->siz=other_cur->siz;
            copy(cur->lc,  other_cur->lc), copy(cur->rc, other_cur->rc);
        }
        void del(node *cur){
            if (!cur) return;
            del(cur->lc), del(cur->rc);
            delete cur;
        }
        node *loc(node *r,const Key &key) const {
            while(r){
                if (cmp(key, r->key->first)) r = r->lc;
                else if (cmp(r->key->first, key)) r = r->rc;
                else break;
            }
            return r;
        }
        pii split(node *r, const Key &key){
            if(r == nullptr)  return pair(nullptr, nullptr);
//        key < r->key.first)
            if(cmp(key,r->key.first)){
                pii res = split(r->lc,key);
                r->lc = res.second;
                return make_pair(res.first,r);
            }
//        key >= r->key.first)
            else{
                pii res = split(r->rc, key);
                r->rc = res.first;
                return mkp(r, res.second);
            }
        }
        node *merge(node *u, node *v){
            if(u == nullptr) return v;
            if(v == nullptr) return u;
            if(u -> priority < v -> priority){
                u -> rc = merge(u -> rc, v);
                return u;
            }else{
                v -> lc = merge(u, v -> lc);
                return v;
            }
        }
        bool insert(node *r,node *x){
            auto key1 = x->key.first;
            pii res = split(r, key1);
            auto key2 = res.first->key.first;
//        如果key本来存在树中，则res.first.key.first应该为key
            if(!cmp(key1,key2) && !cmp(key2,key1))
                return false;
            else
                res.first = merge(res.first, x);
//        if(!find(root , key)) res.first = merge(res.first, new node(key));
            r = merge(res.first, res.second);
        }
        void erase(node *r,node *x){
//        o的右子树的根节点为key
            auto key = x->key.first;
            pii o = split(r, key - 1);
            pii p = split(o.second, key);
            delete p.first;
            r = merge(o.first, p.second);
        }
        node* getmin(node *cur){
            node *ret = cur;
            for (; cur != nullptr; ret = cur, cur = cur->lc) ;
            return ret;
        }
        node *getmax(node *cur) const {
            node *ret = cur;
            for (; cur != nullptr; ret = cur, cur = cur->rc) ;
            return ret;
        }
        node *prv(node *r,node *cur) const {
            if (cur->lc != nullptr) return getmax(cur->lc);
            else {
                if (!r) return nullptr;
                node *ans;
                if(cmp(r->key.first,cur->key.first)){
                    ans = prv(r->rc,cur);
                    if (!ans)
                        ans = r;
                }
                else{
                    ans = prv(r->lc,cur);
                }
                return ans;
            }
        }
        node *suf(node *r,node *cur) const {
            if (cur->lr != nullptr) return getmin(cur->lr);
            else {
                if (!r) return nullptr;
                node *ans;
                if(cmp(cur->key.first,r->key.first)){
                    ans = suf(r->lc,cur);
                    if (!ans)
                        ans = r;
                }
                else{
                    ans = suf(r->rc,cur);
                }
                return ans;
            }
        }

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
    private:

    public:
        class const_iterator;
        class iterator {
            friend class map;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            node *node_ptr;
            map *map_ptr;
        public:
            iterator() : node_ptr(nullptr), map_ptr(nullptr) {}
            iterator(node *node_ptr_, map *map_ptr_) : node_ptr(node_ptr_), map_ptr(map_ptr_) {}
            iterator(const iterator &other) : node_ptr(other.node_ptr), map_ptr(other.map_ptr) {}
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if (node_ptr == nullptr) throw invalid_iterator();
                iterator ret = *this;
                node_ptr = map_ptr->suf(node_ptr);
                return ret;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                if (node_ptr == nullptr) throw invalid_iterator();
                node_ptr = map_ptr->suf(node_ptr);
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                if (node_ptr == map_ptr->root) throw invalid_iterator();
                iterator ret = *this;
                node_ptr = map_ptr->prv(node_ptr);
                return ret;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                if (node_ptr == map_ptr->root) throw invalid_iterator();
                iterator ret = *this;
                node_ptr = map_ptr->prv(node_ptr);
                return ret;
            }
            /**
             * an operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {
                if (node_ptr == nullptr) throw invalid_iterator();
                else return *node_ptr->key;
            }
            bool operator==(const iterator &rhs) const {return node_ptr == rhs.node_ptr; }
            bool operator==(const const_iterator &rhs) const { return node_ptr == rhs.node_ptr;  }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return node_ptr != rhs.node_ptr;   }
            bool operator!=(const const_iterator &rhs) const {return node_ptr != rhs.node_ptr; }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                if (node_ptr == map_ptr->nil) throw invalid_iterator();
                else return node_ptr->key;
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
            friend class map;
        private:
            // data members.
            node *node_ptr;
            const map *map_ptr;
        public:
            const_iterator() : node_ptr(nullptr), map_ptr(nullptr) {}
            const_iterator(node *node_ptr_, const map *map_ptr_) : node_ptr(node_ptr_), map_ptr(map_ptr_) {}
            const_iterator(const const_iterator &other) : node_ptr(other.node_ptr), map_ptr(other.map_ptr) {}
            const_iterator(const iterator &other) : node_ptr(other.node_ptr), map_ptr(other.map_ptr) {}
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                if (node_ptr == nullptr) throw invalid_iterator();
                const_iterator ret = *this;
                node_ptr = map_ptr->suf(node_ptr);
                return ret;
            }
            /**
             * TODO ++iter
             */
            const_iterator & operator++() {
                if (node_ptr == nullptr) throw invalid_iterator();
                node_ptr = map_ptr->suf(node_ptr);
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                if (node_ptr == map_ptr->root) throw invalid_iterator();
                const_iterator ret = *this;
                node_ptr = map_ptr->prv(node_ptr);
                return ret;
            }
            /**
             * TODO --iter
             */
            const_iterator & operator--() {
                if (node_ptr == map_ptr->head) throw invalid_iterator();
                node_ptr = map_ptr->prv(node_ptr);
                return *this;
            }
            /**
             * an operator to check whether two iterators are same (pointing to the same memory).
             */
            const value_type & operator*() const {
                if (node_ptr == nullptr) throw invalid_iterator();
                else return *node_ptr->key;
            }
            bool operator==(const iterator &rhs) const { return node_ptr == rhs.node_ptr; }
            bool operator==(const const_iterator &rhs) const { return node_ptr == rhs.node_ptr; }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const { return node_ptr != rhs.node_ptr; }
            bool operator!=(const const_iterator &rhs) const { return node_ptr != rhs.node_ptr; }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            const value_type* operator->() const noexcept {
                if (node_ptr == nullptr) throw invalid_iterator();
                else return node_ptr->key;
            }
        };
        /**
         * TODO two constructors
         */
        map()=default;
        map(const map &other)  {
            num = other.num;
            copy(root,other.root);
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other) {
            if (this == &other) return *this;
            del(root);
            num = other.num;
            copy(root,other.root);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() { del(root);}
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key) {
            node *tmp = loc(root,key);
            if (tmp == nullptr) throw index_out_of_bound();
            else return tmp->key->second;
        }
        const T & at(const Key &key) const {
            node *tmp = loc(key);
            if (tmp == nullptr) throw index_out_of_bound();
            else return tmp->key->second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key) {
            node *tmp = loc(key);
            if (tmp == nullptr)
                return insert(value_type(key, T())).first.node_ptr->key->second;
            else return tmp->key->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const {
            node *tmp = loc(key);
            if (tmp == nullptr) throw index_out_of_bound();
            else return tmp->key->second;
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin() {return iterator(root, this);  }
        const_iterator cbegin() const {}
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {  }
        const_iterator cend() const {  }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const { }
        /**
         * returns the number of elements.
         */
        size_t size() const {  }
        /**
         * clears the contents
         */
        void clear() {

        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {

        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {

        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {

        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {  }
        const_iterator find(const Key &key) const { }
    };

}

#endif
