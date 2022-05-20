//Task: find the shortest path on a continous 2D plane with circular forbidden areas. 
//Didn't solve to the end due to some bug, but sure of approach. Dijkstra on line 277, geometry otherwise 
//Top level problem on Codewars: https://www.codewars.com/kata/58b617f15705ae68cc0000a9/

#include <bits/stdc++.h>
using namespace std; 
using lli = long long int;
#define forup(i,i0,n) for(int i=i0; i<=(int)n; i++)
#define fordown(i,n,i0) for(int i=n; i>=(int)i0; i--)

double EPS = 1e-12; 
double PI = M_PI;
double INF = 1e10; 
struct Line {
    double a,b,c;
};

double distance(Point a, Point b) {
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y)); 
}

Point dif(Point a, Point b) {
    return Point(a.x-b.x, a.y-b.y);
}

double mod(Point a) {
    return sqrt(a.x*a.x+a.y*a.y);
}

struct Edge {
    int u;
    double d; 
};

void one_tangent(Point c, double r1, double r2, vector <Line> &ans) {
    double r = r1 - r2;
    double z = c.x * c.x + c.y * c.y;
    double d = z - r*r;
    if (d < -EPS) return;
    d = sqrt(abs(d));
    Line l;
    l.a = (c.x * r + c.y * d) / z;
    l.b = (c.y * r - c.x * d) / z;
    l.c = r1;
    ans.push_back(l);
}

vector <Line> tangents(Circle a, Circle b) {
    vector<Line> ans;
    for (int i=-1; i<=1; i+=2) {
        for (int j=-1; j<=1; j+=2) {
            one_tangent(dif(a.ctr, b.ctr), a.r*i, b.r*j, ans);
        }
    }
    forup(i,0,ans.size()-1) ans[i].c -= ans[i].a * a.ctr.x + ans[i].b * a.ctr.y;
    return ans;
}

Point touch(Circle c, Line l) {

    double A,B,C,x,y; 
    if (abs(l.b)>EPS) {
        A = pow(l.a/l.b, 2) + 1;
        B = 2.0*(-c.ctr.x + l.c*l.a/pow(l.b, 2) + c.ctr.y*l.a/l.b);
        C = pow(c.ctr.x, 2) + pow(l.c/l.b, 2) + pow(c.ctr.y, 2) - c.r*c.r + 2*c.ctr.y*l.c/l.b;

        double D = B*B - 4.0*A*C; 
        //if (D < -EPS) cout << "Warning, D = " << D << endl;
        if (D < EPS) D = 0; 
        x = (-B + sqrt(D))/(2*A);
        y = -(l.c + l.a * x) / l.b; 
        return Point(x,y); 
    }

    return Point(-l.c/l.a, c.ctr.y);
}

vector <Point> lineCircleIntersection (Line l, Circle c) {
    double A,B,C,D, x1,y1,x2,y2; 
    if (abs(l.b)>EPS) {
        A = pow(l.a/l.b, 2) + 1;
        B = 2.0*(-c.ctr.x + l.c*l.a/pow(l.b, 2) + c.ctr.y*l.a/l.b);
        C = pow(c.ctr.x, 2) + pow(l.c/l.b, 2) + pow(c.ctr.y, 2) - c.r*c.r + 2*c.ctr.y*l.c/l.b;

        D = B*B - 4.0*A*C; 
        if (D < -EPS) return {};
        if (D < EPS) D = 0; 
        x1 = (-B + sqrt(D))/(2*A);
        y1 = -(l.c + l.a * x1) / l.b; 
        x2 = (-B - sqrt(D))/(2*A);
        y2 = -(l.c + l.a * x2) / l.b; 

    }

    else {
        x1 = -l.c/l.a;
        x2 = x1;
        A = 1;
        B = -2.0*c.ctr.y;
        C = pow(c.ctr.y,2) + pow((x1 - c.ctr.x),2) - pow(c.r,2); 
        
        D = B*B - 4.0*A*C; 
        if (D < -EPS) return {};
        if (D < EPS) D = 0; 
        y1 = (-B + sqrt(D))/(2*A);
        y2 = (-B - sqrt(D))/(2*A);
    }

    return {{x1,y1},{x2,y2}}; 
}

bool noCrossing(Line l, Point pV, Point pI, Circle c) {
    
    auto pts = lineCircleIntersection(l, c);
    if (pts.empty()) return true;

    double x1,x2,y1,y2;
    x1 = pts[0].x;
    y1 = pts[0].y;
    x2 = pts[1].x;
    y2 = pts[1].y; 

    double minX = pV.x < pI.x ? pV.x : pI.x;
    double maxX = pV.x > pI.x ? pV.x : pI.x;

    double minY = pV.y < pI.y ? pV.y : pI.y;
    double maxY = pV.y > pI.y ? pV.y : pI.y;

    bool inside1 = x1 > minX-EPS and x1 < maxX+EPS and y1 > minY-EPS and y1 < maxY+EPS; 
    bool inside2 = x2 > minX-EPS and x2 < maxX+EPS and y2 > minY-EPS and y2 < maxY+EPS;
    return (!inside1) and (!inside2); 

}

vector <Point> circleCircleIntersection(Circle c0, Circle c1) {

    double dx = c1.ctr.x - c0.ctr.x;
    double dy = c1.ctr.y - c0.ctr.y;

    Line l = {
        -2.0*dx,
        -2.0*dy,
        dx*dx + dy*dy + c0.r*c0.r - c1.r*c1.r
    };

    Circle c = {0,0,c0.r}; 

    auto pts = lineCircleIntersection(l, c);

    if (!pts.empty()) {
        return {{pts[0].x + c0.ctr.x, pts[0].y+c0.ctr.y},
                {pts[1].x + c0.ctr.x, pts[1].y+c0.ctr.y}};
    }

    return {};
}

