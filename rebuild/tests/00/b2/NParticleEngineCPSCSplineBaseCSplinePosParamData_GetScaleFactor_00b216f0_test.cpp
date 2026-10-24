#include <cstdio>
#include <math.h>

// Model of CSplinePosParamData::GetScaleFactor (retail 0x00B216F0): |c - a| / |d - b| of the bracketing control points.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CParams { int id; };

static bool g_found = true;
struct CSplinePosParamData {
    bool MaintainSplineShape;
    bool GetClosest(const CParams& params, float pos, C3DVector& a, C3DVector& b, C3DVector& c, C3DVector& d)
    {
        (void)params; (void)pos;
        a = C3DVector(0, 0, 0); b = C3DVector(1, 0, 0);       // original segment points
        c = C3DVector(0, 6, 8); d = C3DVector(1, 3, 4);       // current segment points
        return g_found;
    }
    float GetScaleFactor(const CParams& params, float pos)
    {
        if (MaintainSplineShape) {
            C3DVector a, b, c, d;
            if (GetClosest(params, pos, a, b, c, d)) {
                C3DVector e1 = d - b;
                C3DVector e2 = c - a;
                return e2.GetLength() / e1.GetLength();
            }
        }
        return 1.0f;
    }
};

static bool Near(float a, float b) { float d = a - b; return d > -1e-4f && d < 1e-4f; }

int main()
{
    CParams params; params.id = 0;
    CSplinePosParamData data; data.MaintainSplineShape = false;
    if (!Near(data.GetScaleFactor(params, 0.5f), 1.0f)) { std::printf("00b216f0_TEST FAIL\n"); return 1; }
    data.MaintainSplineShape = true;
    if (!Near(data.GetScaleFactor(params, 0.5f), 2.0f)) { std::printf("00b216f0_TEST FAIL\n"); return 1; }   // |(0,6,8)| / |(0,3,4)| = 10/5
    g_found = false;
    if (!Near(data.GetScaleFactor(params, 0.5f), 1.0f)) { std::printf("00b216f0_TEST FAIL\n"); return 1; }
    std::printf("00b216f0_TEST PASS\n");
    return 0;
}