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

    bool addDai(double amount);
    bool addEth(double amount);
    bool removeDai(double amount);
    bool removeEth(double amount);
    bool addLiquidityTokens(double amount);
    bool removeLiquidityTokens(double amount);


    virtual bool addLiquidity(double daiAmount, double ethAmount) = 0;
    virtual bool removeLiquidity(double tokens) = 0;
    virtual double swapDaiToEth(double daiAmount) = 0;
    virtual double swapEthToDai(double ethAmount) = 0;

    virtual ~LiquidityPool() {}
};

#endif
