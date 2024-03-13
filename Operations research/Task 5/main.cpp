#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

const int NUMBER_OF_VERTICES = 8;
const int NUMBER_OF_EDGES = 13;
std::vector<std::pair<int, int>> vertices(NUMBER_OF_VERTICES);
std::vector<std::pair<const std::pair<int, int>, int>> edges = {
        {{5, 6}, 0},
        {{0, 1}, 0},
        {{0, 2}, 0},
        {{1, 3}, 0},
        {{2, 3}, 0},
        {{2, 4}, 0},
        {{3, 4}, 0},
        {{3, 6}, 0},
        {{4, 5}, 0},
        {{4, 6}, 0},
        {{4, 7}, 0},
        {{2, 7}, 0},
        {{6, 7}, 0}
};

double Laplace(double x) {
    return 0.5 * std::erf(x / std::sqrt(2.0));
}

void dfs(int begin, int target, const std::vector<bool> &criticalVertices, const std::vector<bool> &criticalEdges,
         std::vector<std::vector<int>> &nonCriticalRoots,
         std::vector<int> *root = nullptr) {
    if (begin > target) {
        return;
    }
    if (begin == target) {
        if (root != nullptr) {
            nonCriticalRoots.push_back(*root);
        }
        return;
    }
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        if (!criticalEdges[i] && EDGE_BEGIN == begin && (!criticalVertices[EDGE_END] || EDGE_END == target)) {
            if (root == nullptr) {
                root = new std::vector<int>({i});
            } else {
                root->push_back(i);
            }
            dfs(EDGE_END, target, criticalVertices, criticalEdges, nonCriticalRoots, root);
            if (root != nullptr && !root->empty()) {
                root->pop_back();
            }
        }
    }
}

int findCriticalRootWeight(int start, int finish, const std::vector<bool> &criticalVertices) {
    if (start == finish) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        if (EDGE_BEGIN == start && criticalVertices[EDGE_END]) {
            result += findCriticalRootWeight(EDGE_END, finish, criticalVertices) + edges[i].second;
            break;
        }
    }
    return result;
}

void setWeights(const std::vector<int> &p) {
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        edges[i].second = p[i];
    }
}

void fillGraph() {
    vertices[0] = {0, 0};
    vertices[1] = {vertices[0].first + edges[1].second, 0};
    vertices[2] = {vertices[0].first + edges[2].second, 0};
    vertices[3] = {std::max(vertices[1].first + edges[3].second, vertices[2].first + edges[4].second), 0};
    vertices[4] = {std::max(vertices[2].first + edges[5].second, vertices[3].first + edges[6].second), 0};
    vertices[5] = {vertices[4].first + edges[8].second, 0};
    vertices[6] = {std::max(std::max(vertices[3].first + edges[7].second, vertices[4].first + edges[9].second),
                            vertices[5].first + edges[0].second), 0};
    vertices[7] = {std::max(std::max(vertices[2].first + edges[11].second, vertices[4].first + edges[10].second),
                            vertices[6].first + edges[12].second), 0};
    //---------------------------------------------
    vertices[7].second = vertices[7].first;
    vertices[6].second = vertices[7].second - edges[12].second;
    vertices[5].second = vertices[6].second - edges[0].second;
    vertices[4].second = std::min(std::min(vertices[7].second - edges[10].second, vertices[6].second - edges[9].second),
                                  vertices[5].second - edges[8].second);
    vertices[3].second = std::min(vertices[6].second - edges[7].second, vertices[4].second - edges[6].second);
    vertices[2].second = std::min(std::min(vertices[7].second - edges[11].second, vertices[4].second - edges[5].second),
                                  vertices[3].second - edges[4].second);
    vertices[1].second = vertices[3].second - edges[3].second;
    vertices[0].second = std::min(vertices[2].second - edges[2].second, vertices[1].second - edges[1].second);
}

std::vector<std::pair<int, int>> findTimeReserves() {
    std::vector<std::pair<int, int>> r(NUMBER_OF_EDGES);
    for (int i = 0; i < edges.size(); ++i) {
        r[i].first = vertices[edges[i].first.second].second - vertices[edges[i].first.first].first - edges[i].second;
        r[i].second = vertices[edges[i].first.second].first - vertices[edges[i].first.first].second - edges[i].second;
    }
    return r;
}

std::vector<bool> setCriticalVertices() {
    std::vector<bool> criticalVertices(NUMBER_OF_VERTICES, false);
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (vertices[i].first == vertices[i].second) {
            criticalVertices[i] = true;
        }
    }
    return criticalVertices;
}

std::vector<bool> setCriticalEdges(const std::vector<std::pair<int, int>> &timeReserves) {
    std::vector<bool> criticalEdges(NUMBER_OF_EDGES, false);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (timeReserves[i].first == 0 && timeReserves[i].second == 0) {
            criticalEdges[i] = true;
        }
    }
    return criticalEdges;
}

