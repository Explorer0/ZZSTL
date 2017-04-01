#ifndef		OWN_ITERATOR_TRAIT_H
#define		OWN_ITERATOR_TRAIT_H
namespace zz_iterator_traits{
	//五种迭代器型别
	struct Input_iterator_tag{};
	struct Output_iterator_tag{};
	struct Forward_iterator_tag :Input_iterator_tag{};
	struct Bidirectional_iterator_tag :public Input_iterator_tag{};
	struct Random_access_iterator_tag :public Bidirectional_iterator_tag{};

	template <class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef	typename Iterator::deference_type		deference_type;
	};
	template <class T>
	struct iterator_traits<T*>
	{
		typedef  Random_access_iterator_tag	iterator_category;
		typedef  T							value_type;
		typedef  T*							pointer;
		typedef	 T&							reference;
		typedef	 ptrdiff_t					deference_type;
	};
	template <class T>
	struct  iterator_traits<const T*>
	{
		typedef  Random_access_iterator_tag		iterator_category;
		typedef  T								value_type;
		typedef  const T*						pointer;
		typedef	 const T&						reference;
		typedef	 ptrdiff_t						deference_type;

	};

	template <class Iterator>
	size_t Count_distance(Iterator first, Iterator last)
	{
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		return Count_distance_aux(first, last, iterator_category());
	}
	template <class Iterator>
	size_t Count_distance_aux(Iterator first, Iterator last, Random_access_iterator_tag)
	{
		return last - first;
	}
	template <class Iterator>
	size_t Count_distance_aux(Iterator first, Iterator last, Bidirectional_iterator_tag)
	{
		size_t len = 0;
		for (; first != last; first++)
			len++;
		return len;
	}

	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
}
#endif




