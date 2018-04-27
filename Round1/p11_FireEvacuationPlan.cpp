#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <utility>
using namespace std;

void Z(const string s, vector<int>& z) {
	int L = 0, R = 0;
	int n = s.length();
	for (int i = 1; i < n; i++) {
		if (i > R) {
			L = R = i;
			while (R < n && s[R - L] == s[R]) R++;
			z[i] = R - L; R--;
		}
		else {
			int k = i - L;
			if (z[k] < R - i + 1) z[i] = z[k];
			else {
				L = i;
				while (R < n && s[R - L] == s[R]) R++;
				z[i] = R - L; R--;
			}
		}
	}
}

string itos(int i) // convert int to string
{
	stringstream s;
	s << i;
	return s.str();
}

class Path {
public:
	int cnt_n;
	int cnt_s;
	int cnt_e;
	int cnt_w;
	Path(int n, int s, int e, int w) : cnt_n(n), cnt_s(s), cnt_e(e), cnt_w(w) {}
};

// TLE on test case 4...
void solution(vector<int>& ptrN, vector<int>& ptrS, vector<int>& ptrE, vector<int>& ptrW, int& destinations, unordered_set<string>& memo, string& N, string& S, string& E, string& W, string& L) {
	int n = N.size();
	int s = S.size();
	int e = E.size();
	int w = W.size();

	queue<Path> Q;
	if (ptrN[n + 1] == n) Q.push(Path(1, 0, 0, 0));
	if (ptrS[s + 1] == s) Q.push(Path(0, 1, 0, 0));
	if (ptrE[e + 1] == e) Q.push(Path(0, 0, 1, 0));
	if (ptrW[w + 1] == w) Q.push(Path(0, 0, 0, 1));

	int cnt_n = 0;
	int cnt_s = 0;
	int cnt_e = 0;
	int cnt_w = 0;

	while (!Q.empty()) {
		Path tmp = Q.front(); Q.pop();
		cnt_n = tmp.cnt_n; cnt_s = tmp.cnt_s; cnt_e = tmp.cnt_e; cnt_w = tmp.cnt_w;
		int l = cnt_n * n + cnt_s * s + cnt_e * e + cnt_w * w;
		if (l == L.size()) {
			destinations++;
			continue;
		}

		if (ptrN[n + 1 + l] == n) {
			string p = 'N' + itos(cnt_n + 1) + 'S' + itos(cnt_s) + 'E' + itos(cnt_e) + 'W' + itos(cnt_w);
			if (memo.find(p) == memo.end()) {
				memo.insert(p);
				Q.push(Path(cnt_n + 1, cnt_s, cnt_e, cnt_w));
			}
		}
		if (ptrS[s + 1 + l] == s) {
			string p = 'N' + itos(cnt_n) + 'S' + itos(cnt_s + 1) + 'E' + itos(cnt_e) + 'W' + itos(cnt_w);
			if (memo.find(p) == memo.end()) {
				memo.insert(p);
				Q.push(Path(cnt_n, cnt_s + 1, cnt_e, cnt_w));
			}
		}
		if (ptrE[e + 1 + l] == e) {
			string p = 'N' + itos(cnt_n) + 'S' + itos(cnt_s) + 'E' + itos(cnt_e + 1) + 'W' + itos(cnt_w);
			if (memo.find(p) == memo.end()) {
				memo.insert(p);
				Q.push(Path(cnt_n, cnt_s, cnt_e + 1, cnt_w));
			}
		}
		if (ptrW[w + 1 + l] == w) {
			string p = 'N' + itos(cnt_n) + 'S' + itos(cnt_s) + 'E' + itos(cnt_e) + 'W' + itos(cnt_w + 1);
			if (memo.find(p) == memo.end()) {
				memo.insert(p);
				Q.push(Path(cnt_n, cnt_s, cnt_e, cnt_w + 1));
			}
		}
	}
}

// still TLE on test case 4...
void solution2(vector<int>& ptrN, vector<int>& ptrS, vector<int>& ptrE, vector<int>& ptrW, int& destinations, unordered_set<string>& memo, string& N, string& S, string& E, string& W, string& L) {
	int n = N.size();
	int s = S.size();
	int e = E.size();
	int w = W.size();

	queue<Path> Q;
	if (ptrN[n + 1] == n) Q.push(Path(1, 0, 0, 0));
	if (ptrS[s + 1] == s) Q.push(Path(0, 1, 0, 0));
	if (ptrE[e + 1] == e) Q.push(Path(0, 0, 1, 0));
	if (ptrW[w + 1] == w) Q.push(Path(0, 0, 0, 1));

	int cnt_n = 0;
	int cnt_s = 0;
	int cnt_e = 0;
	int cnt_w = 0;

	while (!Q.empty()) {
		Path tmp = Q.front(); Q.pop();
		cnt_n = tmp.cnt_n; cnt_s = tmp.cnt_s; cnt_e = tmp.cnt_e; cnt_w = tmp.cnt_w;
		string p = 'N' + itos(cnt_n) + 'S' + itos(cnt_s) + 'E' + itos(cnt_e) + 'W' + itos(cnt_w);

		// if we didn't see this pattern so far proceed
		if (memo.find(p) == memo.end()) {
			memo.insert(p);

			int l = cnt_n * n + cnt_s * s + cnt_e * e + cnt_w * w;
			if (l == L.size()) {
				destinations++;
				continue;
			}

			// check if we have north symbol at this position if yes try also that route
			if (ptrN[n + 1 + l] == n) {
				Q.push(Path(cnt_n + 1, cnt_s, cnt_e, cnt_w));
			}
			if (ptrS[s + 1 + l] == s) {
				Q.push(Path(cnt_n, cnt_s + 1, cnt_e, cnt_w));
			}
			if (ptrE[e + 1 + l] == e) {
				Q.push(Path(cnt_n, cnt_s, cnt_e + 1, cnt_w));
			}
			if (ptrW[w + 1 + l] == w) {
				Q.push(Path(cnt_n, cnt_s, cnt_e, cnt_w + 1));
			}

		}

	}
}

