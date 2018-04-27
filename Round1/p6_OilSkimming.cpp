#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

// O(N^2)
// we can notice that the Oil Skimming problem can be reduced to bipartite graph
// and that the solution is actually maximum matching problem in bipartite graph
// we find the maximum matching by adding source and sink nodes to bipartite graph
// and then passing it to some max flow algo like Edmonds-Karp (using BFS)
vvi build_max_flow_graph(vector<string>& v) {
	// bipartite graph size
	int M = 0;
	int N = 0;

	unordered_map<int, int> indices; // stores indices of black tiles with #
	int cnt_b = 0;
	// find the size of bipartite graph G=(U,V,E)
	// (chessboard pattern same color tiles belong to same set of bipartite graph)
	for (int row = 0; row < v.size(); row++) {
		for (int col = 0; col < v[0].size(); col++) {
			if (v[row][col] == '#') {
				int tmp = N;
				// dummy is used only so I can utilize ternary operator and have more concise code
				int dummy = (row & 1) ? ((col & 1) ? M++ : N++) : ((col & 1) ? N++ : M++);
				if (N > tmp) { // we just found black tile with # char i.e. oil
					indices[(row*v[0].size() + col) / 2] = cnt_b;
					cnt_b++;
				}
			}
		}
	}

	// matrix for ford-fulkerson
	vvi ff(M + N + 2, vi(M + N + 2, 0));
	// source node connects to all white tiles i.e. set U
	for (int col = 1; col <= M; col++) {
		ff[0][col] = 1;
	}
	// white tiles connect to black tiles
	int cnt_w = 1;
	for (int row = 0; row < v.size(); row++) {
		for (int col = 0; col < v[0].size(); col++) {
			// if the tile is white and has oil slick
			if (v[row][col] == '#' && ((row % 2 == 0 && col % 2 == 0) || (row & 1 && col & 1))) {
				// check it's surrounding 4 black tiles and if they have oil connect them
				if (col - 1 >= 0 && v[row][col - 1] == '#') {
					ff[cnt_w][M + 1 + indices.find((row*v[0].size() + col - 1) / 2)->second] = 1;
				}
				if (col + 1 < v[0].size() && v[row][col + 1] == '#') {
					ff[cnt_w][M + 1 + indices.find((row*v[0].size() + col + 1) / 2)->second] = 1;
				}
				if (row - 1 >= 0 && v[row - 1][col] == '#') {
					ff[cnt_w][M + 1 + indices.find(((row - 1)*v[0].size() + col) / 2)->second] = 1;
				}
				if (row + 1 < v.size() && v[row + 1][col] == '#') {
					ff[cnt_w][M + 1 + indices.find(((row + 1)*v[0].size() + col) / 2)->second] = 1;
				}
				cnt_w++;
			}
		}
	}
	// all black tiles connect to sink node i.e. set V connects to sink
	for (int row = M + 1; row < ff.size() - 1; row++) {
		ff[row][M + N + 1] = 1;
	}
	return ff;
}

/* Returns true if there is a path from source 's' to sink 't' in
residual graph. Also stores path in parrent */
bool BFS(vvi& residual_graph, int s, int t, vi& parent) {

	int n = residual_graph.size();
	// Create a visited array and mark all vertices as not visited
	vector<int> visited(n, 0);

	// Create a queue, enqueue source vertex and mark source vertex as visited
	queue <int> Q;
	Q.push(s);
	visited[s] = true;
	parent[s] = -1; // source vertex doesn't have parent

	// Standard BFS Loop
	while (!Q.empty()) {
		int u = Q.front(); Q.pop();

		for (int v = 0; v < n; v++) {
			if (visited[v] == false && residual_graph[u][v] > 0) {
				Q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}

	// If we reached the sink node in BFS starting from source, return true else false
	return (visited[t] == true);
}

// Returns the maximum flow from s to t in the given graph
// using Ford-Fulkerson scheme more specifically Edmonds-Karp algo
int max_flow(vvi& graph, int s, int t) {
	int n = graph.size();
	int u, v;

	// initially initilize res graph to graph
	vvi residual_graph(n, vi(n)); 
	for (u = 0; u < n; u++)
		for (v = 0; v < n; v++)
			residual_graph[u][v] = graph[u][v];

	vi parent(n);  // used by BFS

	int max_flow = 0;  // There is no flow initially

	// Augment the flow while there is path from source to sink
	while (BFS(residual_graph, s, t, parent)) {
		// Find minimum residual capacity of the edges along the
		// path filled by BFS
		int path_flow = numeric_limits<int>::max();
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = min(path_flow, residual_graph[u][v]);
		}

		// update residual capacities of the edges and reverse edges
		// along the path
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			residual_graph[u][v] -= path_flow;
			residual_graph[v][u] += path_flow;
		}

		// Add path flow to overall flow
		max_flow += path_flow;
	}

	// Return the overall flow
	return max_flow;
}

int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	int T; // # of test cases
	int N; // grid size
	string tmp; // grid row holder

	cin >> T;

	// for every test case do the following:
	for (int i = 0; i < T; i++) {
		cin >> N;
		vector<string> v;
		for (int j = 0; j < N; j++) {
			cin >> tmp;
			v.push_back(tmp);
		}
		// creates bipartite graph G=(U,V,E) with source and sink nodes connected to U and V respectively
		vvi b = build_max_flow_graph(v);
		
		cout << "Case " << i + 1 << ": " << max_flow(b, 0, b.size() - 1) << endl;
	}

	return 0;
}