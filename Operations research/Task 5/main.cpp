#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

//const int NUMBER_OF_VERTICES = 8;
//const int NUMBER_OF_EDGES = 13;
//std::vector<std::pair<const std::pair<int, int>, int>> edges = {
//        {{5, 6}, 0},
//        {{0, 1}, 0},
//        {{0, 2}, 0},
//        {{1, 3}, 0},
//        {{2, 3}, 0},
//        {{2, 4}, 0},
//        {{3, 4}, 0},
//        {{3, 6}, 0},
//        {{4, 5}, 0},
//        {{4, 6}, 0},
//        {{4, 7}, 0},
//        {{2, 7}, 0},
//        {{6, 7}, 0}
//};

//Определение графа
const int NUMBER_OF_VERTICES = 7;
const int NUMBER_OF_EDGES = 12;
std::vector<std::pair<const std::pair<int, int>, int>> edges = {
        {{3, 5}, 0}, //b0
        {{0, 1}, 0}, //b1
        {{0, 3}, 0}, //b2
        {{0, 2}, 0}, //b3
        {{1, 4}, 0}, //b4
        {{1, 3}, 0}, //b5
        {{2, 3}, 0}, //b6
        {{3, 4}, 0}, //b7
        {{3, 6}, 0}, //b8
        {{4, 6}, 0}, //b9
        {{2, 5}, 0}, //b10
        {{5, 6}, 0}, //b11
};

double Laplace(double x) {
    return 0.5 * std::erf(x / std::sqrt(2.0));
}

void dfs(int begin, int target, const std::vector<bool> &criticalEvents, const std::vector<bool> &criticalEdges,
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
        if (!criticalEdges[i] && EDGE_BEGIN == begin && (!criticalEvents[EDGE_END] || EDGE_END == target)) {
            if (root == nullptr) {
                root = new std::vector<int>({i});
            } else {
                root->push_back(i);
            }
            dfs(EDGE_END, target, criticalEvents, criticalEdges, nonCriticalRoots, root);
            if (root != nullptr && !root->empty()) {
                root->pop_back();
            }
        }
    }
}

int findCriticalRootWeight(int start, int finish, const std::vector<bool> &criticalEvents) {
    if (start == finish) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        int EDGE_WEIGHT = edges[i].second;
        if (EDGE_BEGIN == start && criticalEvents[EDGE_END]) {
            result += findCriticalRootWeight(EDGE_END, finish, criticalEvents) + EDGE_WEIGHT;
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

std::vector<std::pair<int, int>> fillGraph() {
    std::vector<std::pair<int, int>> eventOccurrenceTime(NUMBER_OF_VERTICES);
    eventOccurrenceTime[0] = {0, 0};
    for (int i = 1; i < NUMBER_OF_VERTICES; ++i) {
        eventOccurrenceTime[i].first = INT_MIN;
        for (int j = 0; j < NUMBER_OF_EDGES; ++j) {
            int EDGE_BEGIN = edges[j].first.first;
            int EDGE_END = edges[j].first.second;
            int EDGE_WEIGHT = edges[j].second;
            if (EDGE_END == i) {
                eventOccurrenceTime[i].first = std::max(eventOccurrenceTime[i].first,
                                                        eventOccurrenceTime[EDGE_BEGIN].first + EDGE_WEIGHT);
            }
        }
    }
    eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second = eventOccurrenceTime[NUMBER_OF_VERTICES - 1].first;
    for (int i = NUMBER_OF_VERTICES - 2; i >= 0; --i) {
        eventOccurrenceTime[i].second = INT_MAX;
        for (int j = 0; j < NUMBER_OF_EDGES; ++j) {
            int EDGE_BEGIN = edges[j].first.first;
            int EDGE_END = edges[j].first.second;
            int EDGE_WEIGHT = edges[j].second;
            if (EDGE_BEGIN == i) {
                eventOccurrenceTime[i].second = std::min(eventOccurrenceTime[i].second,
                                                         eventOccurrenceTime[EDGE_END].second - EDGE_WEIGHT);
            }
        }
    }
    return eventOccurrenceTime;
}

std::vector<std::pair<int, int>> findTimeReserves(const std::vector<std::pair<int, int>> &eventOccurrenceTime) {
    std::vector<std::pair<int, int>> timeReserves(NUMBER_OF_EDGES);
    for (int i = 0; i < edges.size(); ++i) {
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        int EDGE_WEIGHT = edges[i].second;
        timeReserves[i].first =
                eventOccurrenceTime[EDGE_END].second - eventOccurrenceTime[EDGE_BEGIN].first - EDGE_WEIGHT;
        timeReserves[i].second =
                eventOccurrenceTime[EDGE_END].first - eventOccurrenceTime[EDGE_BEGIN].second - EDGE_WEIGHT;
    }
    return timeReserves;
}

std::vector<bool> findCriticalEvents(const std::vector<std::pair<int, int>> &eventOccurrenceTime) {
    std::vector<bool> criticalEvents(NUMBER_OF_VERTICES, false);
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (eventOccurrenceTime[i].first == eventOccurrenceTime[i].second) {
            criticalEvents[i] = true;
        }
    }
    return criticalEvents;
}

std::vector<bool> findCriticalEdges(const std::vector<std::pair<int, int>> &timeReserves) {
    std::vector<bool> criticalEdges(NUMBER_OF_EDGES, false);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (timeReserves[i].first == 0 && timeReserves[i].second == 0) {
            criticalEdges[i] = true;
        }
    }
    return criticalEdges;
}

