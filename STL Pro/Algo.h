#ifndef		OWN_ALGO_H
#define		OWN_ALGO_H	
#include "Alloc.h"
#include "Iterator_traits.h"
#include "Type_traits.h"

using namespace zz_type_traits;
using namespace zz_iterator_traits;
using namespace zz_STL;

namespace zz_algo{
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
	copy(InputIterator first, InputIterator last, ForwardIterator des)
	{
		return copy_aux(first, last, des, _false_type());
	}
	template <class T>
	inline T*
	copy(const T *first, const T *last, T *des)
	{
		typedef		type_traits<T>::is_POD		is_POD;
		return copy_aux(first, last, des, is_POD());
	}
	template <class T>
	inline T*
	copy(T *first, T *last, T *des)
	{
		typedef		type_traits<T>::is_POD		is_POD;
		return copy_aux(first, last, des, is_POD());
	}
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
	copy_aux(InputIterator first, InputIterator last, ForwardIterator des, _false_type)
	{
		ForwardIterator cur = des;
		for (; first != last; first++, cur++)
			*cur = *first;
		return cur;
	}
	template <class T>
	inline T*
	copy_aux(const T *first, const T *last, T *des, _true_type)
	{
		memmove(des, first, last - first);
		return des + (last - first);
	}

	template <class ForwardIterator, class T>
	inline void
	fill(ForwardIterator first, ForwardIterator last, const T& val)
	{
		fill_aux(first, last, val);
	}
	template <class ForwardIterator, class T>
	inline void
	fill_aux(ForwardIterator first, ForwardIterator last, const T &val)
	{
		for (; first!= last; first++)
			*first = val;
	}
}
#endif


