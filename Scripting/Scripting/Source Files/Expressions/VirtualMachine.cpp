#include "../../Headers/Expressions/VirtualMachine.h"
#include "../../Headers/Node.h"
#include "../../Headers/StringHelper.h"
#include <fstream>
#include <iostream>
#include <algorithm>

VirtualMachineException::VirtualMachineException(std::string msg) : message(msg) {}

std::string VirtualMachineException::what() {
	return message;
}

VirtualMachine::VirtualMachine() {}

//TODO: add external func call to call command
void VirtualMachine::AddExternalFunction(std::string name, void (*funcPointer)(void)) {
	//functions.insert(std::pair<std::string, int>(name, -1));
	externalFunctions.insert(std::pair < std::string, void(*)()>(name, funcPointer));
}

void VirtualMachine::AddExternalFunction(std::string name, void (*funcPointer)(std::string)) {
	//functions.insert(std::pair<std::string, int>(name, -1));
	externalFunctionsString.insert(std::pair < std::string, void(*)(std::string)>(name, funcPointer));
}

void VirtualMachine::AddExternalFunction(std::string name, void (*funcPointer)(float, float)) {
	//functions.insert(std::pair<std::string, int>(name, -1));
	externalFunctionsFloat.insert(std::pair < std::string, void(*)(float, float)>(name, funcPointer));
}

void VirtualMachine::ExecuteFromScript(std::string script) {
	std::vector<std::string> lines;
	std::string tempString = "";
	for (char item : script) {
		if (item == '\n') {
			lines.push_back(tempString);
			tempString = "";
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
			continue;
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
			size_t pos;
			if (str.find(' ') != std::string::npos) {
				pos = str.find(' ');
				std::string funcName = str.substr(0, pos);
				functions.insert(std::pair<std::string, int>(funcName, row + 1));
			}
			else {
				pos = str.find('(');
				std::string funcName = str.substr(0, pos);
				functions.insert(std::pair<std::string, int>(funcName, row + 1));
			}
		}
		return 0;
	}

	//Check if the first word is not a command but a variable name.
	//It is checked before command name is transformed to all upper case.

	//Case float:
	if (floatVariables.find(commandName) != floatVariables.end()) {
		//If it stays empty, there is no function call, it contains the function name and params otherwise
		std::string funcParamsHelper = "";
		//It is evaluateable only if there is '=' at the begining of the expression.
		if (str[0] == '=') {
			//Building expression tree

			//Root node
			expressionTree = Node();
			//Removing '=' from the begining.
			str = str.substr(1, str.length() - 1);
			str = RemoveSpacesFromBeginning(str);
			//The next node to be determined
			Node* nextExp;
			while (str.length() != 0) {
				//Initializing an empty node
				nextExp = Node::getNextExpression(&expressionTree);
				//If it is a '(' it adds a new empty node.
				if (str[0] == '(') {
					Node::addExpression(nextExp, new Node());
					//Remove '(' from begining
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
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var.find('-') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('-');
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var.find('/') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('/');
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var.find('*') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('*');
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var.find('(') != std::string::npos && var.length() != 1) {
						size_t pos = var.find('(');
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var.find(')') != std::string::npos && var.length() != 1) {
						size_t pos = var.find(')');
						str = var.substr(pos, var.length()) + str;
						var = var.substr(0, pos);
					}
					if (var == "") {
						var = str[0];
						str = str.substr(1, str.length() - 1);
					}
					str = RemoveSpacesFromBeginning(str);
					if (floatVariables.find(var) != floatVariables.end()) {
						Node::addExpression(nextExp, new Node(floatVariables.find(var)->first));
					}
					else if (var == "CALL" || var == "call") {
						Node::addExpression(nextExp, new Node(var));
						funcParamsHelper = str;
						str = "";
					}
					else if (functions.find(var) != functions.end()) {
						Node::addExpression(nextExp, new Node(var));
						funcParamsHelper = var + str;
						str = "";
					}
					else if (CheckIfNum(var)) {
						Node::addExpression(nextExp, new Node(var));
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
							Node::addExpression(nextExp, new Node(var));
						}
					}
					else {
						throw VirtualMachineException("Expression evaluation error. Line:" + row);
					}
				}
			}
		}
		else {
			throw VirtualMachineException("Expression parsing error. Line:" + row);
		}
		// Evaluating expression tree
		std::map<std::string, float>::iterator it = floatVariables.find(commandName);
		if (expressionTree.getData() == "") {
			if (CheckIfNum(expressionTree.getLeft()->getData()) || floatVariables.find(expressionTree.getLeft()->getData()) != floatVariables.end()) {
				it->second = GetNextFloatValue(expressionTree.getLeft()->getData(), this->floatVariables);
			}
			//If it is a function call
			else if (funcParamsHelper != "") {
				//if it has not been evaluated
				if (returnValue.length() == 0) {
					return Call(funcParamsHelper, row - 1);
				}
				//else save return value of function
				else {
					it->second = stof(returnValue);
					returnValue = "";
				}
			}
			else {
				throw VirtualMachineException("Expression evaluation error Line:" + row);
			}
		}
		else {
			it->second = Node::EvaluateExpression(&this->expressionTree, this->floatVariables);
		}
		return 0;
	}
	/// Case String
	else if (stringVariables.find(commandName) != stringVariables.end()) {
		if (str[0] == '=') {
			str = str.substr(1, str.length() - 1);
			str = RemoveSpacesFromBeginning(str);
			if (str[0] == '"' && str[str.length() - 1] == '"') {
				str = str.substr(1, str.length() - 2);
				stringVariables.find(commandName)->second = str;
			}
			else if (stringVariables.find(str) != stringVariables.end()) {
				stringVariables.find(commandName)->second = stringVariables.find(str)->second;
			}
			else if (str.find("call") != std::string::npos || str.find("CALL") != std::string::npos || str.find("Call") != std::string::npos) {
				if (returnValue.length() == 0) {
					str = str.substr(4, str.length() - 1);
					str = RemoveSpacesFromBeginning(str);
					return Call(str, row - 1);
				}
				else {
					stringVariables.find(commandName)->second = returnValue;
					returnValue = "";
				}
			}
			else {
				throw VirtualMachineException("Expression evaluation error Line:" + row);
			}
		}
		else {
			throw VirtualMachineException("Expression parsing error. Line:" + row);
		}
		return 0;
	}
	//Case function call
	else {
		// it could be called like funcion(params) or function (params)
		size_t pos = commandName.find('(');
		if (pos != std::string::npos) {
			std::string tmpStr = commandName.substr(0, pos);
			if (functions.find(tmpStr) != functions.end()) {
				return Call(commandName, row);
			}
		}
		else {
			if (functions.find(commandName) != functions.end()) {
				return Call(commandName + str, row);
			}
		}
	}



	///To all upper case.
	std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

	//Command execution
	if (commandName == "ADD") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str, this->floatVariables);
		it->second = it->second + secondValue;
	}
	else if (commandName == "SUBSTRACT") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str, this->floatVariables);
		it->second = it->second - secondValue;
	}
	else if (commandName == "MULTIPLY") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str, this->floatVariables);
		it->second = it->second * secondValue;
	}
	else if (commandName == "DIVIDE") {
		size_t pos = str.find(" ");
		std::string varName = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::map<std::string, float>::iterator it = floatVariables.find(varName);
		float secondValue = GetNextFloatValue(str, this->floatVariables);
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
			float value = GetNextFloatValue(str, this->floatVariables);
			if (value == FLT_MIN) {
				std::string stringValue = GetNextStringValue(str, this->stringVariables);
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
		float firstValue = GetNextFloatValue(varName, this->floatVariables);
		pos = str.find(" ");
		std::string l_operator = str.substr(0, pos);
		str.erase(0, pos + 1);
		std::string secondVarName = str;
		float secondValue = GetNextFloatValue(secondVarName, this->floatVariables);
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

//Call command inside functions so it can be called by other functions/commands
size_t VirtualMachine::Call(std::string params, size_t row) {
	size_t pos;
	std::string varValue;
	std::string funcName;
	if (params.find(' ') != std::string::npos && (params.find(' ') < params.find('"') || params.find('"') == std::string::npos) && params.find(' ') < params.find('(')) {
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

			if (params.size() != 0) {
				std::vector<std::string> parameters = ReadParams(params); //loading the parameters to a vector

				//load the parameters to floatFuncParams and stringFuncParams vectors
				size_t index = 0;
				for (std::string parameter : parameters) {
					if (CheckIfNum(parameter)) {
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
					else {
						stringFuncParams.insert(std::pair<size_t, std::string>(index, parameter));
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
	//Review that
	else if (externalFunctions.find(funcName) != externalFunctions.end()) {
		externalFunctions.find(funcName)->second();
		return 0;
	}
	else if (externalFunctionsString.find(funcName) != externalFunctionsString.end()) {
		std::vector<std::string> parameters = ReadParams(params);
		externalFunctionsString.find(funcName)->second(parameters[0]);
		return 0;
	}
	else if (externalFunctionsFloat.find(funcName) != externalFunctionsFloat.end()) {
		std::vector<std::string> parameters = ReadParams(params);
		float firstVal = 0.0, secondVal = 0.0;
		if (floatVariables.find(parameters[0]) != floatVariables.end()) {
			firstVal = floatVariables.find(parameters[0])->second;
		}
		else if (CheckIfNum(parameters[0])) {
			firstVal = std::stof(parameters[0]);
		}
		if (floatVariables.find(parameters[1]) != floatVariables.end()) {
			secondVal = floatVariables.find(parameters[1])->second;
		}
		else if (CheckIfNum(parameters[1])) {
			secondVal = std::stof(parameters[1]);
		}
		externalFunctionsFloat.find(funcName)->second(firstVal, secondVal);
		return 0;
	}
	else {
		throw VirtualMachineException("No such method is defined: " + funcName);
	}
}



