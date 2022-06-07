#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

#include "utils.hpp"
#include "RB_Tree_node.hpp"

namespace ft {
	template <typename T, typename Pointer = T*, typename Reference = T&>
	class RBTreeIterator : public ft::iterator<ft::bidirectional_iterator_tag, T> {
		public :
			typedef const T		value_type;
			typedef Pointer		pointer;
			typedef Reference	reference;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::difference_type	difference_type;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::iterator_category	iterator_category;
			typedef ft::RBTreeNode<T> node_type;
		protected :
			node_type* _node;

		public:
			// Construct a new RBTreeIterator object
			// Default constructor
			RBTreeIterator(node_type* ptr = NULL) : _node(ptr) {}
			// Copy constructor
			RBTreeIterator(const RBTreeIterator<T>& copy) : _node(copy.base()) {}
			// Assignment operator
			RBTreeIterator& operator=(const RBTreeIterator& copy) {
				if (this != &copy)
					this->_node = copy.base();
				return (*this);
			}
			// Destructor
			virtual ~RBTreeIterator() {}
			// Getter
			node_type* const &base() const {
				return (this->_node);
			}

			// Operators
			reference operator*() const {
				return (*this->_node->value);
			}
			pointer operator->() const {
				return (this->_node->value);
			}
			RBTreeIterator& operator++() {
				node_type* tmp = NULL;
				// if rightChild exists,
				if (_node->rightChild->value != NULL) {
					tmp = _node->rightChild;
					// search the leftmost of the rightChild.
					while (tmp->leftChild->value != NULL)
						tmp = tmp->leftChild;
				}
				else {	// if rightChild doesn't exist,
					tmp = _node->parent;
					if (tmp->rightChild == _node)
					{	// if current node is rightChild,
						while (tmp->parent->rightChild == tmp)
							tmp = tmp->parent;
						tmp = tmp->parent;
					}
				}
				_node = tmp;
				return (*this);
			}
			RBTreeIterator operator++(int) {
				RBTreeIterator tmp = *this;
				++(*this);
				return (tmp);
			}
			RBTreeIterator& operator--() {
				node_type* tmp = NULL;
				if (_node->value == NULL)
					tmp = _node->parent;
				// if leftChild exists,
				else if (_node->leftChild->value != NULL) {
					tmp = _node->leftChild;
					// search the rightmost of the leftChild.
					while (tmp->rightChild->value != NULL)
						tmp = tmp->rightChild;
				}
				else {	// if leftChild doesn't exist,
					tmp = _node->parent;
					// if current node is leftChild,
					if (tmp->leftChild == _node) {
						while (tmp->parent->leftChild == tmp)
							tmp = tmp->parent;
						tmp = tmp->parent;
					}
				}
				_node = tmp;
				return (*this);
			}
			RBTreeIterator operator--(int) {
				RBTreeIterator tmp = *this;
				--(*this);
				return (tmp);
			}

			// Relational operators
			bool operator==(const RBTreeIterator& iter) {
				return (_node == iter.base());
			}
			bool operator!=(const RBTreeIterator& iter) {
				return (_node != iter.base());
			}
	};
} // namespace ft

#endif
