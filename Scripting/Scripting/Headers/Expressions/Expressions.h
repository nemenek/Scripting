#include <string>
#include <vector>
#include <stack>
#include <map>

class Expressions {
public:
	Expressions(std::string sourceFilePathint);
private:
	int Execute(std::string commandName, std::string value);
	std::map <std::string, float> variables;
	float GetNextValue(std::string str);
};