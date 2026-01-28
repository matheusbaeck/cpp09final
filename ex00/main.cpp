#include "BitcoinExchange.hpp"

bool parseDateKey(const std::string& s, int& dayKey)
{
	int y, m, d;
	char dash1, dash2;

	std::istringstream is(s);

	if (!(is >> y >> dash1 >> m >> dash2 >> d))
		return false;

	if (dash1 != '-' || dash2 != '-')
		return false;

	std::tm t;
	std::memset(&t, 0, sizeof(std::tm));

	t.tm_year = y - 1900;
	t.tm_mon  = m - 1;
	t.tm_mday = d;
	t.tm_isdst = -1;

	time_t when = mktime(&t);
	if (when == -1)
		return false;

	std::tm* norm = localtime(&when);

	if (norm->tm_year != y - 1900 ||
		norm->tm_mon  != m - 1 ||
		norm->tm_mday != d)
		return false;

	dayKey = static_cast<int>(when / 86400);
	return true;
}

std::string trim(const std::string & str, char c)
{
	size_t start = str.find_first_not_of(c);
	size_t end = str.find_last_not_of(c);
	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

void ValidateDate(const std::string & date)
{
	int		year, month, day;
	char	dash1, dash2;
	std::istringstream	is(date);

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

	if (month == 2)
	{
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if ((isLeap && day > 29) || (!isLeap && day > 28))
			throw std::invalid_argument("invalid day.");
	}
}

double ParseInputNumber(const std::string& s)
{
	char* end;
	double value = std::strtod(s.c_str(), &end);

	if (*end != '\0'){
		throw std::invalid_argument("bad value");
	}
	if (value < 0){
		throw std::invalid_argument("not a positive number.");
	}
	return value;
}

double ParseDatabaseNumber(const std::string& s)
{
	char* end;
	double value = std::strtod(s.c_str(), &end);

	if (*end != '\0'){
		throw std::invalid_argument("bad value");
	}
	if (value < 0){
		throw std::invalid_argument("not a positive number.");
	}
	return value;
}

void processFile(const std::string & input_file, const std::map<std::string, double> & data)
{
	std::ifstream infile(input_file.c_str());
	if (!infile.is_open())
		throw std::runtime_error("can not open input file.");
		
	int line = 0;
	std::string header;
	if (std::getline(infile, header))
	{
		if(header != "date | value")
			throw std::runtime_error("bad input file header.");
		++line;
	}

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
			ValidateDate(date);
			double val = ParseInputNumber(value);
			double rate = getBitcoinRate(data, date);

			if (rate > std::numeric_limits<int>::max() / val)
				throw std::overflow_error("value too large.");

			std::cout << date << " => " << val << " = " << rate * val << std::endl;
		} catch (std::exception & e) {
			std::cerr << "[WARNING] line:" << line << " " << e.what() << std::endl;
		}
	}
}

void loadDatabase(const std::string & file_name, std::map<std::string, double>& data)
{
	std::ifstream	file(file_name.c_str());
	if(!file.is_open())
		throw std::runtime_error("can't open database file.");

	std::string date, value;
	while (std::getline(file, date, ',') && std::getline(file, value)){
		ValidateDate(trim(trim(date, ' '), '\t'));
		double val = ParseDatabaseNumber(trim(trim(value, ' '), '\t'));
		data[date] = val;
	}
}

double getBitcoinRate(std::map<std::string, double> const & data, const std::string & date)
{
	std::map<std::string, double>::const_iterator it = data.lower_bound(date);
	
	if (it->first == date)
		return it->second;

	if (it == data.end() || it->first != date)
	{
		if (it == data.begin())
			throw std::invalid_argument("no rate available for this date.");

		return (--it)->second;
	}
}
double getBitcoinRate(std::map<std::string, double> const & data, const std::string & date)
{
	auto it = data.lower_bound(date);

	if (it != data.end() && it->first == date)
		return it->second;

	if (it == data.begin())
		throw std::invalid_argument("no rate available for this date.");

	return (--it)->second;
}


int main(int argc, char** argv)
{
	std::map<std::string, double> data;
	try {
		if (argc != 2)
			throw std::invalid_argument("could not open file.");
		loadDatabase("data.csv", data);
		processFile(argv[1], data);
		return 0;
	} catch (std::exception & e){
		std::cerr << "[ERROR] " << e.what() << std::endl;
	}
	std::cout << "Usage: ./btc <input_file>" << std::endl;
	return 1;
}