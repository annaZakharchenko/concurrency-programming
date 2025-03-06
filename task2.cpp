#include <iostream>
#include <thread>

void multiply(int a, int b, int &result) {
    result = a * b;
}

int main() {
    int a, b;
    int result;

    std::cout << "Enter first number:";
    std::cin >> a;
    std::cout << "Enter second number:";
    std::cin >> b;

    std::thread th(multiply, a, b, std::ref(result));

    th.join();

    std::cout << "Result: " << result << std::endl;


    return 0;
}
