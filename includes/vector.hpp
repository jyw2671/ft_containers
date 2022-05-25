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

		public:
			// empty container constructor (default constructor)
			// -> Constructs an empty container, with no elements.
			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc),
				_start(u_nullptr),
				_end(u_nullptr),
				_end_capacity(u_nullptr) {}

			// fill constructor
			// -> Constructs a container with n elements. Each element is a copy of val.
			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc) {
				this->_start = this->_alloc.allocate(n);
				this->_end = this->_start;
				this->_end_capacity = this->_start + n;
				for (; n > 0; n--) {
					this->_alloc.construct(this->_end, val);
					this->_end++;
				}
			}

			// range constructor
			template <class InputIterator>
			vector (InputIterator first, InputIterator last,
				const allocator_type& alloc = allocator_type());

			// copy constructor
			vector (const vector& x) : _alloc(x._alloc),
					_start(u_nullptr),
					_end(u_nullptr),
					_end_capacity(u_nullptr) {
				difference_type n = x._end - x._start;
				this->_start = this->_alloc.allocate(n);
				this->_end = this->_start;
				iterator tmp_start = x._start;
				iterator tmp_end = x._end;
				for (; tmp_start != tmp_end; tmp_start++) {
					this->_alloc.construct(this->_end, *tmp_start);
					this->_end++;
				}
				this->_end_capacity = this->_end;
			}

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
