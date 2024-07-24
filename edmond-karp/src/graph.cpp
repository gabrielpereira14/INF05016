#include <iostream>
#include <vector>
#include <memory>
#include "graph.hpp"
using namespace std;

Edge::Edge(Node* t, int c, shared_ptr<Edge> b){
    {
        target = t;
        capacity = c;
        backwardsEdge = b;
        criticalCount = 0;
    }
};

Edge::Edge(Node* t, int c){
    {
        target = t;
        capacity = c;
        backwardsEdge = NULL;
        criticalCount = 0;
    }
};

Node::Node(int i) {
    {
        index = i;
    }
}

shared_ptr<Edge> Node::goesTo(Node* node){
    
    if(adj.empty()) return nullptr;
    for(shared_ptr<Edge>& n: adj){
        if (n->target == node)
            return n;
    }
    return nullptr;
}


void Graph::resize(int size) {

    vertices = new Node*[size];
    numberOfNodes = size;
    for (int i = 0; i < size; i++) {
        vertices[i] = new Node(i);
    }
}

shared_ptr<Edge> Graph::addEdge(const int from, const int to, int capacity){

    if (from < 1 || from > static_cast<int>(numberOfNodes) || to < 1 || to > static_cast<int>(numberOfNodes)) {
		cout << "Invalid edge" << endl;
		return nullptr;
	}

    Node* f = vertices[from - 1];
    Node* t = vertices[to - 1];
    auto newEdge = make_shared<Edge>(t, capacity);
    f->adj.push_back(newEdge);
    numberOfEdges += 1;
    return newEdge;
}
vector<shared_ptr<Edge>>& Graph::getNeighbors(int nodeIndex)
{
    return vertices[nodeIndex]->adj;
}
void Graph::destroy()
{
    delete[] vertices;
}

void Graph::addBackwardsEdges(){
    Node* node;
    for (unsigned int i = 0; i < numberOfNodes ; i++){
        node = vertices[i];
        for(shared_ptr<Edge>& edge : (node->adj)){

            Node* neighbor = edge->target;
            shared_ptr<Edge> backwardsEdge;
            if( !(backwardsEdge = neighbor->goesTo(node))){
                backwardsEdge = addEdge(neighbor->index + 1,node->index + 1,0);
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
