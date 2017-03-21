#ifndef		OWN_ALLOCATOR_H
#define		OWN_ALLOCATOR_H
#include "type_traits.h"

//�ⲿ����
//��src���n�������쵽des��
template <class T1, class T2>
void Construct(T1 *des, size_t n, T2 *src)
{
	typedef		size_t	size_type;
	for (size_type i = 0; i < n; i++)
		new(des + i)T1(*(src + i));
}
//��val����des֮���n������
template <class T1, class T2>
void Construct(T1* des, size_t n, T2 &val)
{
	typedef		size_t	size_type;
	for (size_type i = 0; i < n; i++)
		new(des + i)T1(val);
}

//����desָ��Ķ���
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