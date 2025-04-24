#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
queue<int> buffer;
const unsigned int MAX_BUFFER_SIZE = 5;
bool done = false;

void producer() {
    for (int i = 1; i <= 10; ++i) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; });

        buffer.push(i);
        cout << "Produced: " << i << endl;

        lock.unlock();
        cv.notify_all();
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    unique_lock<mutex> lock(mtx);
    done = true;
    lock.unlock();
    cv.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !buffer.empty() || done; });

        while (!buffer.empty()) {
            int value = buffer.front();
            buffer.pop();
            cout << "Consumed: " << value << endl;
        }

        if (done) break;

        lock.unlock();
        cv.notify_all();
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

int main() {
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}
