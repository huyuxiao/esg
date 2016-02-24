#include "simulation.h"

#include <cmath>
#include <iostream>
#include <memory>

#include "constants.h"
#include "scenario.h"

using std::cerr;
using std::endl;
using std::unique_ptr;

Simulation::Simulation() : num_months_(-1) {}

Simulation::~Simulation() {}

void Simulation::AddScenario(const Scenario* scenario) {
  if (num_months_ >= 0 && num_months_ != scenario->size()) {
    cerr << "Existing scenarios have " << num_months_ << " months, while trying"
	 << " to add another with " << scenario->size() << " months." << endl;
    exit(kExitFailure);
  } else if (num_months_ < 0) {
    num_months_ = scenario->size();
  }
  scenarios_.push_back(std::move(unique_ptr<const Scenario>(scenario)));
}

float Simulation::GetAverageAggregatedReturnRate(int steps) const {
  float sum = 0.0;
  int count = 0;
  for (const auto& scenario : scenarios_) {
    sum += exp(scenario->AggregateLogReturn(steps));
    ++count;
  }
  if (count == 0) {
    cerr << "No simulation found." << endl;
    exit(kExitFailure);
  }
  return sum / count;
}

float Simulation::GetAverageLiability(int steps, float shock) const {
  float sum = 0.0;
  int count = 0;
  for (const auto& scenario : scenarios_) {
    sum += scenario->GetLiability(steps, shock);
    ++count;
  }
  if (count == 0) {
    cerr << "No simulation found." << endl;
    exit(kExitFailure);
  }
  return sum / count;
}

void Simulation::PrintScenarios() const {
  for (const auto& scenario : scenarios_) {
    scenario->PrintNodes();
  }
}
