#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <utility>
using namespace std;

void debug() { printf("DEBUG\n"); }
void debug(int i) { printf("DEBUG: %d\n", i); }

class Graph {
public:
    Graph(int n) {
        if(n <= 0)
            throw std::invalid_argument( "n must be positive" );
        size = n;
        edges = new bool*[size];
        for(int i = 0; i < n; i++) {
            edges[i] = new bool[size];
            for(int j = 0; j < n; j++)
                edges[i][j] = false;
        }
    }

    /* destructor */
    ~Graph() {
        internal_destructor();
    }

    /* copy constructor */
    Graph(const Graph &g) {
        internal_copy(g);
    }

    /* assignment operator */
    Graph& operator=(const Graph &g) {
        if(this == &g) return *this;
        internal_destructor();
        internal_copy(g);
        return *this;
    }

    int get_size() { return size; }

    /* returns true iff edge has been added successfully */
    bool add_edge(int from, int to) {
        if(edges[from][to])
            return false;
        else {
            edges[from][to] = true;
            return true;
        }
    }

    /* returns true iff edge has been removed successfully */
    bool rem_edge(int from, int to) {
        if(edges[from][to]) {
            edges[from][to] = false;
            return true;
        } else
            return false;
    }

    set<int> outgoing_edges(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        set<int> og;
        for(int i = 0; i < size; i++)
            if(edges[vertex][i])
                og.insert(i);
        return og;
    }

    bool has_outgoing(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        for(int i = 0; i < size; i++)
            if(edges[vertex][i])
                return true;
        return false;
    }

    set<int> incoming_edges(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        set<int> ic;
        for(int i = 0; i < size; i++)
            if(edges[i][vertex])
                ic.insert(i);
        return ic;
    }

    bool has_incoming(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        for(int i = 0; i < size; i++)
            if(edges[i][vertex])
                return true;
        return false;
    }

private:
    int size;
    bool **edges;

    void internal_destructor() {
        for(int i = 0; i < size; i++) {
            delete[] edges[i];
        }
        delete[] edges;
    }

    void internal_copy(const Graph &g) {
        size = g.size;
        edges = new bool*[size];
        for(int i = 0; i < size; i++) {
            edges[i] = new bool[size];
            for(int j = 0; j < size; j++) {
                edges[i][j] = g.edges[i][j];
            }
        }
    }
};

// L ← Empty list that will contain the sorted elements
// S ← Set of all nodes with no incoming edge
// while S is non-empty do
//     remove a node n from S
//     add n to tail of L
//     for each node m with an edge e from n to m do
//         remove edge e from the graph
//         if m has no other incoming edges then
//             insert m into S
// if graph has edges then
//     return error   (graph has at least one cycle)
// else 
//     return L   (a topologically sorted order)

void top_sort(Graph &g, vector<int> &v) {
    stack<int> noedge; // set of vertices with no incoming edges
    int k;
    set<int> nbrs;
    // initial collect no neighbor and set visited arr to false
    for(int i = 0; i < g.get_size(); i++) {
        if(!g.has_incoming(i))
            noedge.push(i);
    }

    while(!noedge.empty()) {
        k = noedge.top(); noedge.pop();
        v.push_back(k);
        debug(k);

        // remove all edges outgoing from k
        for(int i : g.outgoing_edges(k)) {
            g.rem_edge(k, i);
            if(!g.has_incoming(i))
                noedge.push(i);
        }
    }
}

/*
g++ -std=c++11 g-graduation2.cpp -o k
*/

int main() {
    Graph g = Graph(9);
    vector<int> v;

    // https://en.wikipedia.org/wiki/Topological_sorting
    g.add_edge(1, 4);
    g.add_edge(2, 4);
    g.add_edge(2, 5);
    g.add_edge(3, 5);
    g.add_edge(3, 8);
    g.add_edge(4, 6);
    g.add_edge(4, 7);
    g.add_edge(4, 8);
    g.add_edge(5, 7);

    top_sort(g, v);
    for(int i : v) printf("%d ", i);
    cout << endl;

    return 0;
}
