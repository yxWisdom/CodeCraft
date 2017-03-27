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
	IntTable flowDistance(g.getNodesIntTable());

	nodeDepth[bgNodeId] = 1;
	bgNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
	NodeToNeedPointInfo &bgNodePreTreatInfo(bgNode->preTreatInfo.find(bgNodeId)->second);
	bgNodePreTreatInfo.maxFlowToNeedPoint = maxNeed;
	bgNodePreTreatInfo.minCostOfMaxFlowToNeedPoint = 0;

	std::deque<Node *> nodes;
	unsigned int flowIn(0);
	for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(bgNode->edges.cbegin()),
		edIt(bgNode->edges.cend()); currIt != edIt; ++currIt)
	{
		Node *anoNode(currIt->second->nodes.second);
		nodeDepth[anoNode->id] = 1;
		nodes.push_back(anoNode);
		
		Edge *reverseEdge(currIt->second->reverseEdge);
		reverseEdge->preTreatInfo.insert(std::make_pair(bgNodeId, EdgeToNeedPointInfo()));
		EdgeToNeedPointInfo &reverseEdgeToNeedPointInfo(reverseEdge->preTreatInfo.find(bgNodeId)->second);

		reverseEdgeToNeedPointInfo.flag = true;
		reverseEdgeToNeedPointInfo.maxFlowToNeedPoint = maxNeed > reverseEdge->flow ? reverseEdge->flow : maxNeed;
		reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint =
			reverseEdgeToNeedPointInfo.maxFlowToNeedPoint * reverseEdge->costPerFlow;

		anoNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
		NodeToNeedPointInfo &anoNodeToNeedPointInfo(anoNode->preTreatInfo.find(bgNodeId)->second);
		anoNodeToNeedPointInfo.BFSDepth = 1;
		anoNodeToNeedPointInfo.maxFlowToNeedPoint = reverseEdgeToNeedPointInfo.maxFlowToNeedPoint;
		anoNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint = reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint;

		flowIn += reverseEdgeToNeedPointInfo.maxFlowToNeedPoint;
	}
	flowDistance[bgNodeId] = flowIn;
	
	unsigned int depth(1), thisDepth(1);
	while (!nodes.empty())
	{
		unsigned int thisDepth(nodes.size());

		/******************************************************************************/
		/* First Step:                                                                */
		/* Calculate the flow distance and find, mark and add the nodes in next depth */
		/******************************************************************************/
		for (unsigned int i(0); i != thisDepth; ++i)
		{
			Node &currNode(*nodes[i]);
			NodeToNeedPointInfo &currNodeToNeedPointInfo(currNode.preTreatInfo.find(bgNodeId)->second);

			unsigned int flowIn(0);
			for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
				edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
			{
				Node *anoNode(currIt->second->nodes.second);
				if (nodeDepth[anoNode->id] == 0)
				{
					// unvisited next depth point
					// mark and add it
					nodeDepth[anoNode->id] = depth + 1;
					nodes.push_back(anoNode);

					anoNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
					NodeToNeedPointInfo &anoNodeToNeedPointInfo(anoNode->preTreatInfo.find(bgNodeId)->second);
					anoNodeToNeedPointInfo.BFSDepth = depth + 1;
				}

				if (nodeDepth[anoNode->id] > depth)
				{
					// visited next depth point
					// set its reverse edge enabled
					Edge *reverseEdge(currIt->second->reverseEdge);
					reverseEdge->preTreatInfo.insert(std::make_pair(bgNodeId, EdgeToNeedPointInfo()));
					EdgeToNeedPointInfo &reverseEdgeToNeedPointInfo(reverseEdge->preTreatInfo.find(bgNodeId)->second);
					reverseEdgeToNeedPointInfo.flag = true;

					// cal the flow in
					flowIn += reverseEdge->flow;
				}
			}
			// FlowOut is same as the nodeToNeedPointInfo.maxFlowToNeedPoint
			flowDistance[currNode.id] = flowIn - currNodeToNeedPointInfo.maxFlowToNeedPoint;
		}

		/************************************************************************/
		/* Second Step:                                                         */
		/* Judge the edges is needed between same depth                         */
		/************************************************************************/
		for (unsigned int i(0); i != thisDepth; ++i)
		{
			Node &currNode(*nodes[i]);
			NodeToNeedPointInfo &currNodeToNeedPointInfo(currNode.preTreatInfo.find(bgNodeId)->second);

			if (flowDistance[currNode.id] < 0)
			{
				for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
					edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
				{
					Node *anoNode(currIt->second->nodes.second);
					if (nodeDepth[anoNode->id] == depth && flowDistance[anoNode->id] > 0)
					{
						Edge *reverseEdge(currIt->second->reverseEdge);
						reverseEdge->preTreatInfo.insert(std::make_pair(bgNodeId, EdgeToNeedPointInfo()));
						EdgeToNeedPointInfo &reverseEdgeToNeedPointInfo(reverseEdge->preTreatInfo.find(bgNodeId)->second);

						unsigned int flowDistanceUInt(-flowDistance[currNode.id]);
						reverseEdgeToNeedPointInfo.flag = true;
						reverseEdgeToNeedPointInfo.maxFlowToNeedPoint = std::min(std::initializer_list<unsigned int>
							{flowDistanceUInt, reverseEdge->flow, (unsigned int)flowDistance[anoNode->id]});
						reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint =
							currNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint
							+ reverseEdgeToNeedPointInfo.maxFlowToNeedPoint * reverseEdge->costPerFlow;
					}
				}
			}
		}

		for (unsigned int i(0); i != thisDepth; ++i)
		{
			Node &currNode(*nodes[i]);
			NodeToNeedPointInfo &currNodeToNeedPointInfo(currNode.preTreatInfo.find(bgNodeId)->second);

			if (flowDistance[currNode.id] > 0)
			{
				unsigned int flowOutToSameDepth(0);
				std::vector<Edge *> flowOutEdgeOfSameDepth;
				for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
					edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
				{
					EdgeToNeedPointInfo &thisEdgeToNeedPointInfo(currIt->second->preTreatInfo.find(bgNodeId)->second);
					if (thisEdgeToNeedPointInfo.flag)
					{
						flowOutEdgeOfSameDepth.push_back(currIt->second);
						flowOutToSameDepth += thisEdgeToNeedPointInfo.maxFlowToNeedPoint;
					}
				}

				std::sort(flowOutEdgeOfSameDepth.begin(), flowOutEdgeOfSameDepth.end(), 
					[bgNodeId](const Edge const * pEdgeA, const Edge const *pEdgeB)->bool
				{
					return pEdgeA->preTreatInfo.find(bgNodeId)->second.minCostOfMaxFlowToNeedPoint 
						< pEdgeB->preTreatInfo.find(bgNodeId)->second.minCostOfMaxFlowToNeedPoint;
				});

				// allocate the flow and calculate the min cost
				// record in the node pretreat information
			}
		}

		/************************************************************************/
		/* Third Step:                                                          */
		/* Judge the reverse edges                                              */
		/************************************************************************/
		
		for (unsigned int i(0); i != thisDepth; ++i)
		{
			Node &currNode(*nodes[i]);
			NodeToNeedPointInfo &currNodeToNeedPointInfo(currNode.preTreatInfo.find(bgNodeId)->second);

			for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
				edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
			{

			}
		}

		nodes.erase(nodes.begin(), nodes.begin() + thisDepth);
		++depth;
	}
}