std::vector<std::vector<int>>
findNonCriticalRoots(const std::vector<bool> &criticalEvents, const std::vector<bool> &criticalEdges) {
    std::vector<std::vector<int>> nonCriticalRoots;
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (criticalEvents[i]) {
            for (int j = i + 1; j < NUMBER_OF_VERTICES; ++j) {
                if (criticalEvents[j]) {
                    dfs(i, j, criticalEvents, criticalEdges, nonCriticalRoots);
                }
            }
        }
    }
    return nonCriticalRoots;
}

std::vector<double>
findTensionCoefficients(const std::vector<std::vector<int>> &nonCriticalRoots, const std::vector<bool> &criticalEvents,
                        std::vector<int> *nonCriticalReserves) {
    std::vector<double> nonCriticalTensionCoefficients;
    for (auto i: nonCriticalRoots) {
        int start = edges[*i.begin()].first.first;
        int finish = edges[*(--i.end())].first.second;
        int a = findCriticalRootWeight(start, finish, criticalEvents);
        int b = 0;
        for (auto j: i) {
            b += edges[j].second;
        }
        nonCriticalReserves->push_back(a - b);
        nonCriticalTensionCoefficients.push_back(double(b) / a);
    }
    return nonCriticalTensionCoefficients;
}

std::vector<int> findExpectedTime(const std::vector<int> &t_pessimistic, const std::vector<int> &t_optimistic,
                                  const std::vector<int> *t_probable = nullptr) {
    std::vector<int> t_expected(NUMBER_OF_EDGES);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (t_probable != nullptr) {
            t_expected[i] = (int) (std::round(
                    (double) (t_pessimistic[i] + 4 * (*t_probable)[i] + t_optimistic[i]) / 6));
        } else {
            t_expected[i] = (int) std::round((double) (3 * t_pessimistic[i] + 2 * t_optimistic[i]) / 5);
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

void
showEventTable(const std::vector<std::pair<int, int>> &eventOccurrenceTime, const std::vector<bool> &criticalEvents) {
    std::string delimiter = "------------------------------------------------";
    std::string s1 = " Event | Early Date | Late Date | Time Reserve |";

    std::cout << "\n" << delimiter << "\n" << s1 << "\n" << delimiter << "\n";
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        int n = 7;
        if (criticalEvents[i]) {
            std::cout << "*";
            --n;
        }
        std::cout << std::setw(n) << i << "|"
                  << std::setw(12) << eventOccurrenceTime[i].first << "|"
                  << std::setw(11) << eventOccurrenceTime[i].second << "|"
                  << std::setw(14) << eventOccurrenceTime[i].second - eventOccurrenceTime[i].first << "|";
        std::cout << "\n";
    }
    std::cout << delimiter << "\n";
}

void showCriticalEvents(const std::vector<bool> &criticalEvents) {
    std::cout << "\nCritical events: ";
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (criticalEvents[i]) {
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
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        int EDGE_WEIGHT = edges[i].second;
        int n = 2;
        if (criticalEdges[i]) {
            std::cout << "*";
        } else {
            std::cout << " ";
        }
        if (i >= 10) {
            --n;
        }
        std::cout << "b" << i << " = (" << EDGE_BEGIN << ", " << EDGE_END << ")" << std::setw(n)
                  << "|" << std::setw(10) << EDGE_WEIGHT
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
        int ROOT_START = edges[nonCriticalRoots[i][0]].first.first;
        int n = 16 - (int) nonCriticalRoots[i].size() * 3;
        std::cout << std::setw(n) << "(" << ROOT_START;
        for (int j: nonCriticalRoots[i]) {
            int EDGE_END = edges[j].first.second;
            std::cout << ", " << EDGE_END;
        }
        std::cout << ") | " << std::setw(13) << R[i] << "|" << std::setw(21) << N[i] << "|\n";
    }
    std::cout << "---------------------------------------------------------\n";
}

void Task1() {
    std::cout << "Task 1" << std::endl;
//    const std::vector<int> t1 = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 12, 9};
    const std::vector<int> t1 = {0, 5, 8, 3, 6, 4, 1, 2, 6, 3, 9, 7};
    setWeights(t1);
    std::vector<std::pair<int, int>> eventOccurrenceTime = fillGraph();
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves(eventOccurrenceTime);
    std::vector<bool> criticalEvents = findCriticalEvents(eventOccurrenceTime);
    std::vector<bool> criticalEdges = findCriticalEdges(timeReserves);
    std::vector<std::vector<int>> nonCriticalRoots = findNonCriticalRoots(criticalEvents, criticalEdges);
    std::vector<int> nonCriticalReserves;
    std::vector<double> nonCriticalTensionCoefficients = findTensionCoefficients(nonCriticalRoots, criticalEvents,
                                                                                 &nonCriticalReserves);
    showEventTable(eventOccurrenceTime, criticalEvents);
    showCriticalEvents(criticalEvents);
    showWorkTable(criticalEdges, timeReserves);
    showCriticalEdges(criticalEdges);
    showNonCriticalRootsTable(nonCriticalRoots, nonCriticalReserves, nonCriticalTensionCoefficients);
}

void Task2Func(const std::vector<int> &t_expected, const std::vector<double> &dispersion, const double &RELIABILITY,
               const double &DEADLINE) {
    setWeights(t_expected);
    std::vector<std::pair<int, int>> eventOccurrenceTime = fillGraph();
    std::vector<bool> criticalEvents = findCriticalEvents(eventOccurrenceTime);
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves(eventOccurrenceTime);
    std::vector<bool> criticalEdges = findCriticalEdges(timeReserves);
    double criticalDispersion = findCriticalDispersion(criticalEdges, dispersion);
    showEventTable(eventOccurrenceTime, criticalEvents);
    showWorkTable(criticalEdges, timeReserves, &dispersion);
    std::cout << "\nDispersion of critical root: " << criticalDispersion << std::endl;
    double probability =
            0.5 + Laplace((DEADLINE - eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second) / criticalDispersion);
    std::cout << "Probability of completion in time: " << probability * 100 << "%\n";
    std::cout << "Guarantied completion time: "
              << eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second - std::round(3 * criticalDispersion)
              << "-" << eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second + std::round(3 * criticalDispersion)
              << std::endl;
    //Извините, мне лень делать обратную функцию Лапласа
//    double a = 1.65;
    double a = 1.96;
    std::cout << "Completion time with " << RELIABILITY * 100 << "% reliability: "
              << std::round(eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second - a * criticalDispersion) << "-"
              << std::round(eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second + a * criticalDispersion) << std::endl;
    a = 1.65;
    std::cout << "Maximum completion time with " << RELIABILITY * 100 << "% reliability: "
              << std::round(eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second + a * criticalDispersion) << std::endl;
}

void Task2() {
    std::cout << "\nTask 2" << std::endl;
//    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 12, 9};
//    const std::vector<int> t_probable = {0, 9, 5, 3, 4, 6, 4, 5, 5, 7, 8, 10, 5};
//    const std::vector<int> t_optimistic = {0, 3, 4, 2, 1, 2, 2, 3, 1, 4, 6, 3, 2};
//    const double RELIABILITY = 0.9;
//    const int DEADLINE = 29;
    const std::vector<int> t_pessimistic = {0, 8, 10, 6, 9, 5, 2, 4, 13, 8, 17, 10};
    const std::vector<int> t_probable = {0, 5, 9, 2, 7, 4, 1, 2, 5, 2, 8, 8};
    const std::vector<int> t_optimistic = {0, 3, 4, 1, 1, 1, 1, 1, 4, 1, 6, 2};
    const std::vector<double> dispersion = findDispersion(t_pessimistic, t_optimistic);
    const double RELIABILITY = 0.95;
    const int DEADLINE = 21;
    std::vector<int> t_expected = findExpectedTime(t_pessimistic, t_optimistic, &t_probable);
    std::cout << "\nThree-parameter model: " << std::endl;
    Task2Func(t_expected, dispersion, RELIABILITY, DEADLINE);
    //-----------------------------------------------------------------------------
    t_expected = findExpectedTime(t_pessimistic, t_optimistic);
    std::cout << "\nTwo-parameter model: " << std::endl;
    Task2Func(t_expected, dispersion, RELIABILITY, DEADLINE);
}

void Task3() {
    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 12, 9};
    const std::vector<int> t_optimistic = {0, 3, 4, 2, 1, 2, 2, 3, 1, 4, 6, 3, 2};
    const std::vector<int> s = {0, 9, 6, 3, 5, 7, 2, 4, 5, 8, 9, 2, 6};
    const int PROJECT_COST = 10;
    setWeights(t_pessimistic);
    std::vector<std::pair<int, int>> eventOccurrenceTime = fillGraph();
    std::vector<bool> criticalEvents = findCriticalEvents(eventOccurrenceTime);
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves(eventOccurrenceTime);
    std::vector<bool> criticalEdges = findCriticalEdges(timeReserves);
    std::vector<std::vector<int>> nonCriticalRoots = findNonCriticalRoots(criticalEvents, criticalEdges);
    std::vector<int> nonCriticalReserves;
    //Да, я вызываю функцию поиска коэффициентов напряженности для нахождения резервов дуги, и что вы мне сделаете
    findTensionCoefficients(nonCriticalRoots, criticalEvents, &nonCriticalReserves);
    int TOTAL_COST = PROJECT_COST * eventOccurrenceTime[NUMBER_OF_VERTICES - 1].second;
}


int main() {
    std::cout << std::setprecision(2);
    Task1();
    Task2();
    Task3();
    return 0;
}
