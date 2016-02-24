#include <iostream>
#include <string>
#include <vector>

#include "constants.h"
#include "node.h"
#include "scenario.h"
#include "simulation.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

void PrintUsage(const char* cmd) {
  cerr << "Usage:" << endl;
  cerr << "    " << cmd << " <type>" << endl;
  cerr << "where <type> can be \"rw\" or \"rn\" or \"rn0\" or \"rn1\"." << endl;
  cerr << "  - rw:  Generate 1000 real-world scenarios" << endl;
  cerr << "  - rn:  Generate 200 risk-neutral scenarios" << endl;
  cerr << "  - rn0: Generate 200x36 risk-neutral log-return matrix from state 0"
       << endl;
  cerr << "  - rn1: Generate 200x36 risk-neutral log-return matrix from state 1"
       << endl;
  cerr << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    PrintUsage(argv[0]);
    return kExitFailure;
  }
  const string type(argv[1]);
  if (type == "test") {
    Simulation risk_neutral_simulation;
    for (int i = 0; i < kNumRiskNeutralSimulations; ++i) {
      risk_neutral_simulation.AddScenario(
	  new Scenario(Scenario::RISK_NEUTRAL,  // RISK_NEUTRAL or REAL_WORLD
		       RWNode(),                // Initial oode
		       i,                       // Index
		       kNumMonths,              // Number of nodes in scenario
		       kInterestRate));         // Interest rate
    }
    risk_neutral_simulation.PrintScenarios();
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average aggregated return rate for " << i << " month(s): "
	   << risk_neutral_simulation.GetAverageAggregatedReturnRate(i) << endl;
    }
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average liability rate for " << i << " month(s): "
	   << risk_neutral_simulation.GetAverageLiability(i) << endl;
    }
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average shocked liability rate for " << i << " month(s): "
	   << risk_neutral_simulation.GetAverageLiability(i, 0.99) << endl;
    }
  } else if (type == "rw") {
    // Generation of risk-neutral scenario.
    for (int i = 0; i < 1000; ++i) {
      Scenario scenario(Scenario::REAL_WORLD,    // RISK_NEUTRAL or REAL_WORLD
			RWNode(),                // Initial oode
			i,                       // Index
			36,                      // Number of nodes in scenario
			0.003);                  // Interest rate
      cout << "Real-World Scenario " << i << endl;
      scenario.PrintNodes();
      cout << endl;
    }
  } else if (type == "rn") {
    // Generation of risk-neutral scenario.
    for (int i = 0; i < 200; ++i) {
      Scenario scenario(Scenario::RISK_NEUTRAL,  // RISK_NEUTRAL or REAL_WORLD
			RWNode(),                // Initial oode
			i,                       // Index
			36,                      // Number of nodes in scenario
			0.003);                  // Interest rate
      cout << "Risk-Neutral Scenario " << i << endl;
      scenario.PrintNodes();
      cout << endl;
    }
  } else if (type == "rn0") {
    // Generation of risk-neutral scenario.
    for (int i = 0; i < 200; ++i) {
      Scenario scenario(Scenario::RISK_NEUTRAL,  // RISK_NEUTRAL or REAL_WORLD
			RWNode(0),               // Initial oode
			i,                       // Index
			36,                      // Number of nodes in scenario
			0.003);                  // Interest rate
      scenario.PrintLogReturns();
    }
  } else if (type == "rn1") {
    // Generation of risk-neutral scenario.
    for (int i = 0; i < 200; ++i) {
      Scenario scenario(Scenario::RISK_NEUTRAL,  // RISK_NEUTRAL or REAL_WORLD
			RWNode(1),               // Initial oode
			i,                       // Index
			36,                      // Number of nodes in scenario
			0.003);                  // Interest rate
      scenario.PrintLogReturns();
    }
  } else {
    PrintUsage(argv[0]);
    return kExitFailure;
  }

  return kExitSuccess;
}
