#include "../Headers/Node.h"

Node::Node(std::string data) : data(data) {
	this->left = nullptr;
	this->right = nullptr;
}

Node::Node() {
	this->data = "";
	this->left = nullptr;
	this->right = nullptr;
}

std::string Node::getData() {
	return this->data;
}

void Node::setData(std::string data) {
	this->data = data;
}

void Node::addLeft(Node* node) {
	this->left = node;
}

void Node::addRight(Node* node) {
	if (this->data != "") {
		this->right = node;
	}
}

Node* Node::getLeft() {
	return this->left;
}

Node* Node::getRight() {
	return this->right;
}

Node* Node::getNextExpression(Node* currentNode) {
	if (currentNode->getData() == "+" || currentNode->getData() == "-"
		|| currentNode->getData() == "*" || currentNode->getData() == "/"
		|| currentNode->getData() == "") {
		if (currentNode->getLeft() == nullptr) {
			return currentNode;
		}
		Node* leftSideResult = getNextExpression(currentNode->getLeft());
		if (leftSideResult != nullptr) {
			return leftSideResult;
		}
		else if (currentNode->getRight() == nullptr) {
			return currentNode;
		}
		else {
			return getNextExpression(currentNode->getRight());
		}
	}
	return nullptr;
}

/// <summary>
/// Adds a child to the parent Node. If the left child is null then the left one will be the new child, the right one otherwise.
/// </summary>
/// <param name="parent">The parent Node to have a child added</param>
/// <param name="child">The child to be added.</param>
void Node::addExpression(Node* parent, Node* child) {
	if (parent->getLeft() == nullptr) {
		parent->addLeft(child);
	}
	else {
		parent->addRight(child);
	}
}

/// <summary>
/// Evaluates the expression tree.
/// </summary>
/// <param name="operation">The root node's operation. (Root node MUST be an operation)</param>
/// <param name="first">The left child of the root node.</param>
/// <param name="second">The right child of the root node.</param>
/// <param name="floatVars"></param>
/// <returns></returns>
float Node::EvaluateExpression(Node* node, std::map<std::string, float> floatVars) {
	float left = 0.0f;
	float right = 0.0f;
	if (isOperation(node->getLeft()->getData())) {
		left = EvaluateExpression(node->getLeft(), floatVars);
	}
	if (isOperation(node->getRight()->getData())) {
		right = EvaluateExpression(node->getRight(), floatVars);
	}
	std::map<std::string, float>::iterator it;
	if (left == 0.0f) {
		it = floatVars.find(node->getLeft()->getData());
		if (it != floatVars.end()) {
			left = it->second;
		}
		else {
			left = std::stof(node->getLeft()->getData());
		}
		delete(node->getLeft());
	}
	if (right == 0.0f) {
		it = floatVars.find(node->getRight()->getData());
		if (it != floatVars.end()) {
			right = it->second;
		}
		else {
			right = std::stof(node->getRight()->getData());
		}
		delete(node->getRight());
	}
	if (node->getData()[0] == '+') {
		return left + right;
	}
	else if (node->getData()[0] == '-') {
		return left - right;
	}
	else if (node->getData()[0] == '/') {
		return left / right;
	}
	else if (node->getData()[0] == '*') {
		return left * right;
	}
	return 0;
}

/// <summary>
/// Checks if the given string is an operation
/// </summary>
/// <param name="str">The string to be checked.</param>
/// <returns>True if the string is an operation character, false otherwise.</returns>
bool Node::isOperation(std::string str) {
	if (str.length() != 1 || (str.find('+') == std::string::npos && str.find('-') == std::string::npos
		&& str.find('*') == std::string::npos && str.find('/') == std::string::npos)) {
		return false;
	}
	return true;
}