#include <string>
#include <vector>
#include <stack>
#include <map>

class ExpressionsException{
public:
	ExpressionsException(std::string msg);
	const char* what();
private:
	std::string message;
};

class Expressions {
public:
	Expressions(std::string sourceFilePathint);
private:
	int Execute(std::string commandName, std::string value);
	std::map <std::string, float> variables;
	float GetNextValue(std::string str);
	bool CheckIfNum(std::string str);
};