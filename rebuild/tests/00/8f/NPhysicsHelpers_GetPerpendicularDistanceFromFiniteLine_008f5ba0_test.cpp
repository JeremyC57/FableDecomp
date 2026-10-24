// Behaviour test for 0x008F5BA0 NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine: |point - closest point|.
#include <cstdio>
#include <cmath>

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
    float GetLength() const
    {
        float lengthSquared = Z * Z;
        lengthSquared += X * X;
        lengthSquared += Y * Y;
        return (float)std::sqrt(lengthSquared);
    }
};

static C3DVector GetClosestPointOnFiniteLineToPoint(const C3DVector& a, const C3DVector& b, const C3DVector& p)
{
    float dx = b.X - a.X, dy = b.Y - a.Y, dz = b.Z - a.Z;
    float len2 = dx * dx + dy * dy + dz * dz;
    float t = len2 > 0.0f ? ((p.X - a.X) * dx + (p.Y - a.Y) * dy + (p.Z - a.Z) * dz) / len2 : 0.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return C3DVector(a.X + dx * t, a.Y + dy * t, a.Z + dz * t);
}

static float GetPerpendicularDistanceFromFiniteLine(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                    const C3DVector& point)
{
    C3DVector closest = GetClosestPointOnFiniteLineToPoint(lineStart, lineEnd, point);
    return (point - closest).GetLength();
}

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    C3DVector a(0.0f, 0.0f, 0.0f), b(0.0f, 10.0f, 0.0f);
    int fail = 0;
    if (!near(GetPerpendicularDistanceFromFiniteLine(a, b, C3DVector(3.0f, 5.0f, 4.0f)), 5.0f)) fail = 1;
    if (!near(GetPerpendicularDistanceFromFiniteLine(a, b, C3DVector(0.0f, -6.0f, 8.0f)), 10.0f)) fail = 1;   // before a
    if (!near(GetPerpendicularDistanceFromFiniteLine(a, b, C3DVector(0.0f, 7.0f, 0.0f)), 0.0f)) fail = 1;     // on the line
    if (fail) { std::printf("008f5ba0_TEST FAIL\n"); return 1; }
    std::printf("008f5ba0_TEST PASS\n");
    return 0;
}