#ifndef POOL_MANAGER_H
#define POOL_MANAGER_H

#include <unordered_map>
#include <memory>
#include <mutex>
#include <stack>
#include <limits>

#include "LiquidityPool.h"

/**
 * Pool Manager will manage a list of pools, where Pools are labelled with a UINT_64t ID.
 * Pool Manager itself uses singleton pattern because there should be only one such instance.
 */
class Pool_Manager {
private:
	std::unordered_map<uint64_t, std::shared_ptr<LiquidityPool>> pools;
    	std::stack<uint64_t> reusableIds; // Stack to store reusable IDs
    	static std::unique_ptr<Pool_Manager> instance;
    	static std::mutex mutex;
    	uint64_t nextPoolId = 0;

    	Pool_Manager() = default;

public:
    	static Pool_Manager* getInstance();

    	uint64_t addPool(std::shared_ptr<LiquidityPool> pool);
    	std::weak_ptr<LiquidityPool> getPool(uint64_t id);
    	std::shared_ptr<LiquidityPool> removePool(uint64_t id);

    	Pool_Manager(const Pool_Manager&) = delete;
    	Pool_Manager& operator=(const Pool_Manager&) = delete;
};

#endif
