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
	FlowSolution getSmallestCostFlow(BoolTable servers, const Graph &g);

	namespace SubFun
	{
		// to do
	};
};