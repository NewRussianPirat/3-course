#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
#include <utility>

const int NUMBER_OF_EVENTS = 8;
const int NUMBER_OF_EDGES = 13;
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
    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 6, 10, 8, 9, 11, 12, 9};
//    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 6, 10, 8, 8, 11, 12, 9};
//    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 2, 10, 8, 8, 11, 12, 9};
//    const std::vector<int> t_pessimistic = {0, 15, 7, 5, 6, 8, 2, 10, 8, 8, 11, 12, 3};
//    const std::vector<int> t_pessimistic = {0, 8, 7, 5, 6, 8, 2, 10, 8, 8, 11, 12, 3};
//------------------------------------- Done -----------------------------------------------
    const std::vector<int> t_probable = {0, 9, 5, 3, 4, 6, 4, 5, 5, 7, 8, 12, 5};
    const std::vector<int> t_optimistic = {0, 3, 4, 2, 1, 2, 2, 3, 1, 4, 6, 3, 2};
//    const std::vector<int> t_optimistic = {0, 3, 4, 2, 1, 3, 2, 6, 4, 4, 6, 9, 2};
    const std::vector<int> workReductionCost = {0, 9, 6, 3, 5, 7, 2, 4, 5, 8, 9, 2, 6};
    const double RELIABILITY = 0.9;
    const int DEADLINE = 29;
    const int PROJECT_DAY_COST = 10;

//Определение графа
//const int NUMBER_OF_EVENTS = 7;
//const int NUMBER_OF_EDGES = 12;
//std::vector<std::pair<const std::pair<int, int>, int>> edges = {
//        {{3, 5}, 0}, //b0
//        {{0, 1}, 0}, //b1
//        {{0, 3}, 0}, //b2
//        {{0, 2}, 0}, //b3
//        {{1, 4}, 0}, //b4
//        {{1, 3}, 0}, //b5
//        {{2, 3}, 0}, //b6
//        {{3, 4}, 0}, //b7
//        {{3, 6}, 0}, //b8
//        {{4, 6}, 0}, //b9
//        {{2, 5}, 0}, //b10
//        {{5, 6}, 0}, //b11
//};
//const std::vector<int> t_pessimistic = {0, 8, 10, 6, 9, 5, 2, 4, 13, 8, 17, 10};
//const std::vector<int> t_probable = {0, 5, 9, 2, 7, 4, 1, 2, 5, 2, 8, 8};
//const std::vector<int> t_optimistic = {0, 3, 4, 1, 1, 1, 1, 1, 4, 1, 6, 2};
//const std::vector<int> workReductionCost = {0, 6, 8, 4, 6, 3, 2, 3, 9, 5, 10, 7};
//const double RELIABILITY = 0.95;
//const int DEADLINE = 21;
//const int PROJECT_DAY_COST = 10;

double Laplace(double x) {
    return 0.5 * std::erf(x / std::sqrt(2.0));
}

