#pragma once

#include "SmallestCostFlow.h"
#include "GenerateIntialSolutions.h"

struct Solution
{
	FlowSolution flowSolution;
	std::vector<unsigned int> servers; // node id

	std::string to_string() const;
};

namespace GeneticAlgorithm
{
	Solution generateSolution(const Graph &g);

	namespace SubFun
	{
		// to do
	};
};