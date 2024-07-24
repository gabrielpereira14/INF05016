#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
using namespace std;

#include <sstream>
#include <unordered_set>
#include <chrono>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

int HEAP_K = 5;
int inserts = 0;
int updates = 0;
int deleteMins = 0;
int siftDowns = 0;
int siftUpsInsert = 0;
int siftUpsUpdate = 0;

int heapSize = 0;

class Heap{
    public:
    std::vector<pair<int,int>> arr;
    std::vector<int> pos;

    int k;

    Heap(int k){
        this->k = k;
    }

    void siftUp(int i, int sift){
        int parent = (i-1)/k;
        
        while(parent >= 0 && arr[i].second < arr[parent].second){
            if (sift == 1){
                siftUpsInsert++;
            }else{
                siftUpsUpdate++;
            }
            swap(arr[i],arr[parent]);
            swap(pos[arr[i].first],pos[arr[parent].first]);
            i = parent;
            parent = (i-1)/k;
        }

    }

    void siftDown(int i){
        int childIndex = k * i + 1;
        int smallestChild = i;
        int finished = false;
        
        while(!finished){
            while(childIndex < arr.size() && childIndex <= (k * (i+1))){
                if(arr[childIndex].second < arr[i].second && arr[smallestChild].second > arr[childIndex].second){
                    smallestChild = childIndex;
                }
                childIndex += 1;
            }
            if (smallestChild != i){
                siftDowns++;
                swap(arr[i],arr[smallestChild]);
                swap(pos[arr[i].first],pos[arr[smallestChild].first]);
            }else{
                finished = true;
            }

            i = smallestChild;
            childIndex = k * i + 1;
        } 
        
    }

    void insert(int source, int cost){
        inserts += 1;
        arr.push_back({source, cost});
        if( source >= pos.size()) pos.resize(source+1);
        pos[source] = arr.size()-1;
        siftUp(arr.size()-1,1);
    }

    pair<int,int> deleteMin(){
        
        pair<int,int> p = arr[0];
    
        deleteMins += 1;

        swap(arr[0], arr[arr.size()-1]);
        swap(pos[arr[0].first],pos[arr[arr.size()-1].first]);

        arr.erase(arr.end() - 1);

        siftDown(0);
        
        
        return p;

    }

    void update(int source, int value){
        updates += 1;
        
        int i = pos[source];
        arr[i].second = value;
        int parent = (i-1)/k;

        if (arr[parent].second > value){
            siftUp(i,0);
        }
        
    }

    int size(){
        return (sizeof(vector<pair<int,int>>) + (sizeof(pair<int,int>) * this->arr.size())) + (sizeof(vector<int>) + (sizeof(int) * this->pos.size()));
    }

    void print(){
        for (auto element : arr) {
            cout << "(" << element.first << "," << element.second << ") ";
        }
        cout << endl;
    }
};


struct Node {
    int index; 
    typedef pair<Node*, int> Edge;
    vector<Edge> adj;

    Node(int i) : index(i) {}
};


class Graph {
public:
    Node **vertices;
    unsigned int numberOfNodes = 0;

    void resize(int size) {
        vertices = new Node*[size];
        numberOfNodes = size;
        for (int i = 0; i < size; ++i) {
            vertices[i] = new Node(i); 
        }
    }

    void addEdge(const int from, const int to, int cost) {
        Node* f = vertices[from-1];
        Node* t = vertices[to-1];
        f->adj.push_back(make_pair(t,cost));
    }

    vector<pair<Node*, int>> getNeighbors(int nodeIndex){
        return vertices[nodeIndex]->adj;
    }

    void destroy(){
        delete [] vertices;
    }
};



void readDimacs(Graph& graph, std::istream& in){
    std::string line="", dummy;

    
    while (line.substr(0,4) != "p sp")
        getline(in,line);
 
    unsigned int n, m;
    stringstream linestr;
    linestr.str(line);
    linestr >> dummy >> dummy >> n >> m;
    graph.resize(n);

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

unsigned int djikstra(Graph& graph, int source, int target){
    --source;--target;

    unsigned int *distances = new unsigned int[graph.numberOfNodes];

    for (int i = 0; i < graph.numberOfNodes ; i++){
        distances[i] = numeric_limits<unsigned int>::max();
    }

    unordered_set<Node*> visited;

    Heap* heap = new Heap(6);

    heap->insert(source,0);
    while (heap->arr.size() > 0)
    {
        if (heapSize < heap->size()){
            heapSize = heap->size() + (graph.numberOfNodes * (sizeof(unsigned int))) + visited.size() * sizeof(Node*);

        }
        auto [nodeIndex, dv] = heap->deleteMin();
        visited.insert(graph.vertices[nodeIndex]);

        /*
        if(nodeIndex == target){
            delete [] distances;
            return dv;
        }
        */

        for (auto [node,dvu] : graph.getNeighbors(nodeIndex)) {
            if (visited.find(node) == visited.end()){
                
                if(distances[node->index] == numeric_limits<unsigned int>::max()){
                    distances[node->index] = dv + dvu;
                    heap->insert(node->index, dv + dvu);
                    
                } else if (dv + dvu < distances[node->index]){
                    distances[node->index] = dv + dvu;
                    heap->update(node->index, dv + dvu);
                }
            }

        }
    }

    unsigned int dist = distances[target];
    delete [] distances;
    delete heap;
    return dist;

}


int main(int argc, char* argv[]){
    Graph graph;

    int source,target;
    
    if (argc == 4){
        source = atoi(argv[1]);
        target = atoi(argv[2]);
        HEAP_K = atoi(argv[3]);
    }else if (argc == 3){
        source = atoi(argv[1]);
        target = atoi(argv[2]);
    } else{
        source = 1;
        target = 2000;
    }


    //cout << "Started reading" << endl;
    auto t1 = high_resolution_clock::now();
    if( !cin){
        ifstream input("USA.gr");
        if (!input) cerr << "Can't open input file!";
        readDimacs(graph,input);
    }else{
        readDimacs(graph,cin);
    }

    /*
    cout << "Finished reading" << endl;
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << "Reading time: " << ms_int.count() << " ms\n\n";
    */


   // TEMPORARIO
   int nodes = source;
   int iter = target;
   boost::random::mt19937 rng;         
   boost::random::uniform_int_distribution<> random_node(0,nodes-1);

   for (size_t i = 0; i < iter ; i++)
   {
        inserts = 0;
        updates = 0;
        deleteMins = 0;
        siftDowns = 0;
        siftUpsInsert = 0;
        siftUpsUpdate = 0;

        
        source = random_node(rng);
        target = random_node(rng);

        t1 = high_resolution_clock::now();
        unsigned int distance = djikstra(graph, source, target);
        auto t2 = high_resolution_clock::now();

        auto ms_int = duration_cast<chrono::microseconds>(t2 - t1);

        //cout << "Distance: " << distance << endl;
        /*
        if (distance != numeric_limits<unsigned int>::max()){
        }
        else{
            cout << "inf"<< endl;
        }
        */
        /*
        cout << "Inserts: " << inserts << endl;
        cout << "Updates: " << updates << endl;
        cout << "Deletemins: " << deleteMins << endl;
        cout << "SiftDowns: " << siftDowns << endl;
        cout << "SiftUpsInsert: " << siftUpsInsert << endl;
        cout << "SiftUpsUpdate: " << siftUpsUpdate << endl;
        */
       
       
        cout << "Heap size: " << heapSize << endl;
        cout << "Execution time: " << ms_int.count()<< " Ms\n";
        cout << "------------------" << endl;
   }
    graph.destroy();
    
    return 0; 
}
