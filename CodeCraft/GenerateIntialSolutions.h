#pragma once

#include "graph.h"

namespace GenerateIntialSolutions
{
	static unsigned int solutionNum = 10;

	std::vector<BoolTable> getIntialSolutions(Graph &g);

	namespace SubFun
	{
		void pretreat(Graph &g);
		void BFSPretreatFrom(Node *bgNode, Graph &g);

		std::vector<BoolTable> generateIntialSolutionsG(const Graph &g);
		void DFSSelectServers(BoolTable currTable, std::unordered_map<unsigned int, unsigned int> flows, 
			const std::vector<Node *> &nodes, std::vector<BoolTable> &intialSolutions);
	};
};