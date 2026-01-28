#include "BitcoinExchange.hpp"

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: ./btc <input_file>" << std::endl;
		return 1;
	}

	std::map<std::string, double> data;
	try {
		loadDatabase("data.csv", data);
		processFile(argv[1], data);
	} catch (std::exception & e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
