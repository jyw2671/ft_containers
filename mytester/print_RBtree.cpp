#include <iostream>
#include <list>
#include <map>
#include <string>

#include "map.hpp"


#define T1 int
#define T2 std::string
#define T3 ft::map< T1, T2 >::value_type

int main() {
	// <int, string> fair map test
	ft::map<int, std::string> m;
	ft::map<int, std::string>::iterator i;

	m.insert( ft::map<int, std::string>::value_type(1, "Hello") );
	m.insert( ft::map<int, std::string>::value_type(2, "World") );
  m.insert( ft::map<int, std::string>::value_type(8, "World") );
  m.insert( ft::map<int, std::string>::value_type(20, "World") );
  m.insert( ft::map<int, std::string>::value_type(3, "World") );
  m.insert( ft::map<int, std::string>::value_type(4, "World") );
  m.insert( ft::map<int, std::string>::value_type(22, "World") );

	// map 순회
	for(i = m.begin(); i != m.end(); i++){
		std::cout << "[" << i->first << "] " << i->second << std::endl;
	}

  m.showTree();
  std::cout << "------------------------" << std::endl;

  std::list< T3 > lst;
  unsigned int lst_size = 10;
  for (unsigned int i = 0; i < lst_size; ++i) {
    lst.push_back(T3(i + 1, std::string(i + 1, i + 65)));
  }
  ft::map< T1, T2 > mp(lst.begin(), lst.end());

  mp.showTree();
	system("leaks map");
}
