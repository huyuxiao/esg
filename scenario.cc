#include "scenario.h"

#include <iostream>

#include "node.h"

const int RWScenario::kNumRWNodes = 37;

RWScenario::RWScenario(const int id)
  : rw_nodes_(kNumRWNodes),
    id_(id) {
  for (int i = 1; i < kNumRWNodes; ++i) {
    rw_nodes_[i].TransFrom(rw_nodes_[i - 1]);
  }
}

RWScenario::~RWScenario() {}

void RWScenario::PrintNodes() const {
  for (int i = 0; i < kNumRWNodes; ++i) {
    std::cout << id_ << "," << i << ","
	      << rw_nodes_[i].ESGOutput() << std::endl;
  }
}
