#ifndef UNISWAP_V2_H
#define UNISWAP_V2_H

#include "LiquidityPool.h"

#include <mutex>

class Uniswap_V2 : public LiquidityPool {

private:
	double swapFee;
	std::mutex pool_mutex;

public:
	Uniswap_V2(double initialDai, double initialEth);
	virtual ~Uniswap_V2() {}

    	bool addLiquidity(double daiAmount, double ethAmount) override;
    	bool removeLiquidity(double tokens) override;
    	double swapDaiToEth(double daiAmount) override;
    	double swapEthToDai(double ethAmount) override;

	double getSwapFee() const;
	bool setSwapFee(double fee);
};

#endif
