#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

int main() {
	void writeIt(std::string param);
	void addNums(float first, float second);

	VirtualMachine vm;
	//external functions
	vm.addExternalFunction("writeIt", writeIt);
	vm.addExternalFunction("addNums", addNums);

	//structs
	vm.initializeStruct("myStruct");
	vm.addFieldToStruct("myStruct", "x", 0.0);
	vm.addFieldToStruct("myStruct", "y", 0.0);
	vm.addFieldToStruct("myStruct", "z", 0.0);

	vm.initializeStruct("structInStruct");
	vm.addStructFieldToStruct("structInStruct", "position", "myStruct");
	vm.addFieldToStruct("structInStruct", "name", "none");

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