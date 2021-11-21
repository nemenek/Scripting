#include "../../Headers/Expressions/Expressions.h"
#include <fstream>
#include <iostream>
#include <algorithm> 

ExpressionsException::ExpressionsException(std::string msg) : message(msg) {}



Expressions::Expressions(std::string path) {

	std::ifstream newFile;
	std::string text;

	std::vector<std::string> lines;

	newFile.open(path);
	if (newFile.is_open()) {
		int lineCount = 0;
		while (newFile.good()) {
			std::getline(newFile, text);
			lines.push_back(text);
			++lineCount;
		}
	}

	size_t pos = 0;
	std::string commandName;
	int jumpTo = 0;
	std::string tempString = "";
	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i] == "") {
			break;
		}
		else if (lines[i][0] == '#') {
			continue;
		}
		tempString = lines[i];
		pos = tempString.find(" ");
		commandName = tempString.substr(0, pos);
		tempString.erase(0, pos + 1);
		jumpTo = Execute(commandName, tempString);
		if (jumpTo != 0) {
			if (jumpTo > 0) {
				i = jumpTo - 2;
			}
			else {
				i -= jumpTo;
			}
		}
	}
}

int Expressions::Execute(std::string commandName, std::string str) {

	std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

	if (commandName == "ADD") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = variables.find(varName);
		float secondValue = GetNextValue(str);
		it->second = it->second + secondValue;
	}
	else if (commandName == "SUBSTRACT") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = variables.find(varName);
		float secondValue = GetNextValue(str);
		it->second = it->second - secondValue;
	}
	else if (commandName == "MULTIPLY") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = variables.find(varName);
		float secondValue = GetNextValue(str);
		it->second = it->second * secondValue;
	}
	else if (commandName == "DIVIDE") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = variables.find(varName);
		float secondValue = GetNextValue(str);
		it->second = it->second / secondValue;
	}
	else if (commandName == "WRITE") {
		float value = GetNextValue(str);
		if (value == FLT_MIN) {
			std::cout << str << std::endl;
		}
		else {
			std::cout << value << std::endl;
		}
	}
	else if (commandName == "IF") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		float firstValue = GetNextValue(varName);
		pos = str.find(" ");
		std::string l_operator = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::string secondVarName = str;
		float secondValue = GetNextValue(secondVarName);
		if (l_operator == "=" && secondValue != firstValue) {
			return -1;
		}
		else if (l_operator == "<" && firstValue >= secondValue) {
			return -1;
		}
		else if (l_operator == ">" && firstValue <= secondValue) {
			return -1;
		}
		else if (l_operator == "!=" && firstValue == secondValue) {
			return -1;
		}
	}
	else if (commandName == "JUMP") {
		int value = std::stoi(str);
		return value;
	}
	else if (commandName == "CREATE") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		if (CheckIfNum(str)) {
			float varValue;
			if (str != "") {
				varValue = std::stof(str);
			}
			else {
				varValue = 0.0f;
			}
			variables.insert(std::pair<std::string, float>(varName, varValue));
		}
		else {
			variables.insert(std::pair<std::string, float>(varName, std::hash<std::string>{}(str)));
			//TODO: Unhash string from float and display it. STRING operations??
		}
	}
	else if (variables.at(commandName) != NULL) {

	}
	return 0;
}

//Gets the value of the variable stored in the map identified by the varName parameter
// if there is no such variable it tries to parse it into float value
float Expressions::GetNextValue(std::string varName) {
	std::map<std::string, float>::iterator it = variables.find(varName);
	if (it == variables.end()) {
		if (CheckIfNum(varName)) {
			return std::stof(varName);
		}
		return FLT_MIN;
	}
	return it->second;
}

//Returns true if the given parameter str is a number (can be float (1.0) number too), returns false otherwise.
bool Expressions::CheckIfNum(std::string str) {
	for (char c : str) {
		if (std::isdigit(c) == 0 && c != '.') {
			return false;
		}
	}
	return true;
}