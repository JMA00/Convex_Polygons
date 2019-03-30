#include <iostream>
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

#include "Point.hh"
#include "ConvexPolygon.hh"

//it inserts the polygon in the map
void polygon(map<string, ConvexPolygon>& ConvexPolygons){
    vector<Point> vp;
    string s;
    getline(cin,s);
    istringstream iss(s);
    double x, y;
    string name;
    iss >> name;
    auto it = ConvexPolygons.find(name);
    if (it != ConvexPolygons.end()) ConvexPolygons.erase(name);//we erase the polygon if it exists, to overwrite it
    while (iss >> x >> y){
        vp.push_back(Point(x,y));
    }
    ConvexPolygons.insert(pair<string, ConvexPolygon>(name, ConvexPolygon(vp)));
    cout << "ok" << endl;
}

//it calls the print function for printing the polygon
void print(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok){
        cout << name << " ";
        ConvexPolygons[name].print();
    }
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it calls the perimeter function to calculate it
void perimeter(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok){
        if (ConvexPolygons[name].get_vector().size() == 0) cout << "error: the polygon is empty and it has no perimeter" << endl;
        else cout << ConvexPolygons[name].perimeter() << endl;
    }
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it calls the vertices function
void vertices(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok) cout << ConvexPolygons[name].vertices() << endl;
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it calls the edges function
void edges(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok) cout << ConvexPolygons[name].edges() << endl;
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it calls the area function
void area(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok){
        if (ConvexPolygons[name].get_vector().size() == 0) cout << "error: the polygon is empty and it has no area" << endl;
        else cout << ConvexPolygons[name].area() << endl;
    }
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it calls the centroid function
void centroid(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    bool tot_ok = true;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) tot_ok = false;
    if (tot_ok){
        if (ConvexPolygons[name].get_vector().size() == 0) cout << "error: the polygon is empty and it has no centroid" << endl;
        else{
            Point c = ConvexPolygons[name].centroid(ConvexPolygons[name].area());
            cout << c.get_x() << " " << c.get_y() << endl;
        }
    }
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it returns the max of the absolute of the numbers
double max(double a, double b){
    if (abs(a) > abs(b)) return a;
    return b;
}

//it is used to scale and move the poligons in order to draw them correctly and returns them in a vector of polygons
vector<ConvexPolygon> scale_for_draw(vector<string> pols, map<string, ConvexPolygon>& ConvexPolygons){
    ConvexPolygon BB;
    //calculate the bbox
    BB = BB.bbox(pols, ConvexPolygons);
    vector<Point> v = BB.get_vector();
    //we want to have the largest side of the bbox
    double max_side = max(v[0].get_x()-v[3].get_x(), v[0].get_y()-v[1].get_y());
    double k; //number by which we will scale the polygons
    k = abs(498/max_side);
    //we scale them and put them in the vector 
    vector<ConvexPolygon> new_cps;
    for (int i = 0; i < int(pols.size()); ++i){
        vector<Point> _p = ConvexPolygons[pols[i]].get_vector();
        vector<double> _c = ConvexPolygons[pols[i]].get_color();
        for (Point& pp : _p){
            pp = {(pp.get_x() - v[0].get_x())*k , (pp.get_y() - v[0].get_y())*k};
        }
        ConvexPolygon cp = {_p,_c};
        new_cps.push_back(cp);
    }
    return new_cps;
}

