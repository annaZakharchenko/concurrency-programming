#include <thread>
#include <iostream>
#include <string>

void run(std::string threadName) {
    for (int i = 0; i < 10; i++) {
        std::cout << "Thread: " << threadName  << "; Flag: " << i << std::endl;
    }
}

int main() {
    std::thread thread1(run, "A");
    std::thread thread2(run, "B");

    thread1.join();
    thread2.join();

    return 0;
}
