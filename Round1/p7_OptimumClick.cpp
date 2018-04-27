#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	const int inf = numeric_limits<int>::max();
	long long k; // number we have to reach
	int n; // given multiplier

	while (true) {
		// read input
		cin >> k >> n;
		// check for exit condition and act accordingly
		if (k == 0 || n == 0)
			break;
		// else evaluate and print in requested format
		else {
			string path;
			int cnt = 0;
			while (k >= 1) {
				cnt++;
				// only special case we have to consider
				// we can arrive to 2 from 1 using S as well as M, S has priority
				if (k == 2 && n == 2) {
					k--;
					path.push_back('S');
				}
				// if we can divide with given multiplier do it
				else if (k % n == 0) {
					k /= n;
					path.push_back('M');
				}
				// else decrement
				else {
					k--;
					path.push_back('S');
				}
			}
			reverse(path.begin(), path.end());
			cout << cnt << " " << path << endl;
		}
	}

	return 0;
}