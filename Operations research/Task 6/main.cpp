#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

int n;
int m;
int s;
int t;
std::vector<std::vector<int>> c;
std::vector<std::vector<int>> c1;

std::vector<int> dijkstraAlgorithm() {
    std::set<std::pair<int, int>> q;
    std::vector<int> source(n, -1);
    std::vector<int> distance(n, INT_MAX);
    std::vector<bool> visited(n, false);
    int current = s;
    int parent = current;
    q.insert({0, current});
    distance[current] = 0;
    source[current] = parent;
    while (!q.empty()) {
        while (!q.empty() && visited[q.begin()->second]) {
            q.erase(q.begin());
        }
        if (q.empty()) {
            break;
        }
        current = q.begin()->second;
        visited[current] = true;
        parent = current;
        for (int j = 0; j < n; ++j) {
            if (c[current][j] != 0) {
                distance[j] = std::min(distance[j], distance[current] + c[current][j]);
                if (source[j] == -1 || distance[j] == distance[current] + c[current][j]) {
                    source[j] = parent;
                }
                q.insert({distance[j], j});
            }
        }
    }
    if (source[t] != -1) {
        std::vector<int> route;
        route.push_back(t);
        int i = t;
        while (source[i] != i) {
            route.push_back(source[i]);
            i = source[i];
        }
        return route;
    }
    return {};
}

int main() {
    std::ifstream in1("../in1.txt");
    if (in1.is_open()) {
        in1 >> n;
        in1 >> m;
        s = n;
        t = n + 1;
        c.resize(n + 2);
        c1.resize(n);
        for (int i = 0; i < n + 2; ++i) {
            c[i].resize(n + 2);
            if (i < n) {
                c1[i].resize(n);
            }
        }
        for (int i = 0; i < n / 2; ++i) {
            int a;
            in1 >> a;
            while (a != 0) {
                c[i][a] = 1;
                c1[i][a] = 1;
                in1 >> a;
            }
            c[n][i] = 1;
            c[n - i - 1][n + 1] = 1;
        }
        in1.close();
        n += 2;
        std::vector<int> route = dijkstraAlgorithm();
        std::vector<std::pair<int, int>> graphMatching;
        while (!route.empty()) {
            int a = INT_MAX;
            for (int i = 0; i < route.size() - 1; ++i) {
                if (c[route[i + 1]][route[i]] < a) {
                    a = c[route[i + 1]][route[i]];
                }
            }
            for (int i = 0; i < route.size() - 1; ++i) {
                c[route[i + 1]][route[i]] -= a;
                c[route[i]][route[i + 1]] += a;
            }
            if (route.size() == 4) {
                graphMatching.emplace_back(route[1], route[2]);
            } else {
                graphMatching.erase(
                        std::find(graphMatching.begin(), graphMatching.end(), std::pair(route[3], route[2])));
                graphMatching.emplace_back(route[1], route[2]);
                graphMatching.emplace_back(route[3], route[4]);
            }
            route = dijkstraAlgorithm();
        }
        std::cout << "Maximum matching: ";
        for (const auto &i : graphMatching) {
            std::cout << "(" << i.second << ", " << i.first << "), ";
        }
        for (const auto &i : graphMatching) {
            c1[i.first][i.second] = 1;
            c1[i.second][i.first] = 0;
        }
    }
    std::ifstream in2("../in2.txt");
    if (in2.is_open()) {
        in2 >> n;
        s = 2 * n;
        t = 2 * n + 1;
        c = std::vector<std::vector<int>>(2 * n + 2, std::vector<int>(2 * n + 2));
        for (int i = 0; i < n; ++i) {
            for (int j = n; j < 2 * n; ++j) {
                int a;
                in2 >> a;
                c[i][j] = a;
            }
            c[2 * n][i] = 100;
            c[n + i][2 * n + 1] = 100;
        }
        n += n + 2;
        in2.close();
        std::vector<int> route = dijkstraAlgorithm();
        std::vector<std::pair<int, int>> graphMatching;
        while (!route.empty()) {
            int a = INT_MAX;
            for (int i = 0; i < route.size() - 1; ++i) {
                if (c[route[i + 1]][route[i]] < a) {
                    a = c[route[i + 1]][route[i]];
                }
            }
            for (int i = 0; i < route.size() - 1; ++i) {
                c[route[i + 1]][route[i]] -= a;
                c[route[i]][route[i + 1]] += a;
            }
            c[route[1]][route[0]] = 0;
            c[route[route.size() - 1]][route[route.size() - 2]] = 0;
            if (route.size() == 4) {
                graphMatching.emplace_back(route[1], route[2]);
            } else {
                graphMatching.erase(
                        std::find(graphMatching.begin(), graphMatching.end(), std::pair(route[3], route[2])));
                graphMatching.emplace_back(route[1], route[2]);
                graphMatching.emplace_back(route[3], route[4]);
            }
            route = dijkstraAlgorithm();
        }
        std::cout << "\nMaximum matching: ";
        for (const auto &i : graphMatching) {
            std::cout << "(" << i.second << ", " << i.first << "), ";
        }
    }
    return 0;
}
