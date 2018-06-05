// Graph.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <iterator>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <functional>
 
using namespace std;
 
class RepresType { 
protected:
    int vertex_num = 0;
    bool weighted;
    bool oriented;
public:
    virtual void readGraph(istream & ist) = 0;
    virtual void addEdge(int from, int to, int weight = 1) = 0;
    virtual void removeEdge(int from, int to) = 0;
    virtual int changeEdge(int from, int to, int newWeight) = 0;
    virtual vector<vector<int>> transformToAdjMatrix() = 0;
    virtual vector<set<pair<int, int>>> transformToAdjList() = 0;
    virtual vector <tuple<int, int, int>> transformToListOfEdges() = 0;
    virtual void writeGraph(string fileName) = 0;
    virtual tuple<bool, bool, int> GetInfo() = 0;
};
 
struct Edge {
    int flow;
    int cap;
};
 
vector<string> SplitingTheString(string str, char symbol) {
    vector<string> string_result;
    string curent_string = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != symbol)
            curent_string.push_back(str[i]);
        else {
            if (curent_string != "")
                string_result.push_back(curent_string);
            curent_string = "";
        }
    }
    if (curent_string != "")
        string_result.push_back(curent_string);
    return string_result;
}
 
class DSU {
private:
    int size = 0;
    vector <int> rank;
    vector <int> parent;
public:
    int find(int x) {
        if (x == parent[x - 1])
            return x;
        return parent[x - 1] = find(parent[x - 1]);
    }
 
    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            if (rank[x - 1] < rank[y - 1])
                swap(x, y);
            parent[y - 1] = x;
            if (rank[x - 1] == rank[y - 1])
                ++rank[x - 1];}
    }
 
    void new_set(int x) {
        if (parent.size() < x) {
            for (int i = size; i < x; i++) {
                rank.push_back(0);
                parent.push_back(0);
            }
            size = x;
        }
        rank[x - 1] = 0;
        parent[x - 1] = x;}
};
 
void QuickSorting(vector<tuple<int, int, int>> &mas, int first, int last)
{
    int middle;
    tuple<int, int, int> count;
    int f = first, l = last;
    middle = get<2>(mas[(f + l) / 2]);
    do
    {
        while (get<2>(mas[f])<middle) f++;
        while (get<2>(mas[l])>middle) l--;
        if (f <= l)
        {
            count = mas[f];
            mas[f] = mas[l];
            mas[l] = count;
            f++;
            l--;
        }
    } while (f < l);
    if (first < l) QuickSorting(mas, first, l);
    if (f < last) QuickSorting(mas, f, last);
}
 
class AdjMatrixGraph :public RepresType {
private:
    vector<vector<int>> adj_matrix;
public:
 
    AdjMatrixGraph() {
 
    }
 
    AdjMatrixGraph(int N) {
        vertex_num = N;
        for (int i = 0; i < N; i++) {
            vector<int> vertex;
            for (int j = 0; j < N; j++)
                vertex.push_back(0);
            adj_matrix.push_back(vertex);
        }
    }
 
    AdjMatrixGraph(vector<vector<int>> matrix, tuple<bool, bool, int> info) {
        adj_matrix = matrix;
        weighted = get<0>(info);
        oriented = get<1>(info);
        vertex_num = get<2>(info);
    }
 
    void readGraph(istream & ist) override {
        ist >> vertex_num;
        ist >> oriented;
        ist >> weighted;
        for (int i = 0; i < vertex_num; i++) {
            vector<int> row;
            for (int j = 0; j < vertex_num; j++) {
                int cell;
                ist >> cell;
                row.push_back(cell);
            }
            adj_matrix.push_back(row);
        }
    }
 
    void addEdge(int from, int to, int weight = 1) override {
        adj_matrix[from - 1][to - 1] = weight;
        if (!oriented)
            adj_matrix[to - 1][from - 1] = weight;
    }
 
    void removeEdge(int from, int to) override {
        adj_matrix[from - 1][to - 1] = 0;
        adj_matrix[to - 1][from - 1] = 0;
    }
 
    int changeEdge(int from, int to, int newWeight) override {
        int weight = adj_matrix[from - 1][to - 1];
        adj_matrix[to - 1][from - 1] = newWeight;
        if (!oriented)
            adj_matrix[from - 1][to - 1] = newWeight;
        return weight;
    }
 
