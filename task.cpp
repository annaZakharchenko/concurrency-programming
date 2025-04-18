#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

using namespace std;

vector<int> parallelQuickSort(vector<int> vec) {
    if (vec.size() <= 1)
        return vec;

    int pivot = vec[vec.size() / 2];
    vector<int> less, greater;

    for (int i = 0; i < vec.size(); ++i) {
        if (i == vec.size() / 2) continue;
        if (vec[i] < pivot)
            less.push_back(vec[i]);
        else
            greater.push_back(vec[i]);
    }

    auto futureLess = async(launch::async, parallelQuickSort, less);
    auto futureGreater = async(launch::async, parallelQuickSort, greater);

    less = futureLess.get();
    greater = futureGreater.get();

    vector<int> result;
    result.reserve(less.size() + 1 + greater.size());
    result.insert(result.end(), less.begin(), less.end());
    result.push_back(pivot);
    result.insert(result.end(), greater.begin(), greater.end());

    return result;
}

int main() {
    vector<int> data = {10, 3, 5, 1, 4, 8, 2, 9, 6, 7};

    cout << "Original array: ";
    for (int x : data) cout << x << " ";
    cout << endl;

    vector<int> sorted = parallelQuickSort(data);

    cout << "Sorted array: ";
    for (int x : sorted) cout << x << " ";
    cout << endl;

    return 0;
}
