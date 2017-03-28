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
	unsigned int edNodeId(g.nodes.size() - 1);
	unsigned int maxNeed(bgNode->need);
	UIntTable nodeDepth(g.getNodesUIntTable());
	IntTable flowDistance(g.getNodesIntTable());

	nodeDepth[bgNodeId] = 2;
	nodeDepth[edNodeId] = 1;
	bgNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
	NodeToNeedPointInfo &bgNodePreTreatInfo(bgNode->preTreatInfo.find(bgNodeId)->second);
	bgNodePreTreatInfo.maxFlowToNeedPoint = maxNeed;
	bgNodePreTreatInfo.minCostOfMaxFlowToNeedPoint = 0;

	std::deque<Node *> nodes;
	unsigned int flowIn(0);
	for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(bgNode->edges.cbegin()),
		edIt(bgNode->edges.cend()); currIt != edIt; ++currIt)
	{
		Edge *reverseEdge(currIt->second->reverseEdge);
		if (reverseEdge == nullptr)
			continue;

		Node *anoNode(currIt->second->nodes.second);
		nodeDepth[anoNode->id] = 3;
		nodes.push_back(anoNode);
		
		reverseEdge->preTreatInfo.insert(std::make_pair(bgNodeId, EdgeToNeedPointInfo()));
		EdgeToNeedPointInfo &reverseEdgeToNeedPointInfo(reverseEdge->preTreatInfo.find(bgNodeId)->second);

		reverseEdgeToNeedPointInfo.flag = true;
		reverseEdgeToNeedPointInfo.maxFlowToNeedPoint = maxNeed > reverseEdge->flow ? reverseEdge->flow : maxNeed;
		reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint =
			reverseEdgeToNeedPointInfo.maxFlowToNeedPoint * reverseEdge->costPerFlow;

		anoNode->preTreatInfo.insert(std::make_pair(bgNodeId, NodeToNeedPointInfo()));
		NodeToNeedPointInfo &anoNodeToNeedPointInfo(anoNode->preTreatInfo.find(bgNodeId)->second);
		anoNodeToNeedPointInfo.BFSDepth = 3;
		anoNodeToNeedPointInfo.maxFlowToNeedPoint = reverseEdgeToNeedPointInfo.maxFlowToNeedPoint;
		anoNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint = reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint;

		flowIn += reverseEdgeToNeedPointInfo.maxFlowToNeedPoint;
	}
	flowDistance[bgNodeId] = flowIn;
	
	unsigned int depth(3);
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
				std::vector<Edge *> flowOutEdgeOfSameDepth;
				for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
					edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
				{
					Node *anoNode(currIt->second->nodes.second);

					if (nodeDepth[anoNode->id] == depth && flowDistance[anoNode->id] < 0)
					{
						EdgeToNeedPointInfo &thisEdgeToNeedPointInfo(currIt->second->preTreatInfo.find(bgNodeId)->second);
						if (thisEdgeToNeedPointInfo.flag)
							flowOutEdgeOfSameDepth.push_back(currIt->second);
					}
				}

				if (!flowOutEdgeOfSameDepth.empty())
				{
					std::sort(flowOutEdgeOfSameDepth.begin(), flowOutEdgeOfSameDepth.end(),
						[bgNodeId](const Edge * const pEdgeA, const Edge * const pEdgeB)->bool
					{
						return pEdgeA->preTreatInfo.find(bgNodeId)->second.minCostOfMaxFlowToNeedPoint
							< pEdgeB->preTreatInfo.find(bgNodeId)->second.minCostOfMaxFlowToNeedPoint;
					});

					unsigned int restFlow(flowDistance[currNode.id]), addedCost(0);
					for (std::vector<Edge *>::const_iterator currIt(flowOutEdgeOfSameDepth.cbegin()),
						edIt(flowOutEdgeOfSameDepth.cend()); currIt != edIt; ++currIt)
					{
						Edge *thisEdge(*currIt);
						EdgeToNeedPointInfo &thisEdgeToNeedPointInfo(thisEdge->preTreatInfo.find(bgNodeId)->second);

						unsigned int usedFlow(0);
						if (restFlow > thisEdgeToNeedPointInfo.maxFlowToNeedPoint)
						{
							usedFlow = thisEdgeToNeedPointInfo.maxFlowToNeedPoint;
							restFlow -= usedFlow;
							addedCost += thisEdge->costPerFlow * usedFlow;
						}
						else 
						{
							usedFlow = restFlow;
							restFlow = 0;
							addedCost += thisEdge->costPerFlow * usedFlow;
							break;
						}
					}
					currNodeToNeedPointInfo.maxFlowToNeedPoint += flowDistance[currNode.id] - restFlow;
					currNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint += addedCost;
				}
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
				Node *anoNode(currIt->second->nodes.second);

				if (nodeDepth[anoNode->id] > depth)
				{
					Edge *reverseEdge(currIt->second->reverseEdge);
					EdgeToNeedPointInfo &reverseEdgeToNeedPointInfo(reverseEdge->preTreatInfo.find(bgNodeId)->second);

					unsigned int flow = currNodeToNeedPointInfo.maxFlowToNeedPoint > reverseEdge->flow ? 
						reverseEdge->flow : currNodeToNeedPointInfo.maxFlowToNeedPoint;
					unsigned int cost = currNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint * flow /
						currNodeToNeedPointInfo.maxFlowToNeedPoint + flow * reverseEdge->costPerFlow;

					reverseEdgeToNeedPointInfo.maxFlowToNeedPoint = flow;
					reverseEdgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint = cost;
				}
			}
		}

		for (unsigned int i(thisDepth), j(nodes.size()); i != j; ++i)
		{
			Node &currNode(*nodes[i]);
			NodeToNeedPointInfo &currNodeToNeedPointInfo(currNode.preTreatInfo.find(bgNodeId)->second);
			std::vector<Edge *> edges;

			for (std::unordered_map<unsigned int, Edge *>::const_iterator currIt(currNode.edges.cbegin()),
				edIt(currNode.edges.cend()); currIt != edIt; ++currIt)
			{
				Node *anoNode(currIt->second->nodes.second);

				if (nodeDepth[anoNode->id] == depth)
					edges.push_back(currIt->second);
			}

			std::sort(edges.begin(), edges.end(), [bgNodeId](const Edge * const pEdgeA, const Edge * const pEdgeB)
			{
				const EdgeToNeedPointInfo &edgeAToNeedPointInfo(pEdgeA->preTreatInfo.find(bgNodeId)->second);
				const EdgeToNeedPointInfo &edgeBToNeedPointInfo(pEdgeB->preTreatInfo.find(bgNodeId)->second);
				return edgeAToNeedPointInfo.minCostOfMaxFlowToNeedPoint / edgeAToNeedPointInfo.maxFlowToNeedPoint <
					edgeBToNeedPointInfo.minCostOfMaxFlowToNeedPoint / edgeBToNeedPointInfo.maxFlowToNeedPoint;
			});

			unsigned int restFlow(maxNeed), totalCost(0);
			for (unsigned int i(0), j(edges.size()); i != j; ++i)
			{
				const EdgeToNeedPointInfo &edgeToNeedPointInfo(edges[i]->preTreatInfo.find(bgNodeId)->second);
				if (restFlow < edgeToNeedPointInfo.maxFlowToNeedPoint)
				{
					totalCost += edgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint 
						* restFlow / edgeToNeedPointInfo.maxFlowToNeedPoint;
					restFlow = 0;
					break;
				}
				totalCost += edgeToNeedPointInfo.minCostOfMaxFlowToNeedPoint;
				restFlow -= edgeToNeedPointInfo.maxFlowToNeedPoint;
			}
			currNodeToNeedPointInfo.maxFlowToNeedPoint = maxNeed - restFlow;
			currNodeToNeedPointInfo.minCostOfMaxFlowToNeedPoint = totalCost;
		}

		nodes.erase(nodes.begin(), nodes.begin() + thisDepth);
		++depth;
	}
}

std::vector<BoolTable> GenerateIntialSolutions::SubFun::generateIntialSolutionsG(const Graph & g)
{
	std::vector<BoolTable> intialSolutions;
	std::unordered_map<unsigned int, unsigned int> flows;
	for (std::unordered_set<unsigned int>::const_iterator currIt(g.needPoints.cbegin()), edIt(g.needPoints.cend());
		currIt != edIt; ++currIt)
		flows.insert(std::make_pair(*currIt, 0));

	std::vector<Node *> nodes(g.nodes.size(), nullptr);
	for (unsigned int i(0), j(nodes.size()); i != j; ++i)
		nodes[i] = g.nodes[i].get();
	std::sort(nodes.begin(), nodes.end(), [](const Node * const pNodeA, const Node * const pNodeB)
	{

	});

	DFSSelectServers(g.getNodesBoolTable(), flows, nodes, intialSolutions);
	return std::move(intialSolutions);
}

void GenerateIntialSolutions::SubFun::DFSSelectServers(BoolTable currTable, std::unordered_map<unsigned int, unsigned int> flows, 
	const std::vector<Node *> &nodes, std::vector<BoolTable>& intialSolutions)
{
}
