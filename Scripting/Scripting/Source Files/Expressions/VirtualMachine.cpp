#include "../../Headers/Expressions/VirtualMachine.h"
#include "../../Headers/Node.h"
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
	bool mainFuncReached = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i] == "END" || lines[i] == "end") { //empty line means the code is over
			break;
		}
		else if (lines[i][0] == '#') { //# = comments
			continue;
		}
		else if (lines[i] == "START" || lines[i] == "start") {
			mainFuncReached = true;
		}
		else if (lines[i] == "") {
			continue;
		}

		tempString = lines[i];
		pos = tempString.find(" ");
		commandName = tempString.substr(0, pos); //getting the values from the line
		tempString.erase(0, pos + 1);

		jumpTo = Execute(commandName, tempString, i, mainFuncReached); //jumps in script line accordint to return value
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
	std::string tempString = "";\
		bool mainFuncReached = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i] == "END" || lines[i] == "end") { //empty line means the code is over
			break;
		}
		else if (lines[i][0] == '#') { //# = comments
			continue;
		}
		else if (lines[i] == "START" || lines[i] == "start") {
			mainFuncReached = true;
			continue;
		}
		/*else if (lines[i] == "") {
			continue;
		}*/
		tempString = lines[i];
		pos = tempString.find(" ");
		commandName = tempString.substr(0, pos);
		tempString.erase(0, pos + 1);
		jumpTo = Execute(commandName, tempString, i, mainFuncReached);
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

