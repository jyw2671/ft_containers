#ifndef RB_TREE_NODE_HPP
# define RB_TREE_NODE_HPP

#include <memory>

namespace ft {
	enum RBColor { RED = false, BLACK = true };

	template < typename T, typename Alloc = std::allocator<T> >
	struct RBTreeNode {
	public :
		typedef T	value_type;
		typedef RBTreeNode*	node;

		value_type*	value;
		node	parent;
		node	leftChild;
		node	rightChild;
		RBColor	color;
		Alloc	alloc;

		//default
		RBTreeNode() : value(NULL), parent(NULL), leftChild(NULL), rightChild(NULL), color(BLACK), alloc(Alloc()) {}

		//initialization
		RBTreeNode(const T& val) : value(NULL), parent(NULL), leftChild(NULL), rightChild(NULL), color(RED), alloc(Alloc()) {
			value = alloc.allocate(1);
			alloc.construct(value, val);
		}

		//copy
		RBTreeNode(const RBTreeNode& copy) : value(NULL), parent(NULL), leftChild(NULL), rightChild(NULL), color(RED), alloc(Alloc()) {
			if (copy.value != NULL) {
				value = alloc.allocate(1);
				alloc.construct(value, *copy.value);
				color = copy.color;
			}
		}

		//destruct
		~RBTreeNode() {
			if (value != NULL) {
				alloc.destroy(value);
				alloc.deallocate(value, 1);
			}
		}

		bool operator==(const RBTreeNode& node) const {
			return (*this->value == *node->value);
		}

		bool operator!=(const RBTreeNode& node) const {
			return (*this->value != *node->value);
		}
	};
} // namespace ft

#endif
