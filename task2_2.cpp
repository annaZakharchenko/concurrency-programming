#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

struct Month {
    std::string name;
    double avg_temp;
};

void sort_partial(std::vector<Month>& months, int start, int end) {
    std::sort(months.begin() + start, months.begin() + end,
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });
}

int main() {
    std::vector<Month> months = {
        {"January", -5}, {"February", -2}, {"March", 5},
        {"April", 10}, {"May", 15}, {"June", 20},
        {"July", 25}, {"August", 23}, {"September", 18},
        {"October", 10}, {"November", 5}, {"December", -1}
    };

    int num_threads = 2;
    int part_size = months.size() / num_threads;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? months.size() : start + part_size;
        threads.emplace_back(sort_partial, std::ref(months), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::sort(months.begin(), months.end(),
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });

    for (const auto& month : months) {
        std::cout << month.name << " (" << month.avg_temp << "°C)\n";
    }

    return 0;
}
