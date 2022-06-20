#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <stdexcept>
#include "RB_Tree_iterator.hpp"
#include "printMap.hpp"

namespace ft {
	template < typename T, typename Compare = ft::less<T>, typename Alloc = std::allocator<T> >
	class RBTree {
		public :
			typedef T	value_type;
			typedef T*	pointer;
			typedef const T*	const_pointer;
			typedef T&	reference;
			typedef const T&	const_reference;
			typedef Compare	value_comp;
			typedef Alloc	allocator_type;
			typedef size_t	size_type;
			typedef ft::RBTreeNode<T>	node_type;
			typedef typename ft::RBTreeIterator<T, T*, T&>	iterator;
			typedef typename ft::RBTreeIterator<T, const T*, const T&>	const_iterator;
			typedef typename Alloc::template rebind<node_type>::other	node_allocator_type;
			// rebind - T타입이 아닌 다른 타입에 대한 allocate가 필요

		private:
			node_type*	_root;
			node_type*	_nil;
			size_type	_size;
			value_comp	_comp;
			node_allocator_type	_node_alloc;


		public:
			//Member functions
			//Default constructor
			RBTree() : _root(NULL), _nil(NULL), _size(0), _comp(value_comp()), _node_alloc(node_allocator_type()) {
				this->_nil = make_nil();
				this->_root = this->_nil;
			}

			//Copy constructor
			RBTree(const RBTree& x) : _root(NULL), _nil(NULL), _size(0), _comp(value_comp()), _node_alloc(node_allocator_type()) {
				this->_nil = make_nil();
				copy(x);
				this->_nil->parent = get_max_value_node();
			}

			//Destructor
			~RBTree() {
				clear();
				_node_alloc.destroy(this->_nil);
				_node_alloc.deallocate(this->_nil, 1);
			}

			//Assignment operator
			RBTree& operator=(const RBTree& x) {
				if (this != &x)
					copy(x);
				return (*this);
			}

			void copy(const RBTree& x) {
				clear();
				copy(x._root);
			}

			void copy(node_type* node) {
				if (node->value == NULL)
					return ;
				insert(*node->value);
				if (node->leftChild->value != NULL)
					copy(node->leftChild);
				if (node->rightChild->value != NULL)
					copy(node->rightChild);
			}

			//Iterators
			//가장 작은 값을 찾는다.
			node_type* get_begin() const {
				node_type* tmp = this->_root;
				while (tmp->leftChild->value != NULL)
					tmp = tmp->leftChild;
				return (tmp);
			}

			//nil노드를 가리키게 한다.
			node_type* get_end() const {
				return (this->_nil);
			}

			//Capacity
			bool empty() const {
				return (this->_size == 0);
			}

			size_type size() const {
				return (this->_size);
			}

			size_type max_size() const {
				return (_node_alloc.max_size());
			}

			ft::pair<node_type*, bool> insert(const value_type& val, node_type* hint = NULL) {
				//val 값을 인자로 입력하여 노드를 생성한다.
				node_type* new_node = make_node(val);
				//노드가 삽일될 위치를 탐색한다. tree가 비어있을 경우를 대비해 초기 위치를 root로 설정한다.
				node_type* position = this->_root;
				//tree가 비어있을 경우, 생성한 노드(new_node)를 root로 지정한다.
				if (this->_size == 0) {
					this->_root = new_node;
					this->_root->leftChild = this->_nil;
					this->_root->rightChild = this->_nil;
					this->_root->parent = this->_nil; //여기서 중요한 점이 root의 부모도 nil노드를 가리키게 설정
					this->_root->color = BLACK;
					this->_nil->parent = this->_root; //다시 nil의 부모를 root로 설정
					this->_size++;
					return (ft::make_pair(this->_root, true)); //새로 만든
				}
				//hint의 위치가 유효한지 확인한다.
				//single element의 경우 hint는 null
				if (hint != NULL && hint->value != NULL)
					position = check_hint(val, hint);
				//노드를 삽입할 위치를 탐색한다.
				//leftchild와 rightchild에 삽입을 실패하면 false를 반환
				ft::pair<node_type*, bool> is_valid = get_position(position, new_node);
				if (is_valid.second == false) {
					_node_alloc.destroy(new_node);
					_node_alloc.deallocate(new_node, 1);
					return (is_valid);
				}
				//new_node 삽입 후 rbtree의 규칙(속성)에 따라 균형을 잡아야한다.
				//이는 insert_case에 따라 rotate를 통해 진행한다.
				insert_case1(new_node);
				this->_size++;
				this->_nil->parent = get_max_value_node();
				return (ft::make_pair(new_node, true));
			}

