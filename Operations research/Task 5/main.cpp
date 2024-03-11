#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <utility>

const int NUMBER_OF_VERTICES = 8;
const int NUMBER_OF_EDGES = 13;
std::vector<std::pair<int, int>> vertices(NUMBER_OF_VERTICES);
std::vector<std::pair<const std::pair<int, int>, int>> edges = {
        {{5, 6}, 0},
        {{0, 1}, 15},
        {{0, 2}, 7},
        {{1, 3}, 5},
        {{2, 3}, 6},
        {{2, 4}, 8},
        {{3, 4}, 6},
        {{3, 6}, 10},
        {{4, 5}, 8},
        {{4, 6}, 9},
        {{4, 7}, 11},
        {{2, 7}, 10},
        {{6, 7}, 9}
};

//TODO: Алгоритм ищет все некритичные пути, теперь надо алгоритм, который ищет один критичный путь
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
            root->pop_back();
        }
    }
}

int findCriticalRoot(int start, int finish, const std::vector<bool> &criticalVertices) {
    if (start == finish) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        int EDGE_BEGIN = edges[i].first.first;
        int EDGE_END = edges[i].first.second;
        if (EDGE_BEGIN == start && criticalVertices[EDGE_END]) {
            result += findCriticalRoot(EDGE_END, finish, criticalVertices) + edges[i].second;
            break;
        }
    }
    return result;
}

//TODO: Пригодится?
void Task1() {

}

void Task2() {

}

void Task3() {

}


int main() {
    //TODO: Судя по всему, это надо будет преобразовать в отдельную функцию
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
    //----------------------------------------------
    std::vector<std::pair<int, int>> r(edges.size());
    for (int i = 0; i < edges.size(); ++i) {
        r[i].first = vertices[edges[i].first.second].second - vertices[edges[i].first.first].first - edges[i].second;
        r[i].second = vertices[edges[i].first.second].first - vertices[edges[i].first.first].second - edges[i].second;
    }
    std::vector<bool> criticalVertices(NUMBER_OF_VERTICES, false);
    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        if (vertices[i].first == vertices[i].second) {
            criticalVertices[i] = true;
        }
    }
    std::vector<bool> criticalEdges(NUMBER_OF_EDGES, false);
    for (int i = 0; i < NUMBER_OF_EDGES; ++i) {
        if (r[i].first == 0 && r[i].second == 0) {
            criticalEdges[i] = true;
        }
    }
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
    std::vector<int> R;
    std::vector<double> N;
    for (auto i : nonCriticalRoots) {
        int start = edges[*i.begin()].first.first;
        int finish = edges[*(--i.end())].first.second;
        int a = findCriticalRoot(start, finish, criticalVertices);
        int b = 0;
        for (auto j : i) {
            b += edges[j].second;
        }
        R.push_back(a - b);
        N.push_back(double(b) / a);
    }
    return 0;
}
