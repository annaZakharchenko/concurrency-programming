#include <iostream>
#include <vector>
#include <algorithm>

struct Month {
    std::string name;
    double avg_temp;
};

int main() {
    std::vector<Month> months = {
        {"January", -5}, {"February", -2}, {"March", 5},
        {"April", 10}, {"May", 15}, {"June", 20},
        {"July", 25}, {"August", 23}, {"September", 18},
        {"October", 10}, {"November", 5}, {"December", -1}
    };

    std::sort(months.begin(), months.end(),
        [](const Month& m1, const Month& m2) {
            return m1.avg_temp < m2.avg_temp;
        });

    for (const auto& month : months) {
        std::cout << month.name << " (" << month.avg_temp << "°C)\n";
    }

    return 0;
}
