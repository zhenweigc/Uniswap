#include "LiquidityPool.h"

#include <stdexcept>

LiquidityPool::LiquidityPool(double initialDai, double initialEth) {
       	if (initialDai <= 0 || initialEth <= 0) {
		throw std::invalid_argument("Initial DAI and ETH of a pool must both be positive values.");
	}
    	dai = initialDai;
    	eth = initialEth;
}


double LiquidityPool::getDai() const {
    	return dai;
}

double LiquidityPool::getEth() const {
    	return eth;
}

double LiquidityPool::getK() const {
	return getEth() * getDai();
}

void LiquidityPool::addDai(double amount) {
    	dai += amount;
}

void LiquidityPool::addEth(double amount) {
    	eth += amount;
}

void LiquidityPool::setDai(double amount) {
    	dai = amount;
}

void LiquidityPool::setEth(double amount) {
    	eth = amount;
}

void LiquidityPool::removeDai(double amount) {
	dai -= amount;
}

void LiquidityPool::removeEth(double amount) {
	eth -= amount;
}
