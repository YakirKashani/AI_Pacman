#pragma once
#include "Node.h"

const int MAX_COST = 1000;

class Node;

class Edge
{
private:
	Node* pn1;
	Node* pn2;
	double cost;
	bool isPath;
public:
	Edge();
	void Show();
	void SetNode1(Node* p) { pn1 = p; }
	void SetNode2(Node* p) { pn2 = p; }
	Node* GetNode1() { return pn1; }
	Node* GetNode2() { return pn2; }
	void setCost(double value) { cost = value; }
	double GetCost() { return cost; }
};

