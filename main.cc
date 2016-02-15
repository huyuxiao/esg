#include <iostream>
#include <vector>

#include "scenario.h"

int main() {
  for (int i = 0; i < 1000; ++i) {
    RWScenario scenario(i);
    std::cout << "RWScenario " << i << std::endl;
    scenario.PrintNodes();
    std::cout << std::endl;
  }
  return 0;
}