std::vector<std::vector<int>>
findNonCriticalRoots(const std::vector<bool> &criticalVertices, const std::vector<bool> &criticalEdges) {
    std::vector<std::vector<int>> nonCriticalRoots;
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (criticalVertices[i]) {
            for (int j = i + 1; j < NUMBER_OF_VERTICES; ++j) {
                if (criticalVertices[j]) {
                    dfs(i, j, criticalVertices, criticalEdges, nonCriticalRoots);
                }
            }
        }
    }
    return nonCriticalRoots;
}

std::vector<double>
findTensionCoefficient(const std::vector<std::vector<int>> &nonCriticalRoots, const std::vector<bool> &criticalVertices,
                       std::vector<int> *R) {
    std::vector<double> N;
    for (auto i: nonCriticalRoots) {
        int start = edges[*i.begin()].first.first;
        int finish = edges[*(--i.end())].first.second;
        int a = findCriticalRootWeight(start, finish, criticalVertices);
        int b = 0;
        for (auto j: i) {
            b += edges[j].second;
        }
        R->push_back(a - b);
        N.push_back(double(b) / a);
    }
    return N;
}

std::vector<int> findExpectedTime(const std::vector<int> &t_pessimistic, const std::vector<int> &t_optimistic,
                                  const std::vector<int> *t_probable = nullptr) {
    std::vector<int> t_expected(NUMBER_OF_EDGES);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (t_probable != nullptr) {
            t_expected[i] = (int) (std::round(
                    (double) (t_pessimistic[i] + 4 * (*t_probable)[i] + t_optimistic[i]) / 6));
        } else {
            t_expected[i] = (3 * t_pessimistic[i] + 2 * t_optimistic[i]) / 5;
        }
    }
    return t_expected;
}

std::vector<double> findDispersion(const std::vector<int> &t_pessimistic, const std::vector<int> &t_optimistic) {
    std::vector<double> dispersion(NUMBER_OF_EDGES);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        double a = (double) (t_pessimistic[i] - t_optimistic[i]) / 6;
        dispersion[i] = a * a;
    }
    return dispersion;
}

double findCriticalDispersion(const std::vector<bool> &criticalEdges, const std::vector<double> &dispersion) {
    double result = 0;
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (criticalEdges[i]) {
            result += dispersion[i];
        }
    }
    return std::sqrt(result);
}

void showEventTable(const std::vector<bool> &criticalVertices) {
    std::string delimiter = "------------------------------------------------";
    std::string s1 = " Event | Early Date | Late Date | Time Reserve |";

    std::cout << "\n" << delimiter << "\n" << s1 << "\n" << delimiter << "\n";
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        int n = 7;
        if (criticalVertices[i]) {
            std::cout << "*";
            --n;
        }
        std::cout << std::setw(n) << i << "|"
                  << std::setw(12) << vertices[i].first << "|"
                  << std::setw(11) << vertices[i].second << "|"
                  << std::setw(14) << vertices[i].second - vertices[i].first << "|";
        std::cout << "\n";
    }
    std::cout << delimiter << "\n";
}

void showCriticalVertices(const std::vector<bool> &criticalVertices) {
    std::cout << "\nCritical vertices: ";
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (criticalVertices[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}

void showWorkTable(const std::vector<bool> &criticalEdges, const std::vector<std::pair<int, int>> &timeReserves,
                   const std::vector<double> *dispersion = nullptr) {
    std::string delimiter = "------------------------------------------------------------------------";
    std::string s1 = "    Work     | Duration | Full Time Reserve | Independent Time Reserve |";
    if (dispersion != nullptr) {
        s1 += " Dispersion |";
        delimiter += "-------------";
    }
    std::cout << "\n" << delimiter << "\n" << s1 << "\n" << delimiter << "\n";
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        int n = 2;
        if (criticalEdges[i]) {
            std::cout << "*";
        } else {
            std::cout << " ";
        }
        if (i >= 10) {
            --n;
        }
        std::cout << "b" << i << " = (" << edges[i].first.first << ", " << edges[i].first.second << ")" << std::setw(n)
                  << "|" << std::setw(10) << edges[i].second
                  << "|" << std::setw(19) << timeReserves[i].first
                  << "|" << std::setw(26) << timeReserves[i].second << "|";
        if (dispersion != nullptr) {
            std::cout << std::setw(12) << (*dispersion)[i] << "|";
        }
        std::cout << "\n";
    }
    std::cout << delimiter << "\n";
}

void showCriticalEdges(const std::vector<bool> &criticalEdges) {
    std::cout << "\nCritical Edges: ";
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (criticalEdges[i]) {
            std::cout << "b" << i << " ";
        }
    }
    std::cout << "\n";
}