			size_type erase(node_type* node) {
				//삭제할 노드가 nil 노드인 경우 0을 반환 -> map에서 삭제가 실패한 경우 0을 반환
				if (node->value == NULL)
					return (0);
				//node의 왼쪽 서브트리에서 최댓값 / 오른쪽 서브트리에서 최솟값을 찾은 후 위치를 변경한다.
				//기존 target위치에는 대체할 node가 들어가있다.
				//target 노드 자체를 삭제해야 한다.
				//node와 target의 값을 바꾸고 target을 리턴받음.
				//위치변경 후 target은 child에 non-nil 노드가 최대 1개이다.
				//child는 target노드의 non-nil child가 우선이다.
				node_type* target = replace_erase_node(node);
				node_type* child;
				if (target->rightChild->value == NULL)
					child = target->leftChild;
				else
					child = target->rightChild;
				//1) target이 RED인 경우, 무조건 그 자식 노드들이 nil일 때만 발생한다(BLACK). target을 nil로 바꾸면 해결
				replace_node(target, child);
				if (target->color == BLACK) {
					//2) target이 BLACK이고 child가 RED인 경우,
					//target과 child의 색을 바꾸고 child의 색을 BLACK으로 바꾼다.
					if (child->color == RED)
						child->color = BLACK;
					else
						delete_case1(child);
					//3) target과 child가 모두 BLACK인 경우, child는 무조건 nil이었을 것이다.
					//두 개의 nil노드를 가지고 있는 검은 노드를 지우는 상황에서만 발생
					//사실상 target노드의 두 자식은 모두 nil이다. -> child노드도 nil
					//replace_node에서 child(nil)->parent를 상황에 맞게 설정
				}
				this->_size--;
				if (target->parent->value == NULL)
					this->_root = this->_nil;
				delete target;
				this->_nil->parent = get_max_value_node();
				return (1);
			}

			void swap(RBTree& x) {
				swap(_root, x._root);
				swap(_nil, x._nil);
				swap(_comp, x._comp);
				swap(_node_alloc, x._node_alloc);
				swap(_size, x._size);
			}

			void clear(node_type* node = NULL) {
				if (node == NULL)
					node = this->_root;
				if (node->leftChild->value != NULL) {
					clear(node->leftChild);
					node->leftChild = this->_nil;
				}
				if (node->rightChild->value != NULL) {
					clear(node->rightChild);
					node->rightChild = this->_nil;
				}
				// delete
				if (node->value != NULL) {
					if (node == this->_root)
						this->_root = this->_nil;
					_node_alloc.destroy(node);
					_node_alloc.deallocate(node, 1);
					this->_size--;
				}
			}

			//Operations
			node_type* find(value_type val) const {
				node_type* res = this->_root;
				if (this->_size == 0)
					return (this->_nil);
				while (res->value != NULL && (_comp(val, *res->value) || _comp(*res->value, val))) {
					if (_comp(val, *res->value))
						res = res->leftChild;
					else
						res = res->rightChild;
				}
				return (res);
			}

			// 같은 동작을 하지만, 맵에 va와 같은 키를 가진 요소가 포함되어 있는 경우를 제외하고
			// lower_bound는 그 요소를 가리키는 반복자를 반환
			// upper_bound는 다음 요소를 가리키는 반복자를 반환
			// val보다 크거나 같은 범위를 구하기 위함.
			node_type* lower_bound(const value_type& val) const {
				iterator it(get_begin());
				iterator ite(get_end());
				while (it != ite && _comp(*it, val))
					it++;
				return (it.base());
			}

			//val보다 큰 범위를 구하는 함수
			node_type* upper_bound(const value_type& val) const {
				iterator it(get_begin());
				iterator ite(get_end());
				while (it != ite && !_comp(val, *it))
					it++;
				return (it.base());
			}

