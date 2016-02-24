#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <memory>
#include <vector>

class Node;
class Scenario;

class Simulation {
 public:
  Simulation();
  ~Simulation();

  void AddScenario(const Scenario* scenario);
  float GetAverageAggregatedReturnRate(int steps) const;
  float GetAverageLiability(int steps, float shock = 1.0) const;
  void PrintScenarios() const;

 private:
  std::vector<std::unique_ptr<const Scenario>> scenarios_;
  int num_months_;
};

#endif  // SIMULATION_H_
