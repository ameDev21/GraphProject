#include "../include/Engine.hpp"
#include "../include/Algorithms/Dijkstra.hpp"
#include "../include/Node.hpp"
#include "Graph.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

void Engine::quit() { running = false; }

void Engine::createWindow() {
  const int width = 1936;
  const int height = 1264;
  sf::VideoMode video(width, height);
  const std::string title = "GRAPH TOOL";
  window.create(video, title, sf::Style::Fullscreen);
  std::cout << "[INFO]: Window Created" << std::endl;
}

bool Engine::canPickNode(const sf::Vector2f &mouse_position,
                         const sf::CircleShape &node) {
  const int radius = node.getRadius();
  sf::Vector2f position = node.getPosition();
  if (mouse_position.x > position.x - radius &&
      mouse_position.x < position.x + radius)
    if (mouse_position.y > position.y - radius &&
        mouse_position.y < position.y + radius)
      return true;
  return false;
}

void Engine::run() {
  std::cout << "[INFO]: Running Phase Started" << std::endl;
  while (running) {
    input();
    movement();
    render();
  }
  window.close();
}

void Engine::input() {
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed: {
      quit();
    } break;
    case sf::Event::KeyPressed: {
      switch (event.key.code) {
      case sf::Keyboard::Escape:
        quit();
        break;
      case sf::Keyboard::E:
        on_click = false;
        edge_creation_pending = false;
        break;
      case sf::Keyboard::S: {
        // to delete this is only for debugging
        path_on_click = true;
        Dijkstra(Graph::getMatrix().begin()->first);
        drawShorterPath(
            Dijkstra::getShorterPath(Graph::getMatrix().end()->first));
        break;
      }
      case ::sf::Keyboard::D:
        for (auto &[key, val] : Graph::getMatrix()) {
          if (canPickNode(sf::Vector2f(sf::Mouse::getPosition(window).x,
                                       sf::Mouse::getPosition(window).y),
                          key.shape)) {
            std::cout << "[INFO]: Deleted Node with Id = " << key.ID
                      << std::endl;
            Graph::deleteNode(key);
            break;
          }
        }
        break;
      default:
        break;
      }
    } break;

    case sf::Event::MouseButtonPressed: {
      switch (event.mouseButton.button) {
      case sf::Mouse::Left: {
        if (canPickNode(sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                        nodes_creation_area.shape)) {
          on_click = true;
          break;
        }
        if (on_click) {
          createNewNode();
          break;
        }
        if (path_on_click) {
          for (auto it = Graph::getMatrix().begin();
               it != Graph::getMatrix().end(); ++it)
            if (canPickNode(
                    sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                    it->first.shape)) {
              Dijkstra(it->first);
              path_on_click = false;
              destination_on_click = true;
              break;
            }
        }
        if (destination_on_click) {
          for (auto &[key, value] : Graph::getMatrix())
            if (canPickNode(
                    sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                    key.shape)) {
              drawShorterPath((Dijkstra::getShorterPath(key)));
              destination_on_click = false;
              break;
            }
        }
        for (auto &[key, val] : Graph::getMatrix()) {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                  key.shape)) {
            lock_click = true;
            this->prev_mousepos = sf::Mouse::getPosition(window);
            auto node = Graph::getMatrix().extract(key);
            current_node = &(node.key());
            Graph::getMatrix().insert(std::move(node));
            break;
          }
        }
      } break;
      case sf::Mouse::Right: {
        for (auto &[key, val] : Graph::getMatrix()) {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                  key.shape)) {
            if (edge_creation_pending) {
              if (!(key.ID != pending_node->ID))
                continue;
              createNewEdge(key);
              edge_creation_pending = false;
            } else {
              auto node = Graph::getMatrix().extract(key);
              edge_creation_pending = true;
              pending_node = (&node.key());
              line_pending[0] = pending_node->shape.getPosition();
              auto vec = sf::Mouse::getPosition(window);
              line_pending[1] = sf::Vector2f(vec.x, vec.y);
              std::string str = key.label.getString();
              std::cout << "[INFO]: Started Edge from Node " << str
                        << std::endl;
              Graph::getMatrix().insert(std::move(node));
            }
            break;
          }
        }
      } break;
      default:
        break;
      }
    } break;
    case sf::Event::MouseButtonReleased: {
      switch (event.mouseButton.button) {
      case sf::Mouse::Left:
        lock_click = false;
        break;
      default:
        break;
      }
    } break;
    default:
      break;
    }
  }
}

void Engine::movement() {
  if (lock_click) {
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    sf::Vector2f node_position = current_node->shape.getPosition();
    const int x = node_position.x + (mouse_position.x - this->prev_mousepos.x);
    const int y = node_position.y + (mouse_position.y - this->prev_mousepos.y);
    current_node->shape.setPosition(x, y);
    current_node->label.setPosition(x, y);
    auto node = Graph::getMatrix().extract(*current_node);
    node.key().shape.setPosition(current_node->shape.getPosition());
    Graph::getMatrix().insert(std::move(node));
    this->prev_mousepos = mouse_position;
  }
  if (edge_creation_pending) {
    auto vec = sf::Mouse::getPosition(window);
    line_pending[1] = sf::Vector2f(vec.x, vec.y);
  }
}

