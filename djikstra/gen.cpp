/**
 * \file gen.cpp
 *   \author Marcus Ritt <mrpritt@inf.ufrgs.br> 
 *   \date Time-stamp: <2011-08-24 15:17:49 ritt>
 */

#include <cstdlib>
#include <iostream>
#include <cassert>
#include <fstream>
using namespace std;


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

using namespace boost;

#if !(_SVID_SOURCE || _XOPEN_SOURCE)
double drand48(void) {
    return rand() / (RAND_MAX + 1.0);
}

long int lrand48(void) {
    return rand();
}

long int mrand48(void) {
    return rand() > RAND_MAX / 2 ? rand() : -rand();
}

void srand48(long int seedval) {
    srand(seedval);
}
#endif

// information stored in vertices
struct VertexInformation {
  unsigned component;
};
 
// information stored in edges
struct EdgeInformation {
  unsigned weight;
};
 
const unsigned maxweight = 1000;
 
// graph is an adjacency list represented by vectors
typedef adjacency_list<vecS, vecS, directedS,VertexInformation,EdgeInformation> Graph;
typedef graph_traits<Graph>::vertex_descriptor Node;
typedef graph_traits <Graph>::edge_descriptor Edge;
 
int main(int argc, char *argv[]) {
  assert(argc == 3);
  unsigned n = atoi(argv[1]);
  int m = atoi(argv[2]);
  FILE* f = fopen64("output.txt","w");
  fclose(f); 
  srand48(time(0));

  boost::random::mt19937 rng;         
  boost::random::uniform_int_distribution<> random_node(0,n-1);
 
  // (1) generate random graph
  Graph g;
  for(unsigned i=0; i<n; i++)
    add_vertex(g);
  unsigned count = 0, i, j;
  unsigned maxDegree = (2 *m/n ) + 1;
  vector<int> assignment(n);

  while ( count < m){
    i = random_node(rng);
    j = random_node(rng);

    while(assignment[i] >= maxDegree ){
      i++;
      if (i == n)
        i = 0;
    }

    while(assignment[j] >= maxDegree ){
      j++;
      if (j == n)
        j = 0;
    }

    if (i == j || assignment[i] >= maxDegree || assignment[j] >= maxDegree) 
      continue;

    Edge e = add_edge(i,j,g).first;
    g[e].weight = lrand48()%maxweight;

    count++; assignment[i]++; assignment[j]++;
    
  } 
  /*
  for(unsigned i=0; i < n; i++)
    for(unsigned j=0; j < n; j++)
      if (i != j && drand48() < p) {
        Edge e = add_edge(i,j,g).first;
	      g[e].weight = lrand48()%maxweight;
      }
  */
 
  
  // (2) print example path
  std::ofstream fileOut("output.txt");
  // Redirect cout to write to "output.txt"
  std::cout.rdbuf(fileOut.rdbuf());

  unsigned src = lrand48()%num_vertices(g);
  unsigned dst = lrand48()%num_vertices(g);
 
  vector<unsigned> dist(n);
  vector<unsigned> pred(n);
  
  std::cerr << "";
  dijkstra_shortest_paths(g,src,weight_map(get(&EdgeInformation::weight,g)).distance_map(&dist[0]).predecessor_map(&pred[0]));
 
  std::cerr << "Distance between " << src+1 << " and " << dst+1 << " is " << dist[dst] << endl;
 
  // (3) print out in DIMACS challenge format
  std::cout << "p sp " << num_vertices(g) << " " << num_edges(g) << endl;
  graph_traits<Graph>::edge_iterator eb, ee;
  for ( tie(eb, ee)=edges(g); eb != ee; eb++)
    cout << "a " << source(*eb,g)+1 << " " << target(*eb, g)+1 << " " << g[*eb].weight << endl;
}