			//test end print map function
			void showMap() { ft::printMap(_root, 0); }

		private :
			//노드의 조상노드을 반환한다.
			node_type* get_grandparent(node_type* node) const {
				if (node != NULL && node->parent != NULL)
					return (node->parent->parent);
				else
					return (NULL);
			}

			//노드의 삼촌노드를 반환한다.
			node_type* get_uncle(node_type* node) const {
				node_type* grand = get_grandparent(node);
				if (grand == NULL)
					return (NULL);
				if (grand->leftChild == node->parent)
					return (grand->rightChild);
				else
					return (grand->leftChild);
			}

			//노드의 형제노드를 반환한다.
			node_type* get_sibling(node_type* node) const {
				if (node == node->parent->leftChild)
					return (node->parent->rightChild);
				else
					return (node->parent->leftChild);
			}

			//tree에서 가장 큰 값을 가지는 노드를 찾는다.
			//tree에서 가장 오른쪽에 있는 값이 가장 큰 값이다.
			node_type* get_max_value_node() const {
				node_type* tmp = _root;
				while (tmp->rightChild->value != NULL)
					tmp = tmp->rightChild;
				return (tmp);
			}

			//nil 노드를 만든다.
			//아무런 값이 없는 노드, tree의 leat노드이다.
			node_type* make_nil() {
				node_type* res = _node_alloc.allocate(1);
				_node_alloc.construct(res, node_type());
				res->color = BLACK;
				res->leftChild = res;
				res->rightChild = res;
				res->parent = res;
				res->value = NULL;
				return (res);
			}

			//value 값을 가지는 노드를 만든다.
			//노드의 색/자식/부모는 삽입 후 tree의 속성에 맞게 재조정 후 결정한다.
			node_type* make_node(const value_type& val) {
				node_type* res = _node_alloc.allocate(1);
				_node_alloc.construct(res, node_type(val));
				return (res);
			}

			/**
			 * Hint 쓰는 경우. (hint가 적절한 위치인 경우)
			 * inserted value는 hint node의 right-sub-tree로 들어간다.
			 * 공통 조건 :  hint < inserted value 인 경우.
			 * 1) hint가 leftChild인 경우, inserted value < hint-parent 이면, hint부터 탐색.
			 * 2) hint가 rightChild인 경우, parent를 따라가다가 처음으로 leftChild인  d노드의 parent보다 작으면 hint부터 탐색.
			 */
			node_type* check_hint(value_type val, node_type* hint) {
				if (_comp(*hint->value, *_root->value) && _comp(val, *hint->value))
					return (hint);
				else if (_comp(*hint->value, *_root->value) && _comp(*hint->value, val))
					return (_root);
				else if (_comp(*_root->value, *hint->value) && _comp(val, *hint->value))
					return (_root);
				else if (_comp(*_root->value, *hint->value) && _comp(*hint->value, val))
					return (hint);
				else
					return (_root);
			}

			//노드를 삽입할 위치를 탐색하는 함수이다.
			//make_pair로 한 쌍의 pair를 만든 후 삽입이 가능한지 true/false를 반환한다.
			ft::pair<node_type*, bool> get_position(node_type* position, node_type* node) {
				while (position->value != NULL) {
					//position을 기준으로 leftchild로 들어감
					if (_comp(*node->value, *position->value)) {
						if (position->leftChild->value == NULL) {
							position->leftChild = node;
							node->parent = position;
							node->leftChild = _nil;
							node->rightChild = _nil;
							node->color = RED;
							break;
						}
						else
							position = position->leftChild;
					}
					//position을 기준으로 rightchild로 들어감
					else if (_comp(*position->value, *node->value)) {
						if (position->rightChild->value == NULL) {
							position->rightChild = node;
							node->parent = position;
							node->leftChild = _nil;
							node->rightChild = _nil;
							node->color = RED;
							break;
						}
						else
							position = position->rightChild;
					}
					else
						return (ft::make_pair(position, false));
				}
				return (ft::make_pair(position, true));
			}