    vector<vector<int>> transformToAdjMatrix() override {
        return adj_matrix;
    }
 
    vector<set<pair<int, int>>> transformToAdjList() override {
        vector<set<pair<int, int>>> adj_list;
        for (int i = 0; i < vertex_num; i++) {
            set<pair<int, int>> vertex;
            for (int j = 0; j < vertex_num; j++) {
                if (adj_matrix[i][j] > 0) {
                    vertex.insert(pair<int, int>(j + 1, adj_matrix[i][j]));
                }
            }
            adj_list.push_back(vertex);
        }
        return adj_list;
    }
 
    vector <tuple<int, int, int>> transformToListOfEdges() override {
        vector<tuple<int, int, int>> list_of_edges;
        for (int i = 0; i < vertex_num; i++) {
            for (int j = 0; j < vertex_num; j++) {
                if (adj_matrix[i][j] > 0) {
                    list_of_edges.push_back(tuple<int, int, int>(i + 1, j + 1, adj_matrix[i][j]));
                    if (!oriented)
                        adj_matrix[j][i] = 0;
                }
            }
        }
        return list_of_edges;
    }
 
    void writeGraph(string fileName) override {
        ofstream file(fileName);
        for (int i = 0; i < vertex_num; i++) {
            for (int j = 0; j < vertex_num; j++)
                file << adj_matrix[i][j] << " ";
            file << endl;
        }
    }
 
    tuple<bool, bool, int> GetInfo() override {
        return tuple<bool, bool, int>(weighted, oriented, vertex_num);
    }
 
};
 
class AdjListGraph :public RepresType {
private:
    vector<set<pair<int, int>>> adj_list;
public:
    AdjListGraph() {
 
    }
 
    AdjListGraph(int n) {
        vertex_num = n;
        for (int i = 0; i < n; i++) {
            set<pair<int, int>> vertex;
            adj_list.push_back(vertex);
        }
    }
 
    AdjListGraph(vector<set<pair<int, int>>> list, tuple<bool, bool, int> info) {
        adj_list = list;
        weighted = get<0>(info);
        oriented = get<1>(info);
        vertex_num = get<2>(info);
    }
 
    void readGraph(istream & ist) override {
        ist >> vertex_num;
        ist >> oriented;
        ist >> weighted;
        string cur_vertexList;
        getline(ist, cur_vertexList);
 
        if (weighted) {
            for (int i = 0; i < vertex_num; i++) {
                getline(ist, cur_vertexList);
                vector<string> neigh = SplitingTheString(cur_vertexList, ' ');
                set<pair<int, int>> adjanceciesOfVertex;
                for (int j = 0; j < neigh.size(); j += 2) {
                    pair<int, int> adjancecy(stoi(neigh[j]), stoi(neigh[j + 1]));
                    adjanceciesOfVertex.insert(adjancecy);
                }
                adj_list.push_back(adjanceciesOfVertex);
            }
        }
        else {
            for (int i = 0; i < vertex_num; i++) {
                getline(ist, cur_vertexList);
                vector<string> neigh = SplitingTheString(cur_vertexList, ' ');
                set<pair<int, int>> adjanceciesOfVertex;
                for (int j = 0; j < neigh.size(); j++) {
                    pair<int, int> adjancecy(stoi(neigh[j]), 1);
                    adjanceciesOfVertex.insert(adjancecy);
                }
                adj_list.push_back(adjanceciesOfVertex);
            }
        }
    }
 
    void addEdge(int from, int to, int weight = 1) override {
        if (!weighted) {
            adj_list[from - 1].insert(pair<int, int>(to, 1));
            if (!oriented)
                adj_list[to - 1].insert(pair<int, int>(from, 1));
        }
        else {
            adj_list[from - 1].insert(pair<int, int>(to, weight));
            if (!oriented)
                adj_list[to - 1].insert(pair<int, int>(from, weight));
        }
    }
 
