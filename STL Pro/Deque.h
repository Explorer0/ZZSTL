#ifndef		OWN_DEQUE_H
#define		OWN_DEQUE_H

#include <iostream>
//#include <memory>
#include <cassert>
#include "Type_traits.h"
#include "Construct.h"

template <class T, size_t Buffe_size>
struct deque_iterator
{
	public:
		typedef		deque_iterator<T, Buffe_size>	iterator;

		typedef		T	value_type;
		typedef		T*	pointer;
		typedef		T&	reference;
		typedef		size_t	size_type;
		typedef		T**		map_pointer;

		typedef		Random_access_iterator_tag		iterator_category;
	public:
		T	*cur_;
		T	*first_;
		T	*last_;
		map_pointer node_;

		deque_iterator() :cur_(NULL), first_(NULL), last_(NULL), node_(NULL){};
		deque_iterator(T *p, map_pointer section) :cur_(p), first_(*section), last_(*section + buffe_size()), node_(section){};
		deque_iterator(iterator &rhs) :cur_(rhs.cur_), first_(rhs.first_), last_(rhs.last_), node_(rhs.node_){};
		iterator& operator=(iterator &rhs)
		{
			if (this != &rhs)
			{
				cur_ = rhs.cur_;
				first_ = rhs.first_;
				last_ = rhs.last_;
				node_ = rhs.node_;
			}
		}
		reference operator*()const{ return *cur_; };
		pointer   operator->()const{ return &(operator*()); };

		size_type	operator-(const iterator &rhs)const
		{
			return (buffer_size()*(node_ - rhs.node_ - 1) + (cur_ - first_) + (rhs.last_ - rhs.cur_));
		}
		iterator& operator++()
		{
			++cur_;
			if (cur == last_)
			{
				set_node(node_ + 1);
				cur = first_;
			}
			return *this;
		}
		iterator& operator--()
		{
			if (cur_ == first_)
			{
				set_node(node_ - 1);
				cur_ = last_ ;
			}
			--cur;
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
		iterator& operator+=(size_type n)
		{
			size_type offset = n + (cur_ - first_);
			if (offset >= 0 && offset < static_cast<size_type>(buffer_size()))
				cur_ += n;
			else
			{
				size_type node_offset = offset > 0 ? offset / static_cast<size_type>(buffer_size())
					: -static_cast<size_type>((-offset - 1) / buffer_size()) - 1;
				set_node(node_ + node_offset);
				cur_ = first_ + (offset - node_offset*static_cast<size_type>(buffer_size()));
			}
			return *this;
		};
		iterator& operator-=(size_type n){ return *this += -n; };
		iterator operator+(size_type n)const{ iterator tmp = *this; return tmp += n; };
		iterator operator-(size_type)const{ iterator tmp = *this; return tmp -= n; };

		bool operator==(const iterator &rhs)const{ return cur == rhs.cur_; };
		bool operator!=(const iterator &rhs)const{ return !(*this == rhs); };
		bool operator<(const iterator &rhs)const{ return node_ == rhs.node_ ? (cur_ < rhs.cur_) : (node_ < rhs.node_); };
		bool operator>(const iterator &rhs)const{ return rhs < *this; };
		bool operator<=(const iterator &rhs)const{ return !(*this>rhs); };
		bool operator>=(const iterator &rhs)const{ return !(*this < rhs); };

	private:
		void set_null(){ cur_ = first_ = last_ = NULL; node_ = NULL; };
		void set_node(map_pointer new_section){ node_ = new_section; first_ = *new_section; last_ = first_ + buffer_size(); };

};

#endif
