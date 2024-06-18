#include "Arbitrage_V2.h"
#include "Pool_Manager.h"

#include <cmath>
/**
 * Takes pointers to two pools
 * return optimal value of Eth and Profit(earned Eth)
 */
std::tuple<double, double> Arbitrage_V2::calculateArbitrage2Pools(
    const std::weak_ptr<LiquidityPool>& poolA, 
    const std::weak_ptr<LiquidityPool>& poolB) const {
	
	auto poolA = weakPoolA.lock();
	auto poolB = weakPoolB.lock();

    	if (!poolA || !poolB) {
		return std::make_tuple(0.0, 0.0);    	
	}

	double priceA = poolA->getEth() / poolA->getDai();
    	double priceB = poolB->getEth() / poolB->getDai();

	//We assume that fee rate is the same between pools
	double fee = poolA->getSwapFee();
	double x_a, y_a, x_b, y_b;

	if (priceA == priceB) return std::make_tuple(0.0, 0.0);

	if (priceA > priceB) {
		// ETH worth less in Pool A relative to DAI comparing to that of pool B
		// We should use ETH to exchange for DAI in Pool B, then use that DAI to change back to ETH
		double optimal_val;
		x_a = poolB->getDai();
		y_a = poolB->getEth();
		x_b = poolA->getDai();
		y_b = poolA->getEth();

		optimal_val = calculateOptimalAmountIn(x_a, y_a, x_b, y_b, fee);
		double profit = calculateProfit(x_a, y_a, x_b, y_b, fee, optimal_val);

		return std::make_tuple(optimal_val, profit);
	} else {
		
		double optimal_val;
		x_a = poolA->getDai();
		y_a = poolA->getEth();
		x_b = poolB->getDai();
		y_b = poolB->getEth();

		optimal_val = calculateOptimalAmountIn(x_a, y_a, x_b, y_b, fee);
		double profit = calculateProfit(x_a, y_a, x_b, y_b, fee, optimal_val);

		return std::make_tuple(optimal_val, profit);
	}
}

/**
 * Compute the optimal value of ETH needed
 * x_a: reserve DAI val of first pool(pool with lower ETH/DAI ratio)
 * y_a: reserve ETH val of first pool
 * x_b: reserve DAI val of second pool(the pool where ETH worth less of one DAI)
 * y_b: reserve ETH val of second pool
 * fee1: fee of first pool
 * fee2: fee of the second pool 
 */
double Arbitrage_V2::calculateOptimalAmountIn(
	double x_a, double y_a, double x_b, double y_b, double fee) const {
	
	double k = (1 - fee) * x_b + std::pow((1 - fee), 2.0) * x_a;
	double a = std::pow(k, 2.0);
	double b = 2 * k * y_a * x_b;
    	double c = std::pow((y_a * x_b), 2) - std::pow((1 - fee), 2) * x_a * y_b * y_a * x_b;

    	double discriminant = std::pow(b, 2) - 4 * a * c;
    	if (discriminant < 0) {
        	throw std::runtime_error("Discriminant is negative, no real roots.");
    	}

    	return (-b + std::sqrt(discriminant)) / (2 * a);
}

/**
 * Compute Eth earned in the arbitrage attempt
 */
double Arbitrage_V2::calculateProfit(
		double x_a, double y_a, double x_b, double y_b, double fee, double amountIn) const {

	double DaiOut = (1 - fee) * amountIn * x_a / ((1 - fee) * amountIn + y_a);
	double EthOut = (1 - fee) * Daiout * y_b / ((1 - fee) * Daiout + x_b);
 
	return (EthOut - amountIn);
}

std::tuple<double, double> Arbitrage_V2::calculateArbitrage2Pools(
		uint64_t poolAId,
		uint64_t poolBId) const {
	auto poolManager = Pool_Manager::getInstance();
    	auto poolA = poolManager->getPool(poolAId);
    	auto poolB = poolManager->getPool(poolBId);

    	if (poolA.expired() || poolB.expired()) {
        	return std::make_tuple(0.0, 0.0);
    	}

    	return calculateArbitrage2Pools(poolA, poolB);
}
