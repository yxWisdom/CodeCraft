#pragma once

#include "graph.h"

namespace GenerateIntialSolutions
{
	std::vector<BoolTable> getIntialSolutions(Graph &g);

	namespace SubFun
	{
		void pretreat(Graph &g);
		void BFSPretreatFrom(Node *bgNode, Graph &g);
	};
};