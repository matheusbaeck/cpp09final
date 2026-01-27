#include "BitcoinExchange.hpp"


int main(int argv, char** argc)
{

	if (argv != 2){
		
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}
	BitcoinExchange	bit;
	bit.readDataBaseFromFile ("data.csv");
	bit.printValueExchanged (argc[1]);
	return 0;
}