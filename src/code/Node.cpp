#include "../include/Node.h"

Node::Node()
{
  // shape.setRadius(RADIUS);
  // shape.setOrigin(RADIUS, RADIUS);
  init(RADIUS);
}

Node::Node(const int radius)
{
  init(radius);
}

void Node::setId(int id) { m_id = id; }

void Node::init(const int radius)
{
  font.loadFromFile("fonts/iosevka.ttf");
  shape.setRadius(radius);
  shape.setPosition(radius + 20, radius + 20);
  shape.setOrigin(radius, radius);
  label.setPosition(shape.getPosition());
  label.setFont(font);
  std::cout << label.getPosition().x << " " << label.getPosition().y << std::endl;
  label.setCharacterSize(20);
  label.setFillColor(sf::Color::Green);
  label.setString("Hello, World");
}

const int Node::getId() { return m_id; }
