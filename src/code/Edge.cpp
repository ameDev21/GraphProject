#include "../include/Edge.h"

Edge::Edge(const int cost, const Node node1, const Node node2)
{
  m_cost = cost;
  m_nodes = std::make_tuple(node1, node2);
  auto vec = node1.shape.getPosition();
  auto vec2 = node2.shape.getPosition();
  // starting node
  line[0].position = vec;
  line[0].color = sf::Color::Red;
  // ending node
  line[1].position = vec2;
  line[1].color = sf::Color::Red;
}

void Edge::setId(const int id) { m_id = id; }

const int Edge::getId() { return m_id; }
