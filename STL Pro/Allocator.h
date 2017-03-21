#ifndef		OWN_ALLOCATOR_H
#define		OWN_ALLOCATOR_H
#include "type_traits.h"

//外部函数
//将src后的n个对象构造到des处
template <class T1, class T2>
void Construct(T1 *des, size_t n, T2 *src)
{
	typedef		size_t	size_type;
	for (size_type i = 0; i < n; i++)
		new(des + i)T1(*(src + i));
}
//用val构造des之后的n个对象
template <class T1, class T2>
void Construct(T1* des, size_t n, T2 &val)
{
	typedef		size_t	size_type;
	for (size_type i = 0; i < n; i++)
		new(des + i)T1(val);
}

//析构des指向的对象
template <class T>
void Destory(T *des)
{
	des->~T();
}


template <class Iterator>
size_t Count(Iterator first, Iterator last)
{
	typedef typename traits<Iterator>::iterator_category	iterator_category;
	return _Count(first, last, iterator_category());
}
template <class Iterator>
size_t _Count(Iterator first, Iterator last, Random_access_iterator_tag)
{
	return last - first;
}
template <class Iterator>
size_t _Count(Iterator first, Iterator last, Bidirectional_iterator_tag)
{
	size_t len = 0;
	for (; first != last; first++)
		len++;
	return len;
}
#endif