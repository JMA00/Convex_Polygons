#include "ConvexPolygon.hh"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <cassert>
#include <vector>
#include <sstream>
#include <pngwriter.h>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;


double distance(const Point& p1, const Point& p2){
	double dx = p1.get_x() - p2.get_x();
	double dy = p1.get_y() - p2.get_y();
	return sqrt(dx*dx + dy*dy);
}


bool leftof(Point p1, Point p2, Point p3){
    return (p2.get_x() - p1.get_x()) * (p3.get_y() - p1.get_y()) > (p2.get_y() - p1.get_y()) * (p3.get_x() - p1.get_x());
}


bool comp(Point p1, Point p2){
    if (p1.get_x() == p2.get_x()){
        return p1.get_y() < p2.get_y();
    }
    return p1.get_x() < p2.get_x();
}

//it returns the convex_hull of the polygon given by the vector of points
vector<Point> convex_hull(const vector<Point>& points){
    int n = points.size();
    vector<Point> hull;
    // Pick the leftmost point
    int left = 0;
    for (int i = 1; i < n; i++){
        if (points[i].get_x() < points[left].get_x()) left = i;
    }
    int p = left;
    do {
        hull.push_back(points[p]);  // Add point to the convex hull
        int q = (p + 1)%n; // Pick a point different from p
        for (int i = 0; i < n; i++){
            if (leftof(points[p], points[q], points[i])) q = i;
        }
        p = q; // Leftmost point for the convex hull
    } while (p != left);  // While not closing polygon
    
    return hull;
}

//it returns a boolean of whether the points have the same coordinates
bool equal_points(Point p1, Point p2){
    return (p1.get_x() == p2.get_x() and p1.get_y() == p2.get_y());
}

//it returns a boolean of whether the points are aligned
bool in_line(Point p1, Point p2, Point p3){
    if ((p1.get_x() == p2.get_x() and p2.get_x() == p3.get_x()) or (p1.get_y() == p2.get_y() and p2.get_y() == p3.get_y())) return true;
    return (p2.get_x()-p1.get_x())/(p3.get_x()-p2.get_x()) == (p2.get_y()-p1.get_y())/(p3.get_y()-p2.get_y());
}

//will return a new vector without points in the same line
vector<Point> erase_useless_points(const vector<Point>& vp){
    int n = vp.size();
    vector<Point> vn;
    bool last_three_in_line = false;
    for (int i = 0; i < n-2; ++i){
        if (not last_three_in_line) vn.push_back(vp[i]);
        if (not in_line(vp[i],vp[i+1], vp[i+2])) last_three_in_line = false;
        else last_three_in_line = true;
    }
    if (not last_three_in_line) vn.push_back(vp[n-2]);
    //we see if the last one is in line with his partners
    if (not in_line(vp[n-2], vp[n-1], vp[0]) or int(vn.size()) == 1) vn.push_back(vp[n-1]);
    return vn;
}

//will return a new vector with all its points different
vector<Point> erase_equal_points(const vector<Point>& vp){
    int n = vp.size();
    vector<Point> vn; //new vector with not equal points
    vn.push_back(vp[0]);
    for (int i = 1; i < n; ++i){
        if (not equal_points(vp[i], vp[i-1])) vn.push_back(vp[i]);
    }
    return vn;
}

//depures the polygon by erasing the unnecesary points and doing the convex hull
vector<Point> depure_polygon(vector<Point>& vp){
    sort(vp.begin(), vp.end(), comp);
    if (int(vp.size()) >= 3) vp = convex_hull(vp);
    if (int(vp.size()) > 1) vp = erase_equal_points(vp);
    if (int(vp.size()) > 2) vp = erase_useless_points(vp);
    return vp;
}

//constructor
ConvexPolygon::ConvexPolygon(vector<Point> _p, vector<double> _c){
	p = depure_polygon(_p);
	c = _c;
}

//it prints the polygon
void ConvexPolygon::print() const{
	bool first = true;
	for (int i = 0; i < int(p.size()); ++i){
		if (first){
			cout << p[i].get_x() << " " << p[i].get_y();
			first = false;
		}
		else{
			cout << " " << p[i].get_x() << " " << p[i].get_y();
		}
	}
	cout << endl;
}

