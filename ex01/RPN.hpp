#ifndef RPN_HPP__
# define RPN_HPP__

# include <iostream>
# include <stack>
# include <string>
# include <algorithm>
# include <cstdlib>
# include <climits>
# include <cerrno>
# include <stdexcept>
# include <utility>

bool isOnlyDigit(char *str);
bool isOperator(std::string const &oper, char *arg);
int  getIndex(const std::string &oper, char *arg);
void sum(std::stack<int> &que);
void sub(std::stack<int> &que);
void prod(std::stack<int> &que);
void div(std::stack<int> &que);
int topNpop(std::stack<int> &stk);
std::pair<int,int> getNumbers(std::stack<int> &stk);

# endif
