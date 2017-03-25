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
	sscanf(topo[2], "%u", this->cost_per_service);

	// 创建点集
	for (unsigned int i(0); i != nodeNum; ++i)
	{
		nodes.push_back(std::shared_ptr<Node>(new Node(i)));
	}

	// 创建边集
	for (unsigned int i(4), j(i + edgeNum); i != j; ++i)
	{
		unsigned int node1, node2, flow, costPerFlow;
		sscanf(topo[i], "%u %u %u %u", &node1, &node2, &flow, &costPerFlow);

		Node * pNode1(nodes[node1].get()), *pNode2(nodes[node2].get());
		edges.push_back(std::shared_ptr<Edge>(new Edge(flow, costPerFlow, pNode1, pNode2)));
		pNode1->edges.insert(std::make_pair(node2, edges.back().get()));
		edges.push_back(std::shared_ptr<Edge>(new Edge(flow, costPerFlow, pNode2, pNode1)));
		pNode2->edges.insert(std::make_pair(node1, edges.back().get()));
	}

	// 读取需求点集
	for (unsigned int i(4 + edgeNum), j(i + needNodeNum); i != j; ++i)
	{
		unsigned int node, need;
		sscanf(topo[i], "%u %u %u", nullptr, &node, &need);
		Node * thisNode(nodes[node].get());
		thisNode->isNeed = true;
		thisNode->need = need;
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
