// Behaviour test for 0x00760760 NShape::GetFirstPointThatBreaksConvexity(std::vector<C3DVector>&): walks consecutive
// point triples from the second point and returns the middle point of the first triple whose outgoing edge is not
// between the direction back to the first point and the incoming edge (end() when none breaks convexity).
#include <cstdio>
#include <cmath>
#include <vector>

struct V3 {
    float X, Y, Z;
};

static V3 mk(float x, float y) { V3 v; v.X = x; v.Y = y; v.Z = 0.0f; return v; }
static V3 sub(const V3& a, const V3& b) { V3 v; v.X = a.X - b.X; v.Y = a.Y - b.Y; v.Z = a.Z - b.Z; return v; }
static float dot(const V3& a, const V3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static V3 norm(const V3& a)
{
    float l = (float)std::sqrt(dot(a, a));
    V3 v; v.X = a.X / l; v.Y = a.Y / l; v.Z = a.Z / l; return v;
}

// The landed 0x0075FF00 rule: normalised c is closer in direction to both a and b than a and b are to each other.
static bool IsVectorBetween2OthersInThePlane(const V3& a, const V3& b, const V3& c)
{
    V3 na = norm(a), nb = norm(b), nc = norm(c);
    float ab = dot(na, nb);
    return dot(na, nc) > ab && dot(nb, nc) > ab;
}

static int GetFirstPointThatBreaksConvexity(const std::vector<V3>& shape)   // returns the index, size() for end()
{
    unsigned int it = 0;
    V3 first = shape[it];
    unsigned int last = (unsigned int)shape.size() - 2;
    for (++it; it != last; ++it) {
        V3 p0 = shape[it], p1 = shape[it + 1], p2 = shape[it + 2];
        V3 toFirst = sub(first, p1);
        V3 inEdge = sub(p1, p0);
        V3 outEdge = sub(p2, p1);
        if (!IsVectorBetween2OthersInThePlane(toFirst, inEdge, outEdge))
            return (int)(it + 1);
    }
    return (int)shape.size();
}

int main()
{
    int ok = 1;
    std::vector<V3> tri;                                    // fewer than four points: the loop never runs
    tri.push_back(mk(0, 0)); tri.push_back(mk(1, 0)); tri.push_back(mk(1, 1));
    if (GetFirstPointThatBreaksConvexity(tri) != 3) ok = 0;

    std::vector<V3> s;                                      // outgoing edge (0,-1) at point 2 is not between the
    s.push_back(mk(0, 0)); s.push_back(mk(10, 0));          // directions (-10,-5) and (0,5): point 2 breaks
    s.push_back(mk(10, 5)); s.push_back(mk(10, 4));
    s.push_back(mk(0, 9));
    if (GetFirstPointThatBreaksConvexity(s) != 2) ok = 0;

    std::vector<V3> t;                                      // point 2 passes ((-5,-2) between (-8,-6) and (2,6)),
    t.push_back(mk(0, 0)); t.push_back(mk(6, 0));           // point 3 fails ((-3,5) vs (-3,-4) and (-5,-2))
    t.push_back(mk(8, 6)); t.push_back(mk(3, 4));
    t.push_back(mk(0, 9));
    if (GetFirstPointThatBreaksConvexity(t) != 3) ok = 0;

    std::vector<V3> q;                                      // a square: its one checked corner is convex -> end()
    q.push_back(mk(0, 0)); q.push_back(mk(10, 0));
    q.push_back(mk(10, 10)); q.push_back(mk(0, 10));
    if (GetFirstPointThatBreaksConvexity(q) != 4) ok = 0;

    if ((unsigned int)sizeof(V3) != 12) ok = 0;
    if (!ok) { std::printf("00760760_TEST FAIL\n"); return 1; }
    std::printf("00760760_TEST PASS\n");
    return 0;
}