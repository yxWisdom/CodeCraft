#pragma once

#include "graph.h"

struct Flow
{
	unsigned int serverNodeId;
	std::vector<Edge *> edges;
	unsigned int flow;
	unsigned int totalCost;
};

struct FlowSolution
{
	std::vector<Flow> flows;
	unsigned int totalCost;
};

namespace SmallestCostFlow
{
	FlowSolution getSmallestCostFlow(BoolTable servers, Graph &g);

	namespace SubFun
	{
		void initGraph(BoolTable &server, Graph &g);

		void dijkstra();

		void minCostMaxFlow();

		void removeSourceNode();

		Graph *graph;
		size_t nodeSize;
		// sum of cost per flow
		std::vector<int> distance;
		// linked list for recording flow
		std::vector<int> nextNode;
		int minCost = 0;
		FlowSolution flowSolution;
	};
};