#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include "iterator.hpp"
#include "utils.hpp"

namespace ft {
	// Random access iterators는 포인터와 동일한 기능을 제공하는 임의의 오프셋 위치에서
	// 요소에 액세스하는 데 사용할 수 있는 iterator이다.
	// T	Type of elements pointed by the iterator.
	template < typename T >
	class vector_iterator : public ft::iterator< ft::random_access_iterator_tag, T > {
		public:
			typedef typename ft::iterator< ft::random_access_iterator_tag, T >::value_type value_type;
			typedef typename ft::iterator< ft::random_access_iterator_tag, T >::pointer pointer;
			typedef typename ft::iterator< ft::random_access_iterator_tag, T >::reference reference;
			typedef typename ft::iterator< ft::random_access_iterator_tag, T >::difference_type difference_type;
			typedef typename ft::iterator< ft::random_access_iterator_tag, T >::iterator_category iterator_category;

		protected:
			pointer _ptr;

		public:
			 //default constructor
			vector_iterator(pointer ptr = NULL) : _ptr(ptr) {}
			// copy constructor
			vector_iterator(const vector_iterator<T> &other) : _ptr(other.base()) {}
			//assignment operator
			vector_iterator &operator=(const vector_iterator<T> &other) {
				if (this != &other)
					this->_ptr = other.base();
				return (*this);
			}
			//overloading type casting
			operator vector_iterator<const T>() const { return (vector_iterator<const T>(this->_ptr)); }
			//getter
			pointer const &base() const { return (this->_ptr); }
			// destructor
			virtual ~vector_iterator() {}
			// operator
			reference operator*() const { return (*this->_ptr); }
			pointer operator->() const { return (this->_ptr); }

			vector_iterator &operator++() {
				++this->_ptr;
				return (*this);
			}

			vector_iterator operator++(int) {
				vector_iterator tmp = *this;
				++this->_ptr;
				return (tmp);
			}

			vector_iterator &operator--() {
				--this->_ptr;
				return (*this);
			}

			vector_iterator operator--(int) {
				vector_iterator tmp = *this;
				--this->_ptr;
				return (tmp);
			}

			vector_iterator operator+(difference_type n) const {
				return (vector_iterator(this->_ptr + n));
			}

			vector_iterator operator-(difference_type n) const {
				return (vector_iterator(this->_ptr - n));
			}

			vector_iterator &operator+=(difference_type n) {
				this->_ptr += n;
				return (*this);
			}

			vector_iterator &operator-=(difference_type n) {
				this->_ptr -= n;
				return (*this);
			}

			reference operator[](difference_type n) const {
				return (*(this->_ptr + n));
			}
	}; // class vector_iterator
	//Relational operators
	template <class Iter1, class Iter2>
	bool operator==(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator!=(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator<(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>=(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() >= rhs.base() );
	}


	template <class Iter1, class Iter2>
	bool operator<=(const vector_iterator<Iter1>& lhs, const vector_iterator<Iter2>& rhs) {
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iterator>
	ft::vector_iterator<Iterator> operator+(typename ft::vector_iterator<Iterator>::difference_type n, ft::vector_iterator<Iterator> &it) {
		return (it + n);
	}

	template <class Iterator>
	typename ft::vector_iterator<Iterator>::difference_type operator-(const ft::vector_iterator<Iterator> &lhs, const ft::vector_iterator<Iterator> &rhs) {
		return (lhs.base() - rhs.base());
	}

	//non-member non-const function overload
	template <class Iter>
	bool operator==(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter>
	bool operator!=(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter>
	bool operator<(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter>
	bool operator>(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter>
	bool operator>=(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() >= rhs.base() );
	}

	template <class Iter>
	bool operator<=(const vector_iterator<Iter>& lhs, const vector_iterator<Iter>& rhs) {
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iterator, class Iterator_c>
	typename ft::vector_iterator<Iterator>::difference_type operator-(const ft::vector_iterator<Iterator> &lhs, const ft::vector_iterator<Iterator_c> &rhs) {
		return (lhs.base() - rhs.base());
	}
} // namespace ft

#endif