void dfs(int begin, int target, const std::vector<bool> &criticalEvents, const std::vector<bool> &criticalEdges,
         std::vector<std::vector<int>> &nonCriticalRoots, std::vector<int> *root = nullptr) {
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
    std::vector<std::pair<int, int>> eventOccurrenceTime(NUMBER_OF_EVENTS);
    eventOccurrenceTime[0] = {0, 0};
    for (int i = 1; i < NUMBER_OF_EVENTS; ++i) {
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
    eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second = eventOccurrenceTime[NUMBER_OF_EVENTS - 1].first;
    for (int i = NUMBER_OF_EVENTS - 2; i >= 0; --i) {
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
    std::vector<bool> criticalEvents(NUMBER_OF_EVENTS, false);
    for (int i = 0; i < NUMBER_OF_EVENTS; ++i) {
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
    for (int i = 0; i < NUMBER_OF_EVENTS; ++i) {
        if (criticalEvents[i]) {
            for (int j = i + 1; j < NUMBER_OF_EVENTS; ++j) {
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
                        std::vector<int> &nonCriticalReserves) {
    std::vector<double> nonCriticalTensionCoefficients;
    for (auto i: nonCriticalRoots) {
        int start = edges[*i.begin()].first.first;
        int finish = edges[*(--i.end())].first.second;
        int a = findCriticalRootWeight(start, finish, criticalEvents);
        int b = 0;
        for (auto j: i) {
            b += edges[j].second;
        }
        nonCriticalReserves.push_back(a - b);
        nonCriticalTensionCoefficients.push_back(double(b) / a);
    }
    return nonCriticalTensionCoefficients;
}

std::vector<int> findExpectedTime(bool withT_Probable) {
    std::vector<int> t_expected(NUMBER_OF_EDGES);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (withT_Probable) {
            t_expected[i] = (int) (std::round(
                    (double) (t_pessimistic[i] + 4 * t_probable[i] + t_optimistic[i]) / 6));
        } else {
            t_expected[i] = (int) std::round((double) (3 * t_pessimistic[i] + 2 * t_optimistic[i]) / 5);
        }
    }
    return t_expected;
}

std::vector<double> findDispersion() {
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
findSuitableForReductionWorks(std::vector<std::vector<int>> &works, std::vector<int> &costReduction,
                              const std::vector<std::vector<int>> &criticalRoots,
                              const std::vector<int> &timeReductionReserve,
                              const std::vector<int> &nonCriticalRoot,
                              int index, int totalCostReduction,
                              std::vector<int> *setOfWorks = nullptr) {
    if (index == criticalRoots.size()) {
        return;
    }
    for (int j = 0; j < criticalRoots[index].size(); ++j) {
        int CURRENT_EDGE = criticalRoots[index][j];
        if (timeReductionReserve[CURRENT_EDGE] != 0) {
            if (totalCostReduction - workReductionCost[CURRENT_EDGE] >= 0) {
                if (setOfWorks != nullptr) {
                    setOfWorks->push_back(CURRENT_EDGE);
                } else {
                    setOfWorks = new std::vector<int>({CURRENT_EDGE});
                }
                findSuitableForReductionWorks(works, costReduction, criticalRoots, timeReductionReserve, nonCriticalRoot, index + 1,
                                              totalCostReduction - workReductionCost[CURRENT_EDGE], setOfWorks);
                if (setOfWorks != nullptr && setOfWorks->size() == criticalRoots.size()) {
                    works.push_back(*setOfWorks);
                    costReduction.push_back(totalCostReduction - workReductionCost[CURRENT_EDGE]);

                }
                if (setOfWorks != nullptr) {
                    setOfWorks->pop_back();
                }
            }
        }
    }
}


std::vector<int> findNonCriticalReservesTask3(const std::vector<std::vector<int>> &criticalRoots,
                                              const std::vector<std::vector<int>> &nonCriticalRoots,
                                              const std::vector<std::pair<int, int>> &eventOccurrenceTime) {
    std::vector<int> nonCriticalReserves;
    for (auto i: nonCriticalRoots) {
        bool eventsAreInDifferentRoots = true;
        int start = edges[*i.begin()].first.first;
        int finish = edges[*(--i.end())].first.second;
        std::vector<int> criticalRoot;
        for (auto &j: criticalRoots) {
            bool startInRoot = false;
            bool finishInRoot = false;
            for (auto k: j) {
                if (!startInRoot && edges[k].first.first == start) {
                    startInRoot = true;
                }
                if (!finishInRoot && edges[k].first.second == finish) {
                    finishInRoot = true;
                }
                if (startInRoot && finishInRoot) {
                    eventsAreInDifferentRoots = false;
                    criticalRoot = j;
                    break;
                }
            }
            if (!eventsAreInDifferentRoots) {
                break;
            }
        }
        int b = 0;
        for (auto j: i) {
            b += edges[j].second;
        }
        if (eventsAreInDifferentRoots) {
            nonCriticalReserves.push_back(eventOccurrenceTime[finish].first - eventOccurrenceTime[start].first - b);
        } else {
            int a = 0;
            bool begin = false;
            for (auto j: criticalRoot) {
                if (!begin && edges[j].first.first == start) {
                    begin = true;
                }
                if (begin) {
                    a += edges[j].second;
                }
                if (edges[j].first.second == finish) {
                    break;
                }
            }
            nonCriticalReserves.push_back(a - b);
        }
    }
    return nonCriticalReserves;
}

void findPossibleEdgesCombination(const std::vector<std::vector<int>> &nonCriticalRoots,
                                  const std::vector<int> &parallelNonCriticalRoots,
                                  std::vector<std::pair<int, int>> &minimalReserveRoots,
                                  std::vector<std::vector<int>> &possibleEdgesCombination,
                                  int index,
                                  std::vector<int> *currentCombination = nullptr) {
    const auto &currentRoot = nonCriticalRoots[parallelNonCriticalRoots[index]];
    for (int i = 0; i < currentRoot.size(); ++i) {
        if (currentCombination == nullptr) {
            currentCombination = new std::vector({currentRoot[i]});
        } else {
            currentCombination->push_back(currentRoot[i]);
        }
        for (int j = 0; j < minimalReserveRoots.size(); ++j) {
//TODO: Остановился здесь
        }
    }
}

void
showEventTable(const std::vector<std::pair<int, int>> &eventOccurrenceTime, const std::vector<bool> &criticalEvents) {
    std::string delimiter = "------------------------------------------------";
    std::string s1 = " Event | Early Date | Late Date | Time Reserve |";

    std::cout << "\n" << delimiter << "\n" << s1 << "\n" << delimiter << "\n";
    for (int i = 0; i < NUMBER_OF_EVENTS; ++i) {
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
    for (int i = 0; i < NUMBER_OF_EVENTS; ++i) {
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

void setGraphInfo(std::vector<std::pair<int, int>> &eventOccurrenceTime,
                  std::vector<std::pair<int, int>> &timeReserves,
                  std::vector<bool> &criticalEvents,
                  std::vector<bool> &criticalEdges) {
    eventOccurrenceTime = fillGraph();
    timeReserves = findTimeReserves(eventOccurrenceTime);
    criticalEvents = findCriticalEvents(eventOccurrenceTime);
    criticalEdges = findCriticalEdges(timeReserves);
}


void Task1() {
    std::cout << "Task 1" << std::endl;
    setWeights(t_pessimistic);
    std::vector<std::pair<int, int>> eventOccurrenceTime = fillGraph();
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves(eventOccurrenceTime);
    std::vector<bool> criticalEvents = findCriticalEvents(eventOccurrenceTime);
    std::vector<bool> criticalEdges = findCriticalEdges(timeReserves);
    setGraphInfo(eventOccurrenceTime, timeReserves, criticalEvents, criticalEdges);
    std::vector<std::vector<int>> nonCriticalRoots = findNonCriticalRoots(criticalEvents, criticalEdges);
    std::vector<int> nonCriticalReserves;
    std::vector<double> nonCriticalTensionCoefficients = findTensionCoefficients(nonCriticalRoots, criticalEvents,
                                                                                 nonCriticalReserves);
    showEventTable(eventOccurrenceTime, criticalEvents);
    showCriticalEvents(criticalEvents);
    showWorkTable(criticalEdges, timeReserves);
    showCriticalEdges(criticalEdges);
    showNonCriticalRootsTable(nonCriticalRoots, nonCriticalReserves, nonCriticalTensionCoefficients);
}

void Task2Func(const std::vector<int> &t_expected, const std::vector<double> &dispersion) {
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
            0.5 + Laplace((DEADLINE - eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second) / criticalDispersion);
    std::cout << "Probability of completion in time: " << probability * 100 << "%\n";
    std::cout << "Guarantied completion time: "
              << eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second - std::round(3 * criticalDispersion)
              << "-" << eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second + std::round(3 * criticalDispersion)
              << std::endl;
    //Извините, мне лень делать обратную функцию Лапласа
//    double a = 1.65;
    double a = 1.96;
    std::cout << "Completion time with " << RELIABILITY * 100 << "% reliability: "
              << std::round(eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second - a * criticalDispersion) << "-"
              << std::round(eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second + a * criticalDispersion) << std::endl;
//    a = 1.28;
    a = 1.65;
    std::cout << "Maximum completion time with " << RELIABILITY * 100 << "% reliability: "
              << std::round(eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second + a * criticalDispersion) << std::endl;
}

void Task2() {
    std::cout << "\nTask 2" << std::endl;
    const std::vector<double> dispersion = findDispersion();
    std::vector<int> t_expected = findExpectedTime(true);
    std::cout << "\nThree-parameter model: " << std::endl;
    Task2Func(t_expected, dispersion);
    //-----------------------------------------------------------------------------
    t_expected = findExpectedTime(false);
    std::cout << "\nTwo-parameter model: " << std::endl;
    Task2Func(t_expected, dispersion);
}

void Task3() {
    std::cout << "\nTask 3" << std::endl;
    std::vector<int> timeReductionReserve(NUMBER_OF_EDGES);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        timeReductionReserve[i] = t_pessimistic[i] - t_optimistic[i];
    }
    setWeights(t_pessimistic);
    std::vector<std::pair<int, int>> eventOccurrenceTime = fillGraph();
    std::vector<bool> criticalEvents = findCriticalEvents(eventOccurrenceTime);
    std::vector<std::pair<int, int>> timeReserves = findTimeReserves(eventOccurrenceTime);
    std::vector<bool> criticalEdges = findCriticalEdges(timeReserves);
    std::vector<std::vector<int>> nonCriticalRoots;
    std::vector<int> nonCriticalReserves;
    std::vector<std::vector<int>> criticalRoots(1);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (criticalEdges[i]) {
            criticalRoots[0].push_back(i);
        }
    }
    int TOTAL_COST = PROJECT_DAY_COST * eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second;
//    int MAX_COST = 0;
//    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
//        MAX_COST += timeReductionReserve[i] * workReductionCost[i];
//    }
//    do {
//        nonCriticalRoots = findNonCriticalRoots(criticalEvents, criticalEdges);
//        nonCriticalReserves = findNonCriticalReservesTask3(criticalRoots, nonCriticalRoots,
//                                                           eventOccurrenceTime);
//        int nonCriticalRootIndex = (int) (std::min_element(nonCriticalReserves.begin(), nonCriticalReserves.end()) -
//                                          nonCriticalReserves.begin());
//        int timeReduction = nonCriticalReserves[nonCriticalRootIndex];
//        std::vector<std::vector<int>> works;
//        std::vector<int> costReduction;
////        findSuitableForReductionWorks(works, costReduction, criticalRoots, timeReductionReserve, 0, PROJECT_DAY_COST);
//        if (works.empty()) {
//            break;
//        }
//        std::vector<int> workReductionTimeReserve(works.size(), INT_MAX);
//        for (int i = 0; i < works.size(); ++i) {
//            for (int j = 0; j < works[i].size(); ++j) {
//                workReductionTimeReserve[i] = std::min(workReductionTimeReserve[i], timeReductionReserve[works[i][j]]);
//            }
//        }
//        if (std::accumulate(workReductionTimeReserve.begin(), workReductionTimeReserve.end(), 0) >= timeReduction) {
//            while (timeReduction != 0) {
//                int CURRENT_WORK = (int) (std::max_element(costReduction.begin(), costReduction.end()) -
//                                          costReduction.begin());
//                if (timeReduction > workReductionTimeReserve[CURRENT_WORK]) {
//                    TOTAL_COST -= workReductionTimeReserve[CURRENT_WORK] * costReduction[CURRENT_WORK];
//                    timeReduction -= workReductionTimeReserve[CURRENT_WORK];
//                    for (auto i: works[CURRENT_WORK]) {
//                        timeReductionReserve[i] -= workReductionTimeReserve[CURRENT_WORK];
//                        edges[i].second -= workReductionTimeReserve[CURRENT_WORK];
//                    }
//                    costReduction[CURRENT_WORK] = INT_MIN;
//                } else {
//                    TOTAL_COST -= timeReduction * costReduction[CURRENT_WORK];
//                    for (auto i: works[CURRENT_WORK]) {
//                        timeReductionReserve[i] -= timeReduction;
//                        edges[i].second -= timeReduction;
//                    }
//                    timeReduction = 0;
//                }
//                eventOccurrenceTime = fillGraph();
//            }
//            criticalRoots.push_back(nonCriticalRoots[nonCriticalRootIndex]);
//            for (auto i: nonCriticalRoots[nonCriticalRootIndex]) {
//                criticalEdges[i] = true;
//                criticalEvents[edges[i].first.second] = true;
//            }
//        } else {
//            break;
//        }
//    } while (true);
//    showEventTable(eventOccurrenceTime, criticalEvents);
//    timeReserves = findTimeReserves(eventOccurrenceTime);
//    showWorkTable(criticalEdges, timeReserves);
//    std::cout << "\nOptimal variant: " << "\nMinimum completion time: "
//              << eventOccurrenceTime[NUMBER_OF_EVENTS - 1].first << "\nCost of the project: " << TOTAL_COST
//              << std::endl;
//    setWeights(t_optimistic);
//    eventOccurrenceTime = fillGraph();
//    MAX_COST += eventOccurrenceTime[NUMBER_OF_EVENTS - 1].second * PROJECT_DAY_COST;
//    timeReserves = findTimeReserves(eventOccurrenceTime);
//    criticalEvents = findCriticalEvents(eventOccurrenceTime);
//    criticalEdges = findCriticalEdges(timeReserves);
//    nonCriticalRoots = findNonCriticalRoots(criticalEvents, criticalEdges);
//    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
//        if (criticalEdges[i]) {
//            criticalRoots[0].push_back(i);
//        }
//    }
//    nonCriticalReserves = findNonCriticalReservesTask3(criticalRoots, nonCriticalRoots, eventOccurrenceTime);
//    showEventTable(eventOccurrenceTime, criticalEvents);
//    showWorkTable(criticalEdges, timeReserves);
//    std::vector<int> parallelNonCriticalRoots;
//    std::vector<std::pair<int, int>> minimalReserveRoots;
//    int MINIMAL_RESERVE = *(std::min_element(nonCriticalReserves.begin(), nonCriticalReserves.end()));
//    for (int i = 0; i < nonCriticalRoots.size(); ++i) {
//        if (edges[nonCriticalRoots[i][0]].first.first == 0 &&
//            edges[*(--nonCriticalRoots[i].end())].first.second == NUMBER_OF_EVENTS - 1) {
//            bool haveSame = false;
//            for (auto j: parallelNonCriticalRoots) {
//                if (nonCriticalReserves[j] == nonCriticalReserves[i]) {
//                    haveSame = true;
//                    break;
//                }
//            }
//            if (!haveSame) {
//                parallelNonCriticalRoots.push_back(i);
//            }
//        }
//    }

}

int main() {
    std::cout << std::setprecision(2);
    Task1();
    Task2();
//    Task3();
    return 0;
}
