#ifndef		OWN_VECTOR_H
#define		OWN_VECTOR_H

#include <iostream>
#include <memory>
#include <cassert>
#include "type_traits.h"
#include "Allocator.h"

#define		COMMON_LEN	 12
using namespace std;

template <class  T>
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
	protected:
		iterator	start_;                               //目前可用空间的头
		iterator	finish_;                              //目前已用空间的尾
		iterator	end_of_storage_;                      //目前可用空间的尾巴
		 
		size_type	size_;                                //目前已用空间的大小
		size_type	capacity_;                            //目前可用空间的大小

	public:
		//构造以及析构函数
		Vector() :size_(0), capacity_(0),start_(NULL),finish_(NULL),end_of_storage_(NULL){}    //默认构造函数
		Vector(size_type n) :size_(n), capacity_(2 * n){                //通过全局new来申请空间
			start_ = (T*)::operator new(capacity_*sizeof(T));	        //通过Construct初始化空间
			finish_ = start_+size_; 
			end_of_storage_ = start_ + capacity_;
			Construct(start_, size_, T()); } 
		Vector(T val, size_type n = COMMON_LEN):size_(n),capacity_(2*n){      
			start_ = (T*)::operator new(capacity_*sizeof(T));
			finish_ = start_ + size_;
			end_of_storage_ = start_ + capacity_;
			Construct(start_, size_, val);
		}
		template <class Iterator>
		Vector(Iterator first, Iterator last);
		//Vector(const Vector &rhs);
		Vector(Vector &rhs);

		Vector& operator=(const Vector &rhs);
		//使用delete会调用类型的默认析构函数
		~Vector(){
			destory_and_free();
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
		
		//修改容器内容
		void assign(size_type n);
		void assign(size_type n, const value_type& val);
		template <class Iterator>
		void assign(Iterator first, Iterator last);

		void push_back(const value_type& val);
		void pop_back();
		iterator insert(iterator position,const value_type& val);
		void	 insert(iterator position,const value_type& val, size_type n);
		iterator erase(iterator position);
		void erase(iterator first, iterator last);
		void	 clear(){ erase(start_, finish_); };
		void	 reserve(size_type n);
		void	 swap(Vector &rhs);

	private:
		//析构并且释放整个Vector
		void destory_and_free();
		//析构整个Vector
		void destory();

};
template <class T>
template <class Iterator>
inline Vector<T>::Vector(Iterator first, Iterator last) :size_(Count(first, last)), capacity_(2 * Count(first,last))
{
	//申请获得空间
	start_ = (T*)::operator new(capacity_*sizeof(T));
	finish_ = start_ + size_;
	end_of_storage_ = start_ + capacity_;
	//向空间中填充数据
	Construct(start_, size_, first);
}
template <class T>
inline Vector<T>::Vector(Vector &rhs) :size_(rhs.size()), capacity_(rhs.capacity())
{
	start_ = (T*)::operator new(capacity_*sizeof(T));;
	finish_ = start_ + size_;
	end_of_storage_ = start_ + capacity_;
	Construct(start_, size_, rhs.begin());
}
template <class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T> &rhs)
{
	
	if (this != &rhs)
	{
		size_type len = rhs.size();
		if (len >capacity_)
		{
			destory_and_free();
			size_ = len;
			capacity_ = 2 * len;
			start_ = (T*)::operator new(capacity_*sizeof(T));
			finish_ = start_ + size_;
			end_of_storage_ = start_ + capacity_;	
		}
		else
		{
			size_ = len;
			finish_ = start_ + size_;
		}
		Construct(start_, size_, rhs.begin());
	}
	return *this;
}
template <class T>
inline void Vector<T>::reserve(size_type n)
{
	if (n > capacity_)
	{
		iterator temp = (T*)::operator new(n * 2 * sizeof(T));
		Construct(temp, size_, start_);

		destory_and_free();
		size_ = size_;
		capacity_ = 2 * n;
		start_ = temp;
		finish_ = start_ + size_;
		end_of_storage_ = start_ + capacity_;
	}
}
template <class T>
inline void Vector<T>::push_back(const value_type& val)
{
	if (finish_ == end_of_storage_)
		reserve(capacity_);
	size_++;
	Construct(finish_++, 1, val);
}
template <class T>
inline void Vector<T>::pop_back()
{
	if (finish_ != start_)
	{
		size_--;
		Destory(--finish_);
	}
}
template <class T>
inline void Vector<T>::assign(size_type n)
{
	if (n > capacity_)
	{
		Vector<T> temp(value_type(),n);
		temp.swap(*this);
	}
	else
	{
		destory();
		size_ = n;
		finish_ = start_ + size_;
		Construct(start_, n, T());
	}
}
template <class T>
void Vector<T>::assign(size_type n, const value_type& val)
{
	if (n > capacity_)
	{
		Vector<T> temp(val, n);
		temp.swap(*this);
	}
	else
	{
		destory();
		size_ = n;
		finish_ = start_ + size_;
		Construct(start_, n, val);
	}
}
template <class T>
template <class Iterator>
void Vector<T>::assign(Iterator first, Iterator last)
{
	int len = Count(first, last);
	if (len > capacity_)
	{
		Vector<T> temp(first, last);
		temp.swap(*this);
	}
	else
	{
		destory();
		size_ = len;
		finish_ = start_ + size_;
		Construct(start_, len, first);
	}
}
template <class T>
inline typename  Vector<T>::iterator Vector<T>::insert(iterator position, const value_type& val)
{
	if (finish_ == end_of_storage_)
		reserve(capacity_);
	for (iterator p = finish_; p != position; p--)
		*p = *(p - 1);
	*position = val;
	size_++;
	finish_++;
	return position;
}
template <class T>
inline void Vector<T>::insert(iterator position, const value_type& val, size_type n)
{
	if ((finish_ + n) >= end_of_storage_)
		reserve(size_ + n);
	for (iterator p = finish_ + n - 1; p != position; p--)
		*p = *(p - n);
	for (iterator p = position; p != (position + n); p++)
		*p = val;
	size_ += n;
	finish_ += n;
}
template <class T>
inline typename  Vector<T>::iterator Vector<T>::erase(iterator position)
{
	Destory(position);
	if (position + 1 != finish_)
	{
		for (iterator p = position; p != (finish_-1); p++)
			*p = *(p + 1);
	}
	size_--;
	finish_--;
	return position;
}
template <class T>
inline void Vector<T>::erase(iterator first, iterator last)
{
	if (first >= start_ && last <= finish_)
	{
		size_type len = Count(first, last);
		for (iterator p = first; p != last; p++)
			Destory(p);
		for (iterator p = first; p != last; p++)
			*p = *(p + len);
		size_ -= len;
		finish_ -= len;
	}
}

