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

std::vector<int> dijkstraAlgorithm() {
    std::set<std::pair<int, int>> q;
    std::vector<int> source(n, -1);
    std::vector<int> distance(n, INT_MAX);
    std::vector<bool> visited(n, false);
    int current = s;
    int parent = current;
    q.insert({0,current});
    distance[current] = 0;
    source[current] = parent;
    while(!q.empty()) {
        while(!q.empty() && visited[q.begin()->second]) {
            q.erase(q.begin());
        }
        if(q.empty()) {
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
    std::ifstream fin("../in.txt");
    std::ofstream fout("../out.txt");
    if (fin.is_open()) {
        fin >> n;
        fin >> m;
        c.resize(n);
        for(int i = 0; i < n; ++i) {
            c[i].resize(n);
        }
        for (int i = 0; i < n; ++i) {
            int a = 1;
            while (a != 0) {
                fin >> a;
                if (a - 1 < i) {
                    continue;
                }
                c[i][a - 1] = 1;
                c[a - 1][i] = 1;
            }
        }
        fin >> s;
        --s;
        fin >> t;
        --t;
        fin.close();
        std::vector<int> route = dijkstraAlgorithm();
        int result = 0;
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
            result += a;
            route = dijkstraAlgorithm();
        }
        fout << result;
        fout.close();
    }
    return 0;
}