/// <summary>
/// Executes the given command, according to the given parameters (mainly the value parameter)
/// </summary>
/// <param name="commandName">The name of the command</param>
/// <param name="str">The command parameters. even with more parameters it should be one string, all the parameters seperated with space.</param>
/// <param name="row">The index of the row which is being executed.</param>
/// <returns>Greater than zero, if the program should jump to a specific row in the program. Less than zero if the program should skip a specific number of rows (-2 skips two rows(right now it only uses -1 as return value less than 0)). 0 if the program execution should follow in order.</returns>
size_t VirtualMachine::Execute(std::string commandName, std::string str, int row, bool mainFuncReached) {
	//function defining
	if (!mainFuncReached) {
		std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);
		if (commandName == "FUNC") {
			size_t pos = str.find(' ');
			std::string funcName = str.substr(0, pos);
			functions.insert(std::pair<std::string, int>(funcName, row + 1));
		}
		return 0;
	}

	//Check if the first word is not a command but a variable name.
	//It is checked before command name is transformed to all upper case.
	if (floatVariables.find(commandName) != floatVariables.end()) {
		std::string helper = "";
		if (str[0] == '=') {
			//Building expression tree
			expressionTree = Node();
			str = str.substr(1, str.length() - 1);
			str = RemoveSpacesFromBeginning(str);
			Node* nextExp;
			while (str.length() != 0) {
				nextExp = Node::getNextExpression(&expressionTree);
				if (str[0] == '(') {
					addExpression(nextExp, new Node());
					str = str.substr(1, str.length() - 1);
					str = RemoveSpacesFromBeginning(str);
				}
				else {
					size_t pos = str.find(' ');
					std::string var;
					if (pos != std::string::npos) {
						var = str.substr(0, pos);
						str = str.substr(pos, str.length() - 1);
					}
					else {
						var = str;
						str = "";
					}
					if (var.find('+') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('+');
						str = var.substr(pos, var.length() ) + str;
						var = var.substr(0, pos);
					}
					if (var.find('-') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('-');
						str = var.substr(pos, var.length() - 1) + str;
						var = var.substr(0, pos);
					}
					if (var.find('/') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('/');
						str = var.substr(pos, var.length() - 1) + str;
						var = var.substr(0, pos);
					}
					if (var.find('*') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('*');
						str = var.substr(pos, var.length() - 1) + str;
						var = var.substr(0, pos);
					}
					if (var.find('(') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('(');
						str = var.substr(pos, var.length() - 1) + str;
						var = var.substr(0, pos);
					}
					if (var.find(')') != std::string::npos && var.length() != 1) {
						size_t pos = var.find(')');
						str = var.substr(pos, var.length() - 1) + str;
						var = var.substr(0, pos);
					}
					if (var == "") {
						var = str[0];
						str = str.substr(1, str.length() - 1);
					}
					str = RemoveSpacesFromBeginning(str);
					/*if (var[var.length() - 1] == '+' ||
						var[var.length() - 1] == '-' ||
						var[var.length() - 1] == '*' ||
						var[var.length() - 1] == '/' ||
						var[var.length() - 1] == ')' ||
						var[var.length() - 1] == '(') {
						str = var[var.length() - 1] + str;
						var = var.substr(0, var.length() - 2);
					}
					if (var[0] == '+' ||
						var[0] == '-' ||
						var[0] == '*' ||
						var[0] == '/' ||
						var[0] == ')' ||
						var[0] == '(') {
						str = var.substr(1, var.length() - 1) + str;
					}*/
					if (floatVariables.find(var) != floatVariables.end()) {
						addExpression(nextExp, new Node(floatVariables.find(var)->first));
					}
					else if (var == "call" || var == "CALL") {
						addExpression(nextExp, new Node(var));
						helper = str;
						str = "";
					}
					else if (CheckIfNum(var)) {
						addExpression(nextExp, new Node(var));
					}
					else if (var == ")") {
						/*str = str.substr(1, str.length() - 1);*/
					}
					else if (var.length() == 1) {
						if (nextExp == nullptr) {
							Node* tempNode = new Node(expressionTree.getData());
							tempNode->addLeft(expressionTree.getLeft());
							tempNode->addRight(expressionTree.getRight());
							this->expressionTree = Node();
							expressionTree.setData(var);
							expressionTree.addLeft(tempNode);
						}
						else if (nextExp->getData() == "") {
							nextExp->setData(var);
						}
						else {
							addExpression(nextExp, new Node(var));
						}
					}
					else {
						throw VirtualMachineException("Expression evaluation error");
					}
				}
			}
		}
		// Evaluating expression tree
		std::map<std::string, float>::iterator it = floatVariables.find(commandName);
		if (expressionTree.getData() == "") {
			//TODO should only enter this if, if the expressionTree.getLeft()->getData() value is a number or a float variable
			if (CheckIfNum(expressionTree.getLeft()->getData()) || floatVariables.find(expressionTree.getLeft()->getData()) != floatVariables.end()) {
				it->second = GetNextFloatValue(expressionTree.getLeft()->getData());
			}
			else if (helper != "") {
				if (returnValue.length() == 0) {
					return Call(helper, row - 1);
				}
				else {
					it->second = stof(returnValue);
					returnValue = "";
				}
			}
			else {
				throw VirtualMachineException("Expression evaluation error");
			}
		}
		else {
			it->second = EvaluateExpression(expressionTree.getData()[0], expressionTree.getLeft(), expressionTree.getRight());
		}
		return 0;
	}

	;

	///To all upper case.
	std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

	//Command execution
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
			if (str != "\\n") {
				std::cout << str;
			}
			else {
				std::cout << std::endl;
			}
		}
		else {
			float value = GetNextFloatValue(str);
			if (value == FLT_MIN) {
				std::string stringValue = GetNextStringValue(str);
				std::cout << stringValue;
			}
			else {
				std::cout << value;
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
		return 0;
	}
	else if (commandName == "JUMP") {
		int value = std::stoi(str);
		return value;
	}
	else if (commandName == "CREATE") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(str);
		std::map<std::string, std::string>::iterator strIt = stringVariables.find(str);
		if (CheckIfNum(str)) {
			float varValue = std::stof(str);
			floatVariables.insert(std::pair<std::string, float>(varName, varValue));
		}
		else if (it != floatVariables.end()) {
			floatVariables.insert(std::pair<std::string, float>(varName, it->second));
		}
		else {
			if (str[0] == '"' && str[str.size() - 1] == '"') {
				str = str.substr(1, str.size() - 2);
				stringVariables.insert(std::pair<std::string, std::string>(varName, str));
			}
			else if (strIt != stringVariables.end()) {
				stringVariables.insert(std::pair<std::string, std::string>(varName, strIt->second));
			}
			else {
				throw VirtualMachineException(str + " is not a number, or doesn't start with '\"' ");
			}
		}
	}
	else if (commandName == "DELETE") {
		std::map<std::string, float>::iterator flIt = floatVariables.find(str);
		if (flIt != floatVariables.end()) {
			floatVariables.erase(flIt);
			return 0;
		}
		std::map<std::string, std::string>::iterator strIt = stringVariables.find(str);
		if (strIt != stringVariables.end()) {
			stringVariables.erase(strIt);
			return 0;
		}
	}
	else if (commandName == "CALL") {
		return Call(str, row);

	}
	else if (commandName == "FUNC") {
		size_t pos;
		std::string varValue;
		if (str.find(' ') != std::string::npos) {
			pos = str.find(" ");
			str.erase(0, pos + 1);
		}
		else {
			pos = str.find('(');
			str.erase(0, pos);
		}
		if (str.length() >= 2 && str[0] == '(' && str[str.size() - 1] == ')') {
			str = str.substr(1, str.size() - 2); //delete '(' and ')' from the parameter string
			if (str.size() != 0) {
				std::vector<std::string> parameters = ReadParams(str); //loading the parameters to a vector

				size_t index = 0;

				std::map<size_t, float>::iterator floatIt;
				for (std::string parameter : parameters) {
					floatIt = floatFuncParams.find(index);
					if (floatIt != floatFuncParams.end()) {
						floatVariables.insert(std::pair<std::string, float>(parameter, floatIt->second));
						floatFuncParams.erase(index);
					}
					else if (stringFuncParams.find(index) != stringFuncParams.end()) {
						std::map<size_t, std::string>::iterator strIt = stringFuncParams.find(index);
						stringVariables.insert(std::pair<std::string, std::string>(parameter, strIt->second));
						stringFuncParams.erase(index);
					}
					else {
						throw VirtualMachineException("Error at function parameters! Parameter's name: " + parameter);
					}
					++index;
				}
			}
		}
	}
	else if (commandName == "RETURN") {
		if (str[0] == '"' && str[str.size() - 1] == '"') {
			returnValue = str.substr(1, str.size() - 2);
		}
		else if (CheckIfNum(str)) {
			returnValue = str;
		}
		else if (floatVariables.find(str) != floatVariables.end()) {
			returnValue = std::to_string(floatVariables.find(str)->second);
		}
		else if (stringVariables.find(str) != stringVariables.end()) {
			returnValue = stringVariables.find(str)->second;
		}
		else {
			throw VirtualMachineException("Return value error");
		}
		int value = returnRows[returnRows.size() - 1];
		returnRows.pop_back();
		return value;
	}
	else if (commandName == "") {
		int value = returnRows[returnRows.size() - 1];
		returnRows.pop_back();
		return value;
	}

	else {
		throw VirtualMachineException("No such command: " + commandName);
	}
	return 0;
}

