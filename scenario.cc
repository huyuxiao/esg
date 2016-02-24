#include "scenario.h"

#include <iostream>
#include <memory>

#include "constants.h"
#include "node.h"

using std::cerr;
using std::cout;
using std::endl;
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
  for (const auto& node : nodes_) {
    cout << node->ESGOutput() << " ";
  }
  cout << endl;
}

void Scenario::PrintLogReturns() const {
  bool first_run = true;
  for (const auto& node : nodes_) {
    if (first_run) {
      first_run = false;
    } else {
      cout << ",";
    }
    cout << node->GetLogReturn();
  }
  cout << endl;
}

float Scenario::AggregateLogReturn(int steps) const {
  if (steps <= 0 || unsigned(steps) > nodes_.size()) {
    cerr << "There are " << nodes_.size()
	 << " nodes, but trying to access index " << steps << endl;
    exit(kExitFailure);
  }
  float result = 0.0;
  for (int i = 0; i < steps; ++i) {
    result += nodes_[i]->GetLogReturn();
  }
  return result;
}

float Scenario::GetLiability(int steps, float shock) const {
  const float sum_log_return = AggregateLogReturn(steps);
  const float liability = 1.0 - exp(sum_log_return) * shock;
  return -(liability > 0.0 ? liability : 0.0);
}
