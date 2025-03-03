#include <thread>
#include <iostream>
#include <string>
#include <atomic>

bool flag = true;

void run(std::string thread) {
    if (flag) {
        std::cout << "Thread: " << thread << "; ID: " << std::this_thread::get_id() << "; Flag: " << flag << std::endl;
    }

}

void runChangeFlag() {
    flag = false;
}

int main() {
    std::thread thread1(run, "A");
    std::thread thread2(runChangeFlag);

    thread1.join();
    thread2.join();

    return 0;
}
