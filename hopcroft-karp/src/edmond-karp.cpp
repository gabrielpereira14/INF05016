#include "graph.hpp"
#include "bfs.hpp"

#include <iostream>
#include <fstream>
#include <limits>
using namespace std;
#include <sstream>

#include <chrono>
#include <cassert>
#include <memory>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

static void readDimacs(Graph& graph, std::istream& in, int& source, int& target){
    std::string line="", dummy;

    
    while (line.substr(0,6) != "p edge")
        getline(in,line);
 
    unsigned int n1, n2, m;
    stringstream linestr;
    linestr.str(line);
    linestr >> dummy >> dummy >> n1 >> n2 >> m;
    graph.resize(n1+n2+2);
    graph.v1Size = n1;

    source = n1 + n2;
    target = n1 + n2 + 1;
    
    unsigned i=0;
    while (i < m) {
        getline(in,line);
        if (line.substr(0,2) == "e ") {
        std::stringstream arc(line);
        unsigned u,v,w;
        char ac;
        arc >> ac >> u >> v >> w;
        graph.addEdge(u,v,1.0);
        i++;
        }
    }
    i=0;
    while(i < n1){
        graph.addEdge(source,i,1);
        i++;
    }
    
    i = n1;
    while(i < n1 + n2){
        graph.addEdge(i,target,1);
        i++;
    }    

}


static void update_capacities(int source, int target, int flow, vector<pair<int, shared_ptr<Edge>>>& parent){
    int parentIndex = target;
    shared_ptr<Edge> aux= parent[parentIndex].second;
    for(aux = parent[parentIndex].second; aux != nullptr; aux = parent[parentIndex].second){
        aux->capacity -= flow;
        assert(aux->backwardsEdge != nullptr);
        aux->backwardsEdge->capacity += flow;

        parentIndex = parent[parentIndex].first;


        if (parentIndex == source){
            break;
        }
    }
}



int main(int argc, char* argv[]){
    Graph graph;
    int maxflow = 0;
    int source = 0;
    int target = 0;
    readDimacs(graph,cin, source, target);

    vector<pair<int, shared_ptr<Edge>>> parent(graph.numberOfNodes);
    int bottleneck;


    auto t1 = high_resolution_clock::now();

    graph.addBackwardsEdges();
    while((bottleneck = bfs(graph,source,target,parent))){
        maxflow += bottleneck;
        update_capacities(source, target, bottleneck, parent);
    }
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<chrono::microseconds>(t2 - t1);

    cout << maxflow << ", " << ms_int.count() << endl;
    return 0;
}