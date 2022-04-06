#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include "vector_iterator.hpp"

namespace ft {
	template < typename T, typename Allocator = std::allocator< T > >
	class vector {
		public:
			typedef T value_type;
			typedef Allocator allocator_type;
			typedef typename allocator_type::pointer pointer;
			typedef ft::vector_iterator< T > iterator;
			typedef ft::vector_iterator< const T > const_iterator;
			typedef ft::reverse_iterator< iterator > reverse_iterator;
			typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;
			typedef typename allocator_type::size_type size_type;

		private:
			allocator_type _alloc;
			pointer _begin;
			pointer _end;
			pointer _end_capacity;

		iterator begin() { return (iterator(this->_begin)); }
		const_iterator begin() const { return (const_iterator(this->_begin)); }
		iterator end() { return (iterator(this->_end)); }
		const_iterator end() const { return (const_iterator(this->_end)); }
		reverse_iterator rbegin() { return (reverse_iterator(this->_end)); }
		const_reverse_iterator rbegin() const {
			return (const_reverse_iterator(this->_end));
		}
		reverse_iterator rend() { return (reverse_iterator(this->_begin)); }
		const_reverse_iterator rend() const {
			return (const_reverse_iterator(this->_begin));
		}

		size_type size() const { return (this->_end - this->_begin); }
		size_type max_size() const { return (this->_alloc.max_size()); }
	}; // class vector
}  // namespace ft

#endif
