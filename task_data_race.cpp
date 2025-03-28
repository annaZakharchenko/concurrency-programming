#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

using namespace std;

struct Number {
    int data;
    atomic<Number*> prev;
    atomic<Number*> next;

    Number(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    atomic<Number*> head;
    atomic<Number*> tail;
    mutable mutex mtx;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insertAtEnd(int value) {
        Number* newNumber = new Number(value);

        lock_guard<mutex> lock(mtx);
        Number* oldTail = tail.load();

        if (!oldTail) {
            head.store(newNumber);
            tail.store(newNumber);
        } else {
            oldTail->next.store(newNumber);
            newNumber->prev.store(oldTail);
            tail.store(newNumber);
        }
    }

    void display() const {
        lock_guard<mutex> lock(mtx);

        cout << "List from head to tail: ";
        Number* temp = head.load();
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next.load();
        }
        cout << endl;

        cout << "List from tail to head: ";
        temp = tail.load();
        while (temp) {
            cout << temp->data << " ";
            temp = temp->prev.load();
        }
        cout << endl;
    }

    bool deleteElement(int value) {
        lock_guard<mutex> lock(mtx);

        Number* temp = head.load();
        while (temp) {
            if (temp->data == value) {
                Number* prevNumber = temp->prev.load();
                Number* nextNumber = temp->next.load();

                if (prevNumber) prevNumber->next.store(nextNumber);
                if (nextNumber) nextNumber->prev.store(prevNumber);

                if (temp == head.load()) head.store(nextNumber);
                if (temp == tail.load()) tail.store(prevNumber);

                delete temp;
                return true;
            }
            temp = temp->next.load();
        }
        return false;
    }

    bool isSymmetric() const {
        lock_guard<mutex> lock(mtx);

        Number* left = head.load();
        Number* right = tail.load();

        while (left && right) {
            if (left->data != right->data) {
                return false;
            }
            left = left->next.load();
            right = right->prev.load();
        }

        return left == nullptr && right == nullptr;
    }

    ~DoublyLinkedList() {
        lock_guard<mutex> lock(mtx);
        Number* temp = head.load();
        while (temp) {
            Number* toDelete = temp;
            temp = temp->next.load();
            delete toDelete;
        }
    }
};

void testInsert(DoublyLinkedList& list) {
    int n, value;
    cout << "Enter the number of elements to insert: ";
    cin >> n;

    for (int i = 0; i < n; ++i) {
        cout << "Enter value for element " << i + 1 << ": ";
        cin >> value;
        list.insertAtEnd(value);
    }
}

void testDelete(DoublyLinkedList& list) {
    int value;
    while (true) {
        cout << "Enter the value to delete: ";
        cin >> value;
        if (list.deleteElement(value)) {
            cout << "Element " << value << " deleted successfully.\n";
            break;
        } else {
            cout << "Error: Element " << value << " not found in the list. Try again.\n";
        }
    }
}

void testSymmetry(DoublyLinkedList& list) {
    cout << "Is the list symmetric? " << (list.isSymmetric() ? "Yes" : "No") << endl;
}

int main() {
    DoublyLinkedList list;

    thread t1(testInsert, ref(list));
    t1.join();

    list.display();

    thread t2(testDelete, ref(list));
    t2.join();

    list.display();

    testSymmetry(list);

    return 0;
}
