#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

queue<int> buffer;
const unsigned int MAX_BUFFER_SIZE = 5;

mutex mtx;
condition_variable cv_producer;
condition_variable cv_consumer;

bool finished = false;

void producer() {
    for (int i = 1; i <= 10; ++i) {
        unique_lock<mutex> lock(mtx);
        cv_producer.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; });

        buffer.push(i);
        cout << "Produced: " << i << endl;

        lock.unlock();
        cv_consumer.notify_one();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    unique_lock<mutex> lock(mtx);
    finished = true;
    cv_consumer.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv_consumer.wait(lock, [] { return !buffer.empty() || finished; });

        while (!buffer.empty()) {
            int val = buffer.front();
            buffer.pop();
            cout << "Consumed: " << val << endl;

            lock.unlock();
            cv_producer.notify_one();
            this_thread::sleep_for(chrono::milliseconds(150));
            lock.lock();
        }

        if (finished) break;
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
