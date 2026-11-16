// Behaviour test for 0x00ACA830 QuadricOptimiserInternals::COptimisedPrimitive::Initialise (model of the row's logic):
// plane quadrics of every face (weight longest-edge^2 / 2, only when the face normal is non-zero) are added to its three vertices,
// then every vertex with a positive quadric weight is rescaled to weight scaling^2; for each face edge with an adjacent face whose
// normal makes a negative dot with this face's normal, a crease quadric (normal = sum of both normals, only when non-zero; weight
// -dot) is added to the edge's two vertices; border edges (no adjacent face) add a weight-0 edge quadric and EdgeCost to both
// vertices' Cost; ComputeVertexCost runs per vertex; every unique edge becomes one contraction (sub-contractions of both faces, the
// second only when it differs) kept only when its vertex sets are injective; every pair of actual vertices sharing a unique vertex
// becomes an Unsplit contraction; each kept contraction is registered in both vertices' pair maps and gets its list iterator.
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <set>

struct V3 { double x, y, z; V3(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) { } };
static V3 sub(const V3& a, const V3& b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
static V3 add(const V3& a, const V3& b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
static V3 cross(const V3& a, const V3& b) { return V3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static double dot(const V3& a, const V3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static bool nonzero(const V3& v) { return dot(v, v) > 1e-4 * 1e-4; }
static V3 norm(const V3& v) { double f = 1.0 / sqrt(dot(v, v)); return V3(v.x * f, v.y * f, v.z * f); }

struct Vertex { V3 p; long unique; double weight, cost; std::map<long, int> pairs; };
struct Face { long v[3]; long adj[3]; };
struct Edge { long u1, u2, fa, fb; };
struct Contraction { std::vector<std::pair<long, long> > subs; bool unsplit; double cost; };

static std::vector<Vertex> Vs; static std::vector<Face> Fs; static std::vector<Edge> Es; static std::vector<std::set<long> > Us;
static std::multimap<double, Contraction*> List;

static V3 faceNormal(const Face& f) { V3 n = cross(sub(Vs[f.v[2]].p, Vs[f.v[0]].p), sub(Vs[f.v[1]].p, Vs[f.v[0]].p)); return nonzero(n) ? norm(n) : n; }
static long actual(long unique, const Face& f) { for (int k = 0; k < 3; k++) if (Vs[f.v[k]].unique == unique) return f.v[k]; return -1; }

static void Initialise(double scaling, double edge_cost)
{
    for (size_t i = 0; i < Vs.size(); i++) { Vs[i].weight = 0; Vs[i].cost = 0; }
    for (size_t i = 0; i < Fs.size(); i++) {
        const Face& f = Fs[i]; double l[3];
        for (int k = 0; k < 3; k++) { V3 d = sub(Vs[f.v[(k + 1) % 3]].p, Vs[f.v[k]].p); l[k] = sqrt(dot(d, d)); }
        double longest = l[0] > l[1] ? l[0] : l[1]; if (l[2] > longest) longest = l[2];
        if (nonzero(faceNormal(f))) for (int k = 0; k < 3; k++) Vs[f.v[k]].weight += longest * longest * 0.5;
    }
    for (size_t i = 0; i < Vs.size(); i++) if (Vs[i].weight > 0.0) Vs[i].weight *= scaling * scaling / Vs[i].weight;
    for (size_t i = 0; i < Fs.size(); i++) {
        V3 n = faceNormal(Fs[i]);
        for (int e = 0; e < 3; e++) {
            if (Fs[i].adj[e] == -1) continue;
            V3 o = faceNormal(Fs[Fs[i].adj[e]]); double d = dot(n, o);
            if (d < 0.0 && nonzero(add(o, n))) { Vs[Fs[i].v[e]].weight += -d; Vs[Fs[i].v[(e + 1) % 3]].weight += -d; }
        }
    }
    for (size_t i = 0; i < Fs.size(); i++)
        for (int e = 0; e < 3; e++)
            if (Fs[i].adj[e] == -1) { Vs[Fs[i].v[e]].cost += edge_cost; Vs[Fs[i].v[(e + 1) % 3]].cost += edge_cost; }
    for (size_t i = 0; i < Vs.size(); i++) Vs[i].cost += Vs[i].weight;              // ComputeVertexCost stand-in of the model
    std::map<std::pair<long, long>, Contraction*> cs;
    for (size_t i = 0; i < Es.size(); i++) {
        long u1 = Es[i].u1, u2 = Es[i].u2; if (u1 > u2) { long t = u1; u1 = u2; u2 = t; }
        Contraction*& c = cs[std::make_pair(u1, u2)]; if (!c) { c = new Contraction; c->unsplit = false; }
        std::pair<long, long> a(actual(u1, Fs[Es[i].fa]), actual(u2, Fs[Es[i].fa])), b(actual(u1, Fs[Es[i].fb]), actual(u2, Fs[Es[i].fb]));
        c->subs.push_back(a); if (a != b) c->subs.push_back(b);
    }
    for (std::map<std::pair<long, long>, Contraction*>::iterator it = cs.begin(); it != cs.end(); ++it) {
        std::set<long> s1, s2; Contraction* c = it->second;
        for (size_t s = 0; s < c->subs.size(); s++) { s1.insert(c->subs[s].first); s2.insert(c->subs[s].second); }
        if (s1.size() == c->subs.size() && s2.size() == c->subs.size()) { c->cost = (double)(it->first.first * 10 + it->first.second); List.insert(std::make_pair(c->cost, c)); }
    }
    for (size_t u = 0; u < Us.size(); u++)
        if (Us[u].size() > 1)
            for (std::set<long>::iterator a = Us[u].begin(); a != Us[u].end(); ++a) { std::set<long>::iterator b = a;
                for (++b; b != Us[u].end(); ++b) { Contraction* c = new Contraction; c->unsplit = true; c->subs.push_back(std::make_pair(*a, *b)); c->cost = 100.0 + *a + *b; List.insert(std::make_pair(c->cost, c)); } }
    for (std::multimap<double, Contraction*>::iterator it = List.begin(); it != List.end(); ++it)
        for (size_t s = 0; s < it->second->subs.size(); s++) { Vs[it->second->subs[s].first].pairs[it->second->subs[s].second] = (int)s; Vs[it->second->subs[s].second].pairs[it->second->subs[s].first] = (int)s; }
}

int main()
{
    // A = (0,1,2) z-down; B = (1,2,3) folded (crease with A, dot < 0); C = (0,2,5) coplanar with A (dot > 0: no crease); 4 = copy of 0.
    double P[6][3] = { {0,0,0}, {1,0,0}, {0,1,0}, {1,1,0.5}, {0,0,0}, {-1,0.5,0} };
    long U[6] = { 0, 1, 2, 3, 0, 4 };
    for (int i = 0; i < 6; i++) { Vertex v; v.p = V3(P[i][0], P[i][1], P[i][2]); v.unique = U[i]; v.weight = v.cost = 0; Vs.push_back(v); }
    Face A = { {0,1,2}, {-1,1,2} }, B = { {1,2,3}, {0,-1,-1} }, C = { {0,2,5}, {0,-1,-1} };
    Fs.push_back(A); Fs.push_back(B); Fs.push_back(C);
    Edge E[7] = { {0,1,0,0}, {1,2,0,1}, {2,0,0,2}, {2,3,1,1}, {3,1,1,1}, {2,4,2,2}, {4,0,2,2} };
    for (int i = 0; i < 7; i++) Es.push_back(E[i]);
    Us.resize(5); for (int i = 0; i < 6; i++) Us[U[i]].insert(i);
    Initialise(2.0, 0.25);
    double d = 1.0 / sqrt(1.5);                                                   // -dot(A, B)
    int ok = 1;
    ok &= fabs(Vs[0].weight - 4.0) < 1e-9 && fabs(Vs[3].weight - 4.0) < 1e-9 && fabs(Vs[5].weight - 4.0) < 1e-9;
    ok &= fabs(Vs[1].weight - (4.0 + 2 * d)) < 1e-9 && fabs(Vs[2].weight - (4.0 + 2 * d)) < 1e-9;
    ok &= Vs[4].weight == 0.0;                                                    // no face: never rescaled (no 0/0)
    ok &= fabs(Vs[0].cost - 4.5) < 1e-9 && fabs(Vs[1].cost - (4.0 + 2 * d + 0.5)) < 1e-9 && fabs(Vs[2].cost - (4.0 + 2 * d + 0.5)) < 1e-9;
    ok &= fabs(Vs[3].cost - 4.5) < 1e-9 && fabs(Vs[5].cost - 4.5) < 1e-9 && Vs[4].cost == 0.0;
    ok &= List.size() == 8;                                                       // 7 unique edges + the unsplit pair (0,4)
    ok &= Vs[0].pairs.size() == 4 && Vs[0].pairs.count(4) == 1 && Vs[4].pairs.size() == 1 && Vs[3].pairs.size() == 2;
    ok &= Vs[1].pairs.size() == 3 && Vs[5].pairs.size() == 2 && (--List.end())->second->unsplit;
    std::printf(ok ? "00aca830_TEST PASS\n" : "00aca830_TEST FAIL\n");
    return 0;
}
