#ifndef		OWN_TRAIT_H
#define		OWN_TRAIT_H

//五种迭代器型别
struct Input_iterator_tag{};
struct Output_iterator_tag{};
struct Forward_iterator_tag :Input_iterator_tag{};
struct Bidirectional_iterator_tag :public Input_iterator_tag{};
struct Random_access_iterator_tag :public Bidirectional_iterator_tag{};

template <class Iterator>
struct traits
{
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};
template <class T>
struct  traits<T*>
{
	typedef  Random_access_iterator_tag	iterator_category;
	typedef  T							value_type;
	typedef  T*							pointer;
	typedef	 T&							reference;
};
template <class T>
struct  traits<const T*>
{
	typedef  Random_access_iterator_tag		iterator_category;
	typedef  T	value_type;
	typedef  T*	pointer;
	typedef	 T& reference;

};
#endif




