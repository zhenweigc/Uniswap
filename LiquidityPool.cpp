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
    	std::lock_guard<std::mutex> lock(pool_allow_mutex);
    	return dai;
}

double LiquidityPool::getEth() const {
    	std::lock_guard<std::mutex> lock(pool_mutex);
    	return eth;
}

double LiquidityPool::getTotalLiquidityTokens() const {
    	std::lock_guard<std::mutex> lock(pool_mutex);
    	return totalLiquidityTokens;


bool LiquidityPool::addDai(double amount) {
    	if (amount <= 0) return false;
    	std::lock_guard<std::mutex> lock(pool_mutex);
    	dai += amount;
	return true;
}

bool LiquidityPool::addEth(double amount) {
     	if (amount <= 0) return false;
	std::lock_guard<std::mutex> lock(pool_mutex);
    	eth += amount;
	return true;
}

bool LiquidityPool::removeDai(double amount) {
     	if (amount <= 0) return false;
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > dai) return false;
    	dai -= amount;
	return true;
}

bool LiquidityPool::removeEth(double amount) {
     	if (amount <= 0) return false;
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > eth) return false;
    	eth -= amount;
	return true;
}

bool LiquidityPool::addLiquidityTokens(double amount) {
      	if (amount <= 0) return false;
	std::lock_guard<std::mutex> lock(pool_mutex);
    	totalLiquidityTokens += amount;
	return true;
}

void LiquidityPool::removeLiquidityTokens(double amount) {
     	if (amount <= 0) return false;
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > totalLiquidityTokens) return false;
    	totalLiquidityTokens -= amount;
	return true;
}
