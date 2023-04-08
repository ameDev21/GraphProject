#include "../include/Graph.hpp"
#include <algorithm>
#include <string>
#include <variant>
#include <vector>

graphMatrix Graph::graph_matrix;
int Graph::m_generated_nodes = 0;

void Graph::addNode(Node node_to_add) {
  m_generated_nodes++;
  node_to_add.setId(m_generated_nodes);
  node_to_add.label.setString(std::to_string(m_generated_nodes));
  std::map<Node, int> adj;
  Graph::getMatrix().emplace(node_to_add, adj);
}

void Graph::deleteNode(Node node_to_delete) {
  // checking through all the nodes in order to see
  // if someone is adj with the one to delete
  for (auto &x : Graph::getMatrix()) {
    auto iter = x.second.begin();
    for (; iter != x.second.end();) {
      if (node_to_delete.ID == iter->first.ID) {
        iter = x.second.erase(iter);
      } else {
        ++iter;
      }
    }
  }
  Graph::getMatrix().erase(node_to_delete);
}

graphMatrix &Graph::getMatrix() { return Graph::graph_matrix; }
