#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include "LiquidityPool.h"
#include <memory>

class Arbitrage {
public:
	virtual ~Arbitrage() {}
};


private:
	virtual double calculateArbitrage2Pools(
			const std::weak_ptr<LiquidityPool>& poolA, 
        		const std::weak_ptr<LiquidityPool>& poolB) const = 0;

};
#endif
