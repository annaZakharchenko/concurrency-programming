#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class Task {
    public:
    void run(const std::string& threadName) {
        for (int i = 0; i < 5; i++) {
            std::cout << "Thread: " << threadName << "; ID: " << std::this_thread::get_id() << "; Iteration: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }

    void multiplyWithDelay(const std::string& threadName1, int a, int b) {
        std::cout << "Start" << std::endl;
        std::cout << "Thread m: " << threadName1 << "; ID m: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        int result = a * b;

        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Result: " << result << "; ID m: " << std::this_thread::get_id()  << std::endl;
    }

};

int main() {
    Task task;

    std::thread thread1(&Task::run, &task,"A");
    std::thread thread2(&Task::run, &task,"B");

    std::thread thread3(&Task::multiplyWithDelay, &task,"C", 3, 4);

    thread1.join();
    thread2.join();
    thread3.join();
    return 0;

}
