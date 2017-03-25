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

		for (std::vector<Flow>::const_iterator currIt(flows.cbegin()), 
			edIt(flows.cend()); currIt != edIt; ++currIt)
		{
			out << currIt->serverNodeId << ' ';
			
			for (std::vector<Edge *>::const_iterator currEdgeIt(currIt->edges.cbegin()),
				edEdgeIt(currIt->edges.cend()); currEdgeIt != edEdgeIt; ++currEdgeIt)
				out << (*currEdgeIt)->nodes.second->id << ' ';
			out << currIt->flow << std::endl;
		}

		return std::move(out.str());
	}
}

Solution GeneticAlgorithm::generateSolution(std::vector<BoolTable> intialSolutions, const Graph & g)
{
	if (intialSolutions.empty())
		return std::move(Solution());

	// to do
	return std::move(Solution());
}
