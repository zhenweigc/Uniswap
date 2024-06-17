#ifndef UNISWAP_V2_H
#define UNISWAP_V2_H

#include "LiquidityPool.h"

#include <mutex>

class Uniswap_V2 : public LiquidityPool {

private:
	double swapFee;
	mutable std::mutex pool_mutex; //Mutex for concurrent control of pool

public:
	Uniswap_V2(double initialDai, double initialEth);
	~Uniswap_V2() override {}

    	bool addLiquidity(double daiAmount, double ethAmount) override;
    	bool removeLiquidity(double tokens) override;
    	double swapDaiToEth(double daiAmount) override;
    	double swapEthToDai(double ethAmount) override;

	std::mutex& getMutex() override;

	double getSwapFee() const;
	bool setSwapFee(double fee);
};

#endif
