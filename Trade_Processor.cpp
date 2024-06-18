#include "Trade_Processor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

Trade_Processor::Trade_Processor(PoolVersion version, const std::string& logFile)
    : poolVersion(version) {
	    if (!logFile.empty()) {
		    logFileStream.open(logFile);
		    if (!logFileStream.is_open()) throw std::runtime_error("Failed to open log file: " + logFile);
		    logStream = std::make_unique<std::ofstream>(std::move(logFileStream));
	    } else {
		    logStream = std::make_unique<std::ostream>(std::cout.rdbuf());
	    }
}

Trade_Processor::~Trade_Processor() {
	if (logFileStream.is_open()) {
		logFileStream.close();
	}
}

void Trade_Processor::processTrades(const std::vector<std::string>& filepaths) {
	if (poolVersion == PoolVersion::V3) {
        	log("Pool Version V3 is not implemented yet.");
        	return;
	}
	
	std::vector<std::thread> threads;

	for (const auto& filepath : filepaths) {
        	threads.emplace_back(&Trade_Processor::processFile, this, filepath);
    	}

	for (auto& thread : threads) {
        	if (thread.joinable()) {
			thread.join();
        	}
    	}
}

void Trade_Processor::processFile(const std::string& filepath) {
	std::ifstream file(filepath);
    	if (!file.is_open()) {
        	log("Failed to open file: " + filepath);
        	return;
    	}
	
    	std::string line;
	/**
 	* Each line should either have two, three or four tokens
        * [Action] [Pool ID(if it needs one)] [amount 1] [amount 2(if needed)]
	* Special Actions:
 	* [Sleep] [time]
	* [calculateArbitrage] [PoolID1] [PoolID2]
	* [addPool] [amount 1] [amount 2]
	* [removePool] [PoolId]
	*/
    	while (std::getline(file, line)) {
		std::istringstream iss(line);
        	std::string token;
        	std::vector<std::string> tokens;

        	while (std::getline(iss, token, ' ')) {
			tokens.push_back(token);
		}

        	if (tokens.empty()) {
            		log("Empty line: " + line);
            		continue;
        	}
		
        	std::string action = tokens[0];

        	auto poolManager = Pool_Manager::getInstance();
        	if (action == "addPool") {
			
			if (tokens.size() != 3) {
                		log("Invalid addPool action: " + line);
                		continue;
            		}
            		double amount1 = std::stod(tokens[1]);
            		double amount2 = std::stod(tokens[2]);

            		auto newPool = std::make_shared<Uniswap_V2>(amount1, amount2);
            		uint64_t poolId = poolManager->addPool(newPool);
			log("Added pool with ID: " + std::to_string(poolId) + ", Initial DAI: " + std::to_string(amount1) + ", Initial ETH: " + std::to_string(amount2));
			
        	} else if (action == "removePool") {
            		
			if (tokens.size() != 2) {
				log("Invalid removePool action: " + line);
                		continue;
            		}
            
			uint64_t poolId = std::stoull(tokens[1]);
            		auto removedPool = poolManager->removePool(poolId);
            		if (removedPool) {
				log("Removed pool with ID: " + std::to_string(poolId));
            		} else {
                		log("Failed to remove pool with ID: " + std::to_string(poolId));
            		}

        	} else if (action == "Sleep") {
            
			if (tokens.size() != 2) {
                		log("Invalid Sleep action: " + line);
                		continue;
            		}
            
			int sleepTime = static_cast<int>(std::stod(tokens[1]));
            
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            		log("Slept for " + std::to_string(sleepTime) + " milliseconds");

        	} else if (action == "calculateArbitrage") {
            		if (tokens.size() != 3) {
				log("Invalid calculateArbitrage action: " + line);
                		continue;
            		}
            		
			uint64_t poolAId = std::stoull(tokens[1]);
            		uint64_t poolBId = std::stoull(tokens[2]);
            		Arbitrage_V2 arbitrage;
            		auto result = arbitrage.calculateArbitrage2Pools(poolAId, poolBId);
            		log("Arbitrage: Optimal ETH: " + std::to_string(std::get<0>(result)) + ", Profit: " + std::to_string(std::get<1>(result)));

        	} else {
            		if (tokens.size() < 3) {
                		log("Invalid action: " + line);
                		continue;
            		}
            		
			uint64_t poolId = std::stoull(tokens[1]);
            		double amount1 = std::stod(tokens[2]);
            		double amount2 = tokens.size() > 3 ? std::stod(tokens[3]) : 0.0;

            		auto pool = poolManager->getPool(poolId).lock();
            		if (!pool) {
				log("Invalid pool ID: " + std::to_string(poolId));
                		continue;
            		}

            		if (action == "addLiquidity") {
				
				if (pool->addLiquidity(amount1, amount2)) {
					log("Added liquidity to pool " + std::to_string(poolId) + ": " + std::to_string(amount1) + " DAI, " + std::to_string(amount2) + " ETH");
                		} else {
                    			log("Failed to add liquidity to pool " + std::to_string(poolId));
                		}
				
			} else if (action == "removeLiquidity") {
                		
				if (pool->removeLiquidity(amount1, amount2)) {
                    			log("Removed liquidity from pool " + std::to_string(poolId) + ": " + std::to_string(amount1) + " DAI, " + std::to_string(amount2) + " ETH");
                		} else {
					log("Failed to remove liquidity from pool " + std::to_string(poolId));
                		}
				
            		} else if (action == "swapDaiToEth") {
                		
				double ethOut = pool->swapDaiToEth(amount1);
                		log("Swapped " + std::to_string(amount1) + " DAI for " + std::to_string(ethOut) + " ETH in pool " + std::to_string(poolId));
            		
			} else if (action == "swapEthToDai") {
                		
				double daiOut = pool->swapEthToDai(amount1);
                		log("Swapped " + std::to_string(amount1) + " ETH for " + std::to_string(daiOut) + " DAI in pool " + std::to_string(poolId));
            		
			} else {
				
				log("Unknown action: " + action);
			
			}
		}
	}
}

/**
 * Printing logs to the designated file.
 */
void Trade_Processor::log(const std::string& message) {
	std::lock_guard<std::mutex> lock(print_mutex);
    	(*logStream) << message << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
        	std::cerr << "Usage: " << argv[0] << " <V2/V3> <logFile> <file1> <file2> ... <fileN>" << std::endl;
		return 1;
	}
	
	std::string versionStr = argv[1];
    	PoolVersion version;
    	if (versionStr == "V2") {
		version = PoolVersion::V2;
    	} else if (versionStr == "V3") {
		std::cerr << "Uniswap V3 not implemented yet." << std::endl;
		version = PoolVersion::V3;
		return 1;
    	} else {
        	std::cerr << "Invalid pool version. Choose 'V2' or 'V3'." << std::endl;
        	return 1;
    	}
	
	std::string logFile = argv[2];
    	std::vector<std::string> filepaths(argv + 3, argv + argc);

    	Trade_Processor processor(version, logFile);
    	processor.processTrades(filepaths);

    	return 0;
}
