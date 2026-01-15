/*
For Orin Nano 
lscpu | grep "On-line CPU(s)"
cat /sys/kernel/debug/bpmp/debug/clk/emc/rate
sudo nvpmodel -m 8
sudo jetson_clocks

Compile 
g++ -O3 -march=native memtest2.cpp -o memtest2 -lpthread
*/


#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>
#include <algorithm>
#include <sched.h>

// Increase size to 1GB to ensure we stay out of the shared L3 cache across 6 cores
const long long TOTAL_DATA_SIZE = 1024LL * 1024LL * 1024LL; 
const int NUM_THREADS = 6; // Orin Nano has 6 cores
const int ITERATIONS = 20;

std::atomic<bool> start_flag(false);

void benchmark_thread(int thread_id, char* src, char* dst, long long size, double& thread_time) {
    // 1. PIN THREAD TO CORE
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_id, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

    // Warm-up
    std::memcpy(dst, src, size);

    // Synchronize start
    while (!start_flag) { std::this_thread::yield(); }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        std::memcpy(dst, src, size);
        // Prevent compiler optimization
        asm volatile("" : : "g"(dst) : "memory");
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    thread_time = elapsed.count();
}

int main() {
    std::cout << "Starting Multi-threaded Benchmark on " << NUM_THREADS << " cores..." << std::endl;

    long long size_per_thread = TOTAL_DATA_SIZE / NUM_THREADS;
    std::vector<std::vector<char>> src_bufs(NUM_THREADS, std::vector<char>(size_per_thread, 1));
    std::vector<std::vector<char>> dst_bufs(NUM_THREADS, std::vector<char>(size_per_thread));
    std::vector<double> thread_times(NUM_THREADS, 0.0);
    std::vector<std::thread> threads;

    // Launch threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(benchmark_thread, i, src_bufs[i].data(), dst_bufs[i].data(), size_per_thread, std::ref(thread_times[i]));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Let threads settle
    std::cout << "All threads ready. Executing..." << std::endl;
    
    start_flag = true; // Release the hounds

    for (auto& t : threads) { t.join(); }

    // Use the maximum time taken by any thread as the duration for aggregate calculation
    double max_time = *std::max_element(thread_times.begin(), thread_times.end());
    
    // Traffic = Size * Iterations * 2 (Read + Write)
    double total_bytes = (double)TOTAL_DATA_SIZE * ITERATIONS * 2;
    double bandwidth_gbs = (total_bytes / max_time) / (1024.0 * 1024.0 * 1024.0);

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Aggregate Bandwidth: " << std::fixed << std::setprecision(2) << bandwidth_gbs << " GiB/s" << std::endl;
    std::cout << "Estimated Decimal:   " << bandwidth_gbs * 1.074 << " GB/s" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    return 0;
}
