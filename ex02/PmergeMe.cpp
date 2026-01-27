


#include "PmergeMe.hpp"


std::vector<size_t> jacobsthalOrder(size_t n)
{
	std::vector<size_t> order;
	std::vector<size_t> jac;

	jac.push_back(0);
	jac.push_back(1);

	while (jac.back() < n)
		jac.push_back(jac[jac.size()-1] + 2 * jac[jac.size()-2]);

	for (size_t i = 1; i < jac.size(); ++i)
		for (size_t j = jac[i]; j > jac[i-1]; --j)
			if (j - 1 < n)
				order.push_back(j - 1);

	return order;
}

void binaryInsert(std::vector<int>& chain, int value, size_t maxPos)
{
	size_t left = 0;
	size_t right = maxPos;

	while (left < right)
	{
		size_t mid = (left + right) / 2;
		if (value < chain[mid])
			right = mid;
		else
			left = mid + 1;
	}

	chain.insert(chain.begin() + left, value);
}

std::vector<int> fordJohnson(std::vector<int>& input)
{
	if (input.size() <= 1)
		return input;

	int odd = -1;
	if (input.size() % 2)
	{
		odd = input.back();
		input.pop_back();
	}

	std::vector<Pair> pairs;
	pairs.reserve(input.size() / 2);

	for (size_t i = 0; i + 1 < input.size(); i += 2)
	{
		Pair p;
		if (input[i] < input[i + 1]) { p.small = input[i]; p.big = input[i+1]; }
		else { p.small = input[i+1]; p.big = input[i]; }
		pairs.push_back(p);
	}

	std::vector<int> bigs;
	bigs.reserve(pairs.size());
	for (size_t i = 0; i < pairs.size(); ++i)
		bigs.push_back(pairs[i].big);

	std::vector<int> chain = fordJohnson(bigs);  // recursion by reference could improve

	// Phase 3: insert smalls using Jacobsthal
	std::vector<size_t> order = jacobsthalOrder(pairs.size());
	chain.reserve(chain.size() + pairs.size()); // reserve space for smalls

	for (size_t k = 0; k < order.size(); ++k)
	{
		size_t i = order[k];
		int small = pairs[i].small;
		int big   = pairs[i].big;

		size_t pos = std::distance(chain.begin(), std::lower_bound(chain.begin(), chain.end(), big));
		binaryInsert(chain, small, pos);
	}

	if (odd != -1)
		binaryInsert(chain, odd, chain.size());

	return chain;
}

std::deque<size_t> jacobsthalOrderDeq(size_t n)
{
	std::deque<size_t> order;
	std::deque<size_t> jac;

	jac.push_back(0);
	jac.push_back(1);

	while (jac.back() < n)
		jac.push_back(jac.back() + 2 * *(jac.rbegin() + 1)); // jac[i] = jac[i-1] + 2*jac[i-2]

	for (size_t i = 1; i < jac.size(); ++i)
		for (size_t j = jac[i]; j > jac[i - 1]; --j)
			if (j - 1 < n)
				order.push_back(j - 1);

	return order;
}

void binaryInsert(std::deque<int>& chain, int value, size_t maxPos)
{
	size_t left = 0;
	size_t right = maxPos;

	while (left < right)
	{
		size_t mid = (left + right) / 2;
		if (value < chain[mid])
			right = mid;
		else
			left = mid + 1;
	}

	chain.insert(chain.begin() + left, value);
}


std::deque<int> fordJohnson(std::deque<int>& input)
{
	if (input.size() <= 1)
		return input;

	int odd = -1;
	if (input.size() % 2)
	{
		odd = input.back();
		input.pop_back();
	}

	std::vector<Pair> pairs;
	pairs.reserve(input.size() / 2);

	for (size_t i = 0; i + 1 < input.size(); i += 2)
	{
		Pair p;
		if (input[i] < input[i + 1]) { p.small = input[i]; p.big = input[i+1]; }
		else { p.small = input[i+1]; p.big = input[i]; }
		pairs.push_back(p);
	}

	std::deque<int> bigs;
	for (size_t i = 0; i < pairs.size(); ++i)
		bigs.push_back(pairs[i].big);

	std::deque<int> chain = fordJohnson(bigs);

	std::deque<size_t> order = jacobsthalOrderDeq(pairs.size());

	for (size_t k = 0; k < order.size(); ++k)
	{
		size_t i = order[k];
		int small = pairs[i].small;
		int big   = pairs[i].big;

		size_t pos = 0;
		while (chain[pos] != big)
			++pos;

		binaryInsert(chain, small, pos);
	}

	if (odd != -1)
		binaryInsert(chain, odd, chain.size());

	return chain;
}





long long get_time() 
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000LL) + tv.tv_usec;
}