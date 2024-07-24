#include <limits>
#include <tuple>
using namespace std;
#include "bfs.hpp"

unsigned int bfs(Graph& graph, int source, int target, vector<pair<int, shared_ptr<Edge>>>& parent){
    --source;--target;


    fill(parent.begin(), parent.end(), make_pair(-1,nullptr));
    parent[source].first = -2;
    parent[source].second =  NULL;

    queue<pair<int,int>> q;
    q.push({source, numeric_limits<int>::max()});

    while (!q.empty())
    {

        int currentNodeIndex = q.front().first;
        int flow = q.front().second;
     
        q.pop();

        
        for (shared_ptr<Edge>& edge : graph.getNeighbors(currentNodeIndex)) {
            Node* pNeighbor = edge->target;
            int capacity = edge->capacity;
 
            if (capacity > 0 && parent[pNeighbor->index].first == -1){    
                parent[pNeighbor->index].first = currentNodeIndex;
                parent[pNeighbor->index].second = edge;

                int newFlow = min(capacity,flow);

                if(pNeighbor->index == target){
                    return newFlow;
                }

                q.push({pNeighbor->index,newFlow});
            }
        } 
    }

    return 0;

}