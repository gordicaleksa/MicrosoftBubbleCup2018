// I/O
#include <iostream>
#include <sstream>
// containers
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
// algorithms
#include <algorithm>
#include <numeric>
#include <functional> // std::greater
#include <iterator>
#include <utility> // std::pair
#include <random>
// C libraries
#include <ctime>  
#include <cmath> 
#include <cstdlib>
using namespace std;

// shortcuts
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
#define all(x) (x).begin(), (x).end()
#define part(x,y,z) ((x).begin()+(y)), ((x).end()-(z))
#define pb push_back

// coordinate type
typedef double coord_t;
double M_PI = 3.14159265358979323846;

// convert int to string
string itos(int i) {
	stringstream s;
	s << i;
	return s.str();
}

// my Point2D class from competitive_programming repo
class Point2D {
public:
	// 0. members

	// This is against OOP but it's great for TopCoder-like problems
	coord_t x, y;

	// 1. constructors

	// default constructor
	Point2D() {
		this->x = 0;
		this->y = 0;
	}
	// cartesian coordinates constructor
	Point2D(coord_t x, coord_t y) {
		this->x = x;
		this->y = y;
	}

	// 2. public functions

	// calculate angle between 2 points/vectors using linear algebra (v1)dot(v2)=|v1|*|v2|*cos(angle)
	double angle(const Point2D& p) const {
		coord_t dot = ((*this)*p);
		coord_t v1 = (*this).length();
		coord_t v2 = p.length();
		return acos(dot / (v1*v2));
	}
	// rotate this vector by theta in ccw direction
	Point2D& rotate(double angle, bool is_degree) {
		if (is_degree) angle *= M_PI / 180.0; // convert to radians

		coord_t x_r = x * cos(angle) - y * sin(angle);
		coord_t y_r = x * sin(angle) + y * cos(angle);

		// modify this vector
		x = x_r;
		y = y_r;

		// return reference to it
		return *this;
	}
	// euclidian distance from origin
	double length() const {
		return sqrt(x*x + y * y);
	}
	// make this vector unit vector
	Point2D& normalize() {
		return (*this) / (*this).length();
	}

	// 3. operator overloading

	// vector addition
	Point2D operator +(const Point2D& p) const {
		return Point2D(x + p.x, y + p.y);
	}
	// vector subtraction
	Point2D operator -(const Point2D& p) const {
		return Point2D(x - p.x, y - p.y);
	}
	// vector scaling
	Point2D& operator /(double s) {
		if (s != 0) {
			x /= s;
			y /= s;
		}
		return *this;
	}
	// dot product
	coord_t operator *(const Point2D& p) const {
		return x * p.x + y * p.y;
	}
	// cross product signed intensity
	coord_t operator ^(const Point2D& p) const {
		return x * p.y - y * p.x;
	}
	// compare 2 points
	bool operator <(const Point2D& p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
	// compare equality
	bool operator ==(const Point2D& p) const {
		return x == p.x && y == p.y;
	}
	// compare inequality
	bool operator !=(const Point2D& p) const {
		return x != p.x || y != p.y;
	}
	// prints out this point/vector in format (x,y)
	friend ostream& operator<<(ostream& strm, const Point2D& p) {
		return strm << "(" << p.x << "," << p.y << ")";
	}
};

int main() {

	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	int T; // # of test cases
	int N; // # of cucumbers per test case
	int x1, y1, x2, y2; // cartesian coordinates that define a cucumber
	Point2D ix(1, 0); // unit vector in x direction

	cin >> T;
	double e = 0.0001; // used for solving the rounding error problems

	// for every test case do the following:
	for (int i = 0; i < T; i++) {
		cin >> N;
		vector<Point2D> segments;

		/* 
		   step1:
		   map the cucumber position to the angle it spans when looking from (0,0) point
		   the angle goes from [-180,180). We are basically reducing the problem to 1D.
		*/
		for (int j = 0; j < N; j++) {
			cin >> x1 >> y1 >> x2 >> y2;
			// a and b define a single cucumber
			Point2D a(x1, y1);
			Point2D b(x2, y2);
			// find the angle it creates with origin (0,0)
			double a1 = a.y > 0 ? ((ix.angle(a) / M_PI)*180.0) : -((ix.angle(a) / M_PI)*180.0); // [-180, 180)
			double a2 = b.y > 0 ? ((ix.angle(b) / M_PI)*180.0) : -((ix.angle(b) / M_PI)*180.0);
			// determine which angle is smaller/bigger
			double amin = a1 < a2 ? a1 : a2;
			double amax = a1 > a2 ? a1 : a2;
			// if this happens that means that a cucumber crosses over the negative part of the x-axis
			if (amax - amin > 180) {
				segments.pb(Point2D(-180, amin));
				segments.pb(Point2D(amax, 180));
			}
			else {
				segments.pb(Point2D(amin, amax));
			}
		}

		// step2: sort the segments so that we start examining the number of overlaps from [-180,180)
		sort(all(segments));

		int overlap_cnt = 0;
		int max_overlap = 0;

		// min priority queue
		priority_queue<double, std::vector<double>, std::greater<double>> closing;

		/* step3: 
		   Go through all of the segments and find the maximum # of overlaps.
		   We could have just created a vector containing simple objects with 2 fields:
		   start/end flag and value (degrees), and then just iterate once through it without using the priority queue ...
		*/
		for (Point2D& segment : segments) {
			// opening point of the segment marks a new event
			double left = segment.x;
			// we push the closing point to the min priority queue
			closing.push(segment.y);

			// check how many of the segments were closed before we found this opening segment
			while (!closing.empty() && closing.top() < left - e) {
				overlap_cnt--;
				closing.pop();
			}
			// increment because we found an opening segment
			overlap_cnt++;

			// after each event check if we have a new maximum
			if (overlap_cnt > max_overlap)
				max_overlap = overlap_cnt;
		}

		// print the result in the requested format
		cout << max_overlap << endl;
	}

	return 0;
}