//returns the vector of points
vector<Point> ConvexPolygon::get_vector() const{
	return p;
}

//returns the vector of colors
vector<double> ConvexPolygon::get_color() const{
	return c;
}

//calculates the minimum x of the polygon
double ConvexPolygon::get_min_x() const{
	double min_x = p[0].get_x();
	for (Point _p : p){
		if (min_x > _p.get_x()) min_x = _p.get_x();
	}
	return min_x;
}

//calculates the maximum x of the polygon
double ConvexPolygon::get_max_x() const{
	double max_x = p[0].get_x();
	for (Point _p : p){
		if (max_x < _p.get_x()) max_x = _p.get_x();
	}
	return max_x;
}

//calculates the minimum y of the polygon
double ConvexPolygon::get_min_y() const{
	double min_y = p[0].get_y();
	for (Point _p : p){
		if (min_y > _p.get_y()) min_y = _p.get_y();
	}
	return min_y;
}

//calculates the maximum y of the polygon
double ConvexPolygon::get_max_y() const{
	double max_y = p[0].get_y();
	for (Point _p : p){
		if (max_y < _p.get_y()) max_y = _p.get_y();
	}
	return max_y;
}

//calculates the perimter by adding all its edges
double ConvexPolygon::perimeter() const{
	double perimeter = 0;
	for (int i = 0; i < int(p.size()) - 1; ++i){
		perimeter += distance(p[i], p[i+1]);
	}
	perimeter += distance(p[int(p.size())-1], p[0]);
	return perimeter;
}

//returns the vertices (number of points) of the polygon
int ConvexPolygon::vertices() const{
	return p.size();
}

//returns the edges of the polygon
int ConvexPolygon::edges() const{
	if (p.size() == 0) return 0;
	if (p.size() <= 2) return p.size()-1;
	return p.size();
}

//calculates the area of the given polygon with the formula
double ConvexPolygon::area() const{
	int n = p.size();
	int j = n - 1;
	double sum = 0;
	for (int i = 0; i < n; ++i){
		sum += (p[j].get_x()+p[i].get_x())*(p[j].get_y()-p[i].get_y());
		j = i;
	}
	double area = abs(sum/2);
	return area;
}

//calculates the centroid of the given polygon with the formula
Point ConvexPolygon::centroid(double area) const{
	int n = p.size() - 1;
	if (n == 0) return p[0];
	double cx = 0;
	double cy = 0;
	for (int i = 0; i <= n-1; ++i){
		cx += (p[i].get_x()+p[i+1].get_x()) * (p[i].get_x()*p[i+1].get_y()-p[i+1].get_x()*p[i].get_y());
		cy += (p[i].get_y()+p[i+1].get_y()) * (p[i].get_x()*p[i+1].get_y()-p[i+1].get_x()*p[i].get_y());
	}
	cx /= -6*area;
	cy /= -6*area;
	Point p = {cx, cy};
	return p;
}

//asigns the color wanted to the polygon by changing its vector of colors
void ConvexPolygon::setcol(double r, double g, double b) {
	c[0] = r;
	c[1] = g;
	c[2] = b;
}

//returns true if polygons are equal
bool ConvexPolygon::operator==(const ConvexPolygon& c) const{
	vector<Point> cp = c.get_vector();
	int n = p.size();
	int m = cp.size();
	if (n != m) return false;
	for (int i = 0; i < n; ++i){
		if (p[i] != cp[i]) return false;
	}
	return true;
}

//it returns a polygon which is the polygon union of the given polygons
ConvexPolygon ConvexPolygon::polygon_union(const ConvexPolygon& c) const{
	vector<Point> cp = c.get_vector();
	vector<Point> vu;
	//we put all the points in the same vector
	for (Point _p : p){
		vu.push_back(_p);
	}
	for (Point _p : cp){
		vu.push_back(_p);
	}
	//we depure this vector and it will be the polygon convex union
	vu = depure_polygon(vu);
	ConvexPolygon cu = {vu};
	return cu;
}

//returns true if the first is inside the second polygon
bool ConvexPolygon::inside(const ConvexPolygon& q) const{
	return q == ConvexPolygon(p).polygon_union(q);
}

