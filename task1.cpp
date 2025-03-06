#include <iostream>
#include <thread>
#include <chrono>

void multiplyByConstant(std::vector<int> &vec, int startIdx, int endIdx, int constant) {
    for (int i = startIdx; i < endIdx; ++i) {
        vec[i] *= constant;
    }
}

int main() {
    int n;
    int constant;
    int numThreads;

    std::cout << "Enter the number of elements in the vector:";
    std::cin >> n;

    std::cout << "Enter the constant to multiply by:";
    std::cin >> constant;

    std::cout << "Enter the number of threads to use (max " << std::thread::hardware_concurrency() << "):";
    std::cin >> numThreads;

    if (numThreads < 1 || numThreads > std::thread::hardware_concurrency()) {
        std::cerr << "Invalid number of threads! Using max available threads: " << std::thread::hardware_concurrency() << std::endl;
        numThreads = std::thread::hardware_concurrency();
    }

    std::vector<int> vec(n, 1);

    int chunkSize = n / numThreads;
    std::vector<std::thread> threads;


    for (int i = 0; i < numThreads; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = (i == numThreads - 1) ? n : (i + 1) * chunkSize;


        threads.push_back(std::thread(multiplyByConstant, std::ref(vec), startIdx, endIdx, constant));
    }


    for (auto& th : threads) {
        th.join();
    }


    std::cout << "Modified vector: ";
    for (const int& num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
