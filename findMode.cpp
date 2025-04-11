#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <algorithm>

using namespace std;

mutex mtx;

void countOccurrences(const vector<int>& arr, int start, int end, unordered_map<int, int>& globalCount) {
    unordered_map<int, int> localCount;

    for (int i = start; i < end; ++i) {
        localCount[arr[i]]++;
    }

    lock_guard<mutex> lock(mtx);
    for (const auto& pair : localCount) {
        globalCount[pair.first] += pair.second;
    }
}

int findModeParallel(const vector<int>& arr, int numThreads) {
    int n = arr.size();
    vector<thread> threads;
    unordered_map<int, int> globalCount;

    int chunkSize = n / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? n : (i + 1) * chunkSize;

        threads.emplace_back(countOccurrences, cref(arr), start, end, ref(globalCount));
    }

    for (auto& t : threads) {
        t.join();
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
    vector<int> arr = {1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 7, 7, 7, 7, 8, 9, 10};

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

    int mode = findModeParallel(arr, numThreads);
    cout << "Mode of the array is: " << mode << endl;

    return 0;
}
