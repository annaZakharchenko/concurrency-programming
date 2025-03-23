#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;

    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    mutable mutex mtx;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insertAtEnd(int value) {
        lock_guard<mutex> lock(mtx);

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void display() const {
        lock_guard<mutex> lock(mtx);

        cout << "List from head to tail: ";
        Node* temp = head;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;

        cout << "List from tail to head: ";
        temp = tail;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->prev;
        }
        cout << endl;
    }

    void deleteElement(int value) {
        lock_guard<mutex> lock(mtx);

        Node* temp = head;
        while (temp) {
            if (temp->data == value) {
                if (temp->prev) temp->prev->next = temp->next;
                if (temp->next) temp->next->prev = temp->prev;

                if (temp == head) head = temp->next;
                if (temp == tail) tail = temp->prev;

                delete temp;
                return;
            }
            temp = temp->next;
        }
    }

    void insertBefore(int newValue, int existingValue) {
        lock_guard<mutex> lock(mtx);

        Node* temp = head;
        while (temp) {
            if (temp->data == existingValue) {
                Node* newNode = new Node(newValue);
                newNode->next = temp;
                newNode->prev = temp->prev;

                if (temp->prev) temp->prev->next = newNode;
                temp->prev = newNode;

                if (temp == head) head = newNode;
                return;
            }
            temp = temp->next;
        }
    }

    void insertAfter(int newValue, int existingValue) {
        lock_guard<mutex> lock(mtx);

        Node* temp = head;
        while (temp) {
            if (temp->data == existingValue) {
                Node* newNode = new Node(newValue);
                newNode->prev = temp;
                newNode->next = temp->next;

                if (temp->next) temp->next->prev = newNode;
                temp->next = newNode;

                if (temp == tail) tail = newNode;
                return;
            }
            temp = temp->next;
        }
    }

    bool isSymmetric() const {
        lock_guard<mutex> lock(mtx);

        Node* left = head;
        Node* right = tail;

        while (left && right) {
            if (left->data != right->data) {
                return false;
            }
            left = left->next;
            right = right->prev;
        }

        return left == nullptr && right == nullptr;
    }

    ~DoublyLinkedList() {
        lock_guard<mutex> lock(mtx);
        Node* temp = head;
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
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
    cout << "Enter the value to delete: ";
    cin >> value;
    list.deleteElement(value);
}

void testInsertBefore(DoublyLinkedList& list) {
    int newValue, existingValue;
    cout << "Enter new value to insert before: ";
    cin >> newValue;
    cout << "Enter existing value before which to insert: ";
    cin >> existingValue;
    list.insertBefore(newValue, existingValue);
}

void testInsertAfter(DoublyLinkedList& list) {
    int newValue, existingValue;
    cout << "Enter new value to insert after: ";
    cin >> newValue;
    cout << "Enter existing value after which to insert: ";
    cin >> existingValue;
    list.insertAfter(newValue, existingValue);
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

    thread t3(testInsertBefore, ref(list));
    t3.join();

    list.display();

    thread t4(testInsertAfter, ref(list));
    t4.join();

    list.display();

    testSymmetry(list);

    return 0;
}
