#ifndef UTILS_HPP
# define UTILS_HPP

#include "iterator.hpp"

namespace ft{
	/*
	 * enable_if
	*/
	template <bool Cond, typename T = void>
	struct enable_if {};

	template <typename T>
	struct enable_if<true, T> {
		typedef T type;
	};
}	// namespace ft


#endif
