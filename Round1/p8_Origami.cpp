#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

long double M_PI = 3.141592653589793238462643383279502884;

// coordinate type, should stay at least double because of the functions like length()
typedef long double coord_t;

// class I have in my personal library (check my repo competitive_programming/geometry)
// I had to improve it even more for this problem, not all of the functions are needed for this problem
class Point2D {
public:
	// This is against OOP but it's great for TopCoder-like problems
	coord_t x, y;

	// Constructors

	// default constructor
	Point2D() {
		this->x = 0;
		this->y = 0;
	}
	Point2D(coord_t x, coord_t y) {
		this->x = x;
		this->y = y;
	}
	// calculate angle between 2 points using linear algebra (v1)dot(v2)=|v1|*|v2|*cos(angle)
	coord_t angle(const Point2D& p) const {
		coord_t dot = ((*this)*p);
		coord_t v1 = (*this).length();
		coord_t v2 = p.length();
		return acos(dot / (v1*v2));
	}
	// rotate this vector by theta in ccw direction
	Point2D& rotate(coord_t angle, bool is_degree) {
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
	coord_t length() const {
		return sqrt(x * x + y * y);
	}
	// make this vector unit vector
	Point2D& normalize() {
		return (*this) / (*this).length();
	}

	// vector addition
	Point2D operator +(const Point2D& p) const {
		return Point2D(x + p.x, y + p.y);
	}
	// vector subtraction
	Point2D operator -(const Point2D& p) const {
		return Point2D(x - p.x, y - p.y);
	}
	// vector scaling
	Point2D& operator /(coord_t s) {
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

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord_t cross(const Point2D &O, const Point2D &A, const Point2D &B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
vector<Point2D> convex_hull(vector<Point2D>& points) 
{
	size_t n = points.size(), k = 0;
	if (n <= 3) return points;
	vector<Point2D> H(2 * n);

	// Sort points lexicographically
	sort(points.begin(), points.end());

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], points[i]) <= 0) k--;
		H[k++] = points[i];
	}

	// Build upper hull
	for (int i = n - 2, t = k + 1; i >= 0; --i) {
		while (k >= t && cross(H[k - 2], H[k - 1], points[i]) <= 0) k--;
		H[k++] = points[i];
	}

	H.resize(k - 1);
	return H;
}

// it is important that points are ordered either cw or ccw
// shoelace formula, time complexity: O(n)
coord_t polygon_area(vector<Point2D>& vertices) {

	int n = vertices.size();
	coord_t area = 0;

	// Calculate value of shoelace formula
	int j = n - 1;
	for (int i = 0; i < n; i++)
	{
		area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
		j = i;  // j is previous vertex to i
	}

	// Return absolute value
	return abs(area / (coord_t) 2);
}

// Compute the distance from line formed by A and B to C
// if is_segment is true AB is segment otherwise it's a line
coord_t segment_point_distance(const Point2D& A, const Point2D& B, const Point2D& C, bool is_segment) {
	if (A == B) return (A - C).length();
	Point2D BA = A - B;
	Point2D BC = C - B;
	Point2D AB = B - A;
	Point2D AC = C - A;
	coord_t d = (AB ^ AC) / BA.length();
	if (is_segment) {
		coord_t dot1 = AB * BC;
		if (dot1 > 0) return BC.length();
		coord_t dot2 = BA * AC;
		if (dot2 > 0) return AC.length();
	}
	return abs(d);
}

// finds the current oriented bounding box area if it is smaller than the current OMBB update
void update_area(vector<Point2D>& boundary, int p1, int p2, int p3, int p4, Point2D& vp1, Point2D& vp2, Point2D& vp3, Point2D& vp4, coord_t& min_area) {

	coord_t h = segment_point_distance(boundary[p3], boundary[p3] + vp3, boundary[p1], false);
	coord_t w = segment_point_distance(boundary[p2], boundary[p2] + vp2, boundary[p4], false);

	if (w*h < min_area) min_area = w * h;
}

// utility conversion function
coord_t deg(coord_t angle) {
	return (angle / M_PI) * 180;
}

