#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <map>
#include <ostream>
#include <string>


std::string trim(const std::string & str, char c);
void validateDate(const std::string & date);
double parseInputNumber(const std::string& s);
double parseDatabaseNumber(const std::string& s);

void loadDatabase(const std::string & file_name, std::map<std::string, double>& data);
double getBitcoinRate(const std::map<std::string, double>& data, const std::string& date);

void processFile(const std::string & input_file, const std::map<std::string, double>& data);

bool parseDateKey(const std::string& s, int& dayKey);

#endif
