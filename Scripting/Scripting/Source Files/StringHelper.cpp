#include "../Headers/StringHelper.h"

/// <summary>
/// It gets the value identified by the varName parameter. If there is not such variable,
/// it returns with an empty string.
/// </summary>
/// <param name="varName">The name of the variable.</param>
/// <returns>The value of the variable. Empty string if there is no such variable.</returns>
std::string GetNextStringValue(std::string varName, std::map<std::string, std::string> stringVars) {
	std::map<std::string, std::string>::iterator it = stringVars.find(varName);
	if (it == stringVars.end()) {
		//throw VirtualMachineException("Variable is not declared.");
		return "";
	}
	return it->second;
}

//Returns true if the given parameter "str" is a number (can be float (1.0) number too), returns false otherwise.
bool CheckIfNum(std::string str) {
	for (char c : str) {
		if (std::isdigit(c) == 0 && c != '.') {
			return false;
		}
	}
	return true;
}

/// <summary>
/// Reads the parameters and returns them in a string vector.
/// </summary>
/// <param name="params">The parameters in a string format.</param>
/// <returns>The vector which stores the parameters.</returns>
std::vector<std::string> ReadParams(std::string params) {
	params = params.substr(1, params.size() - 2); //delete () from the parameter string
	std::vector<std::string> parameters;
	size_t pos;
	std::string varValue;
	while (params.length() != 0) {
		params = RemoveSpacesFromBeginning(params);
		if (params[0] == '"') {
			params = params.substr(1, params.length() - 1);
			pos = params.find('"');
			parameters.push_back(params.substr(0, pos));
			params = params.substr(pos + 1, params.length() - 1);
			if (params.find(',') != std::string::npos) {
				pos = params.find(',');
				params = params.substr(pos + 1, params.length() - 1);
			}
		}
		else {
			pos = params.find(',');
			if (pos == std::string::npos) {
				parameters.push_back(params);
				params = "";
			}
			else {
				parameters.push_back(params.substr(0, pos));
				params = params.substr(pos + 1, params.length() - 1);
			}
		}
	}
	return parameters;
}

/// <summary>
/// Well it removes the spaces from the beginning of the given string.
/// </summary>
/// <param name="str">The string to be trimmed.</param>
/// <returns>The str without the spaces.</returns>
std::string RemoveSpacesFromBeginning(std::string str) {
	while (str.find(' ') == 0) { //deleting plus spaces from the beginning of the variable value
		str = str.substr(1, str.size() - 1);
	}
	return str;
}

//Gets the value of the variable stored in the map identified by the varName parameter
// if there is no such variable it tries to parse it into float value, if it could not,
// returns with FLT_MIN
float GetNextFloatValue(std::string varName, std::map<std::string, float> floatVars) {
	std::map<std::string, float>::iterator it = floatVars.find(varName);
	if (it == floatVars.end()) {
		if (CheckIfNum(varName)) {
			return std::stof(varName);
		}
		return FLT_MIN;
	}
	return it->second;
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
std::string GetFirstVariable(std::string str) {
	size_t spacePos = str.find(' ');
	if (spacePos != std::string::npos) {
		str = str.substr(0, spacePos);
	}
	return str;
}