#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Node {
private:
  void init(const int);

public:
  const static int RADIUS = 30; // default radius
  int ID;
  sf::Text label;
  sf::CircleShape shape;
  Node();
  Node(const int radius);
  constexpr static float THICKNESS = 1.2f; // default thickness

  void setId(int);
  inline bool operator<(const Node &rhs) const { return ID < rhs.ID; }
  inline bool operator==(const Node &rhs) { return ID == rhs.ID; }
};
