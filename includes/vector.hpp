#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "vector_iterator.hpp"
#include "utils.hpp"

namespace ft {
	template < typename T, typename Allocator = std::allocator< T > >
	class vector {
		public:
			typedef T value_type; // 컨테이너에 담을 변수 타입 저장
			typedef Allocator allocator_type; // allocator의 타입과 reference, pointer
			typedef typename allocator_type::reference reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer pointer;
			typedef typename allocator_type::const_pointer const_pointer;
			typedef ft::vector_iterator<T> iterator;
			typedef ft::vector_iterator<const T> const_iterator; // 참조된 값을 바꿀 수 없다.
			typedef ft::reverse_iterator<iterator> reverse_iterator; // 제일 마지막 요소가 begin이 되고, ++연산을 하면 역순으로 연산이 이뤄진다.
			typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
			typedef typename allocator_type::size_type size_type; // Unsigned integer type
			typedef typename allocator_type::difference_type difference_type; // Signed integer type, 노드 간 거리 연산을 위한 타입

		private:
			allocator_type _alloc;
			pointer _begin; // 벡터 배열
			pointer _end; // 백터의 현재 위치
			pointer _end_capacity; // 벡터 저장공간의 마지막 위치

		public:
			// empty container constructor (default constructor)
			// -> Constructs an empty container, with no elements.
			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _begin(NULL), _end(NULL), _end_capacity(NULL) {}

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
			// 이 때 first 가 가리키는 원소 부터, last 가 가리키는 원소바로 직전까지 복사가 된다
			template <typename InputIterator>
			vector(InputIterator first, InputIterator last,
				const allocator_type &alloc = allocator_type(),
				typename ft::enable_if< !ft::is_integral< InputIterator >::value, InputIterator >::type* = NULL)
					: _alloc(alloc), _begin(NULL), _end(NULL), _end_capacity(NULL) {
				difference_type n = ft::distance(first, last);

				this->_begin = this->_alloc.allocate(n);
				this->_end = this->_begin;
				this->_end_capacity = this->_begin + n;
				while (n--)
					this->_alloc.construct(this->_end++, *first++);
			}

