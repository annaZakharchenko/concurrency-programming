#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

int pot = 0;
int H;
const int n_bees = 3;
int bear_eats = 0;

mutex mtx;
condition_variable pot_full;
condition_variable pot_empty;

bool bear_awake = false;

void bee(int id) {
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(300));

        unique_lock<mutex> lock(mtx);

        if (bear_eats >= 2) break;

        while (pot == H) {
            pot_empty.wait(lock);
            if (bear_eats >= 2) return;
        }

        pot++;
        cout << "Bee " << id << " brought honey. Pot: " << pot << "/" << H << endl;

        if (pot == H) {
            cout << "Bee " << id << " wakes up the bear!" << endl;
            bear_awake = true;
            pot_full.notify_one();
        }
    }
    cout << "Bee " << id << " finished work.\n";
}

void bear() {
    while (bear_eats < 2) {
        unique_lock<mutex> lock(mtx);
        pot_full.wait(lock, [] { return bear_awake; });

        cout << "Bear woke up and ate all the honey!" << endl;
        pot = 0;
        bear_awake = false;
        bear_eats++;

        pot_empty.notify_all();
    }
    cout << "Bear ate honey twice and fell asleep forever.\n";
}

int main() {
    cout << "Enter the pot capacity (H): ";
    cin >> H;

    thread bear_thread(bear);
    vector<thread> bee_threads;

    for (int i = 0; i < n_bees; ++i) {
        bee_threads.emplace_back(bee, i + 1);
    }

    for (auto& t : bee_threads) t.join();
    bear_thread.join();

    cout << "Program completed.\n";
    return 0;
}