//returns a convex polygon which is the bounding box of the given polygons
ConvexPolygon ConvexPolygon::bbox(vector<string> pols, map<string, ConvexPolygon>& ConvexPolygons) const{
	string pol;
    double min_x, max_x, min_y, max_y; //we will find the four coordinates of the box (which are these)
    //we will initialize their values to the coordinates of the first polygon
    min_x = ConvexPolygons[pols[0]].get_min_x();
    max_x = ConvexPolygons[pols[0]].get_max_x();
    min_y = ConvexPolygons[pols[0]].get_min_y();
    max_y = ConvexPolygons[pols[0]].get_max_y();
    //we will compare the coordinates with every coordinate and we'll change them if necessary
    for (int i = 1; i < int(pols.size()); ++i){
        if (min_x > ConvexPolygons[pols[i]].get_min_x()) min_x = ConvexPolygons[pols[i]].get_min_x();
        if (max_x < ConvexPolygons[pols[i]].get_max_x()) max_x = ConvexPolygons[pols[i]].get_max_x();
        if (min_y > ConvexPolygons[pols[i]].get_min_y()) min_y = ConvexPolygons[pols[i]].get_min_y();
        if (max_y < ConvexPolygons[pols[i]].get_max_y()) max_y = ConvexPolygons[pols[i]].get_max_y();
    }
    vector<Point> bb(4);
    bb[0] = {min_x, min_y}; bb[1] = {min_x, max_y}; bb[2] = {max_x, max_y}; bb[3] = {max_x, min_y};
    ConvexPolygon BB = {bb};
    return BB;
}

//INTERSECTION

