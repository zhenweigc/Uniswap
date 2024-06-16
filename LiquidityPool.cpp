#include "LiquidPool.h"

#include <cmath>
#include <stdexcept>


LiquidityPool::LiquidityPool(double initialDai, double initialEth) {
    if (initialDai <= 0 || initialEth <= 0) {
        throw std::invalid_argument("Initial DAI and ETH of a pool must both be positive values.");
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
}
