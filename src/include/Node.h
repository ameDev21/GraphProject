#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Node
{
private:
  int m_id;
  void init(const int);

public:
  const static int RADIUS = 30; // default radius
  sf::Text label;
  sf::Font font;
  sf::CircleShape shape;
  Node();
  Node(const int radius);

  void setId(int);
  const int getId();
};
