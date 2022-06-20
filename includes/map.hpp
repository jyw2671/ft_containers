#ifndef MAP_HPP
# define MAP_HPP

#include "RB_Tree.hpp"

namespace ft {
	template < class Key,	// map::key_type
			class T,	// map::mapped_type
			class Compare = ft::less< Key >,	// map::key_compare
			class Alloc = std::allocator< ft::pair< const Key, T > > >	// map::allocator_type
	class map {
		public:
			// Member types
			typedef Key key_type;
			typedef T mapped_type;
			typedef ft::pair< const key_type, mapped_type > value_type;
			typedef Compare key_compare;

			// value_compare
			// 두 개의 요소를 비교하여 첫 번째 요소의 키가 두 번째 요소보다 이전인지 확인하는데 사용할 수 있는 comp 객체
			// 반환되는 comp 객체는 멤버타입 map::value_compare의 객체로 내부 comp 객체를 사용하여,
			// 적절한 비교함수 클래스를 생성하는 중첩 클래스
			class value_compare : binary_function<value_type, value_type, bool>
			{
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {}
				// constructed with map's comparison object
				public:
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;
					typedef bool		result_type;
					//construct a new value_compare object
					value_compare() : comp() {}
					//compares two values of type value_type
					bool operator()(const value_type& lhs, const value_type& rhs) const {
						return (comp(lhs.first, rhs.first));
					}
			}; // class value_compare

			typedef Alloc allocator_type;
			typedef typename allocator_type::reference reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer pointer;
			typedef typename allocator_type::const_pointer const_pointer;
			typedef typename ft::RBTreeIterator<value_type, value_type*, value_type&> iterator;
			typedef typename ft::RBTreeIterator<value_type, const value_type*, const value_type&> const_iterator;
			typedef ft::reverse_iterator<iterator> reverse_iterator;
			typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
			typedef typename allocator_type::difference_type difference_type;
			typedef typename allocator_type::size_type size_type;
			typedef ft::RBTree<value_type, value_compare> rb_tree;
			typedef ft::RBTreeNode<value_type> node_type;

		private:
			allocator_type _alloc;
			rb_tree _tree;
			key_compare _comp;

		public:
			// Member function
			// Construct map
			// empty container constructor (default constructor)
			explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _alloc(alloc), _tree(), _comp(comp) {}
			// range constructor
			template <class InputIterator>
			map (InputIterator first, InputIterator last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type(),
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) : _alloc(alloc), _tree(), _comp(comp) {
				insert(first, last);
			}
			// copy constructor
			map(const map& x) : _alloc(x._alloc), _tree(), _comp(x._comp){
				*this = x;
			}
			// Map destructor
 			virtual ~map() {}
			// Copy container content
			map& operator=(const map& x) {
				if (this != &x) {
					this->_tree.copy(x._tree);
				}
				return (*this);
			}

			// Iterators
			iterator begin() { return (iterator(_tree.get_begin())); }
			const_iterator begin() const {
				return (const_iterator(_tree.get_begin()));
			}
			iterator end() { return (iterator(_tree.get_end())); }
			const_iterator end() const { return (const_iterator(_tree.get_end())); }
			reverse_iterator rbegin() { return (reverse_iterator(_tree.get_end())); }
			const_reverse_iterator rbegin() const {
				return (const_reverse_iterator(_tree.get_end()));
			}
			reverse_iterator rend() { return (reverse_iterator(_tree.get_begin())); }
			const_reverse_iterator rend() const {
				return (const_reverse_iterator(_tree.get_begin()));
			}

			//Capacity
			bool empty() const {
				return (this->_tree.empty());
			}
			size_type size() const {
				return (this->_tree.size());
			}
			size_type max_size() const {
				return (this->_tree.max_size());
			}

			// Element access
			// k가 컨테이너에 있는 요소의 키와 일치하면 매핑된 값에 대한 참조를 반환하는 함수
			// k가 컨테이너에 있는 요소의 키와 일치하지 않으면 함수는 해당 키를 사용하여 새 요소를 삽입하고 매핑된 값에 대한 참조를 반환한다.
			// 이렇게 하면 요소에 매핑되니 값이 할당되지 않더라도(기본 생성자를 사용하여 생성됨) 항상 컨테이너 크기가 1씩 증가한다.
			// at과 비슷한 역할을 하지만 map::at은 키가 있는 요소가 존재할 때 같은 동작을 하지만 그렇지 않으면 에러를 발생시킨다. -> c++11에서 사용
			mapped_type& operator[](const key_type& k) {
				return (*(_tree.insert(ft::make_pair(k, mapped_type())).first)).value->second;
			}

