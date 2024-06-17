#include "Arbitrage_V2.h"
#include "Pool_Manager.h"

#include <cmath>

double Arbitrage_V2::calculateArbitrage2Pools(
    const std::weak_ptr<LiquidityPool>& poolA, 
    const std::weak_ptr<LiquidityPool>& poolB) const {

	return 0.0;
}


double Arbitrage_V2::calculateArbitrage2Pools(uint64_t poolAId, uint64_t poolBId) const {
    	auto poolManager = Pool_Manager::getInstance();
    	auto poolA = poolManager->getPool(poolAId);
    	auto poolB = poolManager->getPool(poolBId);

    	if (poolA.expired() || poolB.expired()) {
        	return 0.0;
    	}

    	return calculateArbitrage2Pools(poolA, poolB);
}
