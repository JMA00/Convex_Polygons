#include <iostream>
#include <vector>
#include "Point.hh"
#include <pngwriter.h>
#include <map>
#include <sstream>
using namespace std;

#ifndef ConvexPolygon_hh
#define ConvexPolygon_hh

class ConvexPolygon{
public:
	ConvexPolygon(vector<Point> p={}, vector<double> c={0.0, 0.0, 0.0}); //a polygon is empty and black by default
	vector<Point> get_vector() const; //it returns the vector of points of the polygon
	vector<double> get_color() const; //it returns the vector of colors of the polygon
	double get_min_x() const; //it returns the minimum "x" of the polygon
	double get_max_x() const; //it returns the maximum "x" of the polygon
	double get_min_y() const; //it returns the minimum "y" of the polygon
	double get_max_y() const; //it returns the maximum "y" of the polygon
	void print() const; //it prints the polygon
	double area() const; //it returns the area
	int vertices() const; //it returns the number of vertices
	int edges() const; //it returns the area of vertices
	double perimeter() const; //it returns the perimeter
	Point centroid(double area) const; //it returns the centroid 
	void setcol(double r, double g, double b); //it sets a color to the polygon by changing its vector of colors
	bool operator== (const ConvexPolygon& c) const; //it returns whether the polygons have the same points
	ConvexPolygon polygon_union(const ConvexPolygon& c) const; //it return the convex union of the given polygons
	bool inside(const ConvexPolygon& q) const; //it returns a boolean of whether the first is inside the second polygon
	ConvexPolygon polygon_intersection(ConvexPolygon& cp) const; //it returns the convex intersection of the given polygons
	ConvexPolygon bbox(vector<string> pols, map<string, ConvexPolygon>& ConvexPolygons) const; //it returns the bounding box

private:
	vector<Point> p; //the polygon is defined as a vector of points
	vector<double> c; //and every polygon has its color
};

#endif
