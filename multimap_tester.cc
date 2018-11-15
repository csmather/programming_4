#include <algorithm>
#include <iostream>
#include <vector>

#include "llrb_multimap.h"

// Tester
int main() {
    LLRB_multimap<int, int> set;
    std::vector<int> keys{2, 18, 42, 43, 51, 54, 74, 93, 99};

    // Insert a bunch of keys
    for (auto i : keys) {
        set.Insert(i, i + 1);
        set.Insert(i, i + 2);
    }
    std::cout << "After insertions:" << std::endl;
    set.Print();

    std::cout << "Get 43" << std::endl;
    std::cout << set.Get(43) << std::endl;
    // Delete all the keys in another order
    std::random_shuffle(keys.begin(), keys.end());
    for (auto i : keys) {
        set.Remove(i);
    }
    std::cout << "After deletions:" << std::endl;
    set.Print();

    return 0;
}
