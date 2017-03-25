#pragma once

#include "SmallestCostFlow.h"
#include "GenerateIntialSolutions.h"

struct Solution
{
	FlowSolution flowSolution;
	std::vector<unsigned int> servers; // node id

	std::string to_string(const Graph &g) const;
};

namespace GeneticAlgorithm
{
	Solution generateSolution(std::vector<BoolTable> intialSolutions, const Graph &g);

	namespace SubFun
	{
		// to do
	};
};