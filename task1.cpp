#include <iostream>
#include <thread>
#include <chrono>

const int MULTIPLIER = 5;

void multiplyWithDelay(int number) {
    for (int i = 1; i < 5; i++) {
        std::cout << "\nStart: " << i << std::endl;
        std::cout << "ID thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        int result = number * MULTIPLIER;

        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Result: " << result << std::endl;
        std::cout << "Finish: " << i << std::endl;
    }
}

int main() {
    int number = 10;
    std::thread thread(multiplyWithDelay, number);
    thread.join();
    return 0;
}