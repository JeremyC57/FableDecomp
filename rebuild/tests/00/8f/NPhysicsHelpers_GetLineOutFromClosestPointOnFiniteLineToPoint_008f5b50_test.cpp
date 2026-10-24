// Behaviour test for 0x008F5B50 NPhysicsHelpers::GetLineOutFromClosestPointOnFiniteLineToPoint: point - closest point.
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& v) const
    {
        C3DVector result;
        result.X = X - v.X;
        result.Y = Y - v.Y;
        result.Z = Z - v.Z;
        return result;
    }
};

// stand-in for 0x008F5970: projection onto the segment, clamped to its ends
static C3DVector GetClosestPointOnFiniteLineToPoint(const C3DVector& a, const C3DVector& b, const C3DVector& p)
{
    float dx = b.X - a.X, dy = b.Y - a.Y, dz = b.Z - a.Z;
    float len2 = dx * dx + dy * dy + dz * dz;
    float t = len2 > 0.0f ? ((p.X - a.X) * dx + (p.Y - a.Y) * dy + (p.Z - a.Z) * dz) / len2 : 0.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return C3DVector(a.X + dx * t, a.Y + dy * t, a.Z + dz * t);
}

static C3DVector GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                               const C3DVector& point)
{
    C3DVector closest = GetClosestPointOnFiniteLineToPoint(lineStart, lineEnd, point);
    return point - closest;
}

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    C3DVector a(0.0f, 0.0f, 0.0f), b(10.0f, 0.0f, 0.0f);
    int fail = 0;
    C3DVector r = GetLineOutFromClosestPointOnFiniteLineToPoint(a, b, C3DVector(4.0f, 3.0f, -2.0f));
    if (!near(r.X, 0.0f) || !near(r.Y, 3.0f) || !near(r.Z, -2.0f)) fail = 1;           // perpendicular offset
    C3DVector s = GetLineOutFromClosestPointOnFiniteLineToPoint(a, b, C3DVector(13.0f, 4.0f, 0.0f));
    if (!near(s.X, 3.0f) || !near(s.Y, 4.0f) || !near(s.Z, 0.0f)) fail = 1;            // past the end: from b
    if (fail) { std::printf("008f5b50_TEST FAIL\n"); return 1; }
    std::printf("008f5b50_TEST PASS\n");
    return 0;
}