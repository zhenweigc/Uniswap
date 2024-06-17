#ifndef ARBITRAGE_V2_H
#define ARBITRAGE_V2_H

#include "Arbitrage.h"

#include <cstdint>

class Arbitrage_V2 : public Arbitrage {
public:
	double calculateArbitrage2Pools(uint64_t poolAId, uint64_t poolBId) const;
};

private:
	double calculateArbitrage2Pools(
			const std::weak_ptr<LiquidityPool>& poolA, 
			const std::weak_ptr<LiquidityPool>& poolB) const override;
};
#endif
