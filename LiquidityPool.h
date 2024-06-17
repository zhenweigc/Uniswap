#ifndef LIQUIDITY_POOL_H
#define LIQUIDITY_POOL_H

class LiquidityPool {
private:
    double dai;    
    double eth;

    void addDai(double amount);
    void addEth(double amount);
    void removeDai(double amount);
    void removeEth(double amount);
    void setDai(double amount);
    void setEth(double amount);

public:
    LiquidityPool(double initialDai, double initialEth);

    double getDai() const;
    double getEth() const;
    double getK() const;

    virtual double swapDaiToEth(double daiAmount) = 0;
    virtual double swapEthToDai(double ethAmount) = 0;

    virtual ~LiquidityPool() {}
};

#endif
