#pragma once
#include "Graph.hpp"
#include <map>
#include <queue>
#include <utility>
#include <vector>

// perhaps having a class Algorthms is not a bad idea
// in case of future implementations

struct MyComparator {
  bool operator()(const std::pair<unsigned, Node> &a,
                  const std::pair<unsigned, Node> &b) {
    return a.first > b.first;
  }
};

typedef std::map<Node, std::pair<Node, unsigned>> PredecessorsAndPathMap;

typedef std::priority_queue<std::pair<unsigned, Node>,
                            std::vector<std::pair<unsigned, Node>>,
                            MyComparator>
    PriorityQueue;

typedef std::map<Node, bool> RelaxedNodes;

class Dijkstra {
private:
  static PriorityQueue m_priority_queue;
  static PredecessorsAndPathMap m_predecessors_map;
  static RelaxedNodes m_relaxed_nodes;

  static void relax(Node);
  static void updateHeap();
  static void run();
  // static void resetAll();
public:
  explicit Dijkstra(Node);
  static std::vector<std::pair<Node, unsigned>> getShorterPath(Node);
};
