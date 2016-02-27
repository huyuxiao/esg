#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cmath>

const int kExitSuccess = 0;
const int kExitFailure = 1;

const int kNumRealWorldSimulations = 1000;
const int kNumRiskNeutralSimulations = 200;

const int kNumMonths = 36;
const int kMaxNumStates = 2;

const float kInterestRate = 0.003;
const float kInitialStockPrice = 1000.0;
const float kGuaranteedValue = 1000.0;
const float kShockInterval = 0.01;

const float kBBSSigma = sqrt(0.0020875);


#endif  // CONSTANTS_H_
