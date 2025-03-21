#include <iostream>
#include <algorithm>

struct Student {
    std::string name;
    int score;
};

int main() {
    Student students[] = {
        {"Alice", 85}, {"Michael", 80}, {"Anna", 95}, {"Maksym", 87}, {"Eve", 88}
    };

    auto max_student = std::max_element(std::begin(students), std::end(students),
        [](const Student& s1, const Student& s2) {
            return s1.score < s2.score;
        });

    if (max_student != std::end(students)) {
        std::cout << "Student with highest score:\nName: " << max_student->name
                  << "\nScore: " << max_student->score << "\n";
    }

    return 0;
}