    void removeEdge(int from, int to) override {
        for (auto iter = adj_list[from - 1].begin(); iter != adj_list[from - 1].end(); iter++)
            if ((iter->first) == to) {
                adj_list[from - 1].erase(iter);
                break;
            }
        if (!oriented) {
            for (auto iter = adj_list[to - 1].begin(); iter != adj_list[to - 1].end(); iter++)
                if ((iter->first) == from) {
                    adj_list[to - 1].erase(iter);
                    break;
                }
        }
    }
 
    int changeEdge(int from, int to, int newWeight) override {
        int weight = 0;
        for (auto iter = adj_list[from - 1].begin(); iter != adj_list[from - 1].end(); iter++)
            if ((iter->first) == to) {
                weight = iter->second;
                break;
            }
        this->removeEdge(from, to);
        this->addEdge(from, to, newWeight);
        if (!oriented) {
            this->removeEdge(to, from);
            this->addEdge(to, from, newWeight);
        }
        return weight;
    }
 
    vector<vector<int>> transformToAdjMatrix() override {
        vector<vector<int>> adj_matrix;
        for (int i = 0; i < vertex_num; i++) {
            vector<int> row;
            for (int j = 0; j < vertex_num; j++)
                row.push_back(0);
            adj_matrix.push_back(row);
        }
 
        if (!weighted) {
            for (int i = 0; i < vertex_num; i++) {
                for (auto iter = adj_list[i].begin(); iter != adj_list[i].end(); iter++) {
                    adj_matrix[i][iter->first - 1] = 1;
                    if (!oriented)
                        adj_matrix[iter->first - 1][i] = 1;
                    else
                        adj_matrix[iter->first - 1][i] = -1;
                }
            }
        }
        else {
            for (int i = 0; i < vertex_num; i++) {
                for (auto iter = adj_list[i].begin(); iter != adj_list[i].end(); iter++) {
                    adj_matrix[i][iter->first - 1] = iter->second;
                    if (!oriented)
                        adj_matrix[iter->first - 1][i] = iter->second;
                    else
                        adj_matrix[iter->first - 1][i] = -(iter->second);
                }
            }
        }
        return adj_matrix;
    }
 
    vector<set<pair<int, int>>> transformToAdjList() override {
        return adj_list;
    }
 
    vector <tuple<int, int, int>> transformToListOfEdges() override {
        vector<tuple<int, int, int>> list_of_edges;
        vector<pair<int, int>> vec;
        for (int i = 0; i < vertex_num; i++) {
            for (auto iter = adj_list[i].begin(); iter != adj_list[i].end(); iter++) {
                bool flag = true;
                for (int j = 0; j < vec.size(); j++)
                    if ((vec[j].first == iter->first) && (vec[j].second == i + 1))
                        flag = false;
                if (flag) {
                    list_of_edges.push_back(tuple<int, int, int>(i + 1, iter->first, iter->second));
                    vec.push_back(pair<int, int>(i + 1, iter->first));
                }
            }
        }
        return list_of_edges;
    }
 
    void writeGraph(string fileName) override {
        ofstream file(fileName);
        file << "L " << vertex_num << endl;
        file << oriented << " " << weighted;
        for (int i = 0; i < vertex_num; i++) {
            file << endl;
            for (auto iter = adj_list[i].begin(); iter != adj_list[i].end(); iter++) {
                if (iter != adj_list[i].begin()) file << " ";
                file << iter->first;
                if (weighted)
                    file << " " << iter->second;
            }
        }
        file.close();
    }
 
    tuple<bool, bool, int> GetInfo() override {
        return tuple<bool, bool, int>(weighted, oriented, vertex_num);
    }
 
    vector<tuple<int, int, int>> getSpaingTreePrima() {
        vector<tuple<int, int, int>> spaingTree;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> prior_queue;
        vector<int> distances(vertex_num, INT32_MAX);
        vector<int> parent(vertex_num, -1);
        vector<bool> marked(vertex_num, false);
 
        prior_queue.push(make_pair(0, 1));
        distances[0] = 0;
 
        while (!prior_queue.empty()) {
            int u = prior_queue.top().second - 1;
            prior_queue.pop();
            marked[u] = true;
            for (auto it = adj_list[u].begin(); it != adj_list[u].end(); ++it) {
                int v = it->first - 1;
                int weight = it->second;
                if (marked[v] == false && distances[v] > weight) {
                    distances[v] = weight;
                    prior_queue.push(make_pair(distances[v], v + 1));
                    parent[v] = u;
                }
            }
        }
 
        for (int i = 1; i < parent.size(); ++i)
            spaingTree.push_back(tuple<int, int, int>(parent[i] + 1, i + 1, distances[i]));
        return spaingTree;
    }
 
