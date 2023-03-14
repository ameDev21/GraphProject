#include "../include/Engine.h"
#include "../include/Node.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

void Engine::quit() { running = false; }

void Engine::createWindow() {
  const int width = 1536;
  const int height = 864;
  sf::VideoMode video(width, height);
  const std::string title = "GRAPH TOOL";
  window.create(video, title, sf::Style::Default);
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
      case ::sf::Keyboard::D:
        for (auto &[key, val] : graph.getMatrix()) {
          if (canPickNode(sf::Vector2f(sf::Mouse::getPosition(window).x,
                                       sf::Mouse::getPosition(window).y),
                          key.shape)) {
            std::cout << "[INFO]: Deleted Node with Id = " << key.ID
                      << std::endl;
            graph.deleteNode(key);
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
        for (auto &[key, val] : graph.getMatrix()) {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                  key.shape)) {
            lock_click = true;
            this->prev_mousepos = sf::Mouse::getPosition(window);
            auto node = graph.getMatrix().extract(key);
            current_node = &(node.key());
            graph.getMatrix().insert(std::move(node));
            break;
          }
        }
      } break;
      case sf::Mouse::Right: {
        for (auto &[key, val] : graph.getMatrix()) {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                  key.shape)) {
            if (edge_creation_pending) {
              createNewEdge(key);
              edge_creation_pending = false;
            } else {
              auto node = graph.getMatrix().extract(key);
              edge_creation_pending = true;
              pending_node = (&node.key());
              line_pending[0] = pending_node->shape.getPosition();
              auto vec = sf::Mouse::getPosition(window);
              line_pending[1] = sf::Vector2f(vec.x, vec.y);
              std::string str = key.label.getString();
              std::cout << "[INFO]: Started Edge from Node " << str
                        << std::endl;
              graph.getMatrix().insert(std::move(node));
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
    auto node = graph.getMatrix().extract(*current_node);
    node.key().shape.setPosition(current_node->shape.getPosition());
    graph.getMatrix().insert(std::move(node));
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
  graph.addNode(new_node);
  std::cout << "[INFO]: New Node Created" << std::endl;
}

void Engine::createNewEdge(const Node ending_node) {
  std::cout << "[INFO]: Ended Edge to Node " << ending_node.ID << std::endl;
  graph.getMatrix().at(*pending_node).emplace_back(ending_node);
  graph.getMatrix().at(ending_node).emplace_back(*pending_node);
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

void Engine::drawEdgesFrom(Node key, std::vector<Node> values) {
  const sf::Vector2f keyPosition = key.shape.getPosition();
  sf::Text label;
  label.setFont(font);
  sf::Vertex line[2];
  for (auto &e : values) {
    auto node = graph.getMatrix().extract(e);
    if (!node)
      continue;

    const int cost = Vec2::dist(keyPosition, node.key().shape.getPosition());
    line[0].position = keyPosition;
    line[1].position = node.key().shape.getPosition();
    label.setPosition((keyPosition.x + node.key().shape.getPosition().x) / 2,
                      (keyPosition.y + node.key().shape.getPosition().y) / 2);
    label.setString(std::to_string(cost));
    graph.getMatrix().insert(std::move(node));
    window.draw(line, 2, sf::Lines);
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
  } else {
    text.setString("Navigation Mode, Press 'Escape' for Quit ");
    text.setFillColor(sf::Color::Yellow);
    window.draw(text);
  }

  window.draw(nodes_creation_area.shape);
  window.draw(nodes_creation_area.label);
  // draw all nodes
  for (auto &[key, value] : graph.getMatrix()) {
    drawEdgesFrom(key, value);
  }
  for (auto &[key, val] : graph.getMatrix()) {
    window.draw(key.shape);
    window.draw(key.label);
  }
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
