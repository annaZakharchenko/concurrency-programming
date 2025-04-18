#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

mutex mtx;
condition_variable cv;
string userInput;
bool inputReady = false;

void inputThread() {
    while (true) {
        string temp;
        cout << "Enter a string (or 'exit' to quit): ";
        getline(cin, temp);

        unique_lock<mutex> lock(mtx);
        userInput = temp;
        inputReady = true;
        cv.notify_one();
        if (temp == "exit") break;
    }
}

void processingThread() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return inputReady; });

        if (userInput == "exit") break;

        cout << "Processed string: " << userInput << endl;

        inputReady = false;
    }
}

int main() {
    thread t1(inputThread);
    thread t2(processingThread);

    t1.join();
    t2.join();

    return 0;
}
