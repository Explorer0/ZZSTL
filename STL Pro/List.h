#ifndef		OWN_LIST_H
#define		OWN_LIST_H

#include <iostream>
#include "Alloc.h"
#include "Algo.h"

using namespace zz_STL;
using namespace zz_algo;

template <class T>
struct list_node{
	list_node<T> *prev_;
	list_node<T> *next_;
	T			 data_;
};
template <class T>
class list_iterator{
	protected:
		typedef		list_iterator<T>			iterator;
		typedef		Bidirectional_iterator_tag	iterator_category;
		typedef		T							value_type;
		typedef		T&							reference;
		typedef		T*							pointer;
		typedef		ptrdiff_t					difference_type;
		typedef		size_t						size_type;
	public:
		list_iterator() :iterator_node_(NULL){}
		list_iterator(list_node<T> *p) :iterator_node_(p){}
		list_iterator(const iterator &p) :iterator_node_(p.iterator_node_){}

		iterator& operator=(const iterator &p) { iterator_node_ = p.iterator_node_;return *this; }
		bool operator==(const iterator &p)const{ return iterator_node_ == p.iterator_node_; }
		bool operator!=(const iterator &p)const{ return iterator_node_ != p.iterator_node_; }
		reference operator*(){ return iterator_node_->data_; }
		pointer   operator->(){ return &(operator*()); }
		iterator& operator++()
		{
			iterator_node_ = iterator_node_->next_;
			return *this;
		}
		iterator& operator--()
		{
			iterator_node_ = iterator_node_->prev_;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--*this;
			return tmp;
		}
		template <class T, class Alloc>
		friend    class List;
	protected:
		list_node<T> *iterator_node_;
};
template <class T, class Alloc=_new_alloc>
class List{
	public:
		typedef		simple_allocator<list_node<T>, Alloc>	data_alloc;
		typedef     T                           value_type;
		typedef	    T*							pointer;
		typedef		T&							reference;
		typedef		const T*					const_pointer;
		typedef		const T&					cosnt_reference;
		typedef		list_iterator<T>			iterator;
		typedef     const iterator				const_iterator;
		typedef		list_node<T>*				link_type;
		typedef		size_t						size_type;

		link_type   tail_;
		size_type   size_;
	public:
		List():size_(0){ empyt_init(); }
		List(size_type n) :size_(n){ empyt_init(); for (; n > 0; n--) insert_aux(tail_, T()); };
		List(size_type n, const T &val) :size_(n){ empyt_init(); for (; n > 0; n--) insert_aux(tail_, val); }
		template <class InputIterator>
		List(InputIterator first, InputIterator last):size_(Count_distance(first,last)){ empyt_init(); insert_aux(tail_, first, last); }
		List(const List &rhs):size_(rhs.size()){ empyt_init(); insert_aux(tail_, rhs.begin(), rhs.end()); }
		List& operator=(const List &rhs);
		~List(){ 
			iterator cur, next;
			for (cur = tail_->next_; cur != tail_; cur = next)
			{
				next = cur.iterator_node_->next_;
				destory_node(cur.iterator_node_);
			}
			free_node(tail_);
		};

		iterator insert(iterator pos){ size_++; insert_aux(pos, T()); }
		iterator insert(iterator pos, const T &val){ size_++; insert_aux(pos, val); }
		void     insert(iterator pos, const T &val, size_type n){ size_ += n; (; n > 0; n--)insert_aux(pos, val); }
		template <class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last){ size_ += Count_distance(first, last); insert_aux(pos, first, last); }
		
		iterator begin(){ return tail_->next_; }
		iterator end(){ return tail_; }
		const_iterator begin()const{ return tail_->next_; }
		const_iterator end()const{ return tail_; }

		size_type size()const{ return size_; }
	protected:
		
		void empyt_init(){tail_ = get_node(); tail_->prev_ = tail_->next_ = tail_; }          //获取一个头结点
		link_type get_node(){ return data_alloc::allocate(); }                                //获取节点空间（无初始化）
		void	  free_node(link_type p){ data_alloc::deallocate(p); }                       //释放节点空间（只释放，不析构）
		link_type create_node(const T &val);                                                 //获取节点并初始化（构造）
		void      destory_node(link_type p);                                                 //析构节点后释放节点

		iterator insert_aux(iterator pos, const T &val);
		template <class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last);
};
template<class T, class Alloc>
inline typename List<T,Alloc>::link_type List<T, Alloc>::create_node(const T &val)
{
	link_type p = get_node();
	construct(&p->data_, val);
	return p;
}
template<class T, class Alloc>
inline void List<T, Alloc>::destory_node(link_type p)
{
	destory(&p->data_);
	free_node(p);
}
template<class T, class Alloc>
inline typename List<T, Alloc>::iterator List<T, Alloc>::insert_aux(iterator pos, const T &val)
{
	iterator tmp = create_node(val);
	iterator prev = pos.iterator_node_->prev_;
	tmp.iterator_node_->prev_ = prev.iterator_node_;
	prev.iterator_node_->next_ = tmp.iterator_node_;
	tmp.iterator_node_->next_ = pos.iterator_node_;
	pos.iterator_node_->prev_ = tmp.iterator_node_;

	return tmp;
}
template<class T, class Alloc>
template <class InputIterator>
inline void List<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last)
{
	for (; first != last; first++)
	{
		iterator tmp = create_node(*first);
		iterator prev = pos.iterator_node_->prev_;
		tmp.iterator_node_->prev_ = prev.iterator_node_;
		prev.iterator_node_->next_ = tmp.iterator_node_;
		tmp.iterator_node_->next_ = pos.iterator_node_;
		pos.iterator_node_->prev_ = tmp.iterator_node_;
	}
}

#endif