			// copy constructor
			vector (const vector& x) : _alloc(x._alloc), _begin(NULL), _end(NULL), _end_capacity(NULL) {
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

			// Return size of allocated storage capacity
			// element의 갯수가 아닌, 할당받은 메모리의 갯수. (잠재적 크기)
			size_type capacity() const { return (this->_end_capacity - this->_begin); }

			// Test whether vector is empty
			bool empty() const { return (this->_begin == this->_end); }

			// Request a change in capacity
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

			// 벡터의 뒤에 새로운 element를 추가한다.
			// 늘어난 벡터의 크기가 capacity를 넘어갈 경우, 이전 capacity * 2의 크기로 늘어남.
			void push_back(const value_type &val) {
				if (this->_end == this->_end_capacity) {
					if (this->size() == 0)
						this->reserve(1);
					else
						this->reserve(this->capacity() * 2);
				}
				this->_alloc.construct(this->_end++, val);
			}

			// 벡터의 맨 뒤 요소를 하나 제거한다.
			void pop_back() { this->_alloc.destroy(--this->_end); }

			//1.single element insert
			iterator insert(iterator position, const value_type &val) {
				size_type n = &(*position) - this->_begin;
				this->insert(position, 1, val);
				return (this->begin() + n);
			}

			//2.fill element insert
			void insert(iterator position, size_type n, const value_type &val) {
				if (this->size() + n <= this->capacity()) {
					pointer val_tmp = this->_end;
					size_type range = this->_end - &(*position);
					this->_end += n;
					pointer tmp = this->_end;
					while (range--)
						this->_alloc.construct(--tmp, *(--val_tmp));
					while (n--)
						this->_alloc.construct(--tmp, val);
				}
				else {
					pointer tmp = this->_begin;
					size_type _size = n + this->size();
					size_type front_tmp = &(*position) - this->_begin;
					size_type back_tmp = _end - &(*position);
					this->_begin = _alloc.allocate(_size);
					this->_end = _begin;
					this->_end_capacity = this->_begin + _size;
					while (front_tmp--) {
						_alloc.construct(this->_end++, *tmp);
						_alloc.destroy(tmp++);
					}
					while (n--)
						_alloc.construct(this->_end++, val);
					while (back_tmp--) {
						_alloc.construct(this->_end++, *tmp);
						_alloc.destroy(tmp++);
					}
				}
			}

			//3.range element instert
			template < typename InputIterator >
			void insert(iterator position, InputIterator first, InputIterator last,
				typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = NULL) {
				size_type n = ft::distance(first, last);
				if (this->size() + n <= this->capacity()) {
					pointer val_tmp = this->_end;
					size_type range = this->_end - &(*position);
					this->_end += n;
					pointer tmp = this->_end;
					while (range--)
						this->_alloc.construct(--tmp, *(--val_tmp));
					while (n--)
						this->_alloc.construct(--tmp, *(--last));
				}
				else {
					pointer tmp = this->_begin;
					size_type _size = n + this->size();
					size_type front_tmp = &(*position) - this->_begin;
					size_type back_tmp = this->_end - &(*position);
					this->_begin = this->_alloc.allocate(_size);
					this->_end = this->_begin;
					this->_end_capacity = this->_begin + _size;
					while (front_tmp--) {
						_alloc.construct(this->_end++, *tmp);
						_alloc.destroy(tmp++);
					}
					while (n--)
						_alloc.construct(this->_end++, *first++);
					while (back_tmp--) {
						_alloc.construct(this->_end++, *tmp);
						_alloc.destroy(tmp++);
					}
				}
			}

			//단일 요소(위치) 제거
			iterator erase(iterator position) {
				this->_alloc.destroy(&(*position));
				size_type n = this->_end - &(*position) - 1;
				pointer tmp = &(*position);

				while (n--) {
					this->_alloc.construct(tmp, *(tmp + 1));
					this->_alloc.destroy(tmp++);
				}
				--this->_end;
				return (position);
			}

			//범위[first, last) 제거
			iterator erase(iterator first, iterator last) {
				pointer tmp = &(*first);
				while (tmp != &(*last))
					_alloc.destroy(tmp++);
				size_type n = this->_end - &(*last);
				size_type erase_size = ft::distance(first, last);
				tmp = &(*first);
				while (n--)
				{
					_alloc.construct(tmp, *last++);
					_alloc.destroy(tmp++);
				}
				this->_end -= erase_size;
				return (first);
			}

			void swap(vector &x) {
				if (*this == x)
					return ;

				allocator_type tmp_alloc = x._alloc;
				pointer tmp_begin = x._begin;
				pointer tmp_end = x._end;
				pointer tmp_end_capacity = x._end_capacity;

				x._alloc = this->_alloc;
				x._begin = this->_begin;
				x._end = this->_end;
				x._end_capacity = this->_end_capacity;

				this->_alloc = tmp_alloc;
				this->_begin = tmp_begin;
				this->_end = tmp_end;
				this->_end_capacity = tmp_end_capacity;
			}

			void clear() {
				while (this->_end != this->_begin) {
					this->_alloc.destroy(--this->_end);
				}
			}

			//allocator
			//벡터와 연결된 할당자 객체의 복사본을 반환한다.
			allocator_type get_allocator() const { return (this->_alloc); }
	}; // class vector

	// vector non-member function
	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (lhs.size() == rhs.size()
				&& ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template < typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (!(lhs == rhs));
	}

	template <typename T, typename Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (!(rhs < lhs));
	}

	template <typename T, typename Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (rhs < lhs);
	}

	template <typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (!(lhs < rhs));
	}

	template <typename T, typename Alloc>
	void swap(vector< T, Alloc > &x, vector< T, Alloc > &y) {
		x.swap(y);
	}
}  // namespace ft

#endif
