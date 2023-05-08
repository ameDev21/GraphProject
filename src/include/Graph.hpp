#pragma once
#include "Node.hpp"
#include <iostream>
#include <map>
#include <vector>

// the graph is classfied as a map to adjacents nodes
// within the cost of the edges out of that node
typedef std::map<Node, std::map<Node, int>> graphMatrix;
class Graph {
private:
  static int m_generated_nodes;
  static graphMatrix graph_matrix;

public:
  static void addNode(Node);

  static void deleteNode(Node);

  static graphMatrix &getMatrix();
};
