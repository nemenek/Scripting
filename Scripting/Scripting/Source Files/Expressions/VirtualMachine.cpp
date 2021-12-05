#include "../../Headers/Expressions/VirtualMachine.h"
#include <fstream>
#include <iostream>
#include <algorithm>

VirtualMachineException::VirtualMachineException(std::string msg) : message(msg) {}

std::string VirtualMachineException::what() {
	return message;
}
VirtualMachine::VirtualMachine() {}

void VirtualMachine::ExecuteFromScript(std::string script) {
	std::vector<std::string> lines;
	std::string tempString = "";
	for (char item : script) {
		if (item == '\n') {
			lines.push_back(tempString);
		}
		else {
			tempString += item;
		}
	}
	lines.push_back(tempString);

	size_t pos = 0;
	std::string commandName;
	int jumpTo = 0;

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i] == "") { //empty line means the code is over
			break;
		}
		else if (lines[i][0] == '#') { //# = comments
			continue;
		}

		tempString = lines[i];
		pos = tempString.find(" ");
		commandName = tempString.substr(0, pos); //getting the values from the line
		tempString.erase(0, pos + 1);

		jumpTo = Execute(commandName, tempString); //jumps in script line accordint to return value
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

void VirtualMachine::ExecuteFromFile(std::string path) {

	std::ifstream newFile;
	std::string text;

	std::vector<std::string> lines;

	newFile.open(path);
	if (newFile.is_open()) {
		while (newFile.good()) {
			std::getline(newFile, text); //getting all the lines from the file
			lines.push_back(text);
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

int VirtualMachine::Execute(std::string commandName, std::string str) {

	std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

	if (commandName == "ADD") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str);
		it->second = it->second + secondValue;
	}
	else if (commandName == "SUBSTRACT") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str);
		it->second = it->second - secondValue;
	}
	else if (commandName == "MULTIPLY") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str);
		it->second = it->second * secondValue;
	}
	else if (commandName == "DIVIDE") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str);
		it->second = it->second / secondValue;
	}
	else if (commandName == "PRINT") {
		if (str[0] == '"' && str[str.size() - 1] == '"') {
			str = str.substr(1, str.size() - 2);
			std::cout << str << std::endl;
		}
		else {
			float value = GetNextFloatValue(str);
			if (value == FLT_MIN) {
				std::string stringValue = GetNextStringValue(str);
				std::cout << stringValue << std::endl;
			}
			else {
				std::cout << value << std::endl;
			}
		}
	}
	else if (commandName == "IF") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		float firstValue = GetNextFloatValue(varName);
		pos = str.find(" ");
		std::string l_operator = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::string secondVarName = str;
		float secondValue = GetNextFloatValue(secondVarName);
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
			floatVariables.insert(std::pair<std::string, float>(varName, varValue));
		}
		else {
			if (str[0] == '"' && str[str.size() - 1] == '"') {
				str = str.substr(1, str.size() - 2);
				stringVariables.insert(std::pair<std::string, std::string>(varName, str));
			}
			else {
				throw VirtualMachineException(str + " is not a number, or doesn't start with '\"' ");
			}
		}
	}
	else if (floatVariables.at(commandName) != NULL) {

	}
	return 0;
}

//Gets the value of the variable stored in the map identified by the varName parameter
// if there is no such variable it tries to parse it into float value, if it could not,
// returns with FLT_MIN
float VirtualMachine::GetNextFloatValue(std::string varName) {
	std::map<std::string, float>::iterator it = floatVariables.find(varName);
	if (it == floatVariables.end()) {
		if (CheckIfNum(varName)) {
			return std::stof(varName);
		}
		return FLT_MIN;
	}
	return it->second;
}

/// <summary>
/// It gets the value identified by the varName parameter. If there is not such variable,
/// it returns with an empty string.
/// </summary>
/// <param name="varName">The name of the variable.</param>
/// <returns>The value of the variable. Empty string if there is no such variable.</returns>
std::string VirtualMachine::GetNextStringValue(std::string varName) {
	std::map<std::string, std::string>::iterator it = stringVariables.find(varName);
	if (it == stringVariables.end()) {
		//throw VirtualMachineException("Variable is not declared.");
		return "";
	}
	return it->second;
}

//Returns true if the given parameter "str" is a number (can be float (1.0) number too), returns false otherwise.
bool VirtualMachine::CheckIfNum(std::string str) {
	for (char c : str) {
		if (std::isdigit(c) == 0 && c != '.') {
			return false;
		}
	}
	return true;
}