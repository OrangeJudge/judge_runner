if [ -f judge_compile ]; then
    echo "start running..."
    ./judge_compile $PWD/sample_solution/c.c
fi
if [ -f judge_runner ]; then
    echo "test solution in c"
    ./judge_runner $PWD/sample_problem/1.in
fi