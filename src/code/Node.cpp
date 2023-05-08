#include "../include/Node.hpp"

#include <iostream>

Node::Node() { init(RADIUS); }

Node::Node(const int radius) { init(radius); }

void Node::setId(int id) { ID = id; }

void Node::init(const int radius) {
  shape.setRadius(radius);
  shape.setPosition(radius + 20, radius + 20);
  shape.setOrigin(radius, radius);

  label.setCharacterSize(20);
  label.setPosition(shape.getPosition().x - label.getCharacterSize(),
                    shape.getPosition().y - label.getCharacterSize());
}