    int getnum_of_edges() {
        int num = 0;
        for (int i = 0; i < adj_list.size(); i++) {
            num += adj_list[i].size();
        }
        return num;
    }
  
    int checkBipart() {
        vector<int> marks;
        for (int i = 0; i < adj_list.size(); i++)
            marks.push_back(0);
        queue<int> q;
        q.push(0);
        marks[0] = 1;
        int len = 1;
        int current_mark = 1;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            current_mark *= -1;
            for (auto iter = adj_list[node].begin(); iter != adj_list[node].end(); iter++) {
                if (marks[iter->first - 1] == 0) {
                    marks[iter->first - 1] = current_mark;
                    q.push(iter->first - 1);
                    len++;
                }
                else {
                    if (marks[iter->first - 1] == current_mark) {
                        continue;
                    }
                    else
                        return 0;
                }
                if (len == vertex_num)
                    return len;
            }
        }
        if (len != vertex_num)
            return 0;
        return len;
    }
 
    bool TryKun(int v, vector<bool> &used, vector<int> &vec) {
        if (used[v])  return false;
        used[v] = true;
        for (auto iter = adj_list[v].begin(); iter != adj_list[v].end(); iter++) {
            int to = iter->first - 1;
            if (vec[to] == -1 || TryKun(vec[to], used, vec)) {
                vec[to] = v;
                return true;
            }
        }
        return false;
    }
 
    vector<pair<int, int> > getMaximumMatchingBipart() {
        vector<pair<int, int>> result;
        vector<int> vec;
        vector<bool> used;
        for (int i = 0; i < vertex_num; i++) {
            used.push_back(false);
            vec.push_back(-1);
        }
        for (int i = 0; i < vertex_num; i++)
            TryKun(i, used, vec);
 
        for (int i = 0; i < vertex_num; i++) {
            if (used[i]) {
                result.push_back(pair<int, int>(i + 1, vec[i] + 1));
                used[vec[i]] = false;
                used[i] = false;
            }
        }
        return result;
    }

};
 
class ListOfEdgesGraph :public RepresType {
private:
    int num_of_edges;
    vector <tuple<int, int, int>> list_of_edges;
public:
    ListOfEdgesGraph() {
 
    }
 
    ListOfEdgesGraph(int n) {
        vertex_num = n;
    }
 
    ListOfEdgesGraph(vector <tuple<int, int, int>> list, tuple<bool, bool, int> info) {
        list_of_edges = list;
        weighted = get<0>(info);
        oriented = get<1>(info);
        vertex_num = get<2>(info);
        num_of_edges = list.size();
    }
 
    void readGraph(istream & ist) override {
        ist >> vertex_num;
        ist >> num_of_edges;
        ist >> oriented;
        ist >> weighted;
        string cur_edge;
        getline(ist, cur_edge);
        if (weighted) {
            for (int i = 0; i < num_of_edges; i++) {
                getline(ist, cur_edge);
                vector<string> vertexes = SplitingTheString(cur_edge, ' ');
                list_of_edges.push_back(tuple<int, int, int>(stoi(vertexes[0]), stoi(vertexes[1]), stoi(vertexes[2])));
            }
        }
        else {
            for (int i = 0; i < num_of_edges; i++) {
                getline(ist, cur_edge);
                vector<string> vertexes = SplitingTheString(cur_edge, ' ');
                list_of_edges.push_back(tuple<int, int, int>(stoi(vertexes[0]), stoi(vertexes[1]), 1));
            }
        }
    }
 
    void addEdge(int from, int to, int weight = 1) override {
        if (!weighted) {
            list_of_edges.push_back(tuple<int, int, int>(from, to, 1));
        }
        else {
            list_of_edges.push_back(tuple<int, int, int>(from, to, weight));
        }
        num_of_edges++;
    }
 
