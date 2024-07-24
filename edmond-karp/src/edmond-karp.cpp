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

    
    while (line.substr(0,5) != "p max")
        getline(in,line);
 
    unsigned int n, m;
    stringstream linestr;
    linestr.str(line);
    linestr >> dummy >> dummy >> n >> m;
    graph.resize(n);

    getline(in,line);
    linestr.clear();
    linestr.str(line);
    linestr >> dummy >> source >> dummy;

    getline(in,line);
    linestr.clear();
    linestr.str(line);
    linestr >> dummy >> target >> dummy;

    unsigned i=0;
    while (i < m) {
        getline(in,line);
        if (line.substr(0,2) == "a ") {
        std::stringstream arc(line);
        unsigned u,v,w;
        char ac;
        arc >> ac >> u >> v >> w;
        graph.addEdge(u,v,w);
        i++;
        }
    }

}


static void update_capacities(int source, int target, int flow, vector<pair<int, shared_ptr<Edge>>>& parent){
    int parentIndex = target - 1;
    shared_ptr<Edge> aux= parent[parentIndex].second;
    for(aux = parent[parentIndex].second; aux != nullptr; aux = parent[parentIndex].second){
        aux->capacity -= flow;
        assert(aux->backwardsEdge != nullptr);
        aux->backwardsEdge->capacity += flow;

        parentIndex = parent[parentIndex].first;


        if (parentIndex == (source - 1)){
            break;
        }
    }
}



int main(int argc, char* argv[]){
    Graph graph;
    int maxflow = 0;
    int source = 1;
    int target = 8;
    readDimacs(graph,cin, source, target);
    graph.addBackwardsEdges();

    vector<pair<int, shared_ptr<Edge>>> parent(graph.numberOfNodes);

    auto t1 = high_resolution_clock::now();
    int bottleneck;
    int i = 0;
    while((bottleneck = bfs(graph,source,target,parent))){
        maxflow += bottleneck;
        update_capacities(source, target, bottleneck, parent);
        i++;
    }

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<chrono::microseconds>(t2 - t1);

    cout << maxflow << endl;
    cout << "Execution time: " << ms_int.count()<< " Ms\n";
    
    return 0;
}