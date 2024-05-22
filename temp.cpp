#include "temp.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

Temp::Temp(QObject *parent) : QObject(parent)
{
    ///Сначала необходимо прочитать данные из файла и записать в вектор
    ifstream inputFile("../indicator_temp/data.txt");

    if (!inputFile) {
        cout << "Не найден файл data.txt" << endl;
    } else {
        int Z, T;
        while (inputFile >> Z >> T) {
            data.emplace_back(Z, T); // добавляем пару в вектор
        }

        inputFile.close();

        // Вывод содержимого вектора
        cout << "Прочитанные пары:" << endl;
        for (const auto& pair : data) {
            cout << "(" << pair.first << ", " << pair.second << ")" << endl;
        }
    }

    // Оценка m и k с использованием mEstimator
    auto params = mEstimator(data);
    m = params.first;
    k = params.second;
    cout << "Оцененные параметры: m = " << m << ", k = " << k << endl;
}

pair<double, double> Temp::mEstimator(vector<pair<int, int>>& data)
{
    int size = data.size();
    MatrixXd A(size, 2);
    VectorXd b(size);

    // Формирование матрицы коэффициентов A и вектора b
    for (int i = 0; i < size; i++) {
        int Z = data[i].first;
        int T = data[i].second;
        A(i, 0) = 1;
        A(i, 1) = T - 20;
        b(i) = log10(Z);
    }

    // Решение системы уравнений Ax = b
    VectorXd x = A.colPivHouseholderQr().solve(b);

    // Оценка параметров m и k
    double log_m = x(0);
    double k = x(1);
    double m = pow(10, log_m);

    cout << "Оцененные параметры: m = " << m << ", k = " << k << endl;

    // Итерации метода оценки
    for (int i = 0; i < 50; i++) {
        // Формирование системы уравнений
        VectorXd residuals = b - A * x;
        MatrixXd W = MatrixXd::Identity(size, size);
        for (int j = 0; j < size; j++) {
            W(j, j) = 1.0 / max(0.01, fabs(residuals(j)));
        }

        // Обновление параметров
        x = (A.transpose() * W * A).ldlt().solve(A.transpose() * W * b);

        // Оценка параметров m и k
        log_m = x(0);
        k = x(1);
        m = pow(10, log_m);

        cout << i + 1 << "-я итерация: m = " << m << ", k = " << k << endl;
    }

    // Оценка выбросов
    VectorXd residuals = b - A * x;
    double threshold = 0.05;
    for (int i = 0; i < size; i++) {
        cout << "residuals(" << i<< "):"<<residuals(i) << endl;
        if (fabs(residuals(i)) > threshold) {
            cout << "Выброс для точки (" << data[i].first << ", " << data[i].second << ")" << endl;
        }
    }

    return {m, k};
}

void Temp::FormMatrix(vector<pair<int, int>> &data, MatrixXd &system) ///заполняем матрицу system для МНК
{
    int counter = 0; //индекс в матрице system
    for (auto point : data) {
        auto x = point.first;
        auto y = point.second;
        system(counter, 0) = x * x;
        system(counter, 1) = y * y;
        system(counter, 2) = x * y;
        system(counter, 3) = x;
        system(counter, 4) = y;
        counter++;
    }
}

double Temp::calculateZ(int T, double m, double k)
{
    ///Для изначального выражения введем замену:
    /// m = 1024*(R0/(Rc+R0)) = const
    /// и тогда итоговое выражение можно представить как:
    /// Z = m*10^(k*(T-20))
    return m * pow(10, k * (T - 20)); //Для возведения числа в степень в C++ есть функция pow
}

int Temp::calculateT(double Z, double m, double k)
{
    return 20 + log10(Z / m) / k;
}
