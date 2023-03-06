#include "../include/Graph.h"
#include <algorithm>

Graph::Graph() = default;

void Graph::addNode(Node node_to_add)
{
  m_generated_nodes++;
  node_to_add.setId(m_generated_nodes);
  node_to_add.label.setString("N." + std::to_string(m_generated_nodes));
  m_nodes.emplace_back(node_to_add);
}

void Graph::addEdge(Edge edge_to_add)
{
  ++m_generated_edges;
  edge_to_add.setId(m_generated_edges);
  m_edges.emplace_back(edge_to_add);
}

void Graph::deleteNode(const int id)
{
  m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(),
                               [id](Node node)
                               { return node.getId() == id; }),
                m_nodes.end());
}

void Graph::deleteEdge(const int id)
{
  m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(),
                               [id](Edge edge)
                               { return edge.getId() == id; }),
                m_edges.end());
}

// degubbing function
void Graph::printNodes()
{
  for (auto &e : m_nodes)
    std::cout << e.getId() << std::endl;
}

// this will return the number of the nodes/edges
const int Graph::getEdgesCardinality() { return m_edges.size(); }
const int Graph::getNodesCardinality() { return m_nodes.size(); }

std::vector<Edge> &Graph::getEdges()
{
  return m_edges;
}

std::vector<Node> &Graph::getNodes()
{
  return m_nodes;
}
