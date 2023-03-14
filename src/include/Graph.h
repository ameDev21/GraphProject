#pragma once
#include "Node.h"
#include <iostream>
#include <map>
#include <vector>

typedef std::map<Node, std::vector<Node>> graphMatrix;
class Graph {
private:
  int m_generated_nodes = 0;
  graphMatrix graph_matrix;

public:
  void addNode(Node);

  void deleteNode(Node);

  graphMatrix &getMatrix();
};
