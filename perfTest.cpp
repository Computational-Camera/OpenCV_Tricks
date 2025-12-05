#include <iostream>
#include <vector>
#include <cstring> // for memcpy
#include <chrono>  // for high_resolution_clock
#include <iomanip> // for setprecision

// Define memory block size: 512 MB
// We use a large size to ensure we bypass the CPU L3 Cache (which is only ~3MB on RK3588)
// Add 'LL' to force 64-bit calculation
const long long DATA_SIZE = 1LL * 512 * 1024 * 1024; //modify 1LL if prefer larger chunk of memory
int main() 
    std::cout << "Allocating " << (DATA_SIZE / 1024 / 1024) << " MB of memory..." << std::endl;

    // Allocate Source and Destination buffers
    // Using generic vectors for automatic memory management
    std::vector<char> src(DATA_SIZE, 1);
    std::vector<char> dst(DATA_SIZE);

    std::cout << "Warming up..." << std::endl;
    // Run once to trigger page faults and warm up the system
    std::memcpy(dst.data(), src.data(), DATA_SIZE);

    std::cout << "Starting Benchmark..." << std::endl;

    // Measure time
    auto start = std::chrono::high_resolution_clock::now();
    
    // Perform the copy
    // We loop 10 times to get a stable average
    int iterations = 10;
    for(int i = 0; i < iterations; i++) {
        std::memcpy(dst.data(), src.data(), DATA_SIZE);
        // Prevent compiler optimization skipping the loop
        if(dst[0] == 0) std::cout << "Error"; 
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration in seconds
    std::chrono::duration<double> elapsed = end - start;
    double total_seconds = elapsed.count();

    // Calculate Total Data Moved (Read + Write)
    // Note: memcpy reads SRC and writes DST, so total bus traffic is 2x size
    double total_bytes = (double)DATA_SIZE * iterations * 2; 
    double bandwidth_gbs = (total_bytes / total_seconds) / (1024 * 1024 * 1024);

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Total Time: " << total_seconds << " s" << std::endl;
    std::cout << "Effective Bandwidth: " << std::fixed << std::setprecision(2) << bandwidth_gbs << " GB/s" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    return 0;
}
