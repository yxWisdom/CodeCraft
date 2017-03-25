#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "graph.h"
#include <iostream>
#include <sstream>

Graph::Graph(char * topo[MAX_EDGE_NUM])
{
	unsigned int nodeNum, edgeNum, needNodeNum;
	sscanf(topo[0], "%u %u %u", &nodeNum, &edgeNum, &needNodeNum);
	sscanf(topo[2], "%u", &(this->cost_per_service));

	// 创建点集
	for (unsigned int i(0); i != nodeNum; ++i)
	{
		nodes.push_back(std::shared_ptr<Node>(new Node(i)));
	}

	// 创建边集
	unsigned edgeCounter(-1);
	for (unsigned int i(4), j(i + edgeNum); i != j; ++i)
	{
		unsigned int node1, node2, flow, costPerFlow;
		sscanf(topo[i], "%u %u %u %u", &node1, &node2, &flow, &costPerFlow);

		Node *pNode1(nodes[node1].get()), *pNode2(nodes[node2].get());
		Edge *edge1, *edge2;

		edges.push_back(std::shared_ptr<Edge>(new Edge(flow, costPerFlow, pNode1, pNode2)));
		edge1 = edges.back().get();
		edges.push_back(std::shared_ptr<Edge>(new Edge(flow, costPerFlow, pNode2, pNode1)));
		edge2 = edges.back().get();
		
		edge1->reverseEdge = edge2;
		edge1->id = ++edgeCounter;
		edge2->reverseEdge = edge1;
		edge2->id = ++edgeCounter;
		pNode1->edges.insert(std::make_pair(node2, edge1));
		pNode2->edges.insert(std::make_pair(node1, edge2));
	}

	// 读取需求点集
	for (unsigned int i(4 + edgeNum + 1), j(i + needNodeNum); i != j; ++i)
	{
		unsigned int order, node, need;
		sscanf(topo[i], "%u %u %u", &order, &node, &need);
		Node * thisNode(nodes[node].get());
		thisNode->isNeed = true;
		thisNode->needOrder = order;
		thisNode->need = need;

		needPoints.insert(node);
	}
}

std::vector<bool> Graph::getNodesBoolTable() const
{
	static BoolTable nodeBoolTable(nodes.size(), false);
	return nodeBoolTable;
}

std::vector<bool> Graph::getEdgesBoolTable() const
{
	static BoolTable edgesBoolTable(edges.size(), false);
	return edgesBoolTable;
}