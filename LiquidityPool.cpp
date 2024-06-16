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


void LiquidityPool::addDai(double amount) {
    	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
    	std::lock_guard<std::mutex> lock(pool_mutex);
    	dai += amount;
}

void LiquidityPool::addEth(double amount) {
     	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
    	std::lock_guard<std::mutex> lock(pool_mutex);
    	eth += amount;
}

void LiquidityPool::removeDai(double amount) {
     	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > dai) throw std::runtime_error("Not enough DAI available.");
    	dai -= amount;
}

void LiquidityPool::removeEth(double amount) {
     	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > eth) throw std::runtime_error("Not enough ETH available.");
    	eth -= amount;
}

void LiquidityPool::addLiquidityTokens(double amount) {
      	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
	std::lock_guard<std::mutex> lock(pool_mutex);
    	totalLiquidityTokens += amount;
}

void LiquidityPool::removeLiquidityTokens(double amount) {
     	if (amount <= 0) throw std::invalid_argument("Amount should be a positive number."); 
	std::lock_guard<std::mutex> lock(pool_mutex);
    	if (amount > totalLiquidityTokens) throw std::runtime_error("Not enough liquidity tokens available.");
    	totalLiquidity0Tokens -= amount;
}