/// <summary>
/// Reads the parameters and returns them in a string vector.
/// </summary>
/// <param name="params">The parameters in a string format.</param>
/// <returns>The vector which stores the parameters.</returns>
std::vector<std::string> VirtualMachine::ReadParams(std::string params) {
	std::vector<std::string> parameters;
	size_t pos;
	std::string varValue;
	if (params.find(',') == std::string::npos) {
		parameters.push_back(params);
	}
	else {
		while (params.find(',') != std::string::npos) { //getting values while there is a comma in the parameters.
			pos = params.find(',');
			varValue = params.substr(0, pos);
			params.erase(0, pos + 1);
			varValue = RemoveSpacesFromBeginning(varValue);
			parameters.push_back(varValue);
		}
		params = RemoveSpacesFromBeginning(params);
		parameters.push_back(params); //getting the last parameter
	}
	return parameters;
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

/// <summary>
/// Well it removes the spaces from the beginning of the given string.
/// </summary>
/// <param name="str">The string to be trimmed.</param>
/// <returns>The str without the spaces.</returns>
std::string VirtualMachine::RemoveSpacesFromBeginning(std::string str) {
	while (str.find(' ') == 0) { //deleting plus spaces from the beginning of the variable value
		str = str.substr(1, str.size() - 1);
	}
	return str;
}

//Call command inside functions so it can be called by other functions/commands
size_t VirtualMachine::Call(std::string params, size_t row) {
	size_t pos;
	std::string varValue;
	std::string funcName;
	if (params.find(' ') != std::string::npos) {
		pos = params.find(" ");
		funcName = params.substr(0, pos);
		params.erase(0, pos + 1);
	}
	else {
		pos = params.find('(');
		funcName = params.substr(0, pos);
		params.erase(0, pos);
	}
	std::map<std::string, int>::iterator it = functions.find(funcName);
	if (it != functions.end()) {
		if (params.length() >= 2 && params[0] == '(' && params[params.size() - 1] == ')') {
			params = params.substr(1, params.size() - 2); //delete () from the parameter string
			if (params.size() != 0) {
				std::vector<std::string> parameters = ReadParams(params); //loading the parameters to a vector

				//load the parameters to floatFuncParams and stringFuncParams vectors
				size_t index = 0;
				for (std::string parameter : parameters) {
					if (parameter[0] == '"' && parameter[parameter.size() - 1] == '"') {
						parameter = parameter.substr(1, parameter.size() - 2);
						stringFuncParams.insert(std::pair<size_t, std::string>(index, parameter));
					}
					else if (CheckIfNum(parameter)) {
						floatFuncParams.insert(std::pair<size_t, float>(index, stof(parameter)));
					}
					else if (floatVariables.find(parameter) != floatVariables.end()) {
						std::map<std::string, float>::iterator floatIt = floatVariables.find(parameter);
						floatFuncParams.insert(std::pair<size_t, float>(index, floatIt->second));
					}
					else if (stringVariables.find(parameter) != stringVariables.end()) {
						std::map<std::string, std::string>::iterator stringIt = stringVariables.find(parameter);
						stringFuncParams.insert(std::pair<size_t, std::string>(index, stringIt->second));
					}
					++index;
				}
			}
			//set return row index value


			returnRows.push_back(row + 2);
			return functions.find(funcName)->second;
		}
		else {
			throw VirtualMachineException("Function parameters are not in proper format: " + params);
		}
	}
	else {
		throw VirtualMachineException("No such method is defined: " + funcName);
	}
}

/// <summary>
/// Returns the string lasting to the first ' ' character. If no space is present
/// it returns the whole string.
/// </summary>
/// <param name="str">The string to be checked.</param>
/// <returns>
/// The string value from the first character to the first space
/// or the end of the string if no space is present.
/// </returns>
std::string VirtualMachine::GetFirstVariable(std::string str) {
	size_t spacePos = str.find(' ');
	if (spacePos != std::string::npos) {
		str = str.substr(0, spacePos);
	}
	return str;
}

void VirtualMachine::addExpression(Node* parent, Node* child) {
	if (parent->getLeft() == nullptr) {
		parent->addLeft(child);
	}
	else {
		parent->addRight(child);
	}
}

float VirtualMachine::EvaluateExpression(char operation, Node* first, Node* second) {
	float left = 0.0f;
	float right = 0.0f;
	if (isOperation(first->getData())) {
		left = EvaluateExpression(first->getData()[0], first->getLeft(), first->getRight());
	}
	if (isOperation(second->getData())) {
		right = EvaluateExpression(second->getData()[0], second->getLeft(), second->getRight());
	}
	std::map<std::string, float>::iterator it;
	if (left == 0.0f) {
		it = floatVariables.find(first->getData());
		if (it != floatVariables.end()) {
			left = it->second;
		}
		else {
			left = std::stof(first->getData());
		}
	}
	if (right == 0.0f) {
		it = floatVariables.find(second->getData());
		if (it != floatVariables.end()) {
			right = it->second;
		}
		else {
			right = std::stof(second->getData());
		}
	}
	if (operation == '+') {
		return left + right;
	}
	else if (operation == '-') {
		return left - right;
	}
	else if (operation == '/') {
		return left / right;
	}
	else if (operation == '*') {
		return left * right;
	}
}

bool VirtualMachine::isOperation(std::string str) {
	if (str.length() != 1 || (str.find('+') == std::string::npos && str.find('-') == std::string::npos
		&& str.find('*') == std::string::npos && str.find('/') == std::string::npos)) {
		return false;
	}
	return true;
}