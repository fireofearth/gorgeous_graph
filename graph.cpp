#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
using namespace std;

struct Edge {
    int vertex;
    Edge *next;
};

/**
 * Graph data structure using adjacency list.
 * vertices are integers and edges are linked lists containing
 * integers for vertices
 *
 * Applying the Rule of Three: made destructor, copy constructor
 * and overloaded assignment operator
 */
class Graph {
public:
    Graph(int n) {
        if(n <= 0)
            throw std::invalid_argument( "n must be positive" );
        size = n;
        vertices = new Edge*[size];
        for(int i = 0; i < n; i++) 
            vertices[i] = NULL;
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
        if(from >= size || to >= size || from < 0 || to < 0)
            return false;
        Edge *my_edge = new Edge{to, NULL};
        if(vertices[from]) {
            Edge *curr = vertices[from];
            while(curr->vertex != to && curr->next)
                curr = curr->next;
            if(curr->vertex == to)
                return false;
            curr->next = my_edge;
        } else {
            vertices[from] = my_edge;
        }
        return true;
    }

    /* returns true iff edge has been removed successfully */
    bool rem_edge(int from, int to) {
        if(from >= size || to >= size || from < 0 || to < 0)
            return false;
        if(vertices[from]) {
            Edge *curr = vertices[from];
            if(curr->vertex == to) {
                vertices[from] = curr->next;
                delete curr;
                return true;
            } else {
                Edge *tmp;
                while(curr->next) {
                    if(curr->next->vertex == to) {
                        tmp = curr->next;
                        curr->next = curr->next->next;
                        delete tmp;
                        return true;
                    }
                    curr = curr->next;
                }
            }
        }
        return false;
    }

    Edge* neighbors(int vertex) {
        if(vertex < 0 || vertex >= size)
            return NULL;
        else
            return vertices[vertex];
    }

private:
    int size;
    Edge **vertices;

    void internal_destructor() {
        Edge *temp; Edge *curr;
        for(int i = 0; i < size; i++) {
            curr = vertices[i];
            while(curr) {
                temp = curr; curr = curr->next;
                delete temp;
            }
        }
        delete[] vertices;
    }

    void internal_copy(const Graph &g) {
        size = g.size;
        Edge *curr;
        Edge *g_curr;
        vertices = new Edge*[size];
        for(int i = 0; i < size; i++) {
            g_curr = g.vertices[i];
            if(g_curr) {
                curr = new Edge{g_curr->vertex, NULL};
                vertices[i] = curr;
                while(g_curr->next) {
                    g_curr = g_curr->next;
                    curr->next = new Edge{g_curr->vertex, NULL};
                    curr = curr->next;
                }
            } else {
                vertices[i] = NULL;
            }
        }
    }
};

// note: using signature Graph &g works now because
// I implemented the copy constructor
bool breadth_first_search(Graph &g, int start, int target) {
    bool visited[g.get_size()];
    for(int i = g.get_size() - 1; i >= 0; i--)
        visited[i] = false;
    queue<int> q; q.push(start); // current level
    queue<int> l; // next level
    Edge *nbr;
    int curr;

    while(!q.empty() || !l.empty()) {
        // if q empty, then fetch the next level to visit
        if(q.empty())
            while(!l.empty()) {
                q.push(l.front()); l.pop();
            }

        // get vertex
        curr = q.front(); q.pop();
        // check vertex is visited
        if(!visited[curr]) {
            // check vertex is target
            printf("%d ",curr);
            if(curr == target) {
                printf("\n");
                return true;
            }
            visited[curr] = true;
            // add neighbors to next level
            nbr = g.neighbors(curr);
            while(nbr) {
                l.push(nbr->vertex);
                nbr = nbr->next;
            }
        }
    }

    printf("\n");
    return false;
}

int main() {
    
    Graph g = Graph(4);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    cout << breadth_first_search(g, 0, 3) << endl;
    g.rem_edge(1, 2);
    cout << breadth_first_search(g, 0, 3) << endl;
    g.add_edge(1, 3);
    cout << breadth_first_search(g, 0, 3) << endl;

    return 0;
}
