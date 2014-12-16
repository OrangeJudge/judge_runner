if [ -f judge_runner ]; then
	rm judge_runner
fi
echo "start compiling..."
make
echo "start running..."
if [ -f judge_runner ]; then
	./judge_runner
fi