#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include "Graph.h"

class Engine
{
private:
  Graph graph;
  Vec2 prev_mousepos;
  bool running = true;
  bool edge_creation_pending = false;
  bool lock_click = false;
  sf::RenderWindow window;
  Node nodes_creation_area;
  Node *current_node;

  sf::CircleShape *pending_node;
  sf::Vertex line_pending[2];

  void run();
  void input();
  void render();
  void movement();
  void draw();

  void quit();
  void createWindow();
  void createNodesCreationArea();
  bool canPickNode(const sf::Vector2f &, const sf::CircleShape &);

  void createNewNode();

public:
  void start();
};
