all: judge_compile.cc judge_runner.cc
	g++ -Wall -o judge_compile judge_compile.cc
	g++ -Wall -o judge_runner judge_runner.cc