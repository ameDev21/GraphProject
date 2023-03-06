#pragma once
#include "Node.h"
#include <iostream>
#include <tuple>
#include "Vec2.h"

class Edge {
private:
  int m_cost, m_id;
  std::tuple<Node, Node> m_nodes;

public:
  sf::Vertex line[2]; 
  constexpr static float THICKNESS = 1.2f; // default thickness 

  explicit Edge(const int cost, const Node node1, const Node node2);

  void setId(const int id);
  const int getId();
};
