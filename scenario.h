#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <vector>

class RWNode;

class Scenario {
 public:
  Scenario();
  ~Scenario();

  void PrintNodes() const;

 private:
  static const int kNumRWNodes;
  std::vector<RWNode> rw_nodes_;
};

#endif  // SCENARIO_H_
