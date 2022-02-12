#pragma once
#include <string>

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
private:
	std::string data;
	Node* left;
	Node* right;
};