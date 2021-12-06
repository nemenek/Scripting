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

	std::map <std::string, float> floatVariables; //stores the float variables. The name of the variable is the key. Also stores integer values.

	std::map<std::string, std::string> stringVariables; //stores the string variables. The name of the variable is the key.

	//private functions
	float GetNextFloatValue(std::string str);
	std::string GetNextStringValue(std::string str);
	bool CheckIfNum(std::string str);
	std::vector<std::string> ReadParams(std::string params);

	std::map<std::string, int> functions; //stores the name of the function (key), and the line where the function begins. Initialized at FUNC command call

	bool mainFuncReached; //indicates if the program has reached the main function, it is needed so I can tell if I should start executing the commands

	std::vector<int> returnRows; //set at call command, stores the line where the program should return after finsihed executing called function

	std::map<size_t, float> floatFuncParams;
	std::map<size_t, std::string> stringFuncParams;
};