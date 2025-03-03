#include <iostream>
#include <thread>
#include <chrono>

const int MULTIPLIER = 2;

void multiplyWithDelay(int number, const std::string& threadName) {
    for (int i = 1; i < 2; i++) {
        std::cout << "\nStart: " << i << " Thread: " << threadName << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        int result = number * MULTIPLIER;

        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Result: " << result << std::endl;
        std::cout << "Finish: " << i << " Thread: " << threadName << std::endl;
    }
}

int main() {
    int number = 10;

    std::thread thread1(multiplyWithDelay, number, "A");
    thread1.join();

    std::thread thread2(multiplyWithDelay, number, "B");

    std::thread thread3(multiplyWithDelay, number, "C");
    thread3.detach();

    thread2.join();

    return 0;
}
