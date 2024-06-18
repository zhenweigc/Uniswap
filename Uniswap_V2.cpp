#include "Uniswap_V2.h"

Uniswap_V2::Uniswap_V2(double initialDai, double initialEth)
    : LiquidityPool(initialDai, initialEth), swapFee(0.003) {}

bool Uniswap_V2::addLiquidity(double daiAmount, double ethAmount) {
	if (daiAmount <= 0 || ethAmount <= 0) return false;

    	std::lock_guard<std::mutex> lock(pool_mutex);

    	addDai(daiAmount);
    	addEth(ethAmount);
    	return true;
}

bool Uniswap_V2::removeLiquidity(double daiAmount, double ethAmount) {
	if (daiAmount <= 0 || ethAmount <= 0) return false;

    	std::lock_guard<std::mutex> lock(pool_mutex);

    	if (daiAmount > getDai() || ethAmount > getEth()) return false;

    	removeDai(daiAmount);
    	removeEth(ethAmount);
    	return true;
}

/**
 * Different documents has different way of doing the fee calculation
 * Here we will substract the fee from input Amount first, then compute how much will be given to the trader.
 */
double Uniswap_V2::swapDaiToEth(double daiAmount) {
	if (daiAmount <= 0) return 0.0;

    	std::lock_guard<std::mutex> lock(pool_mutex);

    	double daiAfterFee = daiAmount * (1 - swapFee);
    	double newDai = getDai() + daiAfterFee;
	if (newDai == 0) throw std::runtime_error("Error: Attempting to divide by zero in Swapping.");  

    	double newEth = getK() / newDai;
    	double ethOut = getEth() - newEth;

    	if (ethOut <= 0) return 0.0;

    	addDai(daiAmount);
    	setEth(newEth);
	return ethOut;
}

double Uniswap_V2::swapEthToDai(double ethAmount) {
    	if (ethAmount <= 0) return 0.0;

    	std::lock_guard<std::mutex> lock(pool_mutex);

    	double ethAfterFee = ethAmount * (1 - swapFee);
    	double newEth = getEth() + ethAfterFee;
    	if (newEth == 0) throw std::runtime_error("Error: Attempting to divide by zero in Swapping.");  

	double newDai = getK() / newEth;
    	double daiOut = getDai() - newDai;

    	if (daiOut <= 0) return 0.0;

    	addEth(ethAfterFee);
    	setDai(newDai);

    	return daiOut;
}

void Uniswap_V2::setSwapFee(double fee) {
	if (fee < 0) {
	    throw std::invalid_argument("Swap fee must be non-negative.");
    	}
    	if (fee >= 1) {
	    throw std::invalid_argument("Swap fee must be less than 100%.");
    	}
    	swapFee = fee;
}

double Uniswap_V2::getSwapFee() const {
	std::lock_guard<std::mutex> lock(pool_mutex);
	return swapFee;
}

std::mutex& Uniswap_V2::getMutex() {
    return pool_mutex;
}
