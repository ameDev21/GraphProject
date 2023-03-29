#pragma once
#include "Graph.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>

class Engine {
private:
  Graph graph;
  Vec2 prev_mousepos;
  bool running = true;
  bool edge_creation_pending = false;
  bool lock_click = false;
  bool on_click = false;
  sf::RenderWindow window;
  sf::Font font;
  sf::Text text;
  Node nodes_creation_area;
  Node *current_node;
  sf::Texture addTexture;

  Node *pending_node;
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
  void createNewEdge(const Node);

  void drawEdgesFrom(Node, std::map<Node, int>);

public:
  void start();
};
