#include <vector>
#include <stack>
#include <map>
#include <string>

class VirtualMachineException{
public:
	VirtualMachineException(std::string cause);
	std::string what();
private:
	std::string message;
};

class VirtualMachine {
public:
	VirtualMachine();

	/// <summary>
	/// Executes the given script. Every new line should be separated by '\n'.
	/// </summary>
	/// <param name="script">The scipt in string.</param>
	void ExecuteFromScript(std::string script);

	/// <summary>
	/// Executes the code in the given file.
	/// </summary>
	/// <param name="path">The path to the file.</param>
	void ExecuteFromFile(std::string path);
private:
	int Execute(std::string commandName, std::string value, int row);
	std::map <std::string, float> floatVariables;
	std::map<std::string, std::string> stringVariables;
	float GetNextFloatValue(std::string str);
	std::string GetNextStringValue(std::string str);
	bool CheckIfNum(std::string str);
	std::map<std::string, int> functions;
	bool mainFuncReached;
};