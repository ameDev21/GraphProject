#include "../include/Engine.h"
#include "../include/Node.h"
#include <iostream>

void Engine::quit() { running = false; }

void Engine::createWindow()
{
  const int width = 1536;
  const int height = 864;
  sf::VideoMode video(width, height);
  const std::string title = "GRAPH TOOL";
  window.create(video, title, sf::Style::Default);
  std::cout << "[INFO]: Window Created" << std::endl;
}

bool Engine::canPickNode(const sf::Vector2f &mouse_position, const sf::CircleShape &node)
{
  const int radius = node.getRadius();
  sf::Vector2f position = node.getPosition();
  if (mouse_position.x > position.x - radius &&
      mouse_position.x < position.x + radius)
    if (mouse_position.y > position.y - radius &&
        mouse_position.y < position.y + radius)
      return true;
  return false;
}

void Engine::run()
{
  std::cout << "[INFO]: Running Phase Started" << std::endl;
  while (running)
  {
    input();
    movement();
    render();
  }
  window.close();
}

void Engine::input()
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::Closed:
    {
      quit();
    }
    break;
    case sf::Event::KeyPressed:
    {
      switch (event.key.code)
      {
      case sf::Keyboard::Escape:
        quit();
        break;
      default:
        break;
      }
    }
    break;
    case sf::Event::MouseButtonPressed:
    {
      switch (event.mouseButton.button)
      {
      case sf::Mouse::Left:
      {
        if (canPickNode(
                sf::Vector2f(event.mouseButton.x, event.mouseButton.y), nodes_creation_area.shape))
        {
          createNewNode();
          lock_click = true;
          this->prev_mousepos = sf::Mouse::getPosition(window);
        }
        for (auto &n : graph.getNodes())
        {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y), n.shape))
          {
            lock_click = true;
            this->prev_mousepos = sf::Mouse::getPosition(window);
            current_node = &n;
            break;
          }
        }
      }
      break;
      case sf::Mouse::Right:
      {
        for (auto &n : graph.getNodes())
        {
          if (canPickNode(
                  sf::Vector2f(event.mouseButton.x, event.mouseButton.y), n.shape))
          {
            if (edge_creation_pending)
            {
              // createNewEdge();
              edge_creation_pending = false;
            }
            else
            {
              edge_creation_pending = true;
              pending_node = &(n.shape);
              line_pending[0] = pending_node->getPosition();
              auto vec = sf::Mouse::getPosition(window);
              line_pending[1] = sf::Vector2f(vec.x, vec.y);
              std::string str = n.label.getString();
              std::cout
                  << "[INFO]: Started Edge from Node " << str << std::endl;
            }
            break;
          }
        }
      }
      break;
      default:
        break;
      }
    }
    break;
    case sf::Event::MouseButtonReleased:
    {
      switch (event.mouseButton.button)
      {
      case sf::Mouse::Left:
        lock_click = false;
        break;
      default:
        break;
      }
    }
    break;
    default:
      break;
    }
  }
}

void Engine::movement()
{
  if (lock_click)
  {
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    sf::Vector2f node_position = current_node->shape.getPosition();
    const int x = node_position.x + (mouse_position.x - this->prev_mousepos.x);
    const int y = node_position.y + (mouse_position.y - this->prev_mousepos.y);
    current_node->shape.setPosition(x, y);
    current_node->label.setPosition(x, y);
    this->prev_mousepos = mouse_position;
  }
  if (edge_creation_pending)
  {
    auto vec = sf::Mouse::getPosition(window);
    line_pending[1] = sf::Vector2f(vec.x, vec.y);
  }
}

void Engine::createNewNode()
{
  Node new_node;
  new_node.shape.setFillColor(sf::Color::Red);
  const int radius = new_node.shape.getRadius();
  const int margin = 20;
  new_node.shape.setPosition(radius + margin, radius + margin);
  graph.addNode(new_node);
  current_node = &(graph.getNodes().back());
  std::cout << "[INFO]: New Node Created" << std::endl;
}

void Engine::createNodesCreationArea()
{
  const int radius = Node::RADIUS;
  nodes_creation_area.shape.setRadius(radius);
  nodes_creation_area.shape.setOrigin(radius, radius);
  nodes_creation_area.shape.setFillColor(sf::Color::White);
  const int margin = 20;
  nodes_creation_area.shape.setPosition(radius + margin, radius + margin);
  std::cout << "[INFO]: Nodes Creation Area Created" << std::endl;
}

void Engine::draw()
{
  if (edge_creation_pending)
    window.draw(line_pending, 2, sf::Lines);
  window.draw(nodes_creation_area.shape);
  window.draw(nodes_creation_area.label);
  // draw all nodes
  for (const auto &n : graph.getNodes())
  {
    window.draw(n.shape);
    window.draw(n.label);
  }
}

void Engine::render()
{
  window.clear(sf::Color::Black);
  draw();
  window.display();
}

void Engine::start()
{
  std::cout << "[MAIN]: Engine Started" << std::endl;
  createWindow();
  createNodesCreationArea();
  run();
  std::cout << "[MAIN]: Engine Closed" << std::endl;

  // sf::Font font;
  // sf::Text label;
  // if (!font.loadFromFile("aldo.ttf"))
  // {
  //   std::cerr << "vabbe'" << std::endl;
  // }
  // label.setPosition(100, 100);
  // label.setFont(font);
  // label.setCharacterSize(20);
  // label.setFillColor(sf::Color::Green);
  // label.setString("Hello, World");
}
