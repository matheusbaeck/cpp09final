#ifndef PMERGEME_HPP
#define PMERGEME_HPP


#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>
#include <sstream>



struct Pair
{
	int small;
	int big;
};

std::vector<size_t> jacobsthalOrder(size_t n);
std::deque<size_t> jacobsthalOrderDeq(size_t n);

void binaryInsert(std::vector<int>& chain, int value, size_t maxPos);
void binaryInsert(std::deque<int>& chain, int value, size_t maxPos);

std::vector<int> fordJohnson(std::vector<int>& input);
std::deque<int> fordJohnson(std::deque<int>& input);

template <typename T>
std::string display(const T& container);

template <typename T>
std::string display(const T& container)
{
	std::stringstream ss;
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it) {
		ss << *it << " ";
	}
	return ss.str();
}

long long get_time();

#endif
