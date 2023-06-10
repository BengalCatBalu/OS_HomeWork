#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <cstdlib>


std::string PATH_TO_OUTPUT_FOLDER = "/Users/efimovivan/Desktop/KDZ-3/";

// Структура для представления ребра графа
struct Edge {
    int from, to, weight;
    Edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}
};

// Функция для генерации полного графа
std::vector<Edge> generateCompleteGraph(int numVertices) {
    std::vector<Edge> edges;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            int weight = rand() % 100 + 1;  // Случайный вес ребра
            edges.emplace_back(i, j, weight);
            edges.emplace_back(j, i, weight);  // Добавляем обратное ребро для неориентированного графа
        }
    }
    return edges;
}

// Функция для генерации связного графа с заданным коэффициентом плотности
std::vector<Edge> generateConnectedGraph(int numVertices, double density) {
    std::vector<Edge> edges;

    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (dis(gen) <= density * 100) {  // Генерируем ребро с вероятностью, основанной на коэффициенте плотности
                int weight = rand() % 100 + 1;  // Случайный вес ребра
                edges.emplace_back(i, j, weight);
                edges.emplace_back(j, i, weight);  // Добавляем обратное ребро для неориентированного графа
            }
        }
    }

    return edges;
}

// Функция для генерации разреженного связного графа (дерева)
std::vector<Edge> generateSparseGraph(int numVertices) {
    std::vector<Edge> edges;

    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 1; i < numVertices; ++i) {
        int weight = rand() % 100 + 1;  // Случайный вес ребра
        edges.emplace_back(i, i / 2, weight);
        edges.emplace_back(i / 2, i, weight);  // Добавляем обратное ребро для неориентированного графа
    }

    return edges;
}

// Реализация алгоритма Дейкстры
std::vector<int> dijkstra(const std::vector<Edge>& edges, int numVertices, int source) {
    const int INF = 1e9;
    std::vector<int> distances(numVertices, INF);
    distances[source] = 0;

    std::vector<bool> visited(numVertices, false);

    for (int i = 0; i < numVertices - 1; ++i) {
        int minDistance = INF;
        int u = -1;

        for (int v = 0; v < numVertices; ++v) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                u = v;
            }
        }

        if (u == -1)
            break;

        visited[u] = true;

        for (const Edge& edge : edges) {
            if (edge.from == u && distances[u] + edge.weight < distances[edge.to]) {
                distances[edge.to] = distances[u] + edge.weight;
            }
        }
    }

    return distances;
}

// Реализация алгоритма Флойда-Уоршелла
std::vector<std::vector<int>> floydWarshall(const std::vector<Edge>& edges, int numVertices) {
    const int INF = 1e9;
    std::vector<std::vector<int>> distances(numVertices, std::vector<int>(numVertices, INF));

    for (int i = 0; i < numVertices; ++i) {
        distances[i][i] = 0;
    }

    for (const Edge& edge : edges) {
        distances[edge.from][edge.to] = edge.weight;
        distances[edge.to][edge.from] = edge.weight;
    }

    for (int k = 0; k < numVertices; ++k) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (distances[i][k] != INF && distances[k][j] != INF) {
                    distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
    }

    return distances;
}

// Реализация алгоритма Беллмана-Форда
std::vector<int> bellmanFord(const std::vector<Edge>& edges, int numVertices, int source) {
    const int INF = 1e9;
    std::vector<int> distances(numVertices, INF);
    distances[source] = 0;

    for (int i = 0; i < numVertices - 1; ++i) {
        for (const Edge& edge : edges) {
            if (distances[edge.from] != INF && distances[edge.from] + edge.weight < distances[edge.to]) {
                distances[edge.to] = distances[edge.from] + edge.weight;
            }
        }
    }

    return distances;
}

int main() {
    //std::cout << 1;
    std::vector<int> numVerticesList = {10, 60, 110, 160, 210, 260, 310, 360, 410, 460, 510, 560, 610, 660, 710, 760, 810, 860, 910, 960, 1010};
    int numIterations = 2;

    std::ofstream myfile(PATH_TO_OUTPUT_FOLDER + "Dijkstra.csv");
    myfile << "Number of Vertices, Complete, Connected, Sparse" << std::endl;
    //std::cout << 1;
    for (int numVertices : numVerticesList) {
        std::vector<Edge> completeGraph = generateCompleteGraph(numVertices);
        std::vector<Edge> connectedGraph = generateConnectedGraph(numVertices, 0.4);
        std::vector<Edge> sparseGraph = generateSparseGraph(numVertices);

        long long CompleteTime = 0;
        long long ConnectedTime = 0;
        long long SparseTime = 0;

        for (int i = 0; i < numIterations; ++i) {
            // Измерение времени выполнения алгоритмов
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> dijkstraСompleteResult = dijkstra(completeGraph, numVertices, 0);
            //std::vector<std::vector<int>> floydWarshallResult = floydWarshall(completeGraph, numVertices);
            //std::vector<int> bellmanFordResult = bellmanFord(completeGraph, numVertices, 0);
            auto end = std::chrono::high_resolution_clock::now();
            CompleteTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        for (int i = 0; i < numIterations; ++i) {
            // Измерение времени выполнения алгоритмов
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> dijkstraConnectedResult = dijkstra(connectedGraph, numVertices, 0);
            //std::vector<std::vector<int>> floydWarshallResult = floydWarshall(connectedGraph, numVertices);
            //std::vector<int> bellmanFordResult = bellmanFord(connectedGraph, numVertices, 0);
            auto end = std::chrono::high_resolution_clock::now();
            ConnectedTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        for (int i = 0; i < numIterations; ++i) {
            // Измерение времени выполнения алгоритмов
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> dijkstraSparseResult = dijkstra(sparseGraph, numVertices, 0);
            //std::vector<std::vector<int>> floydWarshallResult = floydWarshall(sparseGraph, numVertices);
            //std::vector<int> bellmanFordResult = bellmanFord(sparseGraph, numVertices, 0);
            auto end = std::chrono::high_resolution_clock::now();
            SparseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        CompleteTime /= numIterations;
        ConnectedTime /= numIterations;
        SparseTime /= numIterations;

        myfile << numVertices << ", " << CompleteTime << ", " << ConnectedTime << ", " << SparseTime << std::endl;
        std::cout << numVertices << ", " << CompleteTime << ", " << ConnectedTime << ", " << SparseTime << std::endl;
    }

    return 0;
}