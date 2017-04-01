#ifndef		OWN_TYPE_TRAITS_H
#define		OWN_TYPE_TRAITS_H
namespace zz_type_traits{
	struct _true_type{};
	struct _false_type{};

	template <class Type>
	struct type_traits
	{
		//typedef     _true_type      this_dummy_member_must_be_first;
		typedef		_false_type		has_trivial_constructor;
		typedef		_false_type		has_trivial_copy_constructor;
		typedef		_false_type		has_trivial_assignment_operator;
		typedef		_false_type		has_trivial_destructor;
		typedef		_false_type		is_POD;
	};
	template <>
	struct type_traits<char>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<signed char>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<unsigned char>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<int>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<unsigned int>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<short>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<unsigned short>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<long>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<unsigned long>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<double>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<long double>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};
	template <>
	struct type_traits<float>
	{
		typedef		_true_type		has_trivial_constructor;
		typedef		_true_type		has_trivial_copy_constructor;
		typedef		_true_type		has_trivial_assignment_operator;
		typedef		_true_type		has_trivial_destructor;
		typedef		_true_type		is_POD;
	};

}


#endif
