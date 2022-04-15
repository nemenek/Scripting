#pragma once
#include <string>
#include <vector>
#include <map>

std::string getNextStringValue(std::string str, std::map<std::string, std::string> stringVars);
bool checkIfNum(std::string str);
std::vector<std::string> readParams(std::string params);
std::string removeSpacesFromBeginning(std::string str);
float getNextFloatValue(std::string str, std::map<std::string, float> floatVars);
std::string getFirstVariable(std::string str);