			// Modifiers
			//1. single element
			//insert 성공 - pair(새로운 element를 가리키는 iterator, true) 반환.
			//insert 실패 - pair(val과 동일한 Key값 갖고있는 iterator, false) 반환.
			pair<iterator, bool> insert(const value_type& val) {
				ft::pair<node_type*, bool> res = _tree.insert(val);
				return (ft::make_pair(iterator(res.first), res.second));
			}

			//2. with hint
			//position은 hint, 강제가 아니다.
			//insert 성공 - 새로운 element를 가리키는 iterator 반환.
			//insert 실패 - val과 동일한 Key값 갖고있는 iterator 반환.
			iterator insert(iterator position, const value_type& val) {
				return (iterator(this->_tree.insert(val, position.base()).first));
			}

			//3. range
			//[first, last) 구간의 element를 insert.
			//이때, 기존 Key값과 중복되는 element는 추가하지 않는다.
			//중복되지 않는 element만 추가한다.
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) {
				while (first != last)
					this->_tree.insert(*first++);
			}

			void erase(iterator position) {
				this->_tree.erase(position.base());
			}

			//지워진 요소의 수를 반환
			//erase 성공 시 1, 실패 시 0 리턴.
			//tree에 없는 key_type k가 들어온 경우 find에서 nil노드를 리턴한다.
			//nil노드 erase실행 시 실패.
			size_type erase(const key_type& k) {
				return (this->_tree.erase(_tree.find(value_type(k, mapped_type()))));
			}

			void erase(iterator first, iterator last) {
				while (first != last)
					erase(first++);
			}

			// 같은 유형의 다른 맵인 x의 요소로 기존 맵 요소를 교환한다. 이 때, 사이즈가 다를 수 있다.
			// 동일한 이름의 swap을 가진 non-member function이 존재하며, 이 멤버 함수와 같이 동작하는 최적화로 알고리즘을 오버로드한다.
			void swap(map& x) {
				this->_tree.swap(x._tree);
			}

			// 모든 요소를 맵에서 제거한다. 컨테이너의 크기는 0이다.
			void clear() {
				this->_tree.clear();
			}

			//Observers
			// 컨테이너가 키를 비교하는데 사용하며 비교 객체의 복사본을 반환한다
			// key_comp가 키가 인수로 전달되는 순서에 관계없이 false를 반환하다면, 두 키는 동등한 것으로 간주한다.
			key_compare key_comp() const {
				return (key_compare());
			}

			// 두 개의 요소를 비교하여 첫 번째 요소의 키가 두 번쨰 요소보다 이전인지 확인하는데 사용할 수 있는 비교 개체(값)를 반환한다.
			// 반환된 비교 개체는 멤버 유형 map::value_compare의 개체로, 내부 비교 개체를 사용하여 적절한 비교 함수 클래스를 생성하는 중첩 클래스입니다.
			// 이 비교 클래스의 공용 구성원은 첫 번째 인수의 키가 두 번째 인수의 키보다 이전인 것으로 간주되면 true를 반환하고, 그렇지 않으면 false를 반환한다.
			// value_compare에는 공용 생성자가 없으므로 맵 멤버 외부에 있는 이 중첩 클래스에서 직접 개체를 만들 수 없습니다.
			value_compare value_comp() const {
				return (value_compare());
			}

			//Operations
			// 컨테이너에서 k에 해당하는 키를 사용하여 요소를 검색하고 발견된 경우 반복자를 반환,
			// 발견되지 않은 경우 반복자를 map::end로 반환
			// 컨테이너의 비교객체가 반사적으로 false를 반환하는 경우(요소가 인자로 전달되는 순서와 관계없이) 두 키는 동일한 것으로 간주
			// map::count를 사용하여 특정키가 존재하는지 확인할 수 있다.
			iterator find(const key_type& k) {
				return (iterator(this->_tree.find(value_type(k, mapped_type()))));
			}

			const_iterator find(const key_type& k) const {
				return (const_iterator(this->_tree.find(value_type(k, mapped_type()))));
			}

			// 컨테이너에서 k에 해당하는 키를 사용하여 요소를 검색하고 일치 항목 수를 반환한다.
			// 맵 컨테이너의 모든 요소가 고유하기 때문에 함수는 1(요소가 발견된 경우) 또는 0(요소가 발견되지 않은 경우)만 반환 가능
			// 컨테이너의 비교객체가 반사적으로 false를 반환하는 경우(요소가 인자로 전달되는 순서와 관계없이) 두 키는 동일한 것으로 간주
			size_type count(const key_type& k) const {
				if (this->_tree.find(value_type(k, mapped_type()))->value != NULL)
					return (1);
				else
					return (0);
			}

