#include "LiquidPool.h"

#include <cmath>
#include <stdexcept>

LiquidityPool::LiquidityPool(double initialDai, double initialEth) {
       	if (initialDai <= 0 || initialEth <= 0) {
		throw std::invalid_argument("Initial DAI and ETH of a pool must both be positive values.")
	}
    	dai = initialDai;
    	eth = initialEth;
    	totalLiquidityTokens = sqrt(dai * eth);
}

double LiquidityPool::getDai() const {
    	return dai;
}

double LiquidityPool::getEth() const {
    	return eth;
}

double LiquidityPool::getTotalLiquidityTokens() const {
    	return totalLiquidityTokens;
}

void LiquidityPool::addDai(double amount) {
    	dai += amount;
}

void LiquidityPool::addEth(double amount) {
    	eth += amount;
}

void LiquidityPool::removeDai(double amount) {
    	dai -= amount;
}

void LiquidityPool::removeEth(double amount) {
    	eth -= amount;
}

void LiquidityPool::addLiquidityTokens(double amount) {
    	totalLiquidityTokens += amount;
}

void LiquidityPool::removeLiquidityTokens(double amount) {
    	totalLiquidityTokens -= amount;
}
