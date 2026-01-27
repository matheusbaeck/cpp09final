#include <iostream>
#include <cstdlib>
#include "PmergeMe.hpp"




int main(int argc, char **argv) 
{
	if (argc < 2) return 1;

	std::vector<int> vec;
	std::deque<int> deq;

	for (int i = 1; i < argc; ++i) 
	{
		int num = std::atoi(argv[i]);
		if (num <= 0) {
			std::cerr << "Error: Invalid input '" << argv[i] << "'. Positive integers only." << std::endl;
			return 1;
		}
		vec.push_back(num);
		deq.push_back(num);
	}

	std::cout << "Before: " << display(vec) << std::endl;;

	long long start_vec = get_time();
	std::vector<int> sorted_vec = fordJohnson(vec);
	long long end_vec = get_time();

	long long start_deq = get_time();
	std::deque<int> sorted_de1 = fordJohnson(deq);
	long long end_deq = get_time();

	std::cout << "After:  " << display(sorted_vec) << std::endl;;

	
	std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << (end_vec - start_vec) << " us" << std::endl;
	std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " << (end_deq - start_deq) << " us" << std::endl;

	return 0;
}