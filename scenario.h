#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <vector>

class RNNode;
class RWNode;

class RWScenario {
 public:
  RWScenario(int id);
  ~RWScenario();

  void PrintNodes() const;

 private:
  static const int kNumRWNodes;
  std::vector<RWNode> rw_nodes_;
  const int id_;
};

class RNScenario {
 public:
  RNScenario(int id);
  ~RNScenario();

  void PrintNodes() const;

 private:
  static const int kNumRWNodes;
  std::vector<RNNode> rn_nodes_;
  const int id_;
};

#endif  // SCENARIO_H_
