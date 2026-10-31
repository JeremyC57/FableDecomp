// Behaviour test for CPlane::GetIntersectionWithTriangle (0x00A42470): same body as src/, with GetIntersectionWithLine
// (0x00A422C0, landed) supplied so the two edge intersections can be checked.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
class CPlane { public:
    C3DVector Normal;
    float     Distance;
    float GetDistanceToPoint(const C3DVector& p) const { return Normal.X * p.X + Normal.Y * p.Y + Normal.Z * p.Z - Distance; }
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* intersection) const
    {
        float den = Normal.X * direction.X + Normal.Y * direction.Y + Normal.Z * direction.Z;
        if (den != 0.0f) {
            float t = (Distance - (point.X * Normal.X + point.Y * Normal.Y + point.Z * Normal.Z)) / den;
            intersection->X = direction.X * t + point.X;
            intersection->Y = direction.Y * t + point.Y;
            intersection->Z = direction.Z * t + point.Z;
            return true;
        }
        return false;
    }
    bool GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const;
};
bool CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const
{
    float da = GetDistanceToPoint(a);
    float db = Normal.Z * b.Z + (Normal.X * b.X + Normal.Y * b.Y) - Distance;
    const C3DVector& n = Normal;
    float dc = c.Z * n.Z;
    dc += n.X * c.X + n.Y * c.Y;
    dc -= Distance;
    C3DVector origin, edge1, edge2;
    if ((da > 0.0f && db > 0.0f) || (da < 0.0f && db < 0.0f)) { origin = c; edge1 = a - origin; edge2 = b - origin; }
    else if ((da > 0.0f && dc > 0.0f) || (da < 0.0f && dc < 0.0f)) { origin = b; edge1 = a - origin; edge2 = c - origin; }
    else if ((db > 0.0f && dc > 0.0f) || (db < 0.0f && dc < 0.0f)) { origin = a; edge1 = b - origin; edge2 = c - origin; }
    else return false;
    GetIntersectionWithLine(origin, edge1, &start);
    GetIntersectionWithLine(origin, edge2, &end);
    return true;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CPlane p; p.Normal = C3DVector(0, 0, 1); p.Distance = 0;   // z = 0 plane
    C3DVector s, e;
    // a,b above, c below: c is alone -> edges c->a and c->b
    bool r = p.GetIntersectionWithTriangle(C3DVector(0, 0, 1), C3DVector(2, 0, 1), C3DVector(0, 2, -1), s, e);
    if (!r || !near(s.X, 0) || !near(s.Y, 1) || !near(s.Z, 0) || !near(e.X, 1) || !near(e.Y, 1) || !near(e.Z, 0)) { std::printf("00a42470_TEST FAIL\n"); return 1; }
    // a above, b below, c above: b alone
    r = p.GetIntersectionWithTriangle(C3DVector(0, 0, 2), C3DVector(0, 0, -2), C3DVector(4, 0, 2), s, e);
    if (!r || !near(s.Z, 0) || !near(s.X, 0) || !near(e.X, 2) || !near(e.Z, 0)) { std::printf("00a42470_TEST FAIL\n"); return 1; }
    // a alone below
    r = p.GetIntersectionWithTriangle(C3DVector(0, 0, -1), C3DVector(0, 2, 1), C3DVector(2, 0, 1), s, e);
    if (!r || !near(s.Y, 1) || !near(e.X, 1)) { std::printf("00a42470_TEST FAIL\n"); return 1; }
    // a and b on the plane: no pair shares a side -> false
    r = p.GetIntersectionWithTriangle(C3DVector(0, 0, 0), C3DVector(1, 0, 0), C3DVector(0, 1, 1), s, e);
    if (r) { std::printf("00a42470_TEST FAIL\n"); return 1; }
    std::printf("00a42470_TEST PASS\n");
    return 0;
}