// rotating calipers: O(n)
// main resource: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.155.5671&rep=rep1&type=pdf
coord_t mbr_rc(vector<Point2D>& boundary) {
	int n = boundary.size();

	// step 1: scale all of the points
	int scale_factor = 200.0; // experimentally found it
	for (int i = 0; i < n; i++)
		boundary[i] = boundary[i] / scale_factor;

	// step 2: find correct points/indices where to attach calipers
	int p1 = 0, p2 = 0, p3 = 0, p4 = 0; // starting indices of calipers
	Point2D fst = boundary[0];
	coord_t min_x = fst.x, max_x = fst.x, min_y = fst.y, max_y = fst.y;
	for (int i = 1; i < n; i++) {
		Point2D c = boundary[i];
		if (c.y <= min_y) p1 = i, min_y = c.y;
		if (c.y >= max_y) p3 = i, max_y = c.y;
		if (c.x <= min_x) p4 = i, min_x = c.x;
		if (c.x >= max_x) p2 = i, max_x = c.x;
	}
	
	// step 3: prepare calipers and other init variables
	// unit vectors representing rotating calipers
	Point2D vp1(1, 0);
	Point2D vp2(0, 1);
	Point2D vp3(-1, 0);
	Point2D vp4(0, -1);
	// this is where area of OMBB (oriented minimum bounding box) will be stored
	coord_t min_area = numeric_limits<double>::infinity();
	coord_t e = 0.0000001; // degrees 

	// step 4: start moving the calipers
	for (int i = 0; i < n; i++) {
		coord_t f1 = vp1.angle((boundary[(p1 + 1) % n] - boundary[p1]).normalize());
		coord_t f2 = vp2.angle((boundary[(p2 + 1) % n] - boundary[p2]).normalize());
		coord_t f3 = vp3.angle((boundary[(p3 + 1) % n] - boundary[p3]).normalize());
		coord_t f4 = vp4.angle((boundary[(p4 + 1) % n] - boundary[p4]).normalize());

		// step 5: find the minmum angle
		coord_t min_f = min(min(min(f1, f2), f3), f4);
	
		// step 6: rotate all of the calipers by that minimum angle
		vp1.rotate(min_f, false);
		vp2.rotate(min_f, false);
		vp3.rotate(min_f, false);
		vp4.rotate(min_f, false);

		// step 7: update minimum bounding box area
		update_area(boundary, p1, p2, p3, p4, vp1, vp2, vp3, vp4, min_area);

		// step 8: advance calipers
		if (deg(abs(f1 - min_f)) <= e) p1 = (p1 + 1) % n;
		if (deg(abs(f2 - min_f)) <= e) p2 = (p2 + 1) % n;
		if (deg(abs(f3 - min_f)) <= e) p3 = (p3 + 1) % n;
		if (deg(abs(f4 - min_f)) <= e) p4 = (p4 + 1) % n;
	}

	return min_area * scale_factor*scale_factor; // area of OMBB
}

int main()
{
	ios_base::sync_with_stdio(false); // gives speed to C++ IO

	int N; // # of points
	int x, y; // point coordinates
	vector<Point2D> origami; // holds points

	cin >> N;

	for (int i = 0; i < N; i++) {
		cin >> x >> y;
		Point2D p(x, y);
		origami.push_back(p);
	}

	// left this in here as a remainder of hell of a debugging process

	// test: read origami points from file instead from stdin
	//string filepath = "origami.txt";
	// when coordinates of origami are smaller than 10*1000*10 we don't get any
	// difference between brute force and rotating calipers algorithm, otherwise calipers break
	//init_file_with_random_points(filepath, 100*1000, 10*1000*1000);
	//ifstream f;
	//f.open(filepath);
	//if (f.is_open()) {
	//	std::string line;
	//	getline(f, line);
	//	istringstream iss(line);
	//	iss >> N;
	//	for (int i = 0; i < N; i++) {
	//		getline(f, line);
	//		std::istringstream iss(line);
	//		iss >> x >> y;
	//		Point2D p(x, y);
	//		origami.push_back(p);
	//	}
	//}
	//else { cout << "couldn't find a file." << endl; return 1; }

	// ------here we have origami loaded in-----------

	// find the convex hull of the point set
	vector<Point2D> boundary = convex_hull(origami);

	cout.precision(20);

	long long res;
	if (boundary.size() > 3) {
		// if we have more than 3 points we apply rotating calipers
		res = (long long)round(mbr_rc(boundary));
	}
	else {
		// otherwise just return the area of triangle times 2
		res = (long long)round(polygon_area(boundary) * 2);
	}

	// print the result in requested format
	cout << res << endl;

	return 0;
}