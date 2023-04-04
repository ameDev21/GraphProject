#include "../include/Graph.hpp"
#include <algorithm>
#include <string>
#include <variant>
#include <vector>

void Graph::addNode(Node node_to_add) {
  m_generated_nodes++;
  node_to_add.setId(m_generated_nodes);
  node_to_add.label.setString(std::to_string(m_generated_nodes));
  std::map<Node, int> adj;
  graph_matrix.emplace(node_to_add, adj);
}

void Graph::deleteNode(Node node_to_delete) {
  // checking through all the nodes in order to see
  // if someone is adj with the one to delete
  for (auto &x : getMatrix()) {
    auto iter = x.second.begin();
    for (; iter != x.second.end();) {
      if (node_to_delete.ID == iter->first.ID) {
        iter = x.second.erase(iter);
      } else {
        ++iter;
      }
    }
  }
  // for (auto &[key, value] : x.second)
  //   if (key.ID == node_to_delete.ID)
  //     x.second.erase(node_to_delete);
  // graph_matrix.at(key).erase(
  //     std::remove_if(graph_matrix.at(key).begin(),
  //     graph_matrix.at(key).end(),
  //                    [&](Node node, int) { return node == node_to_delete;
  //                    }),
  //     graph_matrix.at(key).end());
  //
  graph_matrix.erase(node_to_delete);
}

graphMatrix &Graph::getMatrix() { return graph_matrix; }