void draw(map<string, ConvexPolygon>& ConvexPolygons){
    string file;
    cin >> file;
    string s;
    getline(cin,s);
    istringstream iss(s);
    string pol;
    //we put every polygon in a vector of polygons to store them and work
    vector<string> pols;
    bool tot_ok = true;
    while (iss >> pol and tot_ok){
        auto it = ConvexPolygons.find(pol);
        if (it == ConvexPolygons.end()) tot_ok = false;
        pols.push_back(pol);
    }
    if (tot_ok){
        const int size = 500;
        pngwriter png(size, size, 1.0, file.c_str());
        //scale and move
        vector<ConvexPolygon> cps = scale_for_draw(pols, ConvexPolygons);
        //draw. we sum 1 to every component because we want them to fit in the 498x498 square
        for (int i = 0; i < int(cps.size()); ++i){
            vector<Point> p = cps[i].get_vector();
            vector<double> c = cps[i].get_color();
            int m = p.size()-1;
            for (int i = 0; i < m; ++i){
                png.line(int(p[i].get_x())+1, int(p[i].get_y())+1, int(p[i+1].get_x())+1, int(p[i+1].get_y())+1, c[0], c[1], c[2]);
            }
            png.line(int(p[m].get_x())+1, int(p[m].get_y())+1, int(p[0].get_x())+1, int(p[0].get_y())+1, c[0], c[1], c[2]);
        }
        png.close();
        cout << "ok" << endl;
    }
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it saves the polygons in a file
void save(map<string, ConvexPolygon>& ConvexPolygons){
    string file;
    cin >> file;
    ofstream fs(file);
    string s;
    getline(cin,s);
    istringstream iss(s);
    string name;
    bool tot_ok = true;
    while (iss >> name and tot_ok){
        auto it = ConvexPolygons.find(name);
        if (it == ConvexPolygons.end()) tot_ok = false; //the polygon has not been found so it will return an error later
        else{
            fs << name;
            vector<Point> p = ConvexPolygons[name].get_vector();
            for (int i = 0; i < int(p.size()); ++i){
                fs << " " << p[i].get_x() << " " << p[i].get_y();
            }
            fs << endl;
        }
    }
    fs.close();
    if (tot_ok) cout << "ok" << endl;
    //if the polygon is not in the map, it returns an error
    else cout << "error: undefined identifier" << endl;
}

//it reads the polygons from a file and inserts them in the map
void load(map<string, ConvexPolygon>& ConvexPolygons){
    string file;
    cin >> file;
    ifstream fe;
    fe.open (file);
    if (fe.fail()) cout << "error: the file has not been found" << endl; //if the file is not found, error
    else{
        string p;
        while (getline(fe, p)){
            istringstream iss(p);
            double x, y;
            string name;
            iss >> name;
            vector<Point> vp = {};
            while (iss >> x >> y){
                vp.push_back(Point(x,y));
            }
            auto it = ConvexPolygons.find(name); //it searches the polygon
            if (it != ConvexPolygons.end()) ConvexPolygons.erase(name);//we erase the polygon in case it already exists to overwrite it
            ConvexPolygons.insert(pair<string, ConvexPolygon>(name, ConvexPolygon(vp)));
        }
        cout << "ok" << endl;
        fe.close();
    }
}

//it is used to set a color to the polygon
void setcol(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    double r, g, b;
    cin >> r >> g >> b;
    auto it = ConvexPolygons.find(name);
    if (it == ConvexPolygons.end()) cout << "error: undefined identifier" << endl;
    else{
        ConvexPolygons[name].setcol(r, g, b);
        cout << "ok" << endl;
    }
}

//it creates the bounding box of the given polygons
void bbox(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    cin >> name;
    string polygons;
    getline(cin, polygons);
    istringstream iss(polygons);
    string pol;
    vector<string> pols;
    bool tot_ok = true;
    while (iss >> pol and tot_ok){
        auto it = ConvexPolygons.find(pol);
        if (it == ConvexPolygons.end()) tot_ok = false;
        else pols.push_back(pol);
    }
    if (tot_ok){
        ConvexPolygon bb;
        bb = bb.bbox(pols, ConvexPolygons);
        auto it = ConvexPolygons.find(name);
        if (it != ConvexPolygons.end()) ConvexPolygons.erase(name);
        ConvexPolygons.insert(pair<string, ConvexPolygon>(name, bb));
        cout << "ok" << endl;
    }
    else cout << "error: undefined identifier" << endl;
}

//it writes a list of all the polygons in the map
void list(map<string, ConvexPolygon>& ConvexPolygons){
    bool first = true;
    for (auto e : ConvexPolygons){
        if (first){
            cout << e.first;
            first = false;
        }
        else cout << " " << e.first;
    }
    cout << endl;
}

//it creates the union polygon of the given ones
void polygon_union(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    string p;
    getline(cin, p);
    istringstream iss(p);
    string polygon;
    vector<string> polygons; //we store the given polygons in a vector to be able to know how many have been given
    while (iss >> polygon) polygons.push_back(polygon);
    int n = polygons.size(); //depending on the size, the union is inserted in the map with a different name
    ConvexPolygon cu;
    if (n == 2 or n == 3){
        if (n == 2) cu = ConvexPolygons[polygons[0]].polygon_union(ConvexPolygons[polygons[1]]);
        else if (n == 3) cu = ConvexPolygons[polygons[1]].polygon_union(ConvexPolygons[polygons[2]]);
        auto it = ConvexPolygons.find(polygons[0]);
        if (it != ConvexPolygons.end()) ConvexPolygons.erase(polygons[0]);
        ConvexPolygons.insert(pair<string, ConvexPolygon>(polygons[0], cu));
        cout << "ok" << endl;
    }
    else cout << "error: command with wrong number or type of arguments" << endl;
}

//it creates the intersection polygon of the given ones
void intersection(map<string, ConvexPolygon>& ConvexPolygons){
    string name;
    string p;
    getline(cin, p);
    istringstream iss(p);
    string polygon;
    vector<string> polygons; //we store the given polygons in a vector to be able to know how many have been given
    while (iss >> polygon) polygons.push_back(polygon);
    int n = polygons.size(); //depending on the size, the union is inserted in the map with a different name
    ConvexPolygon cu;
    ConvexPolygon ci;
    if (n == 2 or n == 3){
        if (n == 2) ci = ConvexPolygons[polygons[0]].polygon_intersection(ConvexPolygons[polygons[1]]);
        else if (n == 3) ci = ConvexPolygons[polygons[1]].polygon_intersection(ConvexPolygons[polygons[2]]);
        auto it = ConvexPolygons.find(polygons[0]);
        if (it != ConvexPolygons.end()) ConvexPolygons.erase(polygons[0]);
        ConvexPolygons.insert(pair<string, ConvexPolygon>(polygons[0], ci));
        cout << "ok" << endl;
    }
    else cout << "error: command with wrong number or type of arguments" << endl;
}

//it writes yes or not depending on wheter the first polygon is inside the second 
void inside(map<string, ConvexPolygon>& ConvexPolygons){
    string p, q;
    cin >> p >> q;
    auto it = ConvexPolygons.find(p);
    if (it == ConvexPolygons.end()) cout << "error: undefined identifier" << endl;
    auto it2 = ConvexPolygons.find(q);
    if (it2 == ConvexPolygons.end()) cout << "error: undefined identifier" << endl;
    if (ConvexPolygons[p].inside(ConvexPolygons[q])) cout << "yes";
    else cout << "not";
    cout << endl;
}

//if the input starts with "#" the following is ignored
void ignore_line(){
    string s;
    getline(cin, s);
    cout << "#" << endl;
}

int main () {
    cout.setf(ios::fixed);
    cout.precision(3);
    //we create a map in which the key is the name of the polygon and the value is the ConvexPolygon
    map<string, ConvexPolygon> ConvexPolygons;

    string action;
    while (cin >> action) {
             if (action == "polygon") polygon(ConvexPolygons);
        else if (action == "print") print(ConvexPolygons);
        else if (action == "area") area(ConvexPolygons);
        else if (action == "perimeter") perimeter(ConvexPolygons);
        else if (action == "vertices") vertices(ConvexPolygons);
        else if (action == "edges") edges(ConvexPolygons);
        else if (action == "centroid") centroid(ConvexPolygons);
        else if (action == "setcol") setcol(ConvexPolygons);
        else if (action == "draw") draw(ConvexPolygons);
        else if (action == "save") save(ConvexPolygons);
        else if (action == "load") load(ConvexPolygons);
        else if (action == "list") list(ConvexPolygons);
        else if (action == "union") polygon_union(ConvexPolygons);
        else if (action == "#") ignore_line();
        else if (action == "inside") inside(ConvexPolygons);
        else if (action == "bbox") bbox(ConvexPolygons);
        else if (action == "intersection") intersection(ConvexPolygons);
        
        else cout << "error: unrecognized command" << endl;
    }
}