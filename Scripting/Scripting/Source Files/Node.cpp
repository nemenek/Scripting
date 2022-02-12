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

//TODO: gives wrong answer.
Node* Node::getNextExpression(Node* currentNode) {
	if (currentNode->getData() == "+" || currentNode->getData() == "-"
		|| currentNode->getData() == "*" || currentNode->getData() == "/"
		|| currentNode->getData() == "") {
		if (currentNode->getLeft() == nullptr) {
			return currentNode;
		}
		else if (currentNode->getLeft()->getData() == "") {
			return getNextExpression(currentNode->getLeft());
		}
		else if (currentNode->getRight() == nullptr) {
			return currentNode;
		}
		else {
			Node* leftSideResult = getNextExpression(currentNode->getLeft());
			if (leftSideResult == nullptr) {
				return getNextExpression(currentNode->getRight());
			}
			else {
				return leftSideResult;
			}
		}
	}
	return nullptr;
}