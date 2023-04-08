#include "Algorithms/Dijkstra.hpp"
#include <limits>
#include <utility>

PredecessorsAndPathMap Dijkstra::m_predecessors_map;
PriorityQueue Dijkstra::m_priority_queue;
RelaxedNodes Dijkstra::m_relaxed_nodes;

Dijkstra::Dijkstra(Node source) {
  for (auto &e : Graph::getMatrix()) {
    const unsigned cost = ((e.first.ID == source.ID) ? 0 : 1) *
                          std::numeric_limits<unsigned>::max();
    Dijkstra::m_priority_queue.push(std::make_pair(cost, e.first));
    // setting default predecessor to source
    m_predecessors_map[e.first] = std::make_pair(source, cost);
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
  std::cout << "\n"
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
    if (new_path < Dijkstra::m_predecessors_map.at(key).second)
      Dijkstra::m_predecessors_map.at(key).second = new_path;
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
      std::cout << "im (re)pushing node: " << key.ID << std::endl;
    }
}

void Dijkstra::run() {
  // all the debug prints has to be clean up before pushing on git
  std::cout << "running the algo!" << std::endl;
  while (!Dijkstra::m_priority_queue.empty()) {
    std::cout << "relaxing node: " << Dijkstra::m_priority_queue.top().second.ID
              << std::endl;
    Dijkstra::relax(Dijkstra::m_priority_queue.top().second);
  }
}
