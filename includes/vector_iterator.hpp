#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include "iterator.hpp"

namespace ft {
	template < typename T >
	class vector_iterator : public ft::iterator< ft::random_access_iterator_tag, T > {
		public:
			typedef T value_type;
			typedef T *pointer;
			typedef T &reference;
			typedef typename ft::iterator< ft::random_access_iterator_tag,
					 T >::difference_type difference_type;
			typedef typename ft::iterator< ft::random_access_iterator_tag,
					 T >::iterator_category iterator_category;
		protected:
  			pointer _ptr;
	}; // class vectorIterator
} // namespace ft

#endif
