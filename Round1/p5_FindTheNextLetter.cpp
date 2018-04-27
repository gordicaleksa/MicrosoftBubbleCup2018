#include <iostream>
#include <string>
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	int T; // # of test cases
	int n; // # of letters in a "word"
	string tmp; // "word" string
	cin >> T;

	// for every test case do the following:
	for (int t = 0; t < T; t++) {
		cin >> n;
		cin >> tmp;

		// brute-force is more than enough
		for (int i = 0; i < n; i++) {
			bool flag = false;
			for (int j = i + 1; j < n; j++) {
				// find first greater letter and print it's position
				if (tmp[j] > tmp[i]) {
					cout << j + 1 << " ";
					flag = true;
					break;
				}
			}
			// if we couldn't find greater letter simply print -1
			if (!flag) cout << -1 << " ";
		}

		cout << endl;
	}

	return 0;
}

