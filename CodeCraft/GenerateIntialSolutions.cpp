#include "GenerateIntialSolutions.h"
#include <deque>
#include <algorithm>

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
	unsigned int maxNeed(bgNode->need);
	UIntTable nodeDepth(g.getNodesUIntTable());

	nodeDepth[bgNodeId] = 1;
	bgNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
	NodeToNeedPointInfo &bgNodePreTreatInfo(bgNode->preTreatInfo.find(bgNodeId)->second);
	bgNodePreTreatInfo.maxFlowToNeedPoint = maxNeed;
	bgNodePreTreatInfo.minCostOfMaxFlowToNeedPoint = 0;

	std::deque<Node *> nodes;
	unsigned int depth(2), thisDepth(1);
	while (!nodes.empty())
	{
		Node &currNode(*nodes.front());
		nodes.pop_front();

		unsigned int flowOut(0), flowIn(0);
		// First Step
		NodePreTreatmentInfo &currNodePreTreatInfo(currNode.preTreatInfo);
		for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
			edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
		{

		}

		// Second Step

		// Third Step

		if (--thisDepth == 0)
		{
			thisDepth = nodes.size();
			++depth;
		}
	}
}