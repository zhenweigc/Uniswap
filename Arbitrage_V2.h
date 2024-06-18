#ifndef ARBITRAGE_V2_H
#define ARBITRAGE_V2_H

#include "Arbitrage.h"

#include <cstdint>

class Arbitrage_V2 : public Arbitrage {
public:
	std::tuple<double, double> calculateArbitrage2Pools(uint64_t poolAId, uint64_t poolBId) const;

private:
	std::tuple<double, double> calculateArbitrage2Pools(
			const std::weak_ptr<LiquidityPool>& weakPoolA, 
			const std::weak_ptr<LiquidityPool>& weakPoolB) const override;

	double calculateOptimalAmountIn(
			double x_a, double y_a, double x_b, double y_b, double fee) const;

	double calculateProfit(
			double x_a, double y_a, double x_b, double y_b, double fee, double amountIn) const;

};
#endif
