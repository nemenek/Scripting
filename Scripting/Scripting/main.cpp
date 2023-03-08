#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

int main() {
	void writeIt(std::string param);
	void addNums(float first, float second);

	VirtualMachine vm;
	vm.addExternalFunction("writeIt", writeIt);
	vm.addExternalFunction("addNums", addNums);
	vm.initializeStruct("myStruct");
	vm.addFieldToStruct("myStruct", "field", 2.0);
	try {
		vm.executeFromFile("sample.nk");
	}
	catch (VirtualMachineException e) {
		std::cout << e.what() << std::endl;
	}
}

void writeIt(std::string param) {
	std::cout <<  param << std::endl;
}

void addNums(float first, float second) {
	float result = first + second;
	std::cout << result << std::endl;
}