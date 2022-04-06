#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef> // ptrdiff_t 사용

namespace ft {

	// 각 iterator의 catecory를 구분하기 위한 빈 클래스
	struct input_iterator_tag {}; // 입력반복자
	struct output_iterator_tag {}; // 출력반복자
	struct forward_iterator_tag : public input_iterator_tag {}; // 순방향 반복자
	struct bidirectional_iterator_tag : public forward_iterator_tag {}; // 양방향 반복자
	struct random_access_iterator_tag : public bidirectional_iterator_tag {}; // 임의 접근 반복자

	// iterator type 이름을 고정(특성이 달라도 통일해서 사용할수 있다)
	template < class Iterator >
	struct iterator_traits {
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::iterator_category iterator_category;
	};

	template < class T >
	struct iterator_traits< T* > {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef const T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template < class T >
	struct iterator_traits< const T* > {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template < class Category, class T, class Distance = ptrdiff_t,
			class Pointer = T*, class Reference = T& >
	struct iterator {
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef Category iterator_category;
	};

	template < class Iterator >
	class reverse_iterator {
		public:
			typedef Iterator iterator_type;

		protected:
			Iterator current;
	}; // class reverse_iterator
} // namespace ft

#endif
