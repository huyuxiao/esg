#include "scenario.h"

#include <iostream>
#include <memory>
#include <vector>

#include "constants.h"
#include "node.h"
#include "simulation.h"

using std::cerr;
using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;

Scenario::Scenario(ScenarioType type, int initial_state, int id,
		   int num_nodes, float interest_rate)
  : type_(type),
    id_(id) {
  Node* prev = new RWNode(initial_state);
  nodes_.push_back(unique_ptr<Node>(prev));
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

float Scenario::GetStockPrice(float initial_price, int month) const {
  if (month < 0 || unsigned(month) > nodes_.size()) {
    cerr << "There are " << nodes_.size()
	 << " nodes, but trying to access index " << month << endl;
    exit(kExitFailure);
  }
  float sum_log_return = 0.0;
  for (int i = 1; i <= month; ++i) {
    sum_log_return += nodes_[i]->GetLogReturn();
  }
  return initial_price * exp(sum_log_return);
}

int Scenario::GetState(int month) const {
  return nodes_[month]->GetState();
}

float Scenario::GetLiability(float initial_price, int month,
			     float shock) const {
  const float liability = kGuaranteedValue -
    GetStockPrice(initial_price, month) * shock;
  return -(liability > 0.0 ? liability : 0.0);
}

void Scenario::GetRealWorldStats(
    float initial_price, int month,
    const vector<unique_ptr<Simulation>>& rn_simulations,
    float* stock_price, float* hedge, float* delta, float* bond) const {
  if (stock_price == nullptr || hedge == nullptr || delta == nullptr ||
      bond == nullptr) {
    cerr << "Null pointer!" << endl;
  }
  if (type_ != REAL_WORLD) {
    cerr << "Only available for REAL_WORLD scenario." << endl;
  }
  const int state = GetState(month);
  const int months_left = kNumMonths - month;
  const Simulation* simulation = rn_simulations[state].get();

  // Compute the four important metrics
  *stock_price = GetStockPrice(initial_price, month);
  const float liability = simulation->GetAverageLiability(
      *stock_price, kNumMonths - month);
  const float shocked_liability = simulation->GetAverageLiability(
      *stock_price, kNumMonths - month, 1.0 - kShockInterval);
  const float total_interest = exp(-kInterestRate * months_left);
  *hedge = liability * total_interest;
  *delta = -(liability - shocked_liability) / (kShockInterval * *stock_price) *
    total_interest;
  *bond = *hedge - *delta * *stock_price;
}