			/**
			 * 이진 탐색 트리에서 삭제를 수행할 때에는 왼쪽 서브트리에서의 최댓값이나,
			 * 오른쪽 서브트리에서의 최솟값을 삭제한 노드의 위치에 삽입한다는 것.
			 * 삭제한 노드를 대체할 노드에는 반드시 1개의 자식 노드만 있다는 점이다.
			 * 그 이유는 즉슨, 자식 2개를 보유한 노드일 경우,
			 * 왼쪽 자식 < 대체 노드 < 오른쪽 자식이라는 결론이 도출되므로, 자식 2개를 보유할 가능성은 절대적으로 0이라는 것이다.
			 *
			 * ->node의 leftChild가 있으면, 왼쪽 서브트리에서 최댓값,
			 * ->node의 leftChild가 없으면, 오른쪽 서브트리에서 최솟값을 찾는다.
			 * 찾은 값의 value를 node에 복사하고, 찾은 그 노드는 삭제해야 하므로 리턴한다.
			 */
			node_type* replace_erase_node(node_type* node) {
				node_type* res;
				if (node->leftChild->value != NULL) {
					res = node->leftChild;
					while (res->rightChild->value != NULL)
						res = res->rightChild;
				}
				else if (node->rightChild->value != NULL) {
					res = node->rightChild;
					while (res->leftChild->value != NULL)
						res = res->leftChild;
				}
				else
					return (node);

				node_type* tmp_parent = node->parent;
				node_type* tmp_left = node->leftChild;
				node_type* tmp_right = node->rightChild;
				RBColor tmp_color = node->color;

				//node의 left/rightChild 설정
				node->leftChild = res->leftChild;
				if (res->leftChild->value != NULL)
					res->leftChild->parent = node;
				node->rightChild = res->rightChild;
				if (res->rightChild->value != NULL)
					res->rightChild->parent = node;

				//res를 node->parent의 left/rightChild로 설정
				if (tmp_parent->leftChild == node)
					tmp_parent->leftChild = res;
				else if (tmp_parent->rightChild == node)
					tmp_parent->rightChild = res;

				if (res == tmp_left) {
					//res의 형제를 res의 left/rightChild로 연결
					tmp_right->parent = res;
					res->rightChild = tmp_right;
					//node를 res의 left/rightChild로 연결
					node->parent = res;
					res->leftChild = node;
				}
				else if (res == tmp_right) {
					tmp_left->parent = res;
					res->leftChild = tmp_left;
					node->parent = res;
					res->rightChild = node;
				}
				else {
					//res와 node가 멀리 떨어진 경우
					tmp_left->parent = res;
					res->leftChild = tmp_left;
					tmp_right->parent = res;
					res->rightChild = tmp_right;
					node->parent = res->parent;
					res->parent->rightChild = node;
				}

				//res의 parent 연결
				res->parent = tmp_parent;

				if (res->parent->value == NULL)
					this->_root = res;
				node->color = res->color;
				res->color = tmp_color;

				return (node);
			}

			void replace_node(node_type* node, node_type* child) {
				//노드의 부모가 NULL이 되는 경우를 delete_case에 오지 않게 미리 처리할 수 있다.
				child->parent = node->parent;
				if (node->parent->leftChild == node)
					node->parent->leftChild = child;
				else// if (node->parent->rightChild == node)
					node->parent->rightChild = child;
			}

			//rotate
			//child가 node의 오른쪽 자식일 경우 rotate_left를 한다.
			void rotate_left(node_type* node) {
				node_type* child = node->rightChild;
				node_type* parent = node->parent;
				//node를 기준으로 왼쪽으로 회전하는 경우
				if (child->leftChild->value != NULL)
					child->leftChild->parent = node;
				node->rightChild = child->leftChild;
				node->parent = child;
				child->leftChild = node;
				child->parent = parent;
				//node가 부모의 왼쪽 자식인지 오른쪽 자식인지 판단.
				if (parent->value != NULL) {
					if (parent->leftChild == node)
						parent->leftChild = child;
					else
						parent->rightChild = child;
				}
				else
					this->_root = child;
			}

			//child가 node의 오른쪽 자식일 경우 rotate_left를 한다.
			void rotate_right(node_type* node) {
				node_type* child = node->leftChild;
				node_type* parent = node->parent;
				if (child->rightChild->value != NULL)
					child->rightChild->parent = node;
				node->leftChild = child->rightChild;
				node->parent = child;
				child->rightChild = node;
				child->parent = parent;
				if (parent->value != NULL) {
					if (parent->rightChild == node)
						parent->rightChild = child;
					else
						parent->leftChild = child;
				}
				else
					this->_root = child;
			}

