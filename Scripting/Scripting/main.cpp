#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

int main() {
	void writeIt();

	VirtualMachine vm;
	vm.AddExternalFunction("writeIt", writeIt);
	try {
		vm.ExecuteFromFile("sample.nk");
		//vm.ExecuteFromScript("START\nCREATE i 5\nprint i\nEND");
	}
	catch (VirtualMachineException e) {
		std::cout << e.what() << std::endl;
	}
}

void writeIt() {
	std::cout << "It is written in an external function" << std::endl;
}