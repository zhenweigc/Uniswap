#include "Pool_Manager.h"
#include "Uniswap_V2.h"

std::unique_ptr<Pool_Manager> Pool_Manager::instance = nullptr;
std::mutex Pool_Manager::mutex;

Pool_Manager* Pool_Manager::getInstance() {
	std::lock_guard<std::mutex> lock(mutex);
    	if (!instance) {
        	instance = std::unique_ptr<Pool_Manager>(new Pool_Manager());
    	}
    	return instance.get();
}

uint64_t Pool_Manager::addPool(std::shared_ptr<LiquidityPool> pool) {
    	std::lock_guard<std::mutex> lock(mutex);

    	uint64_t poolId;
    	if (!reusableIds.empty()) {
        	poolId = reusableIds.top();
        	reusableIds.pop();
    	} else {
        	poolId = nextPoolId++;
    	}

    	pools[poolId] = std::move(pool);
    	return poolId;
}

/**
 * Use Weak_ptr to avoid dangling ptr issue.
 * A thread can obtain a ptr to a pool, stay idle, and the pool is destroyed by another thread
 */
std::weak_ptr<LiquidityPool> Pool_Manager::getPool(uint64_t id) {
    	std::lock_guard<std::mutex> lock(mutex);
    	auto it = pools.find(id);
    	if (it != pools.end()) {
        	return it->second;
    	}
    	return std::weak_ptr<LiquidityPool>();
}

std::shared_ptr<LiquidityPool> Pool_Manager::removePool(uint64_t id) {
    	std::lock_guard<std::mutex> lock(mutex);
    	auto it = pools.find(id);
    	if (it != pools.end()) {
        	std::shared_ptr<LiquidityPool> pool = it->second;
        	if (pool) {
            	std::lock_guard<std::mutex> pool_lock(pool->getMutex());
            	pools.erase(it);
            	reusableIds.push(id);
            	return pool;
        	}
    	}
    	return nullptr;
}
