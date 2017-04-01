#ifndef		OWN_VECTOR_H
#define		OWN_VECTOR_H

#include <iostream>
#include "Alloc.h"
#include "Algo.h"

using namespace zz_STL;
using namespace zz_algo;

template <class  T, class Allocator=_new_alloc>
class Vector
{
	public:
		typedef T	value_type;
		typedef T*  pointer;
		typedef T*  iterator;
		typedef T&	reference;
		typedef const T& const_reference;
		typedef const T* const_pointer;
		typedef const T* const_iterator;

		typedef size_t size_type;
		typedef	Allocator	data_alloc;
	protected:
		iterator	start_;                               //目前可用空间的头
		iterator	finish_;                              //目前已用空间的尾
		iterator	end_of_storage_;                      //目前可用空间的尾巴
		 
		size_type	size_;                                //目前已用空间的大小
		size_type	capacity_;                            //目前可用空间的大小

	public:
		//构造以及析构函数
		Vector() :size_(0), capacity_(0),start_(NULL),finish_(NULL),end_of_storage_(NULL){}    //默认构造函数
		Vector(size_type n) :size_(n), capacity_(2 * n){ alloc_and_init(value_type());} 
		Vector(size_type n, const T &val) :size_(n), capacity_(2 * n){ alloc_and_init(val); }
		template <class InputIterator>
		Vector(InputIterator first, InputIterator last){ 
			size_ = Count_distance(first,last); capacity_ = 2 * size_; alloc_and_init(first, last); }
		//Vector(const Vector &rhs);
		Vector(const Vector &rhs){
			size_ = rhs.size(); capacity_ = rhs.capacity(); alloc_and_init(rhs.begin(), rhs.end());
		}
		Vector& operator=(const Vector &rhs);
		//使用delete会调用类型的默认析构函数
		~Vector(){
			free_and_destory();
		}
	public:
		//获取迭代器
	iterator	begin(){ return start_; }
	iterator	end(){ return finish_; }
	const_iterator begin()const{ return start_; }
	const_iterator end()const{ return finish_; }
	//获取容量信息
	bool	is_empty()const{ return !size_; }
	size_type size()const{ return size_; }
	size_type capacity()const{ return capacity_; }
	
	//访问元素
	reference	operator[](size_type n){ return *(start_ + n); }
	const_reference operator[](size_type n)const { return *(start_ + n); }
	reference at(size_type n){ return (*this)[n]; }
	const_reference at(size_type n)const{ return (*this)[n]; }
	reference	front(){ return *start_; }
	const_reference front()const{ return *start_; }
	reference	back(){ return *(finish_ - 1); }
	const_reference back()const{ return *(finish_ - 1); }
	//	
	//	//修改容器内容
	//	void assign(size_type n);
	//	void assign(size_type n, const value_type& val);
	//	template <class Iterator>
	//	void assign(Iterator first, Iterator last);

	void push_back(const value_type& val);
	void pop_back();
	iterator insert(iterator pos, const value_type& val){ insert_aux(pos, val); };
	void	 insert(iterator pos, const value_type& val, size_type n){ insert_aux(pos, val, n); };
	template <class InputIterator>
	void	insert(iterator pos, InputIterator first, InputIterator last);
//	iterator erase(iterator position);
	//	void erase(iterator first, iterator last);
	//	void	 clear(){ erase(start_, finish_); };
	//	void	 reserve(size_type n);
	//	void	 swap(Vector &rhs);

	private:

		void free_and_destory();
		void alloc_and_init(const T &val);
		template <class InputIterator>
		void alloc_and_init(InputIterator first, InputIterator last);
		void insert_aux(iterator pos, const value_type& val, size_type n=1);

};
template<class T, class Allocator>
inline Vector<T, Allocator>&  Vector<T, Allocator>::operator=(const Vector<T, Allocator> &rhs)
{
	if (capacity_ < rhs.capacity())
	{
		//析构现有对象并释放现有空间，重新配置空间并构造对象
		free_and_destory();
		size_ = rhs.size();
		capacity_ = rhs.capacity();
		alloc_and_init(rhs.begin(), rhs.end());
	}
	else
	{
		//仅仅析构现有对象，并且通过复制来构造对象
		destory(start_, finish_);
		uninitialized_copy(rhs.begin(), rhs.end(), start_);
	}
	return *this;
}
template<class T, class Allocator>
inline void Vector<T, Allocator>::pop_back()
{
	if (size_)
	{
		finish_--;
		destory(finish_);
		size_--;
	}
}
template <class T, class Allocator>
inline void Vector<T, Allocator>::push_back(const value_type& val)
{
	if (size_ < capacity_)
	{
		construct(&*finish_, val);
		size_++;
		finish_++;
	}
	else
		insert_aux(finish_, val);
}
template <class T, class Allocator>
inline void Vector<T, Allocator>::insert_aux(iterator pos, const value_type& val, size_type n/* =1 */)
{
	if ((size_ + n) <= capacity_)
	{
		if (pos < finish_)
		{
			uninitialized_copy(finish_ - n, finish_, finish_);
			copy(pos, finish_ - n, pos + n);
			fill(pos, pos + n, val);
		}
		else
			uninitialized_fill_n(pos, n, val);
		size_ += n;
		finish_ = start_ + size_;
	}
	else
	{
		const size_type new_size = size_ + n;
		const size_type new_capacity = 2 * new_size;
		iterator new_start = (T*)data_alloc::allocate(new_capacity*sizeof(T));
		uninitialized_copy(start_, pos, new_start);
		uninitialized_fill_n(new_start + Count_distance(start_, pos), n, val);
		uninitialized_copy(pos, finish_, new_start + Count_distance(start_, pos) + n);

		free_and_destory();
		size_ = new_size;
		capacity_ = new_capacity;
		start_ = new_start;
		finish_ = start_ + size_;
		end_of_storage_ = start_ + capacity_;
	}
}

template<class T, class Allocator>
inline void Vector<T,Allocator>::alloc_and_init(const T &val)
{
	
	start_ = (T*)data_alloc::allocate(capacity_*sizeof(T));                   //申请内存空间
	finish_ = start_ + size_;
	end_of_storage_ = start_ + capacity_;
	uninitialized_fill(start_, finish_, val);                                 //从start_初始化（构造对象）到finish_
}
template<class T, class Allocator>
template <class InputIterator>
inline void Vector<T, Allocator>::alloc_and_init(InputIterator first, InputIterator last)
{
	start_ = (T*)data_alloc::allocate(capacity_*sizeof(T));
	finish_ = start_ + size_;
	end_of_storage_ = start_ + capacity_;
	uninitialized_copy(first, last, start_);
}
template<class T, class Allocator>
inline void Vector<T, Allocator>::free_and_destory()
{
	destory(start_, finish_);                                                  //析构start_-finish_之间的对象
	data_alloc::deallocate(start_,capacity_);                                  //释放start_之后的内存空间
}

#endif