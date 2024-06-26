#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include "iostream"
namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
    T *mStart;
    T *mEnd;
    T *mEndOfStorage;


public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 T *pos;
	public:
        explicit iterator(T* init): pos(init){}
        T* getPos(){
            return pos;
        }
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			//TODO
            return iterator(pos+n);
		}
		iterator operator-(const int &n) const {
			//TODO
            return iterator(pos-n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
            return (pos-rhs.pos);
		}
		iterator& operator+=(const int &n) {
			//TODO
            pos += n;
            return *this;
		}
		iterator& operator-=(const int &n) {
			//TODO
            pos -= n;
            return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            pos++;
            return iterator(pos-1);
        }
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            pos++;
            return *this;
        }
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            pos--;
            return iterator(pos+1);
        }
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            pos--;
            return *this;
        }
		/**
		 * TODO *it
		 */
		T& operator*() const{
            return *pos;
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
            return (this->pos == rhs.pos);
        }
		bool operator==(const const_iterator &rhs) const {
            return (this->pos == rhs.pos);
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return (this->pos != rhs.pos);
        }
		bool operator!=(const const_iterator &rhs) const {
            return (this->pos != rhs.pos);
        }
            };
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
        const T *pos;
    public:
        explicit const_iterator(const T* init): pos(init){}
        const T* getPos(){
            return pos;
        }
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            //TODO
            return const_iterator(pos+n);
        }
        const_iterator operator-(const int &n) const {
            //TODO
            return const_iterator(pos-n);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            //TODO
            return (pos-rhs.pos);
        }
        const_iterator& operator+=(const int &n) {
            //TODO
            pos += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            pos -= n;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            pos++;
            return const_iterator(pos-1);
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            pos++;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            pos--;
            return const_iterator(pos+1);
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            pos--;
            return *this;
        }
        /**
         * TODO *it
         */
        const T& operator*() const{
            return *pos;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return (this->pos == rhs.pos);
        }
        bool operator==(const const_iterator &rhs) const {
            return (this->pos == rhs.pos);
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return (this->pos != rhs.pos);
        }
        bool operator!=(const const_iterator &rhs) const {
            return (this->pos != rhs.pos);
        }
	};
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	vector() {
        mEnd = mStart = mEndOfStorage = nullptr;
    }
	vector(const vector &other) {
//        T *tmp = new T[int(other.capacity())];
        T *tmp = static_cast<T *>(malloc(sizeof(T) * other.capacity()));
        size_t sz = other.size();
        for (int i = 0; i < sz; ++i) {
            tmp[i] = other[i];
        }
        mStart = tmp;
        mEnd = tmp + sz;
        mEndOfStorage = tmp + other.capacity();
    }
	/**
	 * TODO Destructor
	 */
	~vector() {
        for(auto p = mStart; p<mEnd; p++){
            p->~T();
        }
        free(mStart) ;
        mStart = mEnd = mEndOfStorage = nullptr;
    }
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
        if (this == &other)
            return *this;
//        delete []mStart;
        free(mStart);
//        T *tmp = new T[int(other.capacity())];
        T *tmp = static_cast<T *>(malloc(sizeof(T) * other.capacity()));
        size_t sz = other.size();
        for (int i = 0; i < sz; ++i) {
            tmp[i] = other[i];
        }
        mStart = tmp;
        mEnd = tmp + sz;
        mEndOfStorage = tmp + other.capacity();
        return *this;
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
        if (pos<0 || pos >= size())
            throw index_out_of_bound();
        return *(mStart+pos);
    }
	const T & at(const size_t &pos) const {
        if (pos<0 || pos >= size())
            throw index_out_of_bound();
        return *(mStart+pos);
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
        if (pos<0 || pos >= size())
            throw index_out_of_bound();
        return mStart[pos];
    }
	const T & operator[](const size_t &pos) const {
        if (pos<0 || pos >= size())
            throw index_out_of_bound();

        return mStart[pos];
    }
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
        if (size() == 0)
            throw container_is_empty();
        return *mStart;
    }
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
        if (size() == 0)
            throw container_is_empty();
        return *(mEnd-1);
    }
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
        return iterator(mStart);
    }
	const_iterator cbegin() const {
        return const_iterator(mStart);
    }
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        return iterator(mEnd);
    }
	const_iterator cend() const {
        return const_iterator(mEnd);
    }
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
        return size();
    }
	/**
	 * returns the number of elements
	 */
	size_t size() const {
        return mEnd - mStart;
    }
	/**
	 * clears the contents
	 */
	void clear() {
        mEnd = mStart;
    }
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
        if (pos.getPos() == mEndOfStorage||size() == capacity()){
            int n = pos.getPos() - mStart;
            size_t newSize = capacity()==0 ? 2 : capacity()*2;
            reserve(newSize);
            pos = iterator(mStart+n);
        }
        T *cur = mEnd;
        while (cur > pos.getPos()){
            *(cur) = *(cur-1);
            cur--;
        }
        mEnd++;
        *pos = value;

        return pos;

    }
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
        if(ind > size())
            throw index_out_of_bound();
        if (mStart+ind == mEndOfStorage||size() == capacity()){
            size_t newSize = capacity()==0 ? 2 : capacity()*2;
            reserve(newSize);
        }
        int offset = size();
        while (offset > ind){
            *(mStart+offset) = *(mStart+offset-1);
            offset--;
        }
        mEnd++;
        *(mStart+ind) = value;


        return iterator(mStart+ind);
    }
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
        T *it = pos.getPos()+1;
        while (it != mEnd)
        {
            *(it-1) = *it;
            ++it;
        }
        mEnd--;
        return pos;
    }
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {

        if (ind >= size() || ind < 0)
            throw index_out_of_bound();

        size_t pos = ind + 1;
        while (pos+mStart < mEnd){
            mStart[pos-1] = mStart[pos];
            pos++;
        }
        mEnd--;
        return iterator(mStart+pos);

    }
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
        if (size() == capacity()){
            size_t newSize = capacity()==0 ? 2 : capacity()*2;
            reserve(newSize);
        }
//        会调用构造函数
//        *mEnd = value;
//       new可以直接在指定的内存位置上构造对象,但是这里不能使用构造函数,使用了new，但是new出来的空间没有释放，会造成内存泄漏
        new (mEnd) T(value);
//        *mEnd(value);
        mEnd++;

    }
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
        if (size() == 0)
            throw container_is_empty();
        mEnd--;
    }

    size_t capacity() const{
        return mEndOfStorage - mStart;
    }

    void reserve(size_t newSize){
        if (newSize > capacity()){
//            直接用new 会有问题，因为new 会调用构造函数，但是这里只是扩容，并不需要构造函数
//            T *tmp = new T[newSize];
            auto tmp = static_cast<T *>(malloc(sizeof(T) * newSize));
            size_t size1 = size();
            if (mStart){
//                拷贝newSize 份 还是 size1 份呢？
//                for (size_t i = 0; i < size1; ++i)
//                {
//                    tmp[i] = mStart[i];
//                }
                memcpy(tmp, mStart, sizeof(T) * size1);
//                直接delete mStart 会有问题，因为delete 会调用析构函数，但是这里只是扩容，并不需要析构函数
//                delete mStart;
                free(mStart);
            }
            mStart = tmp;
            mEnd = mStart + size1;
            mEndOfStorage = mStart + newSize;
        }
    }
};


}

#endif
