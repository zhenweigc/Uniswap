#ifndef LIQUIDITY_POOL_H
#define LIQUIDITY_POOL_H

#include <mutex>

class LiquidityPool {
private:
    double dai;    
    double eth;
    double totalLiquidityTokens; // Total liquidity tokens issued for the pool
    mutable std::mutex pool_mutex;  // Mutex for concurrent access control

public:
    LiquidityPool(double initialDai, double initialEth);

    double getDai() const;
    double getEth() const;
    double getTotalLiquidityTokens() const;

    void addDai(double amount);
    void addEth(double amount);
    void removeDai(double amount);
    void removeEth(double amount);
    void addLiquidityTokens(double amount);
    void removeLiquidityTokens(double amount);
};

#endif
