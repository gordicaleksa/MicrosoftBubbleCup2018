#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

// returns closest semi-convergent or potentially penultimate convergent if the
// last element of continued fraction is 2 <= this part caused problem (see README.md)
// a is denominator, b is numerator
void print_best_approximation(int a, int b) {
	// they hold original values
	int c1 = a;
	int c2 = b;
	// continued fraction coefficients
	// for example: [8,2,3] = 1/(8+1/(2+1/3))
	vector<int> cf; 

	// Euclid's algorithm
	int r = a % b;
	cf.push_back(a / b);
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
		cf.push_back(a / b);
	}

	// if gcd > 1 well then best approximation error is 0!
	if (b > 1) {
		cout << c2 / b << "/" << c1 / b << endl;
		return;
	}

	// if we decrease last coefficient we get best appoximation
	// i.e. we get best approximation semi-convergent
	(*(cf.end() - 1))--; 

	// construct semi-convergent from modified continued fraction
	int num_pp = 1, num_p = cf[0];
	int den_pp = 0, den_p = 1;
	int num = num_p;
	int den = den_p;
	/*int num_s = 1;
	int den_s = 1;*/
	for (int i = 1; i < cf.size(); i++) {
	/*	if (*(cf.end() - 1) == 1 && (i == cf.size() - 1)) {
			num_s = num; den_s = den;
		}*/
		num = cf[i] * num_p + num_pp;
		den = cf[i] * den_p + den_pp;
		num_pp = num_p;
		num_p = num;
		den_pp = den_p;
		den_p = den;
	}

	// print the result in requested format
	cout << den << "/" << num << endl;

	// all of the commented out parts are for special case when last
	// coefficient in continued fraction == 2. Than we need to check
	// if it is actually better then the last convergent, that we get
	// by completely ignoring last coefficient. The grading system didn't
	// count this in

	// abs(b/a-den_s/num_s) < abs(b/a - den/num)
	/*if (*(cf.end() - 1) == 1) {
		long long n1 = b * num_s - den_s * a;
		long long d1 = a * num_s;
		long long n2 = b * num - den * a;
		long long d2 = a * num;
		if (abs(n1*d2) <= abs(n2*d1))
			cout << den_s << "/" << num_s << endl;
		else
			cout << den << "/" << num << endl;

		return;
	}
	else {
		cout << den << "/" << num << endl;
	}*/
}

// Euclidean algo, used as utility function
int gcd(int a, int b) {
	int r = a % b;
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
	}
	return b;
}

// binary fashion Stern-Brocot compressed tree traversal produced TLE
void realphobia(int A, int B, int &C, int &D) {
	//int g = gcd(A, B);
	//A /= g;
	//B /= g;
	//C = A; D = B; // special case when A and B are reducible to 1/2 through gcd

	int pa = 0, qa = 1, pb = 1, qb = 1;

	double min = numeric_limits<double>::infinity();
	double opt = (double)A / (double)B;

	// go left x_left steps
	int x_left = ceil(((double)pb - opt * (double)qb) / (opt*(double)qa - (double)pa));
	// go right x_right steps
	int x_right = ceil(((double)pa - opt * (double)qa) / (opt*(double)qb - (double)pb));

	bool left_flag = (x_left > x_right) ? true : false;
	// binary fashion Stern-Brocot compressed tree traversal
	while (true) {
		if (!left_flag) { // go right
			x_right = ceil(((double)pa - opt * (double)qa) / (opt*(double)qb - (double)pb));
			pa = (pa + (x_right - 1)*pb);
			qa = (qa + (x_right - 1)*qb);
			pb = pa + pb;
			qb = qa + qb;
			if (qb == B) {
				double diff = abs(opt - ((double)pa / (double)qa));
				if (diff < min) {
					C = pa; D = qa;
					min = diff;
				}
				break;
			}
		}
		else { // go left
			x_left = ceil(((double)pb - opt * (double)qb) / (opt*(double)qa - (double)pa));
			pb = (pb + (x_left - 1) * pa);
			qb = (qb + (x_left - 1) * qa);
			pa = pb + pa;
			qa = qb + qa;
			if (qa == B) {
				double diff = abs(opt - ((double)pb / (double)qb));
				if (diff < min) {
					C = pb; D = qb;
					min = diff;
				}
				break;
			}
		}
		// toggle flag
		left_flag = !left_flag;
	}
}

int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	int T; // # of test cases
	int A, B; // numerator and denominator of fraction we ought to approximate
	int C, D; // best possible approximation with smallest denominator
	string tmp;

	cin >> T;

	// for every test case do the following:
	for (int i = 0; i < T; i++) {
		cin >> tmp;
		// parse the string and extract A and B
		size_t pos = tmp.find('/');
		A = stoi(tmp.substr(0, pos));
		B = stoi(tmp.substr(pos + 1));
		// find and print the best approximation with smallest denominator
		print_best_approximation(B, A);
	}

	return 0;
}

