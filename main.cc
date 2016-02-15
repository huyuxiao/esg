#include <iostream>
#include <string>
#include <vector>

#include "node.h"
#include "scenario.h"

const int kExitSuccess = 0;
const int kExitFailure = 1;

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
  if (type == "rw") {
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
