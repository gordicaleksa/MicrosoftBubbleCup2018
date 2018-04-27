#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
using namespace std;

typedef vector<int> vi;

// utillity function for calculating the weight of the rock using it's name
int weight(string r) {
	int w = 0;
	for (int i = 0; i < r.size(); i++) {
		w += r[i] - 'a' + 1;
	}
	return w;
}

int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	/* variables */
	int C; // # of commands
	char cmd; // command type
	string op1, op2, tmp; // command operands
	int pos1, pos2; // choosen rocks positions
	vector<string> rocks;
	unordered_map<string, int> positions; // O(1) time (amortized) :)
	vector<int> weights;

	cin >> C; 

	// parse commands and execute
	for (int i = 0; i < C; i++) {
		cin >> cmd;
		// debugging command didn't interfere with grading system
		if (cmd == 'P') {
			cout << "rocks: ";
			for (string &s : rocks) cout << s << " ";
			cout << endl;
			cout << "positions: ";
			for (auto it = positions.begin(); it != positions.end(); ++it) {
				cout << "(" << it->first << ", " << it->second << ")" << " ";
			}
			cout << endl;
		}
		// add rock command
		else if (cmd == 'A') {
			cin >> op1;
			// if this rock is already in array we can't add it
			if (positions.find(op1) != positions.end()) {
				cout << "Can't add " << op1 << "\n";
			}
			else {
				// update map and rocks
				positions.insert(pair<string, int>(op1, rocks.size()));
				rocks.push_back(op1);
				weights.push_back(weight(op1)); // calculate the weight of this rock immediately
			}
		}
		// output the number of rocks currently in the line/vector
		else if (cmd == 'N') {
			cout << rocks.size() << "\n";
		}
		// else it's either swap, replace or output the mass command (2 ops commands)
		else {
			cin >> op1;
			cin >> op2;
			auto it1 = positions.find(op1);
			auto it2 = positions.find(op2);
			pos1 = (*it1).second;
			if (cmd != 'R') pos2 = (*it2).second; // if it's replace command than second operand doesn't exist in rocks!
			if (cmd == 'S') {
				// swap rocks
				swap(rocks[pos1], rocks[pos2]);
				// update weights also
				swap(weights[pos1], weights[pos2]);
				// update map
				it1->second = pos2;
				it2->second = pos1;
			}
			else if (cmd == 'M') {
				int sum = 0;
				int pos_left = pos1 > pos2 ? pos2 : pos1;
				int pos_right = pos1 > pos2 ? pos1 : pos2;
				for (int j = pos_left; j <= pos_right; j++) sum += weights[j]; 
				cout << sum << "\n";
			}
			else if (cmd == 'R') {
				// replace rock
				rocks[pos1] = op2;
				weights[pos1] = weight(op2);
				// update map
				positions.erase(it1);
				positions.insert(pair<string, int>(op2, pos1));
			}
		}
	}

	return 0;
}