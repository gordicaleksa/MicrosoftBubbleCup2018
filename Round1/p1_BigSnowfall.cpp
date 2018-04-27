#include <iostream>
#include <vector>
using namespace std;

// just some shortcuts I generally like to use when competing
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

#define all(v) (v).begin(), (v).end()

// I am a big fan of OOP when I have time to implement it on competitions
class Graph
{
	int n;    // # of vertices
	vvi adj;  // adjacency list
	vi in;    // in degree of every intersection/node. We need this later for checking for Eulerian paths
	// modifies visited array by doing DFS starting at node v
	void DFS(int v, vi& visited);
public:
	// Constructor 
	Graph(int n) { this->n = n;  adj = vvi(n); in = vi(n, 0); }

	// Method to add an edge
	void addEdge(int v, int w);

	// returns true if the graph has exactly one connected component
	bool isConnected();
	// returns true if the graph has exactly one weakly-connected component
	bool isWeaklyConnected();

	// returns true if the directed graph has Eulerian Cycle or Path
	bool hasEulerianCycleOrPathDirected();
	// returns true if the undirected graph has Eulerian Cycle or Path
	bool hasEulerianCycleOrPathUndirected();
};

// depth first search that modifies visited array
void Graph::DFS(int v, vector<int>& visited)
{
	// Mark the current node as visited
	visited[v] = 1;

	// Recur for all the vertices adjacent to this vertex
	for (auto it = adj[v].begin(); it != adj[v].end(); ++it)
		if (!visited[*it])
			DFS(*it, visited);
}

void Graph::addEdge(int from, int to)
{
	adj[from].push_back(to); 
	(in[to])++; // increment in-degree of sink node
}

// returns true if we have exactly 1 weakly-connected component
bool Graph::isWeaklyConnected()
{
	// Find the first vertex with non-zero out-degree
	int nz;
	for (nz = 0; nz < n; nz++)
		if (adj[nz].size() > 0)
			break;

	// If there are no edges in the graph, return true
	if (nz == n)
		return true;

	// if there is at least one node from which we can visit all of the nodes
	// with edges then we return true
	for (int i = nz; i < n; i++) {

		vector<int> visited(n, 0);
		DFS(i, visited);
		
		// If the DFS traversal didn't visit all non-zero degree vertices, then try with another node i
		bool connected_flag = true; // assume we can vist all of the nodes from current node i
		for (int j = 0; j < n; j++) {
			// if there is at least one node whose in or out degree is non-zero 
			// and he was left unvisited => we clear the flag
			if ((in[j] != 0 || adj[j].size() != 0) && visited[j] == 0) {
				connected_flag = false;
				break;
			}
		}

		if (connected_flag) return true;
	}

	return false;
}

// returns true if we have exactly 1 connected component
bool Graph::isConnected()
{
	// Mark all the vertices as not visited
	vector<int> visited(n, 0);

	// Find the first vertex with non-zero degree
	int nz;
	for (nz = 0; nz < n; nz++)
		if (adj[nz].size() != 0)
			break;

	// If there are no edges in the graph, return true
	if (nz == n)
		return true;

	// Start DFS traversal from a vertex with non-zero degree
	DFS(nz, visited);

	// Check if all non-zero degree vertices are visited
	for (int i = 0; i < n; i++)
		if (visited[i] == 0 && adj[i].size() > 0)
			return false;

	return true;
}

/* This function returns true if the directed graph has an eulerian
cycle or path, otherwise returns false  */
bool Graph::hasEulerianCycleOrPathDirected()
{
	// Check if all non-zero degree vertices are weakly-connected
	if (!isWeaklyConnected())
		return false;

	// Check if in degree and out degree of every vertex is same
	// or we have 2 vertices whose in and out degree differ by +1 and -1
	bool is_cycle = true;
	int cnt = 0;
	for (int i = 0; i < n; i++)
		if (adj[i].size() != in[i]) {
			if (abs(in[i] - (int)adj[i].size()) > 1) is_cycle = false;
			cnt++;
		}
	
	if (cnt == 0 || (cnt == 2 && is_cycle)) {
		return true;
	}

	return false;
}

bool Graph::hasEulerianCycleOrPathUndirected()
{
	// Check if all non-zero degree vertices are connected
	if (!isConnected())
		return false;

	// Count vertices with odd degree
	int odd = 0;
	for (int i = 0; i < n; i++)
		if (adj[i].size() & 1)
			odd++;

	// If count is more than 2, then graph is not Eulerian
	if (odd > 2)
		return false;

	return true;
}

int main() 
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	int T; // # of test cases
	int N, M; // # of intersections and streets respectively
	int f, s; // id of first and second intersection
	cin >> T;

	// for every test case do the following:
	for (int i = 0; i < T; i++) {
		cin >> N >> M;

		// intersections are nodes/vertices of a graph, while streets are edges
		Graph city(N); 
		Graph city_aborted(N); // graph of the city when the traffic is aborted

		// read street informations from stdin
		for (int j = 0; j < M; j++) {
			// decrement as we use 0-based indices and they gave us 1-based indices
			cin >> f >> s; --f; --s; 
			city.addEdge(f, s);
			city_aborted.addEdge(f, s);
			city_aborted.addEdge(s, f);
		}

		// check if the directed graph has Euler circuit or path
		if (city.hasEulerianCycleOrPathDirected()) {
			cout << "YES" << endl;
			continue;
		}

		// check if the undirected graph has Euler circuit or path
		if (city_aborted.hasEulerianCycleOrPathUndirected()) {
			cout << "TRAFFIC STOPPING NEEDED" << endl;
		}
		else {
			cout << "WAKE UP EARLIER" << endl;
		}
	}

	return 0;
}
