#include <iostream>
#include <algorithm>

struct Month {
    std::string name;
    double avg_temp;
};

int main() {
    Month months[] = {
        {"January", -5}, {"February", -2}, {"March", 5},
        {"April", 10}, {"May", 15}, {"June", 20},
        {"July", 25}, {"August", 23}, {"September", 18},
        {"October", 10}, {"November", 5}, {"December", -1}
    };

    int num_months = sizeof(months) / sizeof(months[0]);

    std::sort(months, months + num_months,
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });

    for (const auto& month : months) {
        std::cout << month.name << " (" << month.avg_temp << "°C)\n";
    }

    return 0;
}
