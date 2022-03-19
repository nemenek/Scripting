#pragma once
#include <string>
#include <vector>
#include <map>

std::string GetNextStringValue(std::string str, std::map<std::string, std::string> stringVars);
bool CheckIfNum(std::string str);
std::vector<std::string> ReadParams(std::string params);
std::string RemoveSpacesFromBeginning(std::string str);
float GetNextFloatValue(std::string str, std::map<std::string, float> floatVars);
std::string GetFirstVariable(std::string str);
