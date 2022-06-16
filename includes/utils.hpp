#ifndef UTILS_HPP
# define UTILS_HPP

#include "iterator.hpp"

namespace ft{
	// The type T is enabled as member type enable_if::type if Cond is true.
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

	// T가 integral type인지 식별하는 class.
	// is_integral_type< T > 를 상속받아 T에 따라 type을 다르게 갖는다.
	template <typename T>
	struct is_integral : public is_integral_type<T> {};	// equal

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

	// iterator범위 사이에서 동일한지 check. : pred로 비교.
	/* pred
		두 요소를 인수로 받아들이고(같은 순서로 두 시퀀스 각각 중 하나) 부울로 변환 가능한 값을 반환하는 이진 함수입니다.
		반환되는 값은 요소가 이 함수의 컨텍스트에서 일치하는 것으로 간주되는지 여부를 나타냅니다.
		함수는 인수를 수정하지 않아야 한다.
		이것은 함수 포인터 또는 함수 객체일 수 있다.
	*/
	template < class InputIterator1, class InputIterator2, class BinaryPredicate >
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
				BinaryPredicate pred) {
		while (first1 != last1) {
			if (!pred(*first1, *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	// lexicographical compare
	// default
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2) {
		while (first1 != last1) {
			if (first2 == last2 || *first2 < *first1)
				return (false);
			else if (*first1 < *first2)
				return (true);
			++first1;
			++first2;
		}
		return (first2 != last2);
	};

	// custom
	// binary comparison function comp
	// comparison function object (i.e. an object that satisfies the requirements of Compare)
	// which returns ​true if the first argument is less than the second.
	template <class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2, Compare comp) {
		while (first1 != last1) {
			if (first2 == last2 || comp(*first2, *first1))
				return (false);
			else if (comp(*first1, *first2))
				return (true);
			++first1;
			++first2;
		}
		return (first2 != last2);
	};

	// 두 객체를 하나의 객체로 취급 할 수 있게 묶어주는 클래스
	template <class T1, class T2>
	struct pair {
		public:
			typedef T1 first_type;
			typedef T2 second_type;

			first_type first;
			second_type second;

			//default
			pair() : first(), second() {}
			//copy
			template<class U, class V> pair (const pair<U, V>& pr) : first(pr.first), second(pr.second) {}
			//initialization
			pair(const first_type& a, const second_type& b) : first(a), second(b) {}
			//assign
			pair& operator= (const pair& pr) {
				if (this != &pr) {
					this->first = pr.first;
					this->second = pr.second;
				}
				return (*this);
			}
	}; // class pair

	// 변수1(x), 변수2(y)가 들어간 pair를 만들어준다
	template <class T1, class T2>
	pair<T1, T2> make_pair(T1 x, T2 y) {
		return (pair<T1, T2>(x,y));
	};

	template <class T1, class T2>
	bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	};

	template <class T1, class T2>
	bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return (!(lhs == rhs));
	};

	template <class T1, class T2>
	bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
	};

	template <class T1, class T2>
	bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return (!(rhs < lhs));
	};

	template <class T1, class T2>
	bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return (rhs < lhs);
	};

	template <class T1, class T2>
	bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
		return !(lhs < rhs);
	};

	// binary function
	// 표준 이진 함수 객체의 기본 클래스
	// 이항 함수 개체를 제공하는 파생 클래스에 상속될 수 있는 형식을 정의하는 빈 기본 구조체
	template < class Arg1, class Arg2, class Result >
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	// operator< 와 동일한 반환값을 가진다.
	template <class T>
	struct less : binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return (x < y);
		}
	};
}	// namespace ft


#endif