			// 삽입된 새로운 노드가 root노드가 아닌 경우
			void insert_case1(node_type* node) {
				if (node->parent->value != NULL)
					insert_case2(node);
				else
					node->color = BLACK;
			}

			// 새로운 노드의 부모 노드가 black이라면,
			// 새로운 노드가 black/red 상관없이 rbtree 속성이 유효하다.
			// 삽입된 새로운 노드의 부모 노드가 red일 때, 문제가 발생할 수 있다.
			// 삽입되는 새로운 노드의 색은 항상 red
			void insert_case2(node_type* node) {
				if (node->parent->color == RED)
					insert_case3(node);
			}

			// 삽입된 새로운 노드의 부모 및 삼촌 노드가 모두 red인 경우 rbtree의 5번 속성을 위반한다.
			// 이를 해결하기 위해, 노드의 부모와 삼촌 노드의 색을 black으로 바꾸고 조상 노드의 색을 red로 바꾼다.
			// ->rbtree가 5번 속성을 만족하고 두 자녀가 같은 색을 가질 때, 부모와 두 자녀의 색을 바꿔줘도 5번 속성은 여전히 만족한다.
			// 이 경우 조상 노드가 2/4번 속성을 만족하지 않을 수 있다.
			// 이를 해결하기 위해 insert_case1~3 까지 재귀적으로 활용한다.
			// -> 이 작업은 삽입과정 중 발생하는 유일한 재귀 호출이며, 회전을 하기 전에 적용해야한다.
			void insert_case3(node_type* node) {
				node_type* uncle = get_uncle(node);
				node_type* grand;
				if (uncle->value != NULL && uncle->color == RED) {
					node->parent->color = BLACK;
					uncle->color = BLACK;
					grand = get_grandparent(node);
					grand->color = RED;
					insert_case1(grand);
				}
				else
					insert_case4(node);
			}

			// 삽입한 새로운 노드의 부모 노드가 red이고 삼촌 노드이 blaak이며,
			// 1)새로운 노드는 부모 노드의 오른쪽 자식이며, 부모 노드는 조상 노드의 왼쪽 자식인 경우
			// ->부모 노드와 새로운 노드의 역할을 변경하기 위해 부모를 기준으로 왼쪽 회전을 한다.
			// 2)새로운 노드는 부모 노드의 왼쪽 자식이며, 부모 노드는 조상 노드의 오른쪽 자식인 경우
			// ->부모 노드와 새로운 노드의 역할을 변경하기 위해 부모를 기준으로 오른쪽 회전을 한다.

			// insert_case4를 통해 rotate를 한 후 부모 노드를 insert_case5에서 처리하게 된다.
			// -> 4번 속성을 만족하기 않았기 떄문
			// If new_node's parent is red and uncle is black,
			void insert_case4(node_type* node) {
				node_type* grand = get_grandparent(node);
				// new_node is parent's rightChild and parent is grand's leftChild,
				if (node == node->parent->rightChild && node->parent == grand->leftChild) {
					rotate_left(node->parent);
					node = node->leftChild;
				} // new_node is parent's leftChild and parent is grand's rightChild,
				else if (node == node->parent->leftChild && node->parent == grand->rightChild) {
					rotate_right(node->parent);
					node = node->rightChild;
				}
				insert_case5(node);
			}

			// 부모 노드가 red, 삼촌 노드가 black, 새로운 노드는 부모의 왼쪽 자식, 부모 노드가 조상 노드의 왼쪽 자식인 경우
			// 조상 노드를 기준으로 오른쪽 회전을 한다.
			// -> 회전 후 기존 부모 노드는 자식 노드로 새로운 노드와 기존 조상 노드를 가진다.
			// -> 부모 노드가 red, 조상 노드가 black이므로 둘의 색을 바꾸면 4번 속성을 만족한다.
			// 5번 속성이 유지되는 이유는 부모 노드를 포함하는 경로는 모드 조상 노드를 지나게 되고,
			// 바꾼 후 조상 노드를 포함하는 경로는 모두 부모 노드를 지나기 때문이다.
			void insert_case5(node_type* node) {
				node_type* grand = get_grandparent(node);
				node->parent->color = BLACK;
				grand->color = RED;
				if (node == node->parent->leftChild)
					rotate_right(grand);
				else
					rotate_left(grand);
			}


