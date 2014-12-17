if [ -f judge_compile ]; then
    rm judge_compile
fi
if [ -f judge_runner ]; then
    rm judge_runner
fi
echo "start making..."
make
if [ -f judge_compile ]; then
    echo "start running..."
    ./judge_compile $PWD/sample_solution/c.c
fi
if [ -f judge_runner ]; then
    echo "test solution in c"
    ./judge_runner $PWD/sample_problem/1.in
fi