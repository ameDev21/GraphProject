#pragma once
#include "Node.h"
#include <iostream>
#include <map>
#include <vector>

// the graph is classfied as a map to adjacents nodes
// within the cost of the edges out of that node
typedef std::map<Node, std::map<Node, int>> graphMatrix;
class Graph {
private:
  int m_generated_nodes = 0;
  graphMatrix graph_matrix;

public:
  void addNode(Node);

  void deleteNode(Node);

  graphMatrix &getMatrix();
};
