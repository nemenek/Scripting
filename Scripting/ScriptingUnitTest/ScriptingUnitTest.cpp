#include "pch.h"
#include "CppUnitTest.h"
#include "Expressions/VirtualMachine.h"
#include <string>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScriptingUnitTest
{
	TEST_CLASS(ScriptingUnitTest)
	{
	public:
		VirtualMachine vm;
		std::string baseScriptDirectory = "E:\\Programozás\\c++\\Scripting\\Scripting\\ScriptingUnitTest\\Test Scripts";

		bool called = false;

		TEST_METHOD(AddTest)
		{
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\AddTest.nk");
			Assert::AreEqual(4.8f, std::stof(GetResultFile()));
		}

		TEST_METHOD(SubstractTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\SubstractTest.nk");
			Assert::AreEqual(2.4f, std::stof(GetResultFile()));
		}

		TEST_METHOD(MultiplyTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\MultiplyTest.nk");
			Assert::AreEqual(5.1f, std::stof(GetResultFile()));
		}

		TEST_METHOD(DivideTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\DivideTest.nk");
			Assert::AreEqual(2.0f, std::stof(GetResultFile()));
		}

		TEST_METHOD(InlineExpressionTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\InlineExpressionTest.nk");
			float result = std::stof(GetResultFile());
			float asd = std::roundf(result * 10);
			result = asd / 10;
			Assert::AreEqual(-0.9f, result);
		}

		TEST_METHOD(MethodCallTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\MethodCallTest.nk");
			Assert::AreEqual(4.9f, std::stof(GetResultFile()));
		}

		TEST_METHOD(IfTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.executeFromFile(this->baseScriptDirectory + "\\IfTest.nk");
			Assert::AreEqual(2.0f, std::stof(GetResultFile()));
		}

		void CallableFunction() {
			this->called = true;
		}

		TEST_METHOD(ExternalFunctionTest) {
			/*this->called = false;
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.addExternalFunction("callableFunction", CallableFunction);
			vm.executeFromFile(this->baseScriptDirectory + "\\ExternalFunctionTest.nk");
			Assert::IsTrue(this->called);*/
		}

		TEST_METHOD(StructTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);
			vm.initializeStruct("struct");
			vm.addFieldToStruct("struct", "field1", 1.5);
			vm.addFieldToStruct("struct", "field2", 3.4);
			vm.initializeStruct("struct2");
			vm.addFieldToStruct("struct2", "struct2Field", 2.0);
			vm.addStructFieldToStruct("struct", "structValue", "struct2");

			vm.executeFromFile(this->baseScriptDirectory + "\\StructTest.nk");
			Assert::AreEqual(6.9f, std::stof(GetResultFile()));
		}

		TEST_METHOD(EnumTest) {
			vm.setSaveDirectory(this->baseScriptDirectory);

			vm.initializeEnum("Type");
			vm.addEnumType("Type", "enumType1");
			vm.addEnumType("Type", "enumType2");

			vm.executeFromFile(this->baseScriptDirectory + "\\EnumTest.nk");
			Assert::AreEqual(2.0f, std::stof(GetResultFile()));
		}

	private:

		std::string GetResultFile() {
			std::string value;
			std::string text;
			std::ifstream file(this->baseScriptDirectory + "\\testResult.txt");
			if (file.is_open()) {
				while (file.good()) {
					std::getline(file, text); //getting all the lines from the file
					value += text;
				}
			}
			return value;
		}
	};
}

