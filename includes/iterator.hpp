#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef> // ptrdiff_t 사용 : 두 포인터를 뺀 결과의 부호 있는 정수 유형

namespace ft {

	// 각 iterator의 catecory를 구분하기 위한 빈 클래스
	struct input_iterator_tag {}; // 입력반복자
	struct output_iterator_tag {}; // 출력반복자
	struct forward_iterator_tag : public input_iterator_tag {}; // 순방향 반복자
	struct bidirectional_iterator_tag : public forward_iterator_tag {}; // 양방향 반복자
	struct random_access_iterator_tag : public bidirectional_iterator_tag {}; // 임의 접근 반복자

	// iterator type 이름을 고정(특성이 달라도 통일해서 사용할수 있다)
	// Generic Definition
	template < class Iterator >
	struct iterator_traits {
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::iterator_category iterator_category;
	};

	//T* specialization
	template < class T >
	struct iterator_traits< T* > {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	//const T* specialization
	template < class T >
	struct iterator_traits< const T* > {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	// iterator 클래스를 파생하는데 사용할 수 있는 기본 클래스 템플릿
	// 해당 클래스는 iterator클래스가 아니며 iterator가 사용할 것으로 예상되는 기능을 제공하지 않는다.
	// std::iterator는 반복자에 필요한 유형의 정의를 단순화하기 위해 제공되는 기본 클래스입니다.
	// std::iterator를 사용하지 못하므로 따로 만들어서 사용
	template < class Category, class T, class Distance = ptrdiff_t,
			class Pointer = T*, class Reference = T& >
	struct iterator {
		typedef T value_type; // 반복자를 참조함으로써 얻을 수 있는 값의 타입, 반복자가 가리키는 요소의 타입
		typedef Distance difference_type; // 반복자 간의 거리를 식별하는데 사용할 수 있는 타입
		typedef Pointer pointer; // (T)에서 반복된 타입에 대한 포인터, 반복자가 가리키는 요소에 대한 포인터
		typedef Reference reference; // (T)에서 반복된 타입에 대한 참조, 반복자가 가리키는 요소에 대한 참조
		typedef Category iterator_category; // 반복자가 속한 카테고리
	};

	template < class Iterator >
	class reverse_iterator {
		public:
			typedef Iterator iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
			typedef typename iterator_traits<Iterator>::difference_type	 difference_type;
			typedef typename iterator_traits<Iterator>::value_type value_type;
			typedef typename iterator_traits<Iterator>::pointer pointer;
			typedef typename iterator_traits<Iterator>::reference reference;

		protected:
			Iterator current;

		public:
			// default constructor
			reverse_iterator() : current() {}
			// initialization constructor
			explicit reverse_iterator (iterator_type it) : current(it) {}
			// copy / type-cast constructor
			template <class Iter>
			reverse_iterator (const reverse_iterator<Iter>& rev_it) : current(rev_it.base()) {}
			//assigns another iterator adaptor
			template <typename Iter>
			reverse_iterator& operator= (const reverse_iterator<Iter>& rev_it) {
				this->current = rev_it.current;
				return (*this);
			}

			iterator_type base() const {
				return (this->current);
			}

			// accesses the pointed-to element
			reference operator*() const {
				iterator_type temp = this->current;
				return (*(--temp));
			}

			pointer operator->() const {
				return &(operator*());
			}

			// accesses an element by index
			// iterator가 현재 가리키는 요소에서 n개 위치에 있는 요소에 엑세스한다.
			reference operator[] (difference_type n) const {
				return (this->current[-n - 1]);
			}

			reverse_iterator& operator++() {
				--(this->current);
				return (*this);
	  		}

			reverse_iterator& operator--() {
				++(this->current);
				return (*this);
	  		}

			reverse_iterator operator++(int) {
	  			reverse_iterator temp = *this;
	  			++(*this);
	  			return (temp);
			}

			reverse_iterator operator--(int) {
				reverse_iterator temp = *this;
				--(*this);
				return (temp);
			}

			//현재 위치에서 n만큼 차이나는 위치의 iterator 반환
			//해당 기능들을 사용하기 위해서는 base iterator가 random_access_iterator여야한다.
			//	-> 해당 기능을 vector에서 사용하므로 vector_iterator에 random_access_iterator를 사용한다.
			reverse_iterator operator+ (difference_type n) const {
				return (reverse_iterator(current - n));
			}

			reverse_iterator operator- (difference_type n) const {
				return (reverse_iterator(current + n));
			}

			reverse_iterator& operator+= (difference_type n) {
				this->current -= n;
				return (*this);
	  		}

			reverse_iterator& operator-= (difference_type n) {
				this->current += n;
				return (*this);
	  		}
	}; // class reverse_iterator
	// non-member function overload
	// reverse_iterator 개체 lhs와 rhs 간에 적절한 비교 작업을 수행합니다.
	template <class Iter1, class Iter2>
	bool operator==(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator!=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator<(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator<=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
		return ( lhs.base() >= rhs.base() );
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& it) {
		return ( it.base() + n );
	}

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return ( rhs.base() - lhs.base() );
	}

	//non-member non-const function overload
	template <class Iter>
	bool operator==(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter>
	bool operator!=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter>
	bool operator<(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter>
	bool operator>(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter>
	bool operator>=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iter>
	bool operator<=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs) {
		return ( lhs.base() >= rhs.base() );
	}

	// 두 iterator adaptors 사이의 거리를 반환
	template <class Iterator, class Iterator_c>
	typename ft::reverse_iterator<Iterator>::difference_type operator-(const ft::reverse_iterator<Iterator> &lhs, const ft::reverse_iterator<Iterator_c> &rhs) {
		return ( rhs.base() - lhs.base() );
	}
} // namespace ft

#endif