			/**
			 * @brief lower_bound & upper_bound
			 *
			 * lower_bound함수의 경우 컨테이너의 오른쪽 원소 중 기준 원소와 같거나 큰 값 중 가장 왼쪽에 있는 원소의 iterator값을 반환한다.
			 * upper_bound함수의 경우 컨테이너의 오른쪽 원소 중 기준 원소보다 큰 값중 가장 왼쪽에 있는 원소의 iterator값을 반환한다.
			 * 차이점은 같은 값을 포함하느냐 마느냐의 차이만 있다.
			 */
			// 키가 k보다 이전인 것으로 간주되지않는 컨테이너의 첫 번째 요소를 가리키는 반복자를 반환한다.

			// 함수는 내부 비교 객체(key_comp)를 사용하여 이를 결정하며,
			// 반복자를 key_comp(element_key, k)가 false를 반환하는 첫 번째 요소를 반환한다.

			// 맵 클래스가 기본 비교 타입(less)로 인스턴스화된 경우 함수는 k이상의 키를 가진 첫 번째 요소로 반복자를 반환한다.

			// lower_bound와 upper_bound는 같은 동작을 하지만, 맵에 val와 같은 키를 가진 요소가 포함되어 있는 경우를 제외하고
			// lower_bound는 그 요소를 가리키는 반복자를 반환
			// upper_bound는 다음 요소를 가리키는 반복자를 반환
			iterator lower_bound(const key_type& k) {
				return (iterator(this->_tree.lower_bound(value_type(k, mapped_type()))));
			}

			const_iterator lower_bound(const key_type& k) const {
				return (const_iterator(this->_tree.lower_bound(value_type(k, mapped_type()))));
			}

			// 키가 k 다음에 오는 것으로 간주되는 컨테이너의 첫 번째 요소를 가리키는 반복자를 반환한다.
			// 함수는 내부 비교 객체(key_comp)를 사용하여 이를 결정하며,
			// 반복자를 key_comp(element_key, k)가 true를 반환하는 첫 번째 요소를 반환한다.

			// 맵 클래스가 기본 비교 타입(less)로 인스턴스화된 경우 함수는 k보다 큰 첫 번째 요소로 반복자를 반환한다.

			// lower_bound와 upper_bound는 같은 동작을 하지만, 맵에 val와 같은 키를 가진 요소가 포함되어 있는 경우를 제외하고
			// lower_bound는 그 요소를 가리키는 반복자를 반환
			// upper_bound는 다음 요소를 가리키는 반복자를 반환
			iterator upper_bound(const key_type& k) {
				return (iterator(this->_tree.upper_bound(value_type(k, mapped_type()))));
			}
			const_iterator upper_bound(const key_type& k) const {
				return (const_iterator(this->_tree.upper_bound(value_type(k, mapped_type()))));
			}

			// k와 동등한 키를 가진 컨테이너의 모든 요소를 포함하는 범위의 경계를 반환한다.
			// 맵 컨테이너의 요소에는 고유한 키가 있으므로 반환되는 범위에는 최대 단일 요소가 포함된다.
			// 일치하는 요소가 없으면 반환되는 범위는 0이며,
			// 두 반복자는 컨테이너의 내부 비교 객체(key_comp)에 따라 k 이후인 것으로 간되는 키를 가진 첫 번째 요소를 가리킨다.
			// 컨테이너의 비교객체가 반사적으로 false를 반환하는 경우(요소가 인자로 전달되는 순서와 관계없이) 두 키는 동일한 것으로 간주
			pair<iterator, iterator> equal_range(const key_type& k) {
				return (ft::make_pair(lower_bound(k), upper_bound(k)));
			}
			pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
				return (ft::make_pair(lower_bound(k), upper_bound(k)));
			}

			// 맵과 연결된 allocator object의 복사본을 반환한다.
			allocator_type get_allocator() const {
				return (this->_alloc);
			}

			void showTree() {
				this->_tree.showMap();
			}

	}; // class map

	// Relational operators
	template <class Key, class T, class Compare, class Alloc>
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (!(rhs < lhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return (!(lhs < rhs));
	}

	// swap
	template <class Key, class T, class Compare, class Alloc>
	void swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y) {
		x.swap(y);
	}
} // namespace ft

#endif
