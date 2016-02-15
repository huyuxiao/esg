#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <memory>
#include <vector>

class Node;

class Scenario {
 public:
  enum ScenarioType {
    REAL_WORLD,
    RISK_NEUTRAL,
  };

  Scenario(ScenarioType type, const Node& initial_node,
	   int id, int num_nodes, float interest_rate);
  ~Scenario();

  void PrintNodes() const;

 private:
  const int id_;
  std::vector<std::unique_ptr<Node>> nodes_;
};

#endif  // SCENARIO_H_
