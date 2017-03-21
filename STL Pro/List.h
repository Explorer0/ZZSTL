#ifndef		OWN_LIST_H
#define		OWN_LIST_H

#include <iostream>
#include <memory>
#include <cassert>
#include "type_traits.h"
#include "Allocator.h"

#define		COMMON_LEN	 12
using namespace std;
template<class T>
class list_node{
	public:
		T	data_;
		list_node<T> *prev_;
		list_node<T> *next_;
		
		list_node(T val = T(), list_node<T> *p = NULL, list_node<T> *n = NULL) :data_(val), prev_(p), next_(n){};
};
template <class T>
class list_iterator{
	public:
		typedef		list_iterator<T>	iterator;
		typedef		Bidirectional_iterator_tag	iterator_category;
		typedef		T		value_type;
		typedef		T*		pointer;
		typedef		T&		reference;
		typedef		size_t	size_type;
		typedef		list_node<T>*	link_type;
		
		link_type	node_;

	public:
		list_iterator() :node_(NULL){};
		list_iterator(link_type x) :node_(x){};
		list_iterator(const iterator  &rhs) :node_(rhs.node_){};

		T& operator*()const{ return (*node_).data_; };
		//??????????????????????????????????????
		T* operator->()const{ return &(operator*()); };

		iterator& operator=(const iterator &rhs)
		{
			if (rhs != *this)
				node_ = rhs.node_;
			return *this;
		}
		iterator& operator++()
		{
			node_ = node_->next_;
			return *this;
		}
		iterator& operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		iterator& operator--()
		{
			node_ = node_->prev_;
			return *this;
		}
		iterator& operator--(int)
		{
			iterator tmp = *this;
			--*this;
			return tmp;
		}
		bool operator==(const iterator &rhs)const{ return node_ == rhs.node_; };
		bool operator!=(const iterator &rhs)const{ return node_ != rhs.node_; };

};
template <class T>
class List{
	public:
		typedef T	value_type;
		typedef	size_t	size_type;

		typedef T*  pointer;
		typedef T&	reference;
		typedef const T& const_reference;
		typedef const T* const_pointer;


		typedef	list_node<T>*	link_type;
		typedef list_iterator<T>	iterator;
		typedef const list_iterator<T> const_iterator;

	protected:
		//指向尾部节点
		link_type	tail_;
		size_type	size_;
	public:
		//构造函数与析构函数
		List():size_(0){ empty_init(); };
		List(size_type n):size_(0){
			empty_init();
			insert(tail_, n, value_type());
		}
		List(size_type n, const T &val) :size_(0)
		{
			empty_init();
			insert(tail_, n, val);
		}
		List(const List<T> &rhs);
		~List(){ destory_and_free(); };

		//迭代器以及访问元素相关操作
		iterator begin()const{ return tail_->next_; };
		iterator end()const{ return tail_; };
		reference front(){ return tail_->next_->data_; };
		reference back(){ return tail_->prev_->data_; };

		//容量查询相关
		bool is_empty()const { return tail_->next_ == tail_; };
		size_type size()const{ return size_; };


		//容量调整
		void assign(size_type n){ assign(n, value_type()); };
		void assign(size_type n, const value_type &val);
		template<class Iterator>
		void assign(Iterator first, Iterator last);
		iterator insert(iterator position){ return insert(position, value_type()); };
		iterator insert(iterator position, const value_type &val);
		void insert(iterator position, size_type n, const value_type &val);
		template <class Iterator>
		void insert(iterator position, Iterator first, Iterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void push_front(const value_type &val){ iterator head(tail_->next_); insert(head, val); };
		void push_back(const value_type &val){insert(tail_,val)};
		void pop_fornt(){ iterator head(tail_->next_); erase(head); };
		void pop_back(){ iterator tmp_tail = tail_; erase(--tmp_tail); };
		void clear();                                                        //clear后还剩下尾节点
		void swap(List<T> &rhs){ link_type tmp = tail_; tail_ = rhs.tail_; rhs.tail_ = tmp;
								size_type tmp_size = size_; size_ = rhs.size_; rhs.size_ = tmp_size;
		};

	protected:
		void empty_init();
		//仅限析构函数调用
		void destory_and_free();

};
template <class T>
inline void List<T>::empty_init()
{
	tail_ = (link_type)::operator new(sizeof(list_node<T>));
	tail_->next_ = tail_->prev_ = tail_;
}
template <class T>
inline typename List<T>::iterator List<T>::insert(iterator position, const value_type &val)
{
	link_type tmp = (link_type)::operator new(sizeof(list_node<T>));
	Construct(&tmp->data_, 1, val);

	tmp->next_ = position.node_;
	tmp->prev_ = position.node_->prev_;
	tmp->prev_->next_ = tmp;
	position.node_->prev_ = tmp;
	size_++;

	return tmp;
}
template <class T>
inline void List<T>::insert(iterator position, size_type n, const value_type &val)
{
	for (size_type i = 0; i < n; i++)
		insert(position, val);
}
template <class T>
template <class Iterator>
inline void List<T>::insert(iterator position, Iterator first, Iterator last)
{
	for (; first != last; first++)
		insert(position, *first);
}
template <class T>
inline typename List<T>::iterator List<T>::erase(iterator position)
{
	iterator tmp_prev = position.node_->prev_;
	iterator tmp_next = position.node_->next_;
	tmp_prev.node_->next_ = tmp_next.node_;
	tmp_next.node_->prev_ = tmp_prev.node_;
	Destory(&(position.node_->data_));
	::operator delete(position.node_);
	size_--;

	return position;
}
template <class T>
inline typename List<T>::iterator List<T>::erase(iterator first, iterator last)
{
	for (; first != last; first++)
		erase(first);

	return first;
}
template <class T>
void List<T>::assign(size_type n, const value_type &val)
{
	if (size_ <= n)
	{
		for (iterator p = tail_->next_; p != tail_; p++)
			*p = val;
		for (; size_ != n;)
			insert(tail_, val);
	}
	else
	{
		for (; size_ != n;)
			erase(tail_->prev_);
		for (iterator p = tail_->next_; p != tail_; p++)
			*p = val;
	}
}
template <class T>
template <class Iterator>
void List<T>::assign(Iterator first, Iterator last)
{
	destory_and_free();
	empty_init();
	last.node->next = tail_;
	tail_->next_ = first;
	size_ = Count(first, last);
}
template <class T>
void List<T>::clear()
{
	for (; size_ > 0;)
		pop_back();
}
template <class T>
inline void List<T>::destory_and_free()
{
	if (tail_)
	{
		link_type cur = tail_->next_;
		for (link_type next; cur != tail_; cur = next)
		{
			next = cur->next_;
			Destory(&cur->data_);
			::operator delete(cur);
		}
		::operator delete(tail_);
	}
}
#endif