void showNonCriticalRootsTable(const std::vector<std::vector<int>> &nonCriticalRoots, const std::vector<int> &R,
                               const std::vector<double> &N) {
    std::cout << "\n---------------------------------------------------------\n"
              << " Non-critical arcs | Time Reserve | Tension Coefficient |"
              << "\n---------------------------------------------------------\n";
    for (int i = 0; i < nonCriticalRoots.size(); ++i) {
        int n = 16 - nonCriticalRoots[i].size() * 3;
        std::cout << std::setw(n) << "(" << edges[nonCriticalRoots[i][0]].first.first;
        for (int j: nonCriticalRoots[i]) {
            std::cout << ", " << edges[j].first.second;
        }
        std::cout << ") | " << std::setw(13) << R[i] << "|" << std::setw(21) << N[i] << "|\n";
    }
    std::cout << "---------------------------------------------------------\n";
}

void Task1() {
    std::cout << "Task 1" << std::endl;
    const std::vector<int> t1 = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 10, 9};
    setWeights(t1);
    fillGraph();
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves();
    std::vector<bool> criticalVertices = setCriticalVertices();
    std::vector<bool> criticalEdges = setCriticalEdges(timeReserves);
    std::vector<std::vector<int>> nonCriticalRoots = findNonCriticalRoots(criticalVertices, criticalEdges);
    std::vector<int> R;
    std::vector<double> N = findTensionCoefficient(nonCriticalRoots, criticalVertices, &R);
    showEventTable(criticalVertices);
    showCriticalVertices(criticalVertices);
    showWorkTable(criticalEdges, timeReserves);
    showCriticalEdges(criticalEdges);
    showNonCriticalRootsTable(nonCriticalRoots, R, N);
}

void Task2() {
    std::cout << "\nTask 2" << std::endl;
    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 12, 9};
    //TODO: не нравится мне здесь 12 на предпоследнем месте, противоречит условию...
    const std::vector<int> t_probable = {0, 9, 5, 3, 4, 6, 4, 5, 5, 7, 8, 10, 5};
    const std::vector<int> t_optimistic = {0, 3, 4, 2, 1, 2, 2, 3, 1, 4, 6, 3, 2};
    const double reliability = 0.9;
    const int deadline = 29;
    std::vector<int> t_expected = findExpectedTime(t_pessimistic, t_optimistic, &t_probable);
    setWeights(t_expected);
    fillGraph();
    std::vector<double> dispersion = findDispersion(t_pessimistic, t_optimistic);
    std::vector<bool> criticalVertices = setCriticalVertices();
    showEventTable(criticalVertices);
    showCriticalVertices(criticalVertices);
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves();
    std::vector<bool> criticalEdges = setCriticalEdges(timeReserves);
    showWorkTable(criticalEdges, findTimeReserves(), &dispersion);
    showCriticalEdges(criticalEdges);
    double criticalDispersion = findCriticalDispersion(criticalEdges, dispersion);
    std::cout << "Dispersion of critical root: " << criticalDispersion << std::endl;
    double probability = 0.5 + Laplace((deadline - (*(--vertices.end())).second) / criticalDispersion);
    std::cout << "Probability of completion in time: " << probability << "%\n";
    std::cout << "Guarantied completion time: " << (*(--vertices.end())).second - std::round(3 * criticalDispersion)
              << "-" << (*(--vertices.end())).second + std::round(3 * criticalDispersion) << std::endl;
    double a = 1.65;
    std::cout << "Completion time with " << reliability * 100 << "% reliability: "
              << std::round((*(--vertices.end())).second - a * criticalDispersion) << "-"
              << std::round((*(--vertices.end())).second + a * criticalDispersion);
    //-----------------------------------------------------------------------------
    t_expected = findExpectedTime(t_pessimistic, t_optimistic);
    setWeights(t_expected);
    fillGraph();
    dispersion = findDispersion(t_pessimistic, t_optimistic);
    criticalVertices = setCriticalVertices();
    showEventTable(criticalVertices);
    showCriticalVertices(criticalVertices);
    timeReserves = findTimeReserves();
    criticalEdges = setCriticalEdges(timeReserves);
    showWorkTable(criticalEdges, findTimeReserves(), &dispersion);
    showCriticalEdges(criticalEdges);
    criticalDispersion = findCriticalDispersion(criticalEdges, dispersion);
    std::cout << "Dispersion of critical root: " << criticalDispersion << std::endl;
    probability = 0.5 + Laplace((deadline - (*(--vertices.end())).second) / criticalDispersion);
    std::cout << "Probability of completion in time: " << probability << "%\n";
    std::cout << "Guarantied completion time: " << (*(--vertices.end())).second - std::round(3 * criticalDispersion)
              << "-" << (*(--vertices.end())).second + std::round(3 * criticalDispersion) << std::endl;
    a = 1.65;
    std::cout << "Completion time with " << reliability * 100 << "% reliability: "
              << std::round((*(--vertices.end())).second - a * criticalDispersion) << "-"
              << std::round((*(--vertices.end())).second + a * criticalDispersion);
}

void Task3() {

}


int main() {
    std::cout << std::setprecision(5);
    Task1();
    Task2();
    return 0;
}
