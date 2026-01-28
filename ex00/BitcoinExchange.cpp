#include "BitcoinExchange.hpp"


std::string trim(const std::string & str, char c)
{
	size_t start = str.find_first_not_of(c);
	if (start == std::string::npos) return "";

	size_t end = str.find_last_not_of(c);
	return str.substr(start, end - start + 1);
}

void validateDate(const std::string & date)
{
	int year, month, day;
	char dash1, dash2;
	std::istringstream is(date);

	if (!(is >> year >> dash1 >> month >> dash2 >> day))
		throw std::invalid_argument("invalid date format.");

	if (dash1 != '-' || dash2 != '-')
		throw std::invalid_argument("invalid date separator.");

	if (month < 1 || month > 12)
		throw std::invalid_argument("invalid month.");

	if (day < 1 || day > 31)
		throw std::invalid_argument("invalid day.");

	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		throw std::invalid_argument("invalid day.");

	if (month == 2) {
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if ((isLeap && day > 29) || (!isLeap && day > 28))
			throw std::invalid_argument("invalid day.");
	}
}

double parseInputNumber(const std::string& s)
{
	char* end;
	double value = std::strtod(s.c_str(), &end);

	if (*end != '\0') 
		throw std::invalid_argument("bad value.");

	if (value < 0)
		throw std::invalid_argument("not a positive number.");

	if (value > 1000)
		throw std::invalid_argument("value too high.");

	return value;
}

double parseDatabaseNumber(const std::string& s)
{
	char* end;
	double value = std::strtod(s.c_str(), &end);

	if (*end != '\0') 
		throw std::invalid_argument("bad value.");

	if (value < 0)
		throw std::invalid_argument("not a positive number.");

	return value;
}


void loadDatabase(const std::string & file_name, std::map<std::string, double>& data)
{
	std::ifstream file(file_name.c_str());
	if (!file.is_open())
		throw std::runtime_error("can't open database file.");
		
	std::string line;
	if (std::getline(file, line)) {
		if (line != "date,exchange_rate")
			throw std::runtime_error("bad database file header.");
	}

	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string date, value;

		if (!std::getline(ss, date, ',') || !std::getline(ss, value))
			continue;

		date = trim(date, ' ');
		value = trim(value, ' ');

		validateDate(date);
		double val = parseDatabaseNumber(value);
		data[date] = val;
	}
}

double getBitcoinRate(const std::map<std::string, double>& data, const std::string& date)
{
	std::map<std::string, double>::const_iterator it = data.lower_bound(date);

	if (it != data.end() && it->first == date)
		return it->second;

	if (it == data.begin())
		throw std::invalid_argument("no rate available for this date. Consider result is Zero");

	--it;
	return it->second;
}


void processFile(const std::string & input_file, const std::map<std::string, double>& data)
{
	std::ifstream infile(input_file.c_str());
	if (!infile.is_open())
		throw std::runtime_error("cannot open input file.");

	int line = 0;
	std::string header;
	while (std::getline(infile, header)) {
		++line;
		header = trim(header, ' ');
		if (!header.empty()) break;
	}
	if (header.empty() || header != "date | value")
		throw std::runtime_error("bad input file header.");

	std::string lineStr;
	while (std::getline(infile, lineStr)) {
		++line;
		if (lineStr.empty()) continue;

		std::stringstream ss(lineStr);
		std::string date, value;

		if (!std::getline(ss, date, '|') || !std::getline(ss, value))
			continue;

		date = trim(date, ' ');
		value = trim(value, ' ');

		try {
			validateDate(date);
			double val = parseInputNumber(value);
			double rate = getBitcoinRate(data, date);

			if (rate > std::numeric_limits<int>::max() / val)
				throw std::overflow_error("value too large.");

			std::cout << date << " => " << val << " = " << rate * val << std::endl;
		} catch (std::exception & e) {
			std::cerr << "[WARNING]:line:" << line << " " << e.what() << std::endl;
		}
	}
}
