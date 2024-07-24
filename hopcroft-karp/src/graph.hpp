#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <unordered_set>
#include <deque>
#include <vector>
#include <memory>
#include <map>


#include <iostream>
#include <fstream>
#include <sstream>
extern int phases;
using namespace std;

class Node;

class Edge{
    public:
    Node* target;
    double capacity;
    bool selected = false;
    shared_ptr<Edge> backwardsEdge = nullptr;
    Edge(Node* t, int c, shared_ptr<Edge> b);
    Edge(Node* t, int c);
};

class Node {
    public:
    int index;
    bool visited = false;
    vector<shared_ptr<Edge>> adj;
    
    Node(int i);
    shared_ptr<Edge> goesTo(Node* node);
};


class Graph {
    static vector<shared_ptr<Edge>> emptyVec;
public:
    Node** vertices = nullptr;
    map<int,int> mate;
    unordered_set<int> v1FreeNodes;
    unordered_set<int> v2FreeNodes;
    shared_ptr<Graph> DAG;
    unsigned int matches = 0;
    unsigned int v1Size = 0;
    unsigned int numberOfNodes = 0;
    unsigned int numberOfEdges = 0;
    
    shared_ptr<Edge> addEdge(unsigned int from, unsigned int to, double capacity);
    vector<shared_ptr<Edge>>& getNeighbors(unsigned int nodeIndex);
    void resize(unsigned int newSize);
    void destroy();
    int hopcroftKarp();
    int hopcroftKarp(double threshold);
    bool searchPaths(double threshold);
    bool searchPaths();
    void extractPath(map<int, int>& parent, vector<int>& path, const int& start);
    void applyPath(vector<int>& path);
    void addBackwardsEdges();
    void findAndApplyPaths(int nodeIndex);
};

#endif