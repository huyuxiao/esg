#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "constants.h"
#include "scenario.h"
#include "simulation.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;

 //cdf of normal distribution
double phi(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
} 
//

int main(int argc, char* argv[]) {
  vector<unique_ptr<Simulation>> rn_simulations;
  for (int initial_state : {0, 1}) {
    Simulation* simulation = new Simulation();
    rn_simulations.push_back(unique_ptr<Simulation>(simulation));
    for (int i = 0; i < kNumRiskNeutralSimulations; ++i) {
      simulation->AddScenario(
          new Scenario(Scenario::RISK_NEUTRAL,  // RISK_NEUTRAL or REAL_WORLD
		       initial_state,           // Initial oode
		       i,                       // Index
		       kNumMonths,              // Number of nodes in scenario
		       kInterestRate));         // Interest rate
    }
    /*
    cout << "Risk neutral simulation with initial state " << initial_state
	 << endl
	 << "assuming initial stock price is " << kInitialStockPrice
	 << endl;
    // simulation->PrintScenarios();
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average stock price for " << i << " month(s): "
	   << simulation->GetAverageStockPrice(kInitialStockPrice, i) << endl;
    }
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average liability for " << i << " month(s): "
	   << simulation->GetAverageLiability(kInitialStockPrice, i) << endl;
    }
    for (int i = 1; i <= kNumMonths; ++i) {
      cout << "Average shocked liability for " << i << " month(s): "
	   << simulation->GetAverageLiability(kInitialStockPrice, i, 0.99)
	   << endl;
    }
    */
  }


 Scenario realworld_scenario(Scenario::REAL_WORLD,
			      0,
			      1,
			      kNumMonths,
			      kInterestRate);
  float sum=0.0f;
  float prev_delta = 0.0, prev_bond = 0.0;
  float prev_delta_BS = 0.0, prev_bond_BS = 0.0;
  for (int i = 0; i <= kNumMonths; ++i) {
    const int state = realworld_scenario.GetState(i);
    float stock_price, hedge, delta, bond;
    realworld_scenario.GetRealWorldStats(kInitialStockPrice, i, rn_simulations,
					 &stock_price, &hedge, &delta, &bond);
    
    //BS formula
    const float d1 = 1/(kBBSSigma*sqrt(kNumMonths-i))*
      (log(stock_price / kGuaranteedValue)+
       (kInterestRate+kBBSSigma*kBBSSigma/2)*(kNumMonths-i));
    const float d2 = d1 - kBBSSigma * sqrt(kNumMonths-i);
    const float delta_BS = - phi(-d1);
    const float bond_BS = phi(-d2)*kGuaranteedValue*
      exp(-kInterestRate*(kNumMonths-i));
    const float hedge_BS = delta_BS * stock_price + bond_BS;
    //
    
 float cash_flow = delta_BS * stock_price + bond_BS -
      prev_delta_BS * stock_price - prev_bond_BS * exp(kInterestRate);

 /*
 float cash_flow = delta * stock_price + bond -
   prev_delta * stock_price - prev_bond * exp(kInterestRate);
 */
  
    if (i == kNumMonths) {
      const float maturity_benefit =
	std::max(kGuaranteedValue - stock_price, 0.0f);
      cash_flow -= maturity_benefit;
    }
    
    const float discounted_cf = cash_flow * exp(-kInterestRate * i);
    sum += discounted_cf;

    /*
    cout << "Month " << i << ", state " << state
	 << ", stock price " << stock_price
	 << ", hedge " << hedge
	 << ", delta " << delta
	 << ", bond " << bond
	 << ", cash flow " << cash_flow << endl;
    */

    cout << "Month " << i << ", state " << state
	 << ", stock price " << stock_price
	 << ", hedge BS " << hedge_BS
	 << ", delta BS " << delta_BS
	 << ", bond BS " << bond_BS
	 << ", cash flow BS " << cash_flow << endl;
   

    prev_delta = delta;
    prev_bond = bond;

    prev_delta_BS = delta_BS;
    prev_bond_BS = bond_BS;
  }
  cout << "Reserve " << sum << endl; 
  return kExitSuccess;
}
