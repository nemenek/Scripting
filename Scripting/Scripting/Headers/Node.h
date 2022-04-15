#pragma once
#include <string>
#include <map>

class Node {
public:
	Node();
	Node(std::string data);
	std::string getData();
	void setData(std::string data);
	void addLeft(Node* left);
	void addRight(Node* right);
	static Node* getNextExpression(Node* currentNode);
	Node* getLeft();
	Node* getRight();

	static bool isOperation(std::string str);
	static void addExpression(Node* parent, Node* child);
	static float evaluateExpression(Node* expressionTree, std::map<std::string, float> floatVars);
private:
	std::string data;
	Node* left;
	Node* right;
};