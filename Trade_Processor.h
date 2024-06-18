#ifndef TRADE_PROCESSOR_H
#define TRADE_PROCESSOR_H

#include "Pool_Manager.h"
#include "Arbitrage_V2.h"
#include "Uniswap_V2.h"

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <memory>

enum class PoolVersion {
    V2,
    V3
};

class Trade_Processor {
public:
    Trade_Processor(PoolVersion version, const std::string& logFile = "");
    ~Trade_Processor();

    void processTrades(const std::vector<std::string>& filepaths);

private:
    void processFile(const std::string& filepath);
    void log(const std::string& message);

    PoolVersion poolVersion;
    std::mutex print_mutex;
    std::unique_ptr<std::ostream> logStream;
    std::ofstream logFileStream;
};

#endif
