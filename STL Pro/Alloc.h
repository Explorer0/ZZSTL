#ifndef		OWN_ALLOC_H
#define		OWN_ALLOC_H
#define		_THROW_ALLOC_FAILURE	std::cerr<<"out of memory."<<std::endl;exit(0);
#include <iostream>
#include "Type_traits.h"
#include "Iterator_traits.h"

using namespace zz_type_traits;
using namespace zz_iterator_traits;

namespace zz_STL{

	template <class T1, class T2>
	void construct(T1 *p, const T2 &val)
	{
		new(p)T2(val);
	}
	template <class InputIterator>
	void destory(InputIterator first, InputIterator last)
	{
		destory_dispatch(first, last, *first);
	}
	template <class InputIterator, class T>
	void destory_dispatch(InputIterator first, InputIterator last, const T&)
	{
		typedef		type_traits<T>::is_POD		is_POD;              //推导目标类型的POD类型
		_destory(first, last, is_POD());

	}
	template <class InputIterator>
	void _destory(InputIterator first, InputIterator last, _true_type){}  //拥有trivial_destructor   
	template <class InputIterator>
	void _destory(InputIterator first, InputIterator last, _false_type)   //未拥有trivial_destructor  
	{
		for (; first != last; first++)
			destory(&*first);
	}
	void destory(char *first, char *last){}
	void destory(wchar_t *first, wchar_t *last){}
	template <class T>
	void destory(T *p)
	{
		p->~T();
	}


	class _new_alloc
	{
	public:
		typedef void(*malloc_handler)();

		static void *allocate(size_t n);
		static void *reallocate(void *p, size_t old_sz, size_t new_sz);
		static void deallocate(void *p, size_t n);
		static void set_alloc_handler(malloc_handler p){ alloc_handler = p; }            //设置分配异常处理例程

	protected:
		//void oom_hint(){ std::cerr << "operator new unable to satisfy request for memory, try allocate memory again.\n"; }
		static void *oom_allocate(size_t n);
		static void *oom_realocate(void *p, size_t new_sz);
		//分配异常处理例程
		static malloc_handler	alloc_handler;

	};
	_new_alloc::malloc_handler _new_alloc::alloc_handler = 0;
	void *_new_alloc::allocate(size_t n)
	{
		void *result = malloc(n);
		if (NULL == result)
			result = oom_allocate(n);                       //调用Out Of Memory型的allocate进行深度分配
		return result;
	}
	void *_new_alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		void *result = realloc(p, new_sz);
		if (NULL == result)
			result = oom_realocate(p, new_sz);
		return result;
	}
	void _new_alloc::deallocate(void *p, size_t n)
	{
		free(p);
	}

	void *_new_alloc::oom_allocate(size_t n)
	{
		void *result = malloc(n);
		while (result == NULL)
		{
			if (!alloc_handler)
			{
				//throw exception
				_THROW_ALLOC_FAILURE
			}
			alloc_handler();                            //调用用户设置的异常处理例程
			result = malloc(n);                         //再次尝试分配内存
		}
		return result;
	}
	void *_new_alloc::oom_realocate(void *p, size_t new_sz)
	{
		void *result = realloc(p, new_sz);
		while (result == NULL)
		{
			if (!alloc_handler)
			{
				//throw exception
				_THROW_ALLOC_FAILURE;
			}
			alloc_handler();
			result = realloc(p, new_sz);
		}
		return result;
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
	uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator des)
	{
		return uninitialized_copy_aux(first, last, des, _false_type());
	}
	template <class T>
	inline T*
	uninitialized_copy(const T *first, const T *last, T *des)
	{
		typedef		type_traits<T>::is_POD		is_POD;
		return uninitialized_copy_aux(first, last, des, is_POD());
	}
	template <class T>
	inline T*
	uninitialized_copy(T *first, T *last, T *des)
	{
		typedef		type_traits<T>::is_POD		is_POD;
		return uninitialized_copy_aux(first, last, des, is_POD());
	}
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
	uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator des,_false_type)
	{
		ForwardIterator cur = des;
		for (; first < last; first++, cur++)
			construct(&*cur, *first);
		return cur;
	}
	template <class T>
	inline T*
	uninitialized_copy_aux(const T *first, const T *last, T *des, _true_type)
	{
		memmove(des, first, last - first);
		return des + (last - first);
	}
	//template <class T>
	//inline T*
	//uninitialized_copy_aux(T *first, T *last, T *des, _true_type)
	//{
	//	memmove(des, first, last - first);
	//	return des + (last - first);
	//}


	template <class ForwardIterator, class T>
	inline void
	uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val)
	{
		uninitialized_fill_aux(first, last, val);
	}
	template <class ForwardIterator, class T>
	inline void
	uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &val)
	{
		for (; first <last; first++)
			construct(&*first, val);
	}

	template <class ForwardIterator,class Size, class T>
	inline ForwardIterator
	uninitialized_fill_n(ForwardIterator des, Size n, const T &val)
	{
		return uninitialized_fill_n_aux(des, n, val);
	}
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator
	uninitialized_fill_n_aux(ForwardIterator des, Size n, const T &val)
	{
		ForwardIterator cur = des;
		for (; n > 0; n--,cur++)
			construct(&*cur, val);
		return cur;
	}
}

#endif
//template <class ForwardIterator, class T>
//inline void
//uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val)
//{
//	typedef	typename type_traits<T>::is_POD		is_POD;
//	uninitialized_fill_aux(first, last, val, is_POD());
//}
//template <class ForwardIterator, class T>
//inline void
//uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &val, _true_type)
//{
//	for (; first <= last; first++)
//		*first = val;
//}
//template <class ForwardIterator, class T>
//inline void
//uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &val, _false_type)
//{
//	for (; first <= last; first++)
//		construct(&*first, val);
//}