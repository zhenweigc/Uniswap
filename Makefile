CXX = g++
CXXFLAGS = -Wall -g --std=c++17

TARGET = uniswap_simulator
OBJS = Trade_Processor.o LiquidityPool.o Pool_Manager.o Uniswap_V2.o Arbitrage_V2.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

Trade_Processor.o: Trade_Processor.cpp
	$(CXX) $(CXXFLAGS) -c Trade_Processor.cpp

LiquidityPool.o: LiquidityPool.cpp
	$(CXX) $(CXXFLAGS) -c LiquidityPool.cpp

PoolOperationsV2.o: Uniswap_V2.cpp
	$(CXX) $(CXXFLAGS) -c PoolOperationsV2.cpp

Arbitrage_V2.o: Arbitrage_V2.cpp
	$(CXX) $(CXXFLAGS) -c Arbitrage_V2.cpp

Pool_Manager.o: Pool_Manager.cpp
	$(CXX) $(CXXFLAGS) -c Pool_Manager.cpp

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

.PHONY: valgrind
valgrind:
	@echo "Valgrind command will be defined here later"

.PHONY: all
all: clean $(TARGET)
