#include <QCoreApplication>
#include "temp.h"
#include <fstream>
#include <cmath>


int main()
{
    Temp temp;
    int Z = 50; // Заданное значение Z

    int T = temp.calculateT(Z, temp.getM(), temp.getK());
    std::cout << "Для Z = " << Z << ", T = " << T << std::endl;
    // Получение параметров m и k
    double m = temp.getM();
    double k = temp.getK();

    std::ofstream outputFile("filenameEnd.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи." << std::endl;
        return -1;
    }

    // Генерация значений Z
    for (int T = 1; T <= 100; T++) {
        // Расчет значения T для текущего Z
        int Z = temp.calculateZ(T, temp.getM(), temp.getK());
        // Запись значения T и Z в файл
        outputFile << Z << " " << T << std::endl;
    }

    outputFile.close();

    return 0;
}


