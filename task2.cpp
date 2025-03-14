#include <iostream>
#include <thread>

using namespace std;

int mult(int a, int b)
{
    return a * b;
}

int main()
{
    int a, b;
    int result;

    cout << "\nEnter first number: " << endl;
    cin >> a;
    cout << "Enter second number: " << endl;

    cin >> b;

    thread t([&result, a, b]()
             { result = mult(a, b); });

    t.join();

    cout << "Result: " << result << endl;
    return 0;
}