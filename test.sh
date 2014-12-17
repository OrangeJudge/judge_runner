if [ -f judge_runner ]; then
	rm judge_runner
fi
echo "start compiling..."
make
if [ -f judge_runner ]; then
    echo "start running..."
    echo "test solution in c"
	./judge_runner $PWD/sample_solution/c.c
fi