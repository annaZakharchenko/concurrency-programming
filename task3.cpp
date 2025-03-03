#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <cmath>

std::vector<double> generate_vector(int n, double min, double max) {
    std::vector<double> vec(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    for (int i = 0; i < n; ++i) {
        vec[i] = dis(gen);
    }

    return vec;
}

void calculate_A(const std::vector<double>& X) {
    double A = 0;
    for (double x : X) {
        A += std::cbrt(x * sin(x * x));
    }
    std::cout << "A = " << A << std::endl;
}

void calculate_B(const std::vector<double>& Y) {
    double B = 0;
    for (double y : Y) {
        B += std::cbrt(y * cos(y * y));
    }
    std::cout << "\nB = " << B << std::endl;
}

int main() {
    int Nx = 10;
    int Ny = 15;
    double Dx_min = 0, Dx_max = 25;
    double Dy_min = -10, Dy_max = 10;

    std::vector<double> X = generate_vector(Nx, Dx_min, Dx_max);
    std::vector<double> Y = generate_vector(Ny, Dy_min, Dy_max);

    std::thread T0(calculate_A, std::ref(X));
    std::thread T1(calculate_B, std::ref(Y));

    T0.join();
    T1.join();

    return 0;
}
