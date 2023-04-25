#include "Headers/Expressions/VirtualMachine.h"
#include <iostream>

struct vec3 {
	float x, y, z;
};

struct Transform {
	vec3 pos;
	vec3 scale;
};

static VirtualMachine vm;
static Transform tr;

void initTransformStruct() {
	vm.initializeStruct("vec3");
	vm.addFieldToStruct("vec3", "x", 0);
	vm.addFieldToStruct("vec3", "y", 0);
	vm.addFieldToStruct("vec3", "z", 0);
}

void getTransform(std::string name) {
	vm.initializeStruct(name);
	vm.addStructFieldToStruct(name, "pos", "vec3");
	vm.addStructFieldToStruct(name, "scale", "vec3");
	vm.setStructField(name + ".pos.x", tr.pos.x);
	vm.setStructField(name + ".pos.y", tr.pos.y);
	vm.setStructField(name + ".pos.z", tr.pos.z);
	vm.setStructField(name + ".scale.x", tr.scale.x);
	vm.setStructField(name + ".scale.y", tr.scale.y);
	vm.setStructField(name + ".scale.z", tr.scale.z);
}

void writeTransform(std::string name) {
	 tr.pos.x = *vm.getNumberStructField(name + ".pos.x");
	 tr.pos.y = *vm.getNumberStructField(name + ".pos.y");
	 tr.pos.z = *vm.getNumberStructField(name + ".pos.z");
	 tr.scale.x = *vm.getNumberStructField(name + ".scale.x");
	 tr.scale.y = *vm.getNumberStructField(name + ".scale.y");
	 tr.scale.z = *vm.getNumberStructField(name + ".scale.z");
}

int main() {
	initTransformStruct();
	vm.addExternalFunction("getTransform", getTransform);
	vm.addExternalFunction("writeTransform", writeTransform);
	try {
		vm.executeFromFile("sample.nk");
	}
	catch (VirtualMachineException e) {
		std::cout << e.what() << std::endl;
	}
}