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

Uniswap_V2.o: Uniswap_V2.cpp
	$(CXX) $(CXXFLAGS) -c Uniswap_V2.cpp

Arbitrage_V2.o: Arbitrage_V2.cpp
	$(CXX) $(CXXFLAGS) -c Arbitrage_V2.cpp

Pool_Manager.o: Pool_Manager.cpp
	$(CXX) $(CXXFLAGS) -c Pool_Manager.cpp

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

.PHONY: clean_logs
clean_logs:
	rm -f Test/logs.txt

.PHONY: valgrind
valgrind: generate_tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) V2 Tests/logs.txt Tests/Arbitrage_test1_f1.txt Tests/Arbitrage_test1_f2.txt
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) V2 Tests/logs.txt Tests/Arbitrage_test2_f1.txt Tests/Arbitrage_test2_f2.txt Tests/Arbitrage_test2_f3.txt
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) V2 Tests/logs.txt Tests/Multifile_test1_f1.txt Tests/Multifile_test1_f2.txt Tests/Multifile_test1_f3.txt Tests/Multifile_test1_f4.txt
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) V2 Tests/logs.txt Tests/Impossible_action_test1_f1.txt	
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) V2 Tests/logs.txt Tests/Benchmark_non_blocking_1.txt Tests/Benchmark_non_blocking_2.txt Tests/Benchmark_non_blocking_3.txt Tests/Benchmark_non_blocking_4.txt Tests/Benchmark_non_blocking_5.txt Tests/Benchmark_non_blocking_6.txt Tests/Benchmark_non_blocking_7.txt Tests/Benchmark_non_blocking_8.txt Tests/Benchmark_non_blocking_9.txt Tests/Benchmark_non_blocking_10.txt

.PHONY: generate_tests
generate_tests:
	python3 Tests/non_blocking_test_case_generating_script.py

.PHONY: clean_benchmark_tests
clean_benchmark_tests:
	rm -f Tests/Benchmark_non_blocking_*.txt

.PHONY: all
all: clean $(TARGET)
