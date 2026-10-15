#include <cstdio>
// Behaviour test for 00b21160: Update is a no-op when offsets apply only at start-up; otherwise the current spline is
// kept as the previous one, optionally reset to the initial spline, and the positions are re-offset.
struct C3DVector { float X, Y, Z; };
static int g_assignCalls = 0;
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other) { ++g_assignCalls; m_begin = other.m_begin; m_end = other.m_end; m_capacity = other.m_capacity; return *this; }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
    CCardinalSpline& operator=(const CCardinalSpline& other)
    {
        ControlPoints = other.ControlPoints;
        Tension = other.Tension;
        EndPointsConnected = other.EndPointsConnected;
        return *this;
    }
};
static int g_offsetCalls = 0;
struct CSplineRandomOffsetData {
    CCardinalSpline*   InitialSpline;
    CCardinalSpline    ThisSpline;
    CCardinalSpline    PreviousSpline;
    bool               OffsetOnlyAtStartup;
    bool               ResetEachFrame;
    void OffsetSplinePositions() { ++g_offsetCalls; ThisSpline.Tension += 1.0f; }
    void Update();
};
void CSplineRandomOffsetData::Update()
{
    if (OffsetOnlyAtStartup)
        return;
    PreviousSpline = ThisSpline;
    if (ResetEachFrame)
        ThisSpline = *InitialSpline;
    OffsetSplinePositions();
}
static void Set(CCardinalSpline& s, C3DVector* p, float t, bool c)
{
    s.ControlPoints.m_begin = p; s.ControlPoints.m_end = p + 1; s.ControlPoints.m_capacity = p + 1; s.Tension = t; s.EndPointsConnected = c;
}
int main()
{
    C3DVector a[1], b[1], c[1];
    CCardinalSpline initial; Set(initial, a, 10.0f, true);
    CSplineRandomOffsetData d;
    d.InitialSpline = &initial; Set(d.ThisSpline, b, 2.0f, false); Set(d.PreviousSpline, c, 0.0f, false);
    d.OffsetOnlyAtStartup = true; d.ResetEachFrame = true;
    d.Update();
    if (g_offsetCalls != 0 || g_assignCalls != 0 || d.PreviousSpline.ControlPoints.m_begin != c) { std::printf("00b21160_TEST FAIL\n"); return 1; }
    d.OffsetOnlyAtStartup = false; d.ResetEachFrame = false;
    d.Update();
    if (g_offsetCalls != 1 || d.PreviousSpline.ControlPoints.m_begin != b || d.PreviousSpline.Tension != 2.0f || d.ThisSpline.Tension != 3.0f) { std::printf("00b21160_TEST FAIL\n"); return 1; }
    d.ResetEachFrame = true;
    d.Update();
    if (g_offsetCalls != 2 || d.PreviousSpline.Tension != 3.0f || d.ThisSpline.ControlPoints.m_begin != a || d.ThisSpline.Tension != 11.0f
        || !d.ThisSpline.EndPointsConnected || g_assignCalls != 3) { std::printf("00b21160_TEST FAIL\n"); return 1; }
    std::printf("00b21160_TEST PASS\n");
    return 0;
}