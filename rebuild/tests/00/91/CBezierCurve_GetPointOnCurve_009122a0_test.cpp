// Behaviour test for CBezierCurve::GetPointOnCurve (retail 0x009122A0): the cubic Bezier evaluation, through a copy of the model.
#include <cstdio>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    const C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
};

inline float GFSqr(float value) { return value * value; }

class CBezierCurve {
public:
    C3DVector Point1, Point2, Point3, Point4;
    C3DVector GetPointOnCurve(float t) const
    {
        C3DVector result = Point1 * (float)pow((double)(1.0f - t), 3.0)
                         + Point2 * 3.0f * t * GFSqr(1.0f - t)
                         + Point3 * 3.0f * GFSqr(t) * (1.0f - t)
                         + Point4 * (float)pow((double)t, 3.0);
        return result;
    }
};

static C3DVector Lerp(const C3DVector& a, const C3DVector& b, float t)
{
    return C3DVector(a.X + (b.X - a.X) * t, a.Y + (b.Y - a.Y) * t, a.Z + (b.Z - a.Z) * t);
}

// Independent reference: de Casteljau's repeated interpolation.
static C3DVector DeCasteljau(const CBezierCurve& c, float t)
{
    C3DVector a = Lerp(c.Point1, c.Point2, t), b = Lerp(c.Point2, c.Point3, t), d = Lerp(c.Point3, c.Point4, t);
    C3DVector e = Lerp(a, b, t), f = Lerp(b, d, t);
    return Lerp(e, f, t);
}

static bool Near(const C3DVector& a, const C3DVector& b)
{
    return fabs(a.X - b.X) < 0.001f && fabs(a.Y - b.Y) < 0.001f && fabs(a.Z - b.Z) < 0.001f;
}

int main()
{
    CBezierCurve c;
    c.Point1 = C3DVector(1.0f, 2.0f, 3.0f);
    c.Point2 = C3DVector(4.0f, -6.0f, 8.0f);
    c.Point3 = C3DVector(-10.0f, 12.0f, 14.0f);
    c.Point4 = C3DVector(16.0f, 18.0f, -20.0f);
    bool ok = true;
    // The end points: only the Point1 term survives at t = 0, only the Point4 term at t = 1.
    if (!Near(c.GetPointOnCurve(0.0f), c.Point1)) ok = false;
    if (!Near(c.GetPointOnCurve(1.0f), c.Point4)) ok = false;
    // The midpoint: weights 1/8, 3/8, 3/8, 1/8.
    if (!Near(c.GetPointOnCurve(0.5f), C3DVector(-0.125f, 4.75f, 6.125f))) ok = false;
    // A quarter: weights 27/64, 27/64, 9/64, 1/64.
    if (!Near(c.GetPointOnCurve(0.25f), C3DVector(0.953125f, 0.28125f, 6.296875f))) ok = false;
    // The whole range against de Casteljau.
    for (int i = 0; i <= 16; ++i) {
        float t = (float)i / 16.0f;
        if (!Near(c.GetPointOnCurve(t), DeCasteljau(c, t))) ok = false;
    }
    // The four weights sum to one: a curve with four equal points is that point everywhere.
    CBezierCurve flat;
    flat.Point1 = flat.Point2 = flat.Point3 = flat.Point4 = C3DVector(7.0f, -3.0f, 0.5f);
    if (!Near(flat.GetPointOnCurve(0.3f), flat.Point1)) ok = false;
    if (!ok) { std::printf("009122a0_TEST FAIL\n"); return 1; }
    std::printf("009122a0_TEST PASS\n");
    return 0;
}