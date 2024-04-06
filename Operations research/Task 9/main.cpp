#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int numberOfIterations = 200;
    std::vector<std::vector<int>> a = {{-1, 2,  0},
                                       {3,  -3, 4},
                                       {4,  -5, 6},
                                       {-5, 6,  -5}};
    std::vector<int> minA(a.size());
    for (int i = 0; i < a.size(); ++i) {
        minA[i] = *(std::min_element(a[i].begin(), a[i].end()));
    }
    int currentIndA = static_cast<int>(std::max_element(minA.begin(), minA.end()) - minA.begin());
    int currentIndB = 0;
    std::vector<int> tableA(a.size(), 0);
    std::vector<int> tableB(a[0].size(), 0);
    std::vector<int> indexA(a.size());
    std::vector<int> indexB(a[0].size());

    for (int i = 0; i < numberOfIterations; ++i) {
        ++indexA[currentIndA];
        for (int j = 0; j < a[currentIndA].size(); ++j) {
            tableB[j] += a[currentIndA][j];
        }
        currentIndB = static_cast<int>(std::min_element(tableB.begin(), tableB.end()) - tableB.begin());
        ++indexB[currentIndB];
        for (int j = 0; j < a.size(); ++j) {
            tableA[j] += a[j][currentIndB];
        }
        currentIndA = static_cast<int>(std::max_element(tableA.begin(), tableA.end()) - tableA.begin());
    }

    std::cout << "Player 1 optimal mixed strategy: ";
    for (auto i: indexA) {
        std::cout << (double) i / numberOfIterations << " ";
    }
    std::cout << "\nPlayer 2 optimal mixed strategy: ";
    for (auto i: indexB) {
        std::cout << (double) i / numberOfIterations << " ";
    }
    std::cout << "\nValue of the game: "
              << (double) (tableB[currentIndB] + tableA[currentIndA]) / (2 * numberOfIterations);

    return 0;
}