template <class T>
inline void Vector<T>::swap(Vector &rhs)
{
	iterator  temp_iterator[3];
	size_type temp_size[2];
	temp_iterator[0] = rhs.begin();
	temp_iterator[1] = rhs.end();
	temp_iterator[2] = end_of_storage_;
	temp_size[0] = rhs.size();
	temp_size[1] = rhs.capacity();

	rhs.start_ = start_;
	rhs.finish_ = finish_;
	rhs.end_of_storage_ = end_of_storage_;
	rhs.size_ = size_;
	rhs.capacity_ = capacity_;

	start_ = temp_iterator[0];
	finish_ = temp_iterator[1];
	end_of_storage_ = temp_iterator[2];
	size_ = temp_size[0];
	capacity_ = temp_size[1];
}
template <class T>
inline void Vector<T>::destory_and_free()
{
	if (size_)
	{
		//逐个析构对象
		for (finish_--; finish_ != start_; finish_--)
			Destory(finish_);
		Destory(finish_);
		
	}
	//释放空间
	if (capacity_)
		::operator delete(start_);
}
template <class T>
inline void Vector<T>::destory()
{
	if (size_)
	{
		//逐个析构对象
		for (finish_--; finish_ != start_; finish_--)
			Destory(finish_);
		Destory(finish_);
	}
}


#endif