#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

class BankAccount {
private:
    double balance;
    mutable mutex mtx;

public:
    BankAccount(double initial_balance) : balance(initial_balance) {}

    double getBalance() const {
        lock_guard<mutex> lock(mtx);
        return balance;
    }

    void deposit(double amount) {
        unique_lock<mutex> lock(mtx);
        balance += amount;
        cout << "Deposited " << amount << ". New balance: " << balance << endl;
    }

    void withdraw(double amount) {
        unique_lock<mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrew " << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Insufficient funds for withdrawal of " << amount << endl;
        }
    }

    void transfer(BankAccount& to, double amount) {
        unique_lock<mutex> lock_from(mtx, defer_lock);
        unique_lock<mutex> lock_to(to.mtx, defer_lock);

        lock(lock_from, lock_to);

        if (balance >= amount) {
            balance -= amount;
            to.balance += amount;
            cout << "Transferred " << amount << " to target account. New balance: " << balance << endl;
            cout << "Target account new balance: " << to.balance << endl;
        } else {
            cout << "Insufficient funds for transfer of " << amount << endl;
        }
    }
};

void testAccountOperations() {
    BankAccount account1(1000);
    BankAccount account2(500);

    cout << "Balance account1 : " << account1.getBalance() << endl;
    cout << "Balance account2 : " << account2.getBalance() << endl;

    thread t1(&BankAccount::deposit, &account1, 500);
    thread t2(&BankAccount::withdraw, &account1, 200);
    thread t4(&BankAccount::deposit, &account1, 300);
    thread t5(&BankAccount::withdraw, &account1, 1500);
    thread t3(&BankAccount::transfer, &account1, ref(account2), 300);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
}

int main() {
    testAccountOperations();
    return 0;
}
