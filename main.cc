#include <iostream>
#include <vector>

#include "scenario.h"

int main() {
  std::vector<Scenario> scenario(1000);
  for (int i = 0; i < 1000; ++i) {
    std::cout << "Scenario " << i << std::endl;
    scenario[i].PrintNodes();
    std::cout << std::endl;
  }
  return 0;
}
