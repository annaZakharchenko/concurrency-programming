#include <iostream>
#include <algorithm>
#include <vector>

struct Student {
    std::string name;
    int score;
};

int main() {
    std::vector<Student> students = {
        {"Alice", 85}, {"Michael", 80}, {"Anna", 95}, {"Maksym", 87}, {"Eve", 88}
    };

    auto max_student = std::max_element(students.begin(), students.end(),
        [](const Student& s1, const Student& s2) {
            return s1.score < s2.score;
        });

    if (max_student != students.end()) {
        std::cout << "Student with highest score:\nName: " << max_student->name
                  << "\nScore: " << max_student->score << "\n";
    }

    return 0;
}
