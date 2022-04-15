#include "../Node.h"
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <functional>

class VirtualMachineException {
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
	void executeFromScript(std::string script);

	/// <summary>
	/// Executes the code in the given file.
	/// </summary>
	/// <param name="path">The path to the file.</param>
	void executeFromFile(std::string path);


	void addExternalFunction(std::string name, void (*funcPointer)(void));

	void addExternalFunction(std::string name, void (*funcPointer)(std::string));

	void addExternalFunction(std::string name, void (*funcPointer)(float, float));

private:
	size_t execute(std::string commandName, std::string value, int row, bool mainFuncReached);

	//private functions
	//The call command in a function, so it can be easily accessed.
	size_t call(std::string params, size_t row);

	
	

	std::map <std::string, float> floatVariables; //stores the float variables. The name of the variable is the key. Also stores integer values.

	std::map<std::string, std::string> stringVariables; //stores the string variables. The name of the variable is the key.

	std::map<std::string, int> functions; //stores the name of the function (key), and the line where the function begins. Initialized at FUNC command call
	std::map<std::string, void (*)()> externalFunctions; //stores external functions with no parameter
	std::map<std::string, void (*)(std::string)> externalFunctionsString; //stores external functions with string parameter
	std::map<std::string, void (*)(float, float)> externalFunctionsFloat; //stores external functions with string parameter

	std::vector<int> returnRows; //set at call command, stores the line where the program should return after finsihed executing called function

	//the values of the function parameters
	std::map<size_t, float> floatFuncParams;
	std::map<size_t, std::string> stringFuncParams;

	//stores the function return value until it is read in the next step
	std::string returnValue;

	//expression tree
	Node expressionTree;
};