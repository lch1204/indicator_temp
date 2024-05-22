#ifndef TEMP_H
#define TEMP_H

#include <QObject>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
#include <utility>

using namespace std;
using namespace Eigen;

class Temp : public QObject
{
    Q_OBJECT
public:
    explicit Temp(QObject *parent = nullptr);
    std::pair<double, double> mEstimator(std::vector<std::pair<int, int>> &data);
    double calculateZ(int T, double m, double k);
    int calculateT(double Z, double m, double k);

    double getM() const { return m; }
    double getK() const { return k; }

private:
    std::vector<std::pair<int, int>> data;//храним здесь набор данных для обучения
    void FormMatrix(std::vector<std::pair<int, int>> &data, Eigen::MatrixXd &system);
    void printOutliers(Eigen::MatrixXd &weights, double level);
    double m;
    double k;


signals:

public slots:
};

#endif // TEMP_H
