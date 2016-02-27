#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <memory>
#include <vector>

class Node;
class Simulation;

class Scenario {
 public:
  enum ScenarioType {
    REAL_WORLD,
    RISK_NEUTRAL,
  };

  Scenario(ScenarioType type, int initial_state,
	   int id, int num_nodes, float interest_rate);
  ~Scenario();

  void PrintNodes() const;
  void PrintLogReturns() const;
  // Compute the aggregated log return for given number of month.
  float GetStockPrice(float initial_price, int month) const;
  int GetState(int month) const;
  float GetLiability(float initial_price, int month, float shock) const;
  int size() const { return nodes_.size(); }
  void GetRealWorldStats(
    float initial_price, int month,
    const std::vector<std::unique_ptr<Simulation>>& rn_simulations,
    float* stock_price, float* hedge, float* delta,
    float* bond) const;

 private:
  const ScenarioType type_;
  const int id_;
  std::vector<std::unique_ptr<Node>> nodes_;
};

#endif  // SCENARIO_H_
