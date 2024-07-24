#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Node;

class Edge{
    public:
    Node* target;
    int capacity;
    int criticalCount = 0;
    shared_ptr<Edge> backwardsEdge = nullptr;
    Edge(Node* t, int c, shared_ptr<Edge> b);
    Edge(Node* t, int c);
};

class Node {
    public:
    int index;
    vector<shared_ptr<Edge>> adj;
    Node(int i);
    shared_ptr<Edge> goesTo(Node* node);
};

class Graph {
public:
    Node** vertices = nullptr;
    unsigned int numberOfNodes = 0;
    unsigned int numberOfEdges = 0;
    void resize(int size);
    shared_ptr<Edge> addEdge(const int from, const int to, int cost);
    vector<shared_ptr<Edge>>& getNeighbors(int nodeIndex);
    void destroy();
    void addBackwardsEdges();
};
#endif