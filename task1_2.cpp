#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <vector>

struct Student {
    std::string name;
    int score;
};

std::mutex mtx;
Student max_student = {"", 0};

void find_max(const std::vector<Student>& students, int start, int end) {
    auto max_local = *std::max_element(students.begin() + start, students.begin() + end,
        [](const Student& s1, const Student& s2) {
            return s1.score < s2.score;
        });

    std::lock_guard<std::mutex> lock(mtx);
    if (max_local.score > max_student.score) {
        max_student = max_local;
    }
}

int main() {
    std::vector<Student> students = {
        {"Alice", 85}, {"Michael", 80}, {"Anna", 95}, {"Maksym", 87}, {"Eva", 95}
    };

    int num_students = students.size();
    int num_threads = 2;
    int part_size = num_students / num_threads;
    std::vector<std::thread> threads(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? num_students : start + part_size;
        threads[i] = std::thread(find_max, std::ref(students), start, end);
    }

    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    std::cout << "Student with highest score:\nName: " << max_student.name
              << "\nScore: " << max_student.score << "\n";

    return 0;
}
