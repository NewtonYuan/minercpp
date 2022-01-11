echo "Building Miner"
g++ -std=c++17 miner.cpp -I taskflow/taskflow -I PicoSHA2 -O2 -pthread -o miner
echo "Miner compiled. Running"
TF_ENABLE_PROFILER=hw.json ./miner