    void removeEdge(int from, int to) override {
        for (int i = 0; i < num_of_edges; i++) {
            if ((get<0>(list_of_edges[i]) == from) && (get<1>(list_of_edges[i]) == to)) {
                list_of_edges.erase(list_of_edges.begin() + i);
                break;
            }
        }
        num_of_edges--;
    }
 
    int changeEdge(int from, int to, int newWeight) override {
        int weight = 0;
        for (int i = 0; i < num_of_edges; i++) {
            if ((get<0>(list_of_edges[i]) == from) && (get<1>(list_of_edges[i]) == to)) {
                weight = get<2>(list_of_edges[i]);
                break;
            }
        }
        this->removeEdge(from, to);
        this->addEdge(from, to, newWeight);
        return weight;
    }
 
    vector<vector<int>> transformToAdjMatrix() override {
        vector<vector<int>> adj_matrix;
        for (int i = 0; i < vertex_num; i++) {
            vector<int> row;
            for (int j = 0; j < vertex_num; j++)
                row.push_back(0);
            adj_matrix.push_back(row);
        }
 
        for (int i = 0; i < num_of_edges; i++) {
            adj_matrix[get<0>(list_of_edges[i]) - 1][get<1>(list_of_edges[i]) - 1] = get<2>(list_of_edges[i]);
            if (!oriented)
                adj_matrix[get<1>(list_of_edges[i]) - 1][get<0>(list_of_edges[i]) - 1] = get<2>(list_of_edges[i]);
            else
                adj_matrix[get<1>(list_of_edges[i]) - 1][get<0>(list_of_edges[i]) - 1] = -get<2>(list_of_edges[i]);
        }
        return adj_matrix;
    }
 
    vector<set<pair<int, int>>> transformToAdjList() override {
        vector<set<pair<int, int>>> adj_list;
        for (int i = 0; i < vertex_num; i++) {
            set<pair<int, int>> vertex;
            adj_list.push_back(vertex);
        }
 
        for (int i = 0; i < num_of_edges; i++) {
            adj_list[get<0>(list_of_edges[i]) - 1].insert(pair<int, int>(get<1>(list_of_edges[i]), get<2>(list_of_edges[i])));
            if (!oriented)
                adj_list[get<1>(list_of_edges[i]) - 1].insert(pair<int, int>(get<0>(list_of_edges[i]), get<2>(list_of_edges[i])));
        }
        return adj_list;
    }
 
    vector <tuple<int, int, int>> transformToListOfEdges() override {
        return list_of_edges;
    }
 
    void writeGraph(string fileName) override {
        ofstream file(fileName);
        file << "E " << vertex_num << " " << list_of_edges.size() << endl;
        file << oriented << " " << weighted << endl;
        for (int i = 0; i < num_of_edges; i++) {
            file << get<0>(list_of_edges[i]) << " " << get<1>(list_of_edges[i]);
            if (weighted)
                file << " " << get<2>(list_of_edges[i]);
            file << endl;
        }
        file.close();
    }
 
    tuple<bool, bool, int> GetInfo() override {
        return tuple<bool, bool, int>(weighted, oriented, vertex_num);
    }
 
    vector <tuple<int, int, int>> getSpaingTreeKruscal() {
        vector <tuple<int, int, int>> spaingTree;
        QuickSorting(list_of_edges, 0, list_of_edges.size() - 1);
 
        DSU dsu;
        for (int i = 0; i < vertex_num; i++)
            dsu.new_set(i + 1);
        for (int queue = 0; queue < num_of_edges; queue++) {
            tuple<int, int, int> edge = list_of_edges[queue];
            if (dsu.find(get<0>(edge)) != dsu.find(get<1>(edge))) {
                dsu.unite(dsu.find(get<0>(edge)), dsu.find(get<1>(edge)));
                spaingTree.push_back(edge);
            }
        }
        return spaingTree;
    }
 
    vector <tuple<int, int, int>> getSpaingTreeBoruvka() {
        vector <tuple<int, int, int>> spaingTree;
        DSU dsu;
        for (int i = 0; i < vertex_num; i++)
            dsu.new_set(i + 1);
 
        while (spaingTree.size() < vertex_num - 1) {
            auto min_edges = map<int, int>();
            for (int i = 0; i < vertex_num; ++i)
                min_edges[i] = -1;
            for (int i = 0; i < list_of_edges.size(); ++i)
            {
                auto edge = list_of_edges[i];
                int from = get<0>(edge);
                int to = get<1>(edge);
                int weight = get<2>(edge);
                int fromComponent = dsu.find(from);
                int toComponent = dsu.find(to);
                if (fromComponent != toComponent) {
                    if (min_edges[fromComponent] == -1 || get<2>(list_of_edges[min_edges[fromComponent]]) > weight)
                        min_edges[fromComponent] = i;
                    if (min_edges[toComponent] == -1 || get<2>(list_of_edges[min_edges[toComponent]]) > weight)
                        min_edges[toComponent] = i;
                }
            }
            for (int i = 0; i < min_edges.size(); i++) {
                if (min_edges[i] != -1) {
                    auto edge = list_of_edges[min_edges[i]];
                    dsu.unite(get<0>(edge), get<1>(edge));
                    spaingTree.push_back(tuple<int, int, int>(get<0>(edge), get<1>(edge), get<2>(edge)));
                }
            }
        }
        return spaingTree;
    }
};
 
class Graph {
private:
    RepresType* repres = nullptr;
public:
    Graph() {
 
    }
 
    Graph(int n) {
        repres = new AdjListGraph(n);
    }
 
    void readGraph(string fileName) {
        ifstream file(fileName);
        char repr;
        file >> repr;
        switch (repr)
        {
        case 'C':   repres = new AdjMatrixGraph(); break;
        case 'L':   repres = new AdjListGraph(); break;
        case 'E':   repres = new ListOfEdgesGraph(); break;
        }
        repres->readGraph(file);
        file.close();
    }
 
    void addEdge(int from, int to, int weight = 1) {
        repres->addEdge(from, to, weight);
    }
 
    void removeEdge(int from, int to) {
        repres->removeEdge(from, to);
    }
 
    int changeEdge(int from, int to, int newWeight) {
        return repres->changeEdge(from, to, newWeight);
    }
 
    void transformToAdjMatrix() {
        repres = new AdjMatrixGraph(repres->transformToAdjMatrix(), repres->GetInfo());
    }
 
    void transformToAdjList() {
        repres = new AdjListGraph(repres->transformToAdjList(), repres->GetInfo());
    }
 
    void transformToListOfEdges() {
        repres = new ListOfEdgesGraph(repres->transformToListOfEdges(), repres->GetInfo());
    }
 
    void writeGraph(string fileName) {
        repres->writeGraph(fileName);
    }
 
    Graph getSpaingTreePrima() {
        this->transformToAdjList();
        vector<tuple<int, int, int>> minimalSpanningTree = reinterpret_cast<AdjListGraph*>(repres)->getSpaingTreePrima();
        Graph* spanningTree = new Graph();
        spanningTree->repres = new ListOfEdgesGraph(minimalSpanningTree, repres->GetInfo());
        return *spanningTree;
    }
 
    Graph getSpaingTreeKruscal() {
        this->transformToListOfEdges();
        vector <tuple<int, int, int>> minimalSpanningTree = reinterpret_cast<ListOfEdgesGraph*>(repres)->getSpaingTreeKruscal();
        Graph* spaingTree = new Graph();
        spaingTree->repres = new ListOfEdgesGraph(minimalSpanningTree, repres->GetInfo());
        return *spaingTree;
    }
 
    Graph getSpaingTreeBoruvka() {
        this->transformToListOfEdges();
        vector <tuple<int, int, int>> minimalSpanningTree = reinterpret_cast<ListOfEdgesGraph*>(repres)->getSpaingTreeKruscal();
        Graph* spaingTree = new Graph();
        spaingTree->repres = new ListOfEdgesGraph(minimalSpanningTree, repres->GetInfo());
        return *spaingTree;
    }
 
    int checkBipart(vector<char> &marks) {
        this->transformToAdjList();
        return reinterpret_cast<AdjListGraph*>(repres)->checkBipart();
    }
 
    vector<pair<int, int> > getMaximumMatchingBipart() {
        this->transformToAdjList();
        return reinterpret_cast<AdjListGraph*>(repres)->getMaximumMatchingBipart();
    }
};
 
int main()
{
    return 0;
}

