#pragma once
#include "Edge.h"
#include "Node.h"
#include <iostream>
#include <vector>

class Graph {
private:
  int m_generated_nodes = 0;
  int m_generated_edges = 0;
  std::vector<Edge> m_edges;
  std::vector<Node> m_nodes;

public:
  Graph();

  void addNode(const Node node_to_add);
  void addEdge(const Edge edge_to_add);

  void deleteNode(const int id);
  void deleteEdge(const int id);

  const int getEdgesCardinality();
  const int getNodesCardinality();
  std::vector<Edge> &getEdges();
  std::vector<Node> &getNodes();
  void printNodes();
};
