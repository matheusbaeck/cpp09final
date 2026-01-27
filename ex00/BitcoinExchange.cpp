#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & obj){*this = obj;}

BitcoinExchange::~BitcoinExchange(){}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange & obj){

	if (this != &obj)
		_data = obj._data;
	return *this;
}

std::string	noSpace(const std::string & str)
{
	size_t	start = str.find_first_not_of(' ');
	if (start == std::string::npos)
		return str;
	size_t	end = str.find_last_not_of(' ');
	return str.substr(start, end - start + 1);
}

std::pair<std::string, std::string> split(const std::string & line, char deliminator)
{
	size_t	p = line.find(deliminator);
	if (p == std::string::npos)
		throw std::invalid_argument("Error: bad input => " + line);
	std::string	key = noSpace(line.substr(0, p));
	std::string valStr = noSpace(line.substr(p + 1));
	return std::make_pair(key, valStr);
}

void	BitcoinExchange::readDataBaseFromFile(const std::string & file_name)
{
	std::ifstream	file(file_name.c_str());
	if(!file.is_open())
		throw std::runtime_error("Error: can not open the file!!!");
	std::string		new_line;
	std::getline(file, new_line);
	while (std::getline(file, new_line)){

		std::pair<std::string, std::string> key_valStr = split (new_line, ',');
		char	*end;
		double val = std::strtod(key_valStr.second.c_str(), &end);
		if (*end != '\0')
			throw std::runtime_error("Error: read file!!!");
		_data[key_valStr.first] = val;	
	}
}

double	BitcoinExchange::getBitcoinRate(const std::string & date) const
{
	std::map<std::string, double>::const_iterator	it = _data.lower_bound(date);
	if (it->first == date)
		return it->second;
	if (it == _data.end() || it->first != date)
		if (it == _data.begin())
			throw std::invalid_argument("Can not find the val in this date!!!");
	return (--it)->second;
}

void	BitcoinExchange::printValueExchanged(const std::string & input_file) const
{
	std::ifstream	infile(input_file.c_str());
	if (!infile.is_open())
		throw std::runtime_error("Error: can not open the input file!!!");
	std::string		new_line;
	std::getline(infile, new_line);
	while (std::getline(infile, new_line))
	{
		try{
			std::pair<std::string, std::string> key_valStr = split(new_line, '|');
			std::string	searchDate = key_valStr.first;
			try{
				char *end;
				double	val = std::strtod(key_valStr.second.c_str(), &end);
				if (*end != '\0'){
					std::cout << "No current value!" << std::endl;
					continue;
				}
				if (val < 0){

					std::cout << "Error: not a positive number." << std::endl;
					continue ;
				}
				if (this->getBitcoinRate(searchDate) * val> std::numeric_limits<int>::max()){
					
					std::cout << "Error: too large a number." << std::endl;
					continue ;
				}
				std::cout << searchDate << " => " << val << " = " << this->getBitcoinRate(searchDate) * val << std::endl;
			}catch (std::exception & e){
				std::cout << "No current value!" << std::endl;
			}
		}
		catch(std::exception & e){
			std::cerr << e.what() << std::endl;
		}
	}
}

