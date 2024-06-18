#ifndef UNISWAP_V2_H
#define UNISWAP_V2_H

#include "LiquidityPool.h"

class Uniswap_V2 : public LiquidityPool {

private:
	double swapFee;
	mutable std::mutex pool_mutex; //Mutex for concurrent control of pool

public:
	Uniswap_V2(double initialDai, double initialEth);
	~Uniswap_V2() override = default;

    	bool addLiquidity(double daiAmount, double ethAmount) override;
    	bool removeLiquidity(double daiAmount, double ethAmount) override;
    	double swapDaiToEth(double daiAmount) override;
    	double swapEthToDai(double ethAmount) override;

	std::mutex& getMutex() override;

	double getSwapFee() const override;
	void setSwapFee(double fee) override;
};

#endif
