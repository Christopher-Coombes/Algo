#include "General/everything.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
	std::vector<int> test{ 0, 1, 5, 3, 2, 5 };
	std::sort(test.begin(), test.end());

	tester::linked_list::all(std::cout);

	return 0;
}
