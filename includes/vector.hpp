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
			typedef typename allocator_type::const_pointer const_pointer;
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
				_begin(u_nullptr),
				_end(u_nullptr),
				_end_capacity(u_nullptr) {}

			// fill constructor
			// -> Constructs a container with n elements. Each element is a copy of val.
			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc) {
				this->_begin = this->_alloc.allocate(n);
				this->_end = this->_begin;
				this->_end_capacity = this->_begin + n;
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
					_begin(u_nullptr),
					_end(u_nullptr),
					_end_capacity(u_nullptr) {
				difference_type n = x._end - x._begin;
				this->_begin = this->_alloc.allocate(n);
				this->_end = this->_begin;
				iterator tmp_begin = x._begin;
				iterator tmp_end = x._end;
				for (; tmp_begin != tmp_end; tmp_begin++) {
					this->_alloc.construct(this->_end, *tmp_begin);
					this->_end++;
				}
				this->_end_capacity = this->_end;
			}

			// destructor
			~vector() {
				this->clear();
				this->_alloc.deallocate(this->_begin, this->capacity());
			}

			// Assign content
			vector &operator=(const vector &x) {
				if (this != &x) {
					this->clear();
					this->assign(x.begin(), x.end());
				}
				return (*this);
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

			// n개의 val값들을 갖는 vector로 resize한다.
			// n이 현재 컨테이너 크기보다 작으면, 그 이상의 요소를 제거하고 크기를 축소시킨다.
			// n이 현재 컨테이너 크기보다 크면, n의 크기만큼 새로운 값을 삽입한다.
			// value가 지정되면 새 요소가 value의 복사본으로 초기화되고, 그렇지 않으면 값이 초기화된다.
			void resize(size_type n, value_type val = value_type()) {
				if (n < size())
					erase(_begin + n, _end);
				else if (n > size())
					insert(_end, n - size(), val);
			}

			// element의 갯수가 아닌, 할당받은 메모리의 갯수. (잠재적 크기)
			size_type capacity() const { return (this->_end_capacity - this->_begin); }
			bool empty() const { return (this->_begin == this->_end); }

			// 벡터의 용량(재할당 없이 벡터가 보유할 수 있는 총 요소 수)을 보다 크거나 같은 값으로 늘 new_cap립니다.
			// 현재 capacity() new_cap 보다 크면 새 스토리지가 할당되고, 그렇지 않으면 함수가 아무 작업도 수행하지 않습니다.
			void reserve(size_type n) {
				if (n > max_size()) //최대 크기를 넘어가면 에러
					throw(std::length_error("Error: ft::vector::reserve"));
				else if (n > this->capacity()) {
					pointer prev_begin = this->_begin;
					pointer prev_end = this->_end;
					pointer prev_end_capacity = this->_end_capacity;

					this->_begin = this->_alloc.allocate(n);
					this->_end = this->_begin;
					this->_end_capacity = this->_begin + n;
					pointer tmp = prev_begin;
					while (tmp != prev_end) {
						this->_alloc.construct(this->_end++, *tmp);
						this->_alloc.destroy(tmp++);
					}
					this->_alloc.deallocate(prev_begin, prev_end_capacity - prev_begin);
				}
			}

			//access element
			reference operator[](size_type n) { return (*(this->_begin + n)); }
			//const access element
			const_reference operator[](size_type n) const { return (*(this->_begin + n)); }

			//access element
			reference at(size_type n) {
				if (n >= this->size())
					throw(std::out_of_range("Error: ft::vector::at"));
				return ((*this)[n]);
			}
			//const access element
			const_reference at(size_type n) const {
				if (n >= this->size())
					throw(std::out_of_range("Error: ft::vector::at"));
				return ((*this)[n]);
			}

			// 벡터의 첫번째 element를 리턴.
			reference front() { return (*(this->_begin)); }
			const_reference front() const { return (*(this->_begin)); }

			// 벡터의 마지막 element를 리턴.
			reference back() { return (*(this->_end - 1)); }
			const_reference back() const { return (*(this->_end - 1)); }

			//assign range
			//벡터에 새 내용을 할당하고 현재 내용을 교체하고 그에 따라 크기를 수정한다.
			template < typename InputIterator >
			void assign(InputIterator first, InputIterator last,
						typename ft::enable_if< !ft::is_integral< InputIterator >::value,
						InputIterator >::type * = NULL) {
				this->clear();
				size_type n = ft::distance(first, last);
				if (n <= this->capacity()) {
					while (n--)
						this->_alloc.construct(this->_end++, *first++);
				}
				else {
					pointer prev_begin = this->_begin;
					pointer prev_end_capacity = this->_end_capacity;

					this->_begin = this->_alloc.allocate(n);
					this->_end = this->_begin;
					this->_end_capacity = this->_begin + n;
					const_pointer tmp = &(*first);
					while(tmp != &(*last))
						this->_alloc.construct(this->_end++, *tmp++);
					this->_alloc.deallocate(prev_begin, prev_end_capacity - prev_begin);
				}
			}
			//assign fill
			void assign(size_type n, const value_type &val) {
				this->clear();
				if (n <= this->capacity()) {
					while (n--)
						this->_alloc.construct(this->_end++, val);
				}
				else {
					pointer prev_begin = this->_begin;
					pointer prev_end_capacity = this->_end_capacity;

					this->_begin = this->_alloc.allocate(n);
					this->_end = this->_begin;
					this->_end_capacity = this->_begin + n;
					while (n--)
						this->_alloc.construct(this->_end++, val);
					this->_alloc.deallocate(prev_begin, prev_end_capacity - prev_begin);
				}
			}
	}; // class vector
}  // namespace ft

#endif