			// 2번 속성을 위반한 case
			// 인자로 넘어온 node는 삭제할 노드와 삭제할 노드의 자식을 치환 후, 삭제할 노드의 부모가 된 삭제할 노드의 자식 노드이다.
			// 치환 후 자식 노드의 부모가 없을 경우, 자식 노드가 root가 되므로 삭제할 노드를 그냥 삭제하면 된다.

			// 이 경우가 아닌 경우, delete_case2로 넘어간다.
			void delete_case1(node_type* node) {
				if (node->parent->value != NULL)
					delete_case2(node);
			}

			// node(치환한 자식 노드)의 형제 노드가 red인 case
			// ->부모의 자식인 형제 노드가 red이므로 부모 노드는 black이다.

			// ->부모 노드와 형제 노드의 색을 바꾸고
			// ->부모 노드를 기준으로 왼쪽으로 회전하면 자식 노드의 조상 노드는 형제 노드가 된다.

			// 아직 5번 속성을 만족하지 않으며,
			// black인 자식 노드와 red인 부모 노드를 가지고 있으므로 delete_case4,5,6(case2,3,4)을 진행한다.
			// 새로운 형제 노드는 red였던 형제 노드(조상 노드)의 자식 노드였으므로 black이다.
			// (red의 자식은 black이라는 속성)
			void delete_case2(node_type* node) {
				node_type* sibling = get_sibling(node);
				if (sibling->color == RED) {
					node->parent->color = RED;
					sibling->color = BLACK;
					if (node == node->parent->leftChild)
						rotate_left(node->parent);
					else
						rotate_right(node->parent);
				}
				delete_case3(node);
			}

			// delete_case2를 통과하면 자식 노드와 형제 노드는 반드시 black이 된다.
			// 통과 후 나오는 경우의 수는 아래의 case에서 해결 가능하다.
			// 부모, 형제의 왼쪽, 형제의 오른쪽 = B,B,B -> case3
			// 부모, 형제의 왼쪽, 형제의 오른쪽 = R,B,B -> case4
			// 부모, 형제의 왼쪽, 형제의 오른쪽 = B,R,B / R,R,B -> case5
			// 부모, 형제의 왼쪽, 형제의 오른쪽 = B,B,R / R,B,R / B,R,R / R,R,R -> case6

			// delete_case3,4
			// 형제 노드, 형제 노드의 자식 전부 black인 경우에서
			// 부모 노드가 red, black인 경우 나눠서 생각

			// delete_case3 -> case2
			// 부모 노드와 형제 노드, 형제 노드의 자식이 black인 case.

			// 형제 노드를 레드로 색상 변환하면 되지만,
			// 블랙 노드가 하나 부족한 것이 부모 노드로 전이된다.

			// -> 간단히 형제 노드를 red로 바꿔주기만 하면 된다.
			// -> 그러면 형제 노드를 지나는 모든 경로들은 하나의 black node를 적게 가지게 된다.
			// -> 이는 삭제할 노드를 삭제하는 과정에서 그 자식 노드가 지나는 모든 경로가 하나 줄어들게 되므로 양쪽은 같은 수의 black node경로를 가지게 된다.
			// -> 그러나 부모 노드를 지나는 모든 경로는 부모 노드를 지나지 않는 모든 경로에 대해 black노드를 하나 덜 가지게 되어 5번 속성을 위반하게 된다.
			// -> 이를 해결하기위해 delete_case1부터 시작하는 rebalancing 과정을 수행해야 한다.
			void delete_case3(node_type* node) {
				node_type* sibling = get_sibling(node);
				if (node->parent->color == BLACK && sibling->color == BLACK && sibling->leftChild->color == BLACK && sibling->rightChild->color == BLACK) {
					sibling->color = RED;
					delete_case1(node->parent);
				}
				else
					delete_case4(node);
			}


			// delete_case4 -> case2
			// 형제 노드와 형제 노드의 자식은 black, 부모 노드는 red인 case

