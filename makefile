all: judge_compile.cc judge_runner.cc
	g++ -Wall -std=c++11 -o judge_compile judge_compile.cc
	g++ -Wall -std=c++11 -o judge_runner judge_runner.cc