#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

int main() {
	VirtualMachine vm;
	try {
		vm.ExecuteFromFile("sample.nk");
	}
	catch (VirtualMachineException e) {
		std::cout << e.what() << std::endl;
	}
	
	//vm.ExecuteFromScript("CREATE i 5\nprint i\nprint Hello World!");
}