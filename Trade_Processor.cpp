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
    	while (std::getline(file, line)) {
		
		std::istringstream iss(line);
        	std::string token;
        	std::vector<std::string> tokens;

		while (std::getline(iss, token, ',')) {
			tokens.push_back(token);
        	}

		/**
		 * Each line should either have two, three or four tokens
		 * [Pool ID] [Action] [Amount 1] [Amount 2]
		 * [Pool ID] [calculateArbitrage] [Pool ID 2]
		 * [Pool ID] [removePool]
		 * [Pool ID] [addPool] [amount 1] [amount 2] (Pool ID input does not affect what Pool ID it is assigned to)
		 * [Pool ID] [sleep] [msec] (Pool Id is useless here, this action lets the thread to sleep for given msec.)
		 */
        	if (tokens.size() < 2) {
            		log("Malformed line: " + line);
            		continue;
        	}

        	uint64_t poolId = std::stoull(tokens[0]);
        	std::string action = tokens[1];

        	auto poolManager = Pool_Manager::getInstance();
        	auto pool = poolManager->getPool(poolId).lock();

        	if (!pool) {
			log("Invalid pool ID: " + std::to_string(poolId));
            		continue;
        	}

        	std::lock_guard<std::mutex> lock(pool->getMutex());

        	if (action == "addLiquidity" && tokens.size() >= 4) {
	
			double amount1 = std::stod(tokens[2]);
            		double amount2 = std::stod(tokens[3]);
            		pool->addLiquidity(amount1, amount2);
       
	       	} else if (action == "removeLiquidity" && tokens.size() >= 4) {
            		
			double amount1 = std::stod(tokens[2]);
            		double amount2 = std::stod(tokens[3]);
            		pool->removeLiquidity(amount1, amount2);
        
		} else if (action == "swapDaiToEth"  && tokens.size() >= 3) {
            
			double amount1 = std::stod(tokens[2]);
            		double ethOut = pool->swapDaiToEth(amount1);
            		log("Swapped " + std::to_string(amount1) + " DAI for " + std::to_string(ethOut) + " ETH in pool " + std::to_string(poolId));
        
		} else if (action == "swapEthToDai"  && tokens.size() >= 3) {
            		
			double amount1 = std::stod(tokens[2]);
            		double daiOut = pool->swapEthToDai(amount1);
            		log("Swapped " + std::to_string(amount1) + " ETH for " + std::to_string(daiOut) + " DAI in pool " + std::to_string(poolId));
        
		} else if (action == "calculateArbitrage") {

			if (tokens.size() < 3) {
                		log("Malformed line: " + line);
                		continue;
            		}
            		
			uint64_t poolId2 = std::stoull(tokens[2]);
            		Arbitrage_V2 arbitrage;
            		auto result = arbitrage.calculateArbitrage2Pools(poolId, poolId2);
            		log("Arbitrage: Optimal ETH: " + std::to_string(std::get<0>(result)) + ", Profit: " + std::to_string(std::get<1>(result)));
        	
		} else if (action == "addPool") {

			double amount1 = std::stod(tokens[2]);
            		double amount2 = std::stod(tokens[3]);
	     		auto newPool = std::make_shared<Uniswap_V2>(amount1, amount2);
			poolId = poolManager->addPool(newPool);
			log("Added pool " + std::to_string(poolId) + " with " + std::to_string(amount1) + " DAI and " + std::to_string(amount2) + " ETH.");
	
		} else if (action == "removePool") {
			
			auto removedPool = poolManager->removePool(poolId);
            		if (removedPool) {
				log("Removed pool " + std::to_string(poolId));
            		} else {
				log("Failed to remove pool " + std::to_string(poolId) + ": Pool not found.");
            		}
		
		} else if (action == "sleep") {
			
			double amount1 = std::stod(tokens[2]);
			int sleepDuration = static_cast<int>(amount1);
            		log("Sleeping for " + std::to_string(sleepDuration) + " milliseconds.");
            		std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));

		} else {
            
			log("Unknown action: " + action);
        	
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