// point q lies on line segment 'pr' 
bool onSegment(Point p, Point q, Point r) 
{ 
    if (q.get_x() <= max(p.get_x(), r.get_x()) && q.get_x() >= min(p.get_x(), r.get_x()) && 
        q.get_y() <= max(p.get_y(), r.get_y()) && q.get_y() >= min(p.get_y(), r.get_y())) 
       return true; 
  
    return false; 
} 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    double val = (q.get_y() - p.get_y()) * (r.get_x() - q.get_x()) - 
              (q.get_x() - p.get_x()) * (r.get_y() - q.get_y()); 
  
    if (val <= 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 
  
// The main function that returns true if line segment 'ab' 
// and 'cd' intersect. 
bool intersect(Point a, Point b, Point c, Point d) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(a, b, c); 
    int o2 = orientation(a, b, d); 
    int o3 = orientation(c, d, a); 
    int o4 = orientation(c, d, b); 
  
    // General case 
    if (o1 != o2 && o3 != o4) return true; 
  
    // Special Cases 
    // a, b and c are colinear and c lies on segment ab 
    if (o1 == 0 && onSegment(a, c, b)) return true; 
  
    // a, b and d are colinear and d lies on segment ab 
    if (o2 == 0 && onSegment(a, d, b)) return true; 
  
    // c, d and a are colinear and a lies on segment cd 
    if (o3 == 0 && onSegment(c, a, d)) return true; 
  
     // c, d and b are colinear and b lies on segment cd 
    if (o4 == 0 && onSegment(c, b, d)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
}

bool in_the_same_line(Point A, Point B, Point C, Point D){
	// Line AB represented as a1x + b1y = c1 
    double a1 = B.get_y() - A.get_y(); 
    double b1 = A.get_x() - B.get_x(); 
  
    // Line CD represented as a2x + b2y = c2 
    double a2 = D.get_y() - C.get_y();
    double b2 = C.get_x() - D.get_x();  
  
    double determinant = a1*b2 - a2*b1;

    return determinant == 0; 
}

Point line_intersection(Point A, Point B, Point C, Point D) 
{ 
    // Line AB represented as a1x + b1y = c1 
    double a1 = B.get_y() - A.get_y(); 
    double b1 = A.get_x() - B.get_x(); 
    double c1 = a1*(A.get_x()) + b1*(A.get_y()); 
  
    // Line CD represented as a2x + b2y = c2 
    double a2 = D.get_y() - C.get_y();
    double b2 = C.get_x() - D.get_x(); 
    double c2 = a2*(C.get_x()) + b2*(C.get_x()); 
  
    double determinant = a1*b2 - a2*b1; 
  
    if (determinant == 0) 
    { 
        // The lines are parallel. This is simplified
        assert(false);
    } 
    else
    { 
        double x = (b2*c1 - b1*c2)/determinant; 
        double y = (a1*c2 - a2*c1)/determinant;
        if (x <= 1e-9) x = 0;
        if (y <= 1e-9) y = 0; 
        Point p = {x, y};
        return p; 
    }
} 

// Returns true if the point p lies inside the polygon
bool point_inside(vector<Point>& v, Point p) 
{ 
    int n = v.size();
    // There must be at least 3 vertices in polygon[] 
    if (n < 3)  return false; 
    double INF = 1000000;
    // Create a point for line segment from p to infinite 
    Point extreme = {INF, p.get_y()}; 
  
    // Count intersections of the above line with sides of polygon 
    int count = 0, i = 0; 
    do
    { 
        int next = (i+1)%n; 
  
        // Check if the line segment from 'p' to 'extreme' intersects 
        // with the line segment from 'polygon[i]' to 'polygon[next]' 
        if (intersect(v[i], v[next], p, extreme)) 
        { 
            // If the point 'p' is colinear with line segment 'i-next', 
            // then check if it lies on segment. If it lies, return true, 
            // otherwise false 
            if (orientation(v[i], p, v[next]) == 0) 
               return onSegment(v[i], p, v[next]); 
  
            count++; 
        } 
        i = next; 
    } while (i != 0); 
  
    // Return true if count is odd, false otherwise 
    return count&1;  // Same as (count%2 == 1) 
} 

ConvexPolygon ConvexPolygon::polygon_intersection(ConvexPolygon& P2) const{
    vector<Point> cpi;
    vector<Point> p1 = p;
    vector<Point> p2 = P2.get_vector();
    if (P2.inside(ConvexPolygon(p))) return P2;
    if (ConvexPolygon(p).inside(P2)) return ConvexPolygon(p);

    //We compare the edges of the polygons to see if they intersect and add the intersection point to the vector
    for (int i = 0; i < p2.size()-1; ++i){
    	for (int j = 0; j < p1.size()-1; ++j){
    		if (intersect(p1[i], p1[i+1], p2[j], p2[j+1]) and not in_the_same_line(p1[i], p1[i+1], p2[j], p2[j+1])){
    			cpi.push_back(line_intersection(p1[i], p1[i+1], p2[j], p2[j+1]));
    		}
    	}
    }

    for (int k = 0; k < p1.size()-1; ++k){
    	//If the point is inside the other polygon, it has to be in the intersection
    	if (point_inside(p2, p1[k])) cpi.push_back(p1[k]);
    	//Last edge of the polygons (from the last "for")
    	if (intersect(p1[k], p1[k+1], p2[0], p2[p2.size()-1]) and not in_the_same_line(p1[k], p1[k+1], p2[0], p2[p2.size()-1])){
    		cpi.push_back(line_intersection(p1[k], p1[k+1], p2[0], p2[p2.size()-1]));
    	}
    }

    for (int l = 0; l < p2.size()-1; ++l){
    	//If the point is inside the other polygon, it has to be in the intersection
    	if (point_inside(p1, p2[l])) cpi.push_back(p2[l]);
    	//Last edge of P1
    	if (intersect(p1[0], p1[p1.size()-1], p2[l], p2[l+1]) and not in_the_same_line(p1[0], p1[p1.size()-1], p2[l], p2[l+1])){
    		cpi.push_back(line_intersection(p1[0], p1[p1.size()-1], p2[l], p2[l+1]));
    	}
    }

    //Last point of P2 (check if it is inside P1)
    if (point_inside(p1, p2[p2.size()-1])) cpi.push_back(p2[p2.size()-1]);
    //Last point of P1 (check if it is inside P2)
    if (point_inside(p2, p1[p1.size()-1])) cpi.push_back(p1[p1.size()-1]);
    //Last last edge of P1 and P2, together
    if (intersect(p2[0], p2[p2.size()-1], p1[p1.size()-1], p1[0]) and not in_the_same_line(p2[0], p2[p2.size()-1], p1[p1.size()-1], p1[0])){
    	cpi.push_back(line_intersection(p2[0], p2[p2.size()-1], p1[p1.size()-1], p1[0]));
    }

    return ConvexPolygon(cpi);
}