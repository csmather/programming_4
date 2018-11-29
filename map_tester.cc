#include <algorithm>
#include <iostream>
#include <vector>

#include "llrb_map.h"

// Tester
int main() {
	LLRB_map<int, int> set;
	std::vector<int> keys{2, 18, 42, 43, 51, 54, 74, 93, 99};

	// Try to use Get() on empty tree
	try {
		std::cout << "Get 1 from empty map: "
		          << set.Get(1) << std::endl;
	} catch (std::exception &e){
		std::cerr << "Error: empty map" << std::endl;
	}

	// Insert a bunch of keys
	for (auto i : keys) {
		// Value for each key is (key + 1)
		set.Insert(i, i + 1);
	}
	std::cout << "After insertions:" << std::endl;
	set.Print();

	std::cout << "Get 43:" << std::endl;
	std::cout << set.Get(43) << std::endl;
	std::cout << "Get 99:" << std::endl;
	std::cout << set.Get(99) << std::endl;
	
	// Remove all the keys in random order
	std::random_shuffle(keys.begin(), keys.end());
	for (auto i : keys) {
		set.Remove(i);
	}
	std::cout << "After deletions:" << std::endl;
	set.Print();

	return 0;
}