void Engine::createNewNode() {
  Node new_node;
  new_node.shape.setFillColor(sf::Color::Transparent);
  new_node.label.setFont(font);
  new_node.shape.setOutlineThickness(Node::THICKNESS);
  const int radius = Node::RADIUS;
  const int margin = 20;
  new_node.shape.setPosition(radius + margin, radius + margin);
  // new_node.shape.setPosition(0, 0);
  this->prev_mousepos = sf::Mouse::getPosition(window);
  new_node.shape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
  new_node.label.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
  Graph::addNode(new_node);
  std::cout << "[INFO]: New Node Created" << std::endl;
}

void Engine::createNewEdge(const Node ending_node) {
  std::cout << "[INFO]: Ended Edge to Node " << ending_node.ID << std::endl;
  Graph::getMatrix().at(*pending_node).emplace(ending_node, 12);
}

void Engine::createNodesCreationArea() {
  const int radius = Node::RADIUS;
  nodes_creation_area.shape.setRadius(radius);
  nodes_creation_area.shape.setOrigin(radius, radius);
  nodes_creation_area.shape.setFillColor(sf::Color::White);
  nodes_creation_area.shape.setTexture(&addTexture);
  const int margin = 20;
  nodes_creation_area.shape.setPosition(radius + margin, radius + margin);
  std::cout << "[INFO]: Nodes Creation Area Created" << std::endl;
}

void Engine::drawEdgesFrom(Node started, std::map<Node, int> adjacents) {
  const sf::Vector2f keyPosition = started.shape.getPosition();
  sf::Text label;
  label.setFont(font);
  sf::Vertex line[2];
  sf::Vector2f size;
  // create an empty shape
  sf::CircleShape arrow;
  // resize it to 3 points
  arrow.setPointCount(3);
  const int radius_arrow = 10;
  arrow.setRadius(radius_arrow);
  arrow.setOrigin(radius_arrow, radius_arrow);
  for (auto &[key, value] : adjacents) {
    auto node = Graph::getMatrix().extract(key);
    if (!node)
      continue;
    Graph::getMatrix().at(started).at(key) =
        Vec2::dist(keyPosition, node.key().shape.getPosition());
    // std::cout << "this is the cost of the edges from " << started.ID
    // << " to: " << key.ID
    // << " is: " << Graph::getMatrix().at(started).at(key) << std::endl;
    const int cost = value;
    line[0].position = keyPosition;
    line[1].position = node.key().shape.getPosition();
    label.setPosition((keyPosition.x + node.key().shape.getPosition().x) / 2,
                      (keyPosition.y + node.key().shape.getPosition().y) / 2);
    const float theta = std::atan2(line[1].position.y - line[0].position.y,
                                   line[1].position.x - line[0].position.x);
    label.setString(std::to_string(cost));
    arrow.setPosition(
        line[1].position.x -
            (node.key().shape.getRadius() + radius_arrow) * cos(theta),
        line[1].position.y -
            (node.key().shape.getRadius() + radius_arrow) * sin(theta));

    line[0].position.x =
        keyPosition.x + (node.key().shape.getRadius() * cos(theta));
    line[0].position.y =
        keyPosition.y + (node.key().shape.getRadius() * sin(theta));
    Graph::getMatrix().insert(std::move(node));
    arrow.setRotation(-30 + (180 / M_PI) * theta);

    line[1].position = arrow.getPosition();
    window.draw(line, 2, sf::Lines);
    window.draw(arrow);
    window.draw(label);
  }
}

void Engine::draw() {
  if (edge_creation_pending) {
    window.draw(line_pending, 2, sf::Lines);
    text.setString("Edge Creation Mode, Press 'E' for Leave ");
    text.setFillColor(sf::Color::Red);
    window.draw(text);
  } else if (on_click) {
    text.setString("Node Creation Mode, Press 'E' for Leave ");
    text.setFillColor(sf::Color::White);
    window.draw(text);
  } else if (path_on_click) {
    text.setString("Selecting shorter path");
    text.setFillColor(sf::Color::Green);
    window.draw(text);
  } else {
    text.setString("Navigation Mode, Press 'Escape' for Quit ");
    text.setFillColor(sf::Color::Yellow);
    window.draw(text);
  }

  window.draw(nodes_creation_area.shape);
  window.draw(nodes_creation_area.label);
  // draw all nodes
  for (auto &[key, value] : Graph::getMatrix())
    drawEdgesFrom(key, value);

  for (auto &[key, val] : Graph::getMatrix()) {
    window.draw(key.shape);
    window.draw(key.label);
  }
}

void Engine::drawShorterPath(std::vector<std::pair<Node, unsigned>>) {
  // TODO(me): implement here the shorter path coloring
  // be careful cause the drawing this time is not in
  // the draw function this could cause some problem
  // in this case change the location of the calling
}

void Engine::render() {
  window.clear(sf::Color::Black);
  draw();
  window.display();
}

void Engine::start() {
  std::cout << "[MAIN]: Engine Started" << std::endl;
  addTexture.loadFromFile("bin/images/addSign.png");
  font.loadFromFile("bin/fonts/tech.ttf");
  text.setFont(font);
  text.setCharacterSize(20);
  createWindow();
  text.setPosition(0, window.getSize().y - text.getCharacterSize() - 5);
  createNodesCreationArea();
  run();
  std::cout << "[MAIN]: Engine Closed" << std::endl;
}
