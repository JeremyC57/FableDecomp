#include <cstdio>

// CPSCSpline::WriteBinary: bounce flag, speed-scaling flag, animation speed, then the base data.
static int g_order = 0;
static int g_boolOrder[2];
static bool g_bools[2];
static int g_boolCount = 0;
static float g_float = 0.0f;
static int g_floatOrder = 0, g_baseOrder = 0;
struct CDataOutputStream {
    void WriteBool(bool value) { g_bools[g_boolCount] = value; g_boolOrder[g_boolCount++] = ++g_order; }
    void WriteFloat(float value) { g_float = value; g_floatOrder = ++g_order; }
};
static CDataOutputStream* g_baseStream = 0;
struct CPSCSplineBase {
    void WriteBinary(CDataOutputStream* out) { g_baseStream = out; g_baseOrder = ++g_order; }
};
struct CPSCSpline : CPSCSplineBase {
    float SplineCurrentPosition;
    bool  ReverseSpline;
    bool  SplineBounce;
    bool  ScaleSplineSpeed;
    float SplineAnimSpeed;
    void WriteBinary(CDataOutputStream* out);
};
void CPSCSpline::WriteBinary(CDataOutputStream* out)
{
    out->WriteBool(SplineBounce);
    out->WriteBool(ScaleSplineSpeed);
    out->WriteFloat(SplineAnimSpeed);
    CPSCSplineBase::WriteBinary(out);
}

int main()
{
    CPSCSpline s;
    s.SplineCurrentPosition = 0.25f; s.ReverseSpline = true; s.SplineBounce = true; s.ScaleSplineSpeed = false; s.SplineAnimSpeed = 3.5f;
    CDataOutputStream out;
    s.WriteBinary(&out);
    if (g_boolCount != 2 || g_bools[0] != true || g_bools[1] != false || g_float != 3.5f || g_baseStream != &out
        || g_boolOrder[0] != 1 || g_boolOrder[1] != 2 || g_floatOrder != 3 || g_baseOrder != 4) {
        std::printf("00b1aff0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b1aff0_TEST PASS\n");
    return 0;
}
