#include "scenario.h"

#include <iostream>
#include <memory>

#include "node.h"

using std::unique_ptr;

Scenario::Scenario(ScenarioType type, const Node& initial_node, int id,
		   int num_nodes, float interest_rate)
  : id_(id) {
  const Node* prev = &initial_node;
  for (int i = 0; i < num_nodes; ++i) {
    Node* new_node;
    switch (type) {
    case REAL_WORLD:
      new_node = new RWNode();
      break;
    case RISK_NEUTRAL:
      new_node = new RNNode(interest_rate);
      break;
    default:
      break;
    }
    new_node->TransFrom(prev);
    prev = new_node;
    nodes_.push_back(unique_ptr<Node>(new_node));
  }
}

Scenario::~Scenario() {}

void Scenario::PrintNodes() const {
  int idx = 0;
  for (const auto& node : nodes_) {
    std::cout << id_ << "," << idx++ << ","
	      << node->ESGOutput() << std::endl;
  }
}
