#ifndef UTILS_HPP
# define UTILS_HPP

#include "iterator.hpp"

namespace ft{
	/*
	 * enable_if
	The type T is enabled as member type enable_if::type if Cond is true.
	*/
	template <bool Cond, typename T = void>
	struct enable_if {};

	template <typename T>
	struct enable_if<true, T> {
		typedef T type;
	};

	/*
	 * is_integral
	Checks whether T is an integral type.
	bool, char, char16_t, char32_t, wchar_t, short, int, long, long long,
	unsinged char, unsigned short, unsigned int, unsigned long, unsigned long long,
	or any inplementation-defined extended integer types,
	including any signed, unsigned, and cv-qualified variants.
	Otherwise, value is equal to false.
	*/
	template <bool is_integral, typename T>
	struct integral_traits {
		typedef T type;
		static const bool value = is_integral;
	};

	//float, double, long double은 false로 처리된다.
	//char16_t, char32_t, long long, unsigned long long -> c++11
	template <typename>
	struct is_integral_type : public integral_traits<false, bool> {};

	// template 특수화
	// bool 타입에 대해 특수화
	template <>
	struct is_integral_type<bool> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<char> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<signed char> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<wchar_t> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<short int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<long int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<long long int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<unsigned char> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<unsigned short> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<unsigned int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<unsigned long int> : public integral_traits<true, bool> {};

	template <>
	struct is_integral_type<unsigned long long int> : public integral_traits<true, bool> {};

	// equal
	/* first1, last1
		첫 번째 시퀀스의 초기 및 최종 위치에 반복자를 입력합니다 .
		사용된 범위는 first1 이 가리키는 요소를 포함 하지만 last1 이 가리키는 요소가 아닌
		first1 과 last1[first1,last1) 사이의 모든 요소를 ​​포함합니다 .
	* first2
		두 번째 시퀀스의 초기 위치에 반복자를 입력합니다 .
		비교에는 범위에 있는 요소 수만큼 이 시퀀스의 요소가 포함됩니다 [first1,last1)
	*/
	template <class InputIterator1, class InputIterator2>
	bool equal (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
		while (first1 != last1) {
			if (!(*first1! == *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	};
}	// namespace ft


#endif
