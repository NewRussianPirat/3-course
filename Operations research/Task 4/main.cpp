#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    const int N = 3;    //Количество предметов
    const int W = 10;   //Максимальный вес
    const std::vector<int> v = {0, 3, 7, 16};   //Вектор стоимости предметов
    const std::vector<int> w = {0, 2, 3, 6};    //Вектор веса предметов
    //Матрица, показывающая максимальную стоимость любого количества вещей типов от 1 до i, с определённым суммарным весом weight
    std::vector<std::vector<int>> f(N + 1, std::vector<int>(W + 1, 0));
    //Матрица, которая показывает, содержится ли в f[i][weight] предмет i-ого типа
    std::vector<std::vector<int>> p(N + 1, std::vector<int>(W + 1, 0));
    //Алгоритм нахождения максимальной стоимости
    for (int i = 1; i <= N; ++i) {
        for (int weight = 1; weight <= W; ++weight) {
            if (weight >= w[i]) {
                f[i][weight] = std::max(f[i - 1][weight], v[i] + f[i][weight - w[i]]);
            } else {
                f[i][weight] = f[i - 1][weight];
            }
            if (f[i][weight] == 0 || f[i][weight] == f[i - 1][weight]) {
                p[i][weight] = 0;
            } else {
                p[i][weight] = 1;
            }
        }
    }
    //Обратный ход
    std::vector<int> x = {0, 0, 0, 0};
    int weight = W;
    int i = N;
    int result = f[i][weight];
    while (weight != 0 && i != 0) {
        if (p[i][weight] != 0) {
            ++x[i];
            weight -= w[i];
        } else {
            --i;
        }
    }
    //Красивый вывод, не обращать внимания
    std::cout << "Table:" << std::endl;
    std::cout << "w   f1   p1   f2   p2   f3   p3" << std::endl;
    for (weight = 0; weight <= W; ++weight) {
        std::cout << std::left << std::setw(4) << weight;
        for (i = 1; i <= N; ++i) {
            std::cout << std::setw(5) << f[i][weight] << std::setw(5) << p[i][weight];
        }
        std::cout << std::endl;
    }
    std::cout << "Optimal cost: " << result << std::endl;
    std::cout << "Used items: ";
    for (i = 1; i <= N; ++i) {
        std::cout << "x" << i << ": " << x[i] << "  ";
    }
    return 0;
}
