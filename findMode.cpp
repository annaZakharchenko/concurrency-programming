#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <algorithm>

using namespace std;

mutex mtx;

void countOccurrences(const vector<int>& arr, int start, int end, unordered_map<int, int>& local_count) {
    for (int i = start; i < end; ++i) {
        local_count[arr[i]]++;
    }
}

int findMode(const vector<int>& arr, int numThreads) {
    int n = arr.size();
    vector<thread> threads;
    vector<unordered_map<int, int>> localCounts(numThreads);

    int chunkSize = n / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? n : (i + 1) * chunkSize;
        threads.emplace_back(countOccurrences, cref(arr), start, end, ref(localCounts[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    unordered_map<int, int> globalCount;
    for (const auto& local : localCounts) {
        for (const auto& entry : local) {
            lock_guard<mutex> lock(mtx);
            globalCount[entry.first] += entry.second;
        }
    }

    int mode = arr[0];
    int maxCount = globalCount[mode];
    for (const auto& entry : globalCount) {
        if (entry.second > maxCount) {
            mode = entry.first;
            maxCount = entry.second;
        }
    }

    return mode;
}

int main() {
    vector<int> arr = {1, 2, 3, 3, 4, 4, 4, 5, 5, 6, 7, 7, 7, 7, 8, 9, 10};

    cout << "Array contents: ";
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;

    int numThreads;
    cout << "Enter the number of threads to use (max " << thread::hardware_concurrency() << "): ";
    cin >> numThreads;

    if (numThreads < 1 || numThreads > thread::hardware_concurrency()) {
        cerr << "Invalid number of threads! Using max available threads: " << thread::hardware_concurrency() << endl;
        numThreads = thread::hardware_concurrency();
    }

    int mode = findMode(arr, numThreads);
    cout << "Mode of the array is: " << mode << endl;

    return 0;
}
