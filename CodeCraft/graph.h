#pragma once

#include "lib_io.h"
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>

struct Edge;
struct Node;
struct Graph;

using BoolTable = std::vector<bool>;

struct Edge
{
	Edge() {}
	Edge(unsigned int _flow, unsigned int _costPerFlow, Node * _node1, Node * _node2) :
		flow(_flow), costPerFlow(_costPerFlow), nodes(std::make_pair(_node1, _node2)) {}
	unsigned int id = 0;
	unsigned int flow = 0;
	unsigned int costPerFlow = 0;
	std::pair<Node *, Node *> nodes = std::make_pair(nullptr, nullptr);
};

struct NeedInfo
{
	std::unordered_map<unsigned int, unsigned int> maxNeedToNeedPoint;
	std::unordered_map<unsigned int, unsigned int> ToNeedPoint;
	std::unordered_map<unsigned int, unsigned int> nextPointOfMinCostRouteToNeedPoint;
};

struct Node
{
	Node() {}
	Node(unsigned int _id) : id(_id) {}
	unsigned int id = 0;
	std::unordered_map<int, Edge *> edges;
	
	bool isNeed = false;
	unsigned int need = 0;

	NeedInfo needInfo;
};

struct Graph
{
	Graph() {}
	Graph(char * topo[MAX_EDGE_NUM]);
	BoolTable getNodesBoolTable() const;
	BoolTable getEdgesBoolTable() const;

	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<std::shared_ptr<Edge>> edges;
	unsigned int cost_per_service = 0;

	std::unordered_set<unsigned int> needPoints;
};