#include "scenario.h"

#include <iostream>

#include "node.h"

const int Scenario::kNumRWNodes = 37;

Scenario::Scenario() : rw_nodes_(kNumRWNodes) {
  for (int i = 1; i < kNumRWNodes; ++i) {
    rw_nodes_[i].TransFrom(rw_nodes_[i - 1]);
  }
}

Scenario::~Scenario() {}

void Scenario::PrintNodes() const {
  for (int i = 0; i < kNumRWNodes; ++i) {
    std::cout << "Node " << i << ":  "
	      << rw_nodes_[i].ESGOutput() << std::endl;
  }
}
