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

	std::deque<Node *> nodes;
	while (!nodes.empty())
	{
	}
}