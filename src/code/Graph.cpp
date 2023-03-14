#include "../include/Graph.h"
#include <algorithm>
#include <string>
#include <variant>
#include <vector>

void Graph::addNode(Node node_to_add) {
  m_generated_nodes++;
  node_to_add.setId(m_generated_nodes);
  node_to_add.label.setString(std::to_string(m_generated_nodes));
  std::vector<Node> adj;
  graph_matrix.emplace(node_to_add, adj);
}

void Graph::deleteNode(Node node_to_delete) {
  for (auto &e : graph_matrix.at(node_to_delete))
    graph_matrix.at(e).erase(
        std::remove_if(graph_matrix.at(e).begin(), graph_matrix.at(e).end(),
                       [&](Node node) { return node == node_to_delete; }),
        graph_matrix.at(e).end());

  graph_matrix.erase(node_to_delete);
}

graphMatrix &Graph::getMatrix() { return graph_matrix; }
