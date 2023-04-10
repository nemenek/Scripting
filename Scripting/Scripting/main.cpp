#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

int main() {
	void writeIt(std::string param);
	void addNums(float first, float second);
	void asd();

	VirtualMachine vm;
	//external functions
	vm.addExternalFunction("asd", asd);
	vm.initializeEnum("Animal");
	vm.addEnumType("Animal", "Cat");
	vm.addEnumType("Animal", "Dog");
	vm.addEnumType("Animal", "Monkey");

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

void asd() {
	std::cout << "asd" << std::endl;
}