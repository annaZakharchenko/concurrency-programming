#include <iostream>
#include <algorithm>
#include <thread>

struct Month {
    std::string name;
    double avg_temp;
};

void sort_partial(Month* months, int start, int end) {
    std::sort(months + start, months + end,
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });
}

int main() {
    Month months[] = {
        {"January", -5}, {"February", -2}, {"March", 5},
        {"April", 10}, {"May", 15}, {"June", 20},
        {"July", 25}, {"August", 23}, {"September", 18},
        {"October", 10}, {"November", 5}, {"December", -1}
    };

    int num_months = sizeof(months) / sizeof(months[0]);
    int num_threads = 2;
    int part_size = num_months / num_threads;
    std::thread threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? num_months : start + part_size;
        threads[i] = std::thread(sort_partial, months, start, end);
    }

    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    std::sort(months, months + num_months,
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });

    for (const auto& month : months) {
        std::cout << month.name << " (" << month.avg_temp << "°C)\n";
    }

    return 0;
}
