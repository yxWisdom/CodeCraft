#include "GeneticAlgorithm.h"
#include <iostream>
#include <sstream>

std::string Solution::to_string() const
{
	if (servers.empty())
	{
		return std::string("NA");
	}
	else
	{
		std::ostringstream out;
		const std::vector<Flow> &flows(flowSolution.flows);

		out << flows.size() << std::endl;
		out << std::endl;

		for (const Flow &flow : flows)
		{
			out << flow.serverNodeId << ' ';
			for (const Edge * edge : flow.edges)
			{
				out << edge->nodes.second->id << ' ';
			}
			out << flow.flow << std::endl;
		}

		return std::move(out.str());
	}
}

Solution GeneticAlgorithm::generateSolution(std::vector<BoolTable> intialSolutions, const Graph & g)
{
	// to do

	return std::move(Solution());
}
