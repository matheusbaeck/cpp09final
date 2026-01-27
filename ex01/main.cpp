#include "RPN.hpp"

int main(int argc, char **argv)
{
	try {
		std::stack<int>      stk;
		const std::string    operators = "+-*/";
		void                 (*fptr[4])(std::stack<int> &) = {sum, sub, prod, div};

		for (int i = 1; i < argc; ++i)
		{
			if (!argv[i] || argv[i][0] == '\0')
				throw std::invalid_argument("Invalid argument");

			std::string::const_iterator oper =
				std::find(operators.begin(), operators.end(), argv[i][0]);

			if (argv[i][1] == '\0' && oper != operators.end())
			{
				fptr[oper - operators.begin()](stk);
			}
			else
			{
				char *end = 0;
				errno = 0;
				long value = std::strtol(argv[i], &end, 10);

				if (*end != '\0' || errno != 0 || value < INT_MIN || value > INT_MAX)
					throw std::invalid_argument("Invalid number");

				stk.push(static_cast<int>(value));
			}
		}

		if (stk.size() != 1)
			throw std::invalid_argument("incomplete operation");
		else
			std::cout << stk.top() << std::endl;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}
