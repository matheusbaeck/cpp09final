#ifndef HEAD_COIN_HPP
# define HEAD_COIN_HPP
# include <limits>
# include <iostream>
# include <map>
# include <algorithm>
# include <fstream>
# include <sstream>
# include <iterator>

class BitcoinExchange
{

	private:
		std::map<std::string, double> _data;
		double getBitcoinRate(const std::string & date) const;

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange & obj);
		~BitcoinExchange();
		BitcoinExchange& operator=(const BitcoinExchange & obj);
		void	readDataBaseFromFile(const std::string & file_name);
		void	printValueExchanged(const std::string & input_file) const;
};


#endif