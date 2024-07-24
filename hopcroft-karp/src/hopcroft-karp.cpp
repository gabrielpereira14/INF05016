#include "graph.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

static void readDimacs(Graph& graph, std::istream& in){
    std::string line="", dummy;

    
    while (line.substr(0,6) != "p edge"){
        getline(in,line);
    }
        
    unsigned int n1, n2, m;
    stringstream linestr;
    linestr.str(line);
    linestr >> dummy >> dummy >> n1 >> n2 >> m;
    graph.resize(n1+n2);
    graph.v1Size = n1;



    unsigned i=0;
    while (i < m) {
        getline(in,line);
        if (line.substr(0,2) == "e ") {
        std::stringstream arc(line);
        unsigned int u,v;
        double w;
        char ac;
        arc >> ac >> u >> v >> w;
        graph.addEdge(u,v,w);
        i++;
        
        }
    }

}


int phases = 0;
int main(int argc, char *argv[]){
    Graph graph;
    //ifstream inputFile;
    //inputFile.open("C:\\Users\\Gabriel\\Desktop\\Alg av\\hopcroft-karp\\a.txt");

    readDimacs(graph, std::cin);

    int result = -1;
    
    auto t1 = high_resolution_clock::now();

    result =  graph.hopcroftKarp();

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<chrono::microseconds>(t2 - t1);

    //cout << graph.v1Size << ", " << graph.numberOfNodes - graph.v1Size << ", " << graph.numberOfEdges << ", " <<  result << ", " << phases  <<", " << ms_int.count();
    cout << graph.numberOfNodes << ", " << graph.v1Size << ", " << graph.numberOfNodes - graph.v1Size << ", " << graph.numberOfEdges << ", " <<  result << ", " << ms_int.count() << ", " << phases <<  endl;

    graph.destroy();
    return 0;
}