			// 삭제하려는 노드를 삭제하게 되면,
			// 부모 노드 기준에서 좌측과 우측의 블랙 노드 개수가 맞지 않게 된다.
			// 이 때는 형제 노드를 레드로 색상 변환하고 부모 노드는 블랙으로 바꾸면 된다.
			// -> 부모 노드와 형제 노드의 색을 바꿔주면 된다.
			// -> 형제 노드를 지나는 경로의 black수는 영향을 주지않지만,
			// -> 자식 노드를 지나는 경로에 대해서 black수를 1증가 시칸다.
			void delete_case4(node_type* node) {
				node_type* sibling = get_sibling(node);
				if (node->parent->color == RED && sibling->color == BLACK && sibling->leftChild->color == BLACK && sibling->rightChild->color == BLACK) {
					sibling->color = RED;
					node->parent->color = BLACK;
				}
				else
					delete_case5(node);
			}

			// delete_case5 -> 위의 설명한 case3

			// 형제 노드가 black, 형제 노드의 (왼쪽) 자식이 red, (오른쪽) 자식이 black, 형제 노드가 부모의 오른쪽 자식인 case

			// 색 red를 형제 노드의 오른쪽으로 옮겨 case4, delete_cas6를 적용하여 해결
			// -> 형제 노드와 형제 노드의 왼쪽 자식과 색을 바꾼 후 형제 노드를 기준으로 오른쪽으로 회전
			// -> 형제 노드를 오른쪽 회전 후 형제 노드의 왼쪽 자식을 형제 노드 자신의 부모 노드이자, 새로운 형제 노드로 만든다.
			// -> 기존 형제 노드의 색을 부모 노드(기존의 형제 노드의 왼쪽 자식)의 색과 바꾼다.
			// -> delete_case6를 적용하여 해결
			void delete_case5(node_type* node) {
				node_type* sibling = get_sibling(node);

				if (sibling->color == BLACK) {
					if (node == node->parent->leftChild && sibling->rightChild->color == BLACK && sibling->leftChild->color == RED) {
						sibling->color = RED;
						sibling->leftChild->color = BLACK;
						rotate_right(sibling);
					}
					else if (node == node->parent->rightChild && sibling->leftChild->color == BLACK && sibling->rightChild->color == RED) {
						sibling->color = RED;
						sibling->rightChild->color = BLACK;
						rotate_left(sibling);
					}
				}
				delete_case6(node);
			}

			// delete_case6 -> 위의 설명한 case4

			// 형제 노드가 black, 형제 노드의 (오른쪽) 자식이 red, 형제 노드가 부모의 (오른쪽) 자식인 case
			// -> 삭제하려는 노드의 형제 노드는 블랙이고 형제 노드의 오른쪽 자식이 레드일 때

			// 색 red를 자식 노드의 위로 옮긴 후 red-and-black을 만들어 제거
			// -> 형제 노드의 색을 부모 노드의 색으로, 형제의 오른쪽 자식을 black으로, 부모는 black으로 바꾼 후, 부모를 기준으로 왼쪽으로 회전
			// -> 부모 노드를 기준으로 왼쪽 회전 후 형제 노드가 부모 노드의 부모가 되게 한다.
			// -> 그 후 부모 노드와 형제 노드의 색을 바꾸고, 형제 노드의 오른쪽 자식을 black으로 바꾼다.

			// 결과론적인 방법
			// (오른쪽) 형제는 부모의 색으로, (오른쪽) 형제의 (오른쪽) 자녀는 black으로 부모는 black으로 바꾼 후에 부모를 기준으로 (왼쪽)으로 회전하여 해결
			void delete_case6(node_type* node) {
				node_type* sibling = get_sibling(node);
				sibling->color = node->parent->color;
				node->parent->color = BLACK;
				if (node == node->parent->leftChild) {
					sibling->rightChild->color = BLACK;
					rotate_left(node->parent);
				}
				else {
					sibling->leftChild->color = BLACK;
					rotate_right(node->parent);
				}
			}


			template <typename _T>
			void swap(_T& a, _T& b) {
				_T tmp(a);
				a = b;
				b = tmp;
			}
	}; // class RB_Tree
} // namespace ft

#endif
