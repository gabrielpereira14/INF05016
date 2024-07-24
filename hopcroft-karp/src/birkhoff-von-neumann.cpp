#include "graph.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <math.h>
#include <algorithm>

int phases = 0;

static void readMatrixMarket(Graph& graph, std::istream& in,vector<double>& coefficients,double **&matrix, unsigned int &n1, unsigned int &n2, unsigned int &m){
    while (in.peek() == '%') in.ignore(2048, '\n');


    
    in >> n1 >> n2 >> m;
    assert(n1 == n2);
    graph.resize(n1+n2);
    graph.v1Size = n1;

    matrix = new double*[n1];
    for (unsigned int i = 0; i < n1; ++i) {
        matrix[i] = new double[n2]();
    }


    unsigned i=0;
    while (i < m) {
        unsigned int u,v;
        double w;
        in >> u >> v >> w;
        graph.addEdge(u - 1,v + n1 - 1,w);
        matrix[u-1][v-1] = w;
        coefficients.push_back(w);
        i++;
    }

}


void updateMatrix(double **&matrix, map<int,int> &mate,double &lambda, unsigned int &n1){
    unsigned int i = 0;

    for ( i = 0; i < n1; i++)
    {
        if(mate[i] == -1) continue;
        assert(mate[i] >= n1);
        matrix[i][mate[i] - n1] -= lambda;
    }
}

void updateGraph(Graph& graph, double lambda) {
    for (unsigned int i = 0; i < graph.numberOfNodes; i++) {
        auto& neighbors = graph.getNeighbors(i);
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                      [lambda](shared_ptr<Edge>& edge) { return edge->capacity <= lambda; }),
            neighbors.end()
        );
    }
}


int main(int argc, char *argv[]){
    Graph graph;
    ifstream inputFile;
    inputFile.open("C:\\Users\\Gabriel\\Desktop\\Alg av\\hopcroft-karp\\bcspwr10.dsm.mtx");
    vector<double> coefficients;
    
    double **matrix = nullptr;
    unsigned int n1 = 0, n2 = 0, m = 0;

    double accumulatedLambda = 0;
    const double epsilon = 1e-5f;

    int upper = coefficients.size();
    int lower = 0;
    int current = 0;
    int iterations = 0;


    
    readMatrixMarket(graph, std::cin, coefficients, matrix, n1, n2, m);
    vector<double> lambdas(coefficients);

    sort(coefficients.begin(), coefficients.end());
    sort(lambdas.begin(), lambdas.end(), greater<int>());

    

    while (true)
    {
        upper = coefficients.size();
        lower = 0;
        while(lower < upper){
            current = floor((upper + lower)/2);

            if(graph.hopcroftKarp(coefficients[current]) == -1){
                upper = current;
            }else{
                lower = current;
            }
        }

        accumulatedLambda += (lambdas.back() - accumulatedLambda);

        while(!lambdas.empty() && ((lambdas.back() - accumulatedLambda) < epsilon)){
            lambdas.pop_back();
        }

        if(lambdas.empty()){
            break;
        }

        updateMatrix(matrix, graph.mate, accumulatedLambda, n1);
        updateGraph(graph, accumulatedLambda);

        cout << iterations << " " << lambdas.back() << " " << accumulatedLambda << endl;
        iterations++;
    }
    
    

    cout << iterations << endl;

    for (unsigned int i = 0; i < n1; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    graph.destroy();
    return 0;
}