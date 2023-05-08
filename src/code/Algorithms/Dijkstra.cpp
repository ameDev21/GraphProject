#include "Algorithms/Dijkstra.hpp"
#include <limits>
#include <ostream>
#include <tuple>
#include <utility>

PredecessorsAndPathMap Dijkstra::m_predecessors_map;
PriorityQueue Dijkstra::m_priority_queue;
RelaxedNodes Dijkstra::m_relaxed_nodes;
Node Dijkstra::m_destination_node;

Dijkstra::Dijkstra(const Node source) {
  for (auto &e : Graph::getMatrix()) {
    const unsigned cost = ((e.first.ID == source.ID) ? 0 : 1) *
                          std::numeric_limits<unsigned>::max();
    Dijkstra::m_priority_queue.push(std::make_pair(cost, e.first));
    // setting default predecessor to -1
    m_predecessors_map[e.first] = std::make_pair(NULL, cost);
    m_predecessors_map.at(e.first).first.ID = -1;
    m_relaxed_nodes[e.first] = false;
  }
  std::cout << "\n";
  // relaxation and updating testing
  // Dijkstra::relax(Dijkstra::m_priority_queue.top().second);
  // prints for debugging
  // std::cout << "priority queue after the update" << std::endl;
  // while (!Dijkstra::m_priority_queue.empty()) {
  //   std::cout << "(" << Dijkstra::m_priority_queue.top().first << ", "
  //             << Dijkstra::m_priority_queue.top().second.ID << ") ";
  //   Dijkstra::m_priority_queue.pop();
  // }
  Dijkstra::run();
  for (auto &[key, value] : m_predecessors_map)
    std::cout << key.ID << " -> " << value.first.ID << " <-> " << value.second
              << std::endl;
  std::cout << "this is the shorter path to the node: "
            << std::prev(Dijkstra::m_predecessors_map.end())->first.ID
            << std::endl;
  auto prendecessors =
      Dijkstra::getShorterPath(std::prev(m_predecessors_map.end())->first);
  for (auto &e : prendecessors)
    std::cout << " node-> " << e.first.ID << " to";
  std::cout << " source "
            << "\n"
            << "predecessor map: ";
  // running the algorithm
  for (auto &[key, value] : Dijkstra::m_predecessors_map)
    std::cout << "(" << value.second << ", " << key.ID << ") ";
  std::cout << "\n";
}

// void Dijkstra::resetAll() {
//   while (!Dijkstra::m_priority_queue.empty()) {
//     Dijkstra::m_priority_queue.pop();
//   }
//   Dijkstra::m_predecessors_map.clear();
// }

void Dijkstra::relax(Node node) {
  m_relaxed_nodes.at(node) = true;
  for (auto &[key, value] : Graph::getMatrix().at(node)) {
    const unsigned new_path = Dijkstra::m_predecessors_map.at(node).second +
                              Graph::getMatrix().at(node).at(key);
    if (new_path < Dijkstra::m_predecessors_map.at(key).second) {
      Dijkstra::m_predecessors_map.at(key).second = new_path;
      Dijkstra::m_predecessors_map.at(key).first = node;
    }
  }
  // updating the priority_queue in case of path modification
  Dijkstra::updateHeap();
}

void Dijkstra::updateHeap() {
  // popping all the elements
  while (!Dijkstra::m_priority_queue.empty())
    Dijkstra::m_priority_queue.pop();
  // (re)pushing all the elements updated
  for (auto &[key, value] : Dijkstra::m_predecessors_map)
    if (!(m_relaxed_nodes.at(key))) {
      Dijkstra::m_priority_queue.push(std::make_pair(value.second, key));
      std::cout << "i'm (re)pushing node: " << key.ID << std::endl;
    }
}

void Dijkstra::run() {
  // all the debug prints has to be clean up before pushing on git
  std::cout << "running Dijkstra!" << std::endl;
  while (!Dijkstra::m_priority_queue.empty()) {
    std::cout << "relaxing node: " << Dijkstra::m_priority_queue.top().second.ID
              << std::endl;
    Dijkstra::relax(Dijkstra::m_priority_queue.top().second);
  }
}

std::vector<std::pair<Node, unsigned>> Dijkstra::getShorterPath(Node node) {
  std::cout << "the node from where we want the predecessors by is: " << node.ID
            << std::endl;
  std::cout << "the node that we want the shorter path has prendecessor: "
            << m_predecessors_map.at(node).first.ID << std::endl;
  std::cout << "this ought to be 0 " << m_predecessors_map.at(node).first.ID
            << std::endl;
  // inf distance case
  if (m_predecessors_map.at(node).first.ID == -1)
    return {};

  std::vector<std::pair<Node, unsigned>> predecessors_path;
  // take a look at this weight
  predecessors_path.push_back(std::make_pair(node, 0));
  predecessors_path.push_back(Dijkstra::m_predecessors_map.at(node));
  while (Dijkstra::m_predecessors_map.at(predecessors_path.back().first).second)
    predecessors_path.push_back(
        Dijkstra::m_predecessors_map.at(predecessors_path.back().first));
  std::cout << "here we have finished! " << std::endl;
  return predecessors_path;
}

void Dijkstra::setDestination(const Node destination) {
  Dijkstra::m_destination_node = destination;
}

Node Dijkstra::getDestination() { return Dijkstra::m_destination_node; }