class Point2D {
public:
	int x;
	int y;
	Point2D(int xx, int yy) : x(xx), y(yy) {}

	// vector addition
	Point2D operator +(const Point2D& p) const {
		return Point2D(x + p.x, y + p.y);
	}

	bool operator ==(const Point2D& p) const {
		return x == p.x && y == p.y;
	}

	// prints out this point/vector in format (x,y)
	friend ostream& operator<<(ostream& strm, const Point2D& p) {
		return strm << "(" << p.x << "," << p.y << ")";
	}

};

// needed to implement this so that I could hash Point2D into hash sets/maps
namespace std {
	template<>
	struct hash<Point2D> {
		size_t operator()(const Point2D & p) const {
			string id = "x" + itos(p.x) + "y" + itos(p.y);
			return hash<std::string>()(id);
		}
	};
}

// TLE on test case 4...
void solution3(vector<int>& ptrN, vector<int>& ptrS, vector<int>& ptrE, vector<int>& ptrW, int& destinations, unordered_set<string>& memo, string& N, string& S, string& E, string& W, string& L) {
	int n = N.size();
	int s = S.size();
	int e = E.size();
	int w = W.size();

	// let's do dynamic-geometric programming! <3
	// hold all different points where we can get to
	vector<unordered_set<Point2D>> dsts(L.size());
	for (int i = 0; i < L.size(); i++) {
		unordered_set<Point2D> vectors;
		if (ptrN[ptrN.size() - 1 - i] == n) {
			Point2D base(0, 1); // north vector
			if (i - n >= 0) {
				// after going north move to i-n and check where could you finish using vectors already accumulated there
				for (auto it = dsts[i - n].begin(); it != dsts[i - n].end(); it++) {
					vectors.insert(base + (*it));
				}
			}
			else {
				vectors.insert(base);
			}
		}
		if (ptrS[ptrS.size() - 1 - i] == s) {
			Point2D base(0, -1); // south vector
			if (i - s >= 0) {
				for (auto it = dsts[i - s].begin(); it != dsts[i - s].end(); it++) {
					vectors.insert(base + (*it));
				}
			}
			else {
				vectors.insert(base);
			}
		}
		if (ptrE[ptrE.size() - 1 - i] == e) {
			Point2D base(1, 0); // east vector
			if (i - e >= 0) {
				for (auto it = dsts[i - e].begin(); it != dsts[i - e].end(); it++) {
					vectors.insert(base + (*it));
				}
			}
			else {
				vectors.insert(base);
			}
		}
		if (ptrW[ptrW.size() - 1 - i] == w) {
			Point2D base(-1, 0); // west vector
			if (i - w >= 0) {
				for (auto it = dsts[i - w].begin(); it != dsts[i - w].end(); it++) {
					vectors.insert(base + (*it));
				}
			}
			else {
				vectors.insert(base);
			}
		}
		dsts[i] = vectors;
	}

	/*for (unordered_set<Point2D>& s : dsts) {
	for (auto it = s.begin(); it != s.end(); it++) cout << (*it) << " ";
	cout << endl;
	}*/

	destinations = (*(dsts.end() - 1)).size();
}

int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	string N, S, E, W, L; // encodings for north, south, east, west and evacuation route
	int destinations = 0; // # of different destinations

	cin >> N;
	cin >> S;
	cin >> E;
	cin >> W;
	cin >> L;

	unordered_set<string> memo(L.size());

	// construct patterns as input for Z-algorithm
	string pattern_N = N + '$' + L;
	string pattern_S = S + '$' + L;
	string pattern_E = E + '$' + L;
	string pattern_W = W + '$' + L;

	int n_N = pattern_N.size();
	int n_S = pattern_S.size();
	int n_E = pattern_E.size();
	int n_W = pattern_W.size();

	vector<int> ptrN(n_N);
	vector<int> ptrS(n_S);
	vector<int> ptrE(n_E);
	vector<int> ptrW(n_W);

	// finds where positions where N, S, E and W occur inside of evacuation plan L
	Z(pattern_N, ptrN);
	Z(pattern_S, ptrS);
	Z(pattern_E, ptrE);
	Z(pattern_W, ptrW);

	solution(ptrN, ptrS, ptrE, ptrW, destinations, memo, N, S, E, W, L);

	cout << destinations << endl;

	return 0;
}
