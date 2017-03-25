#include "GenerateIntialSolutions.h"
#include <deque>

std::vector<BoolTable> GenerateIntialSolutions::getIntialSolutions(Graph & g)
{
	SubFun::pretreat(g);

	return std::vector<BoolTable>();
}

void GenerateIntialSolutions::SubFun::pretreat(Graph & g)
{
	for (unsigned int needPoint : g.needPoints)
	{
		BFSPretreatFrom(g.nodes[needPoint].get(), g);
	}
}

void GenerateIntialSolutions::SubFun::BFSPretreatFrom(Node *bgNode, Graph &g)
{
	unsigned int bgNodeId(bgNode->id);
	BoolTable hasVisited(g.getNodesBoolTable());
	hasVisited[bgNodeId] = true;
	bgNode->needInfo.maxFlowToNeedPoint.insert(bgNodeId, bgNode->need);
	bgNode->needInfo.minCostOfMaxFlowRouteToNeedPoint.insert(bgNodeId, 0u);

	std::deque<Node *> nodes;
	while (!nodes.empty())
	{
		Node &currNode(*nodes.front());
		nodes.pop_front();
		if (hasVisited[currNode.id])
			continue;
		hasVisited[currNode.id] = true;
		if (currNode.isNeed)
			continue;
		
		for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
				edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
		{
			
		}
	}
}