#include "RPN.hpp"

bool isOnlyDigit(char *str)
{
	int i = -1;

	while(str[++i]) {
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}


void sum(std::stack<int> &stk)
{
	std::pair<int,int> arr = getNumbers(stk);
	stk.push(arr.first + arr.second);
}

void sub(std::stack<int> &stk)
{
	std::pair<int,int> arr = getNumbers(stk);
	stk.push(arr.first - arr.second);
}

void prod(std::stack<int> &stk)
{
	std::pair<int,int> arr = getNumbers(stk);
	stk.push(arr.first * arr.second);
}

void div(std::stack<int> &stk)
{
	std::pair<int,int> arr = getNumbers(stk);

	if (arr.second == 0)
		throw std::domain_error("Zero division");
	stk.push(arr.first / arr.second);
}

int topNpop(std::stack<int> &stk)
{
	if (stk.empty())
		throw std::underflow_error("Stack is empty");
	int val = stk.top();
	stk.pop();
	return (val);
}

std::pair<int,int> getNumbers(std::stack<int> &stk)
{
    if (stk.size() < 2)
        throw std::invalid_argument("Not enough numbers in the stack");

    int second = topNpop(stk);
    int first = topNpop(stk);
    return std::make_pair(first, second);
}