double posOnCircle(Point a, Circle c) {
    Point vecA = Point(0,1); 
    Point vecB = Point(a.x - c.ctr.x , a.y - c.ctr.y); 
    double angle = acos( ( vecA.x * vecB.x + vecA.y * vecB.y ) / (mod(vecB)) );
    if (a.x > c.ctr.x) return angle*c.r; 
    return (2*PI-angle)*c.r; 
}

bool comp(vector <double> par0, vector <double> par1) {
    return par0[1] < par1[1]; 
}

vector <vector <double> > forbiddenAreasOnCircle(int p, vector <Circle> c) {
    int n = c.size();
    vector <vector <double> > faoc;
    forup(i,0,n-1) {
        if (p != i) {
            vector <double> par;
            auto pts = circleCircleIntersection(c[p],c[i]);
            for (auto u : pts) {
                par.push_back(posOnCircle(u, c[p]));
            }
            if (!par.empty()) {
                sort(par.begin(), par.end());
                faoc.push_back(par); 
            }
        }
    }
    if (!faoc.empty()) sort(faoc.begin(), faoc.end(), comp);
    return faoc; 
}


vector <double> distOnCircle(double posA, double posB, Circle c, vector <double> faocL, vector <double> faocR) {

    if (c.r < EPS) return {0}; 

    int n = faocR.size();
    double d = abs(posA-posB);
    double per = 2*PI*c.r;

    if (n == 0) return {d, per - d};

    // if (posA < faocL[0] and posB < faocL[0]) return {d};
    // if (posA < faocL )

    int lbA = lower_bound(faocR.begin(), faocR.end(), posA) - faocR.begin();
    int lbB = lower_bound(faocR.begin(), faocR.end(), posB) - faocR.begin();

    if (lbA == 0 and posA < faocL[0] and lbB == n) return {posA + per - posB};
    if (lbA == n and posB < faocL[0] and lbB == 0) return {posB + per - posA};
    if (lbA == n and lbB == n) return {d}; 

    if (lbA == n or lbB == n) return {};

    if (posA < faocL[lbA] and posB < faocL[lbA] and lbA==lbB) return {d}; 

    return {};

}

void make_graph(vector <Circle> c, map <int, vector <Edge> > &graph, vector <Point> &vert, vector <vector <int> > &belong) {
    int n = c.size(); 
    for (int v = 0; v < n; v++) {
        for (int i = v+1; i < n; i++) {
            auto lines = tangents(c[v], c[i]);
            for (auto l : lines) {
                Point pV = touch(c[v], l);
                Point pI = touch(c[i], l);
                bool ok = true;
                for (int j = 0; j < n; j++) {
                    if (j != v and j != i) {
                        ok = ok and noCrossing(l, pV, pI, c[j]);
                    }
                }
                if (ok) {
                    vert.push_back(pV);
                    vert.push_back(pI);
                    double d = distance(pV, pI); 
                    graph[vert.size()-1].push_back({(int)vert.size()-2, d});
                    graph[vert.size()-2].push_back({(int)vert.size()-1, d});
                    belong[v].push_back((int)vert.size()-2);
                    belong[i].push_back((int)vert.size()-1);
                }
            }
        }
    }

}

void add_circular(vector <Circle> c, map <int, vector <Edge> > &graph, vector <Point> vert, vector <vector <int> > belong) {
    int n = c.size();
    forup(i,0,n-1) {

        auto faoc = forbiddenAreasOnCircle(i, c); 
        
        vector <double> faocL;
        vector <double> faocR;

        for (auto par : faoc) {
            faocL.push_back(par[0]);
            faocR.push_back(par[1]);
        }

        int m = belong[i].size(); 

        forup(j,0,m-1) {
            forup(k,0,m-1) {
                if (j!=k) {
                    double posA = posOnCircle(vert[belong[i][j]], c[i]);
                    double posB = posOnCircle(vert[belong[i][k]], c[i]);
                    auto doc = distOnCircle(posA, posB, c[i], faocL, faocR);
                    for (auto d : doc) graph[belong[i][j]].push_back({belong[i][k], d});
                }
            }
        }
    }
}

double shortest_path_length(Point a, Point b, vector<Circle> c) {

    c.push_back({a.x, a.y, 0});
    c.push_back({b.x, b.y, 0});
    
    int n = c.size();

    map <int, vector <Edge> > graph;
    vector <Point> vert;
    vector <vector <int> > belong(n); 

    make_graph(c, graph, vert, belong);

    add_circular(c, graph, vert, belong);

    if (belong[n-2].empty() or belong[n-1].empty()) return -1; 
    
    int m = graph.size(); 

    int s = belong[n-2][0];
    int f = belong[n-1][0]; 

    vector <bool> visit(m, false);

    vector <double > dist(m, INF);
    dist[s] = 0;

    priority_queue <pair <double, int> > q;

    forup(i,0,m-1) {
        q.push({-dist[i], i});
    }

    vector <int> parent(m); 

    while(!q.empty()) {
        auto x = q.top();
        q.pop();
        int v = x.second;
        if (!visit[v]) {
            visit[v] = true;
            for(auto edge : graph[v]) {
                double upd = edge.d + dist[v];
                if (dist[edge.u] > upd) {
                    dist[edge.u] = upd;
                    parent[edge.u] = v; 
                    q.push({-dist[edge.u], edge.u}); 
                }
            }
        }
    }

    if (dist[f] < INF) return dist[f]; 
    return -1; 
}
 