#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include "graph.hpp"
#include <string.h>
#include <queue>
#include <stack>
using namespace std;



vector<shared_ptr<Edge>> Graph::emptyVec;

Edge::Edge(Node* t, int c, shared_ptr<Edge> b){
    {
        target = t;
        capacity = c;
        backwardsEdge = b;
    }
};

Edge::Edge(Node* t, int c){
    {
        target = t;
        capacity = c;
        backwardsEdge = NULL;
    }
};

Node::Node(int i) {
    {
        index = i;
    }
}

void Graph::resize(unsigned int newSize){
    if (newSize <= numberOfNodes) return; 

    unsigned int i;

    Node** newVertices = new Node*[newSize];

    memcpy(newVertices, vertices, numberOfNodes * sizeof(Node*));

    delete[] vertices;

    for(i = numberOfNodes; i < newSize; i++){
        newVertices[i] = new Node(i);
    }

    dist.resize(newSize);
    vertices = newVertices;
    numberOfNodes = newSize;
}

std::shared_ptr<Edge> Graph::addEdge(unsigned int from, unsigned int to, double capacity) {
    if (from < 0 || to < 0) {
        std::cout << "Invalid edge" << std::endl;
        return nullptr;
    }

    if (from >= numberOfNodes || to >= numberOfNodes) {
        resize(std::max(from, to) + 1);
    }

    Node* f = vertices[from];
    Node* t = vertices[to];

    auto newEdge = std::make_shared<Edge>(t, capacity);
    f->adj.push_back(newEdge);
    numberOfEdges += 1;
    return newEdge;
}

void Graph::destroy()
{
    delete[] vertices;
}

shared_ptr<Edge> Node::goesTo(Node* node){
    
    if(adj.empty()) return nullptr;
    for(shared_ptr<Edge>& n: adj){
        if (n->target == node)
            return n;
    }
    return nullptr;
}


void Graph::addBackwardsEdges(){
    Node* node;
    for (unsigned int i = 0; i < numberOfNodes ; i++){
        node = vertices[i];
        for(shared_ptr<Edge>& edge : (node->adj)){

            Node* neighbor = edge->target;
            shared_ptr<Edge> backwardsEdge;
            if( !(backwardsEdge = neighbor->goesTo(node))){
                backwardsEdge = addEdge(neighbor->index,node->index,0);
            }

            if (backwardsEdge == nullptr) {
                cout << "Error adding backwards edge" << endl;
                continue;
            }
            
            edge->backwardsEdge = backwardsEdge;
            backwardsEdge->backwardsEdge = edge;

        }
    }
}


vector<shared_ptr<Edge>>& Graph::getNeighbors(unsigned int nodeIndex)
{
    return vertices[nodeIndex]->adj;
}

void Graph::findAndApplyPaths(int nodeIndex){
    int currentIndex = 0;
    stack<int> stack;
    unordered_set<int> visited;
    map<int,int> parent;
    vector<int> path;

    stack.push(nodeIndex);
    while (!stack.empty()) {
        currentIndex = stack.top();
        stack.pop();

        if (visited.find(currentIndex) != visited.end()) {
            continue;
        }

        visited.insert(currentIndex);

        vector<shared_ptr<Edge>> neighbors = DAG->getNeighbors(currentIndex);

        if (neighbors.empty()) {
            if (v1FreeNodes.find(currentIndex) != v1FreeNodes.end()) {
                extractPath(parent,path,currentIndex);
                applyPath(path);
                return; 
            } 
        } else {
            for (shared_ptr<Edge> edge : neighbors) {
                if (visited.find(edge->target->index) == visited.end()) {
                    stack.push(edge->target->index);
                    parent[edge->target->index] = currentIndex;
                }
            }
        }
    }
  
}


void Graph::extractPath(map<int,int>& parent,vector<int>& path,const int& start){
    int aux = start;
    while (parent.find(aux) != parent.end()) {
        path.push_back(aux);
        aux = parent[aux];
    }
    path.push_back(aux);
}




void Graph::applyPath(vector<int>& path){

    if (path.size() == 1){
        DAG->vertices[0]->visited = true;
        return;
    }

    unsigned int i =0;
    for(i = 0; i < path.size(); i+=2){
        mate[path[i]]=path[i+1];
        mate[path[i+1]]=path[i];
        DAG->vertices[path[i]]->visited = true;
        DAG->vertices[path[i+1]]->visited = true;
        v1FreeNodes.erase(path[i]);
        v1FreeNodes.erase(path[i+1]);
    }
    matches ++;
}


bool Graph::searchPaths(){
    if (DAG != nullptr){
        DAG->destroy();
    }
    
    v2FreeNodes.clear();
    DAG = std::make_shared<Graph>();

    unordered_set<int> visited;

    unordered_set<int> u1;
    unordered_set<int> u2;

    bool found = false;

    for (unsigned int i = 0; i < numberOfNodes; i++) {
        dist[i] = 0;
    }

    for (unsigned int i = 0; i < v1Size; i++) {
        if(mate[i] == -1){
            u1.insert(i);
        }
    }


    do{
        u2.clear();
        for (int nodeIndex : u1){
            visited.insert(nodeIndex);

            for ( shared_ptr<Edge> edge :getNeighbors(nodeIndex)){
                int targetNodeIndex = edge->target->index;
                if (mate[nodeIndex] != targetNodeIndex && visited.find(targetNodeIndex) == visited.end()){
                    u2.insert(targetNodeIndex);
                    DAG->addEdge(targetNodeIndex,nodeIndex,10);
                }
            }
        }

        u1.clear();
        found = false;



        if (u2.empty()) break;

        for (int nodeIndex : u2){
            visited.insert(nodeIndex);
            if (mate[nodeIndex] == -1){

                found = true;
                v2FreeNodes.insert(nodeIndex);
            }else{
                int v = mate[nodeIndex];
                u1.insert(v);
                DAG->addEdge(v,nodeIndex,10);
            }
        }
    }while (!found);

    return found;
}

bool Graph::searchPaths(double threshold){
    if (DAG != nullptr){
        DAG->destroy();
    }
    
    v2FreeNodes.clear();
    DAG = std::make_shared<Graph>();

    unordered_set<int> visited;

    unordered_set<int> u1;
    unordered_set<int> u2;

    bool found = false;

    for (unsigned int i = 0; i < numberOfNodes; i++) {
        dist[i] = 0;
    }

    for (unsigned int i = 0; i < v1Size; i++) {
        if(mate[i] == -1){
            u1.insert(i);
        }
    }

    
    do{
        u2.clear();
        for (int nodeIndex : u1){
            visited.insert(nodeIndex);


            for ( shared_ptr<Edge> edge :getNeighbors(nodeIndex)){
                int targetNodeIndex = edge->target->index;
                if (mate[nodeIndex] != targetNodeIndex && visited.find(targetNodeIndex) == visited.end() && edge->capacity > threshold){
                    u2.insert(targetNodeIndex);
                    DAG->addEdge(targetNodeIndex,nodeIndex,10);
                }
            }
        }

        u1.clear();
        found = false;



        if (u2.empty()) break;

        for (int nodeIndex : u2){
            visited.insert(nodeIndex);
            if (mate[nodeIndex] == -1){

                found = true;
                v2FreeNodes.insert(nodeIndex);
            }else{
                int v = mate[nodeIndex];
                u1.insert(v);
                DAG->addEdge(v,nodeIndex,10);
            }
        }
    }while (!found);
    return found;
}

int Graph::hopcroftKarp(){

    unsigned int i =0;
    for ( i = 0; i < numberOfNodes; i++) {
        mate[i] = -1;
    }

    for (i = 0; i < v1Size; i++) {
        v1FreeNodes.insert(i);
    }
    
    while(searchPaths()){
        for (const auto& elem: v2FreeNodes) {
            if( mate[elem] == -1 ){
                findAndApplyPaths(elem);
            }
        }    

        
    }
    return matches;
}

int Graph::hopcroftKarp(double threshold){
    unsigned int i =0;
    for ( i = 0; i < numberOfNodes; i++) {
        mate[i] = -1;
    }

    for (i = 0; i < v1Size; i++) {
        v1FreeNodes.insert(i);
    }
    
    while(searchPaths(threshold)){
        for (const auto& elem: v2FreeNodes) {
            if( mate[elem] == -1 ){
                findAndApplyPaths(elem);
            }
        }    
    }
    for(unsigned int i = 0; i < numberOfNodes; i++){
        if(mate[i] == -1) return -1;
    }
    return matches;
}




