// Behaviour test for 00b21090: src/00b21090.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time) -- copy control point `index` of the current spline (or of last frame's spline) into `out`
// Leaf: GetPointAtIndex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21090, 197 bytes, `ret 0Ch` (the time argument is unused).
// Identity: Ego_r.pdb `?GetPointAtIndex@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@JM@Z`.
// Callee: CArray<C3DVector>::operator= @ 0x0075B8D0 (the inlined CCardinalSpline assignment).
// Shape: the sibling of the landed GetPointAtPos 0x00B20FD0 with a control point read instead of an evaluation:
//   an offset-at-startup spline reads ThisSpline directly; otherwise, when the previous frame's spline lost sync
//   with the current one (control-point counts differ, ThisSpline's count computed first), it is re-snapshotted
//   (array, Tension, EndPointsConnected) and the current point is returned; else last frame's point. The 12-byte
//   C3DVector copies are POD dword moves; the model is the native 20-byte CCardinalSpline of the landed siblings.
struct C3DVector { float X, Y, Z; };
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other);                 // 0x0075B8D0
    long Size() const { return (long)(m_end - m_begin); }
    T* begin_() const { return m_begin; }
    T& operator[](long i) { return *(begin_() + i); }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float Tension;
    bool EndPointsConnected;
    CCardinalSpline& operator=(const CCardinalSpline& other) {
        ControlPoints = other.ControlPoints; Tension = other.Tension; EndPointsConnected = other.EndPointsConnected; return *this;
    }
};
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline ThisSpline;
    CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup;
    bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    void GetPointAtIndex(C3DVector& out, long index, float time);
};
void CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time)
{
    if (OffsetOnlyAtStartup) { out = ThisSpline.ControlPoints[index]; return; }
    if (PreviousSpline.ControlPoints.Size() != ThisSpline.ControlPoints.Size()) {
        PreviousSpline = ThisSpline;
        out = ThisSpline.ControlPoints[index];
    }
    else
        out = PreviousSpline.ControlPoints[index];
}

// ---- stubs and driver ----
static int g_assign = 0;
template<> CArray<C3DVector>& CArray<C3DVector>::operator=(const CArray<C3DVector>& other) { ++g_assign; m_begin = other.m_begin; m_end = other.m_end; m_capacity = other.m_capacity; return *this; }
int main()
{
    C3DVector a[3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    C3DVector b[2] = { {10, 11, 12}, {13, 14, 15} };
    CSplineRandomOffsetData d; memset(&d, 0, sizeof(d));
    d.ThisSpline.ControlPoints.m_begin = a; d.ThisSpline.ControlPoints.m_end = a + 3; d.ThisSpline.Tension = 0.5f; d.ThisSpline.EndPointsConnected = true;
    d.PreviousSpline.ControlPoints.m_begin = b; d.PreviousSpline.ControlPoints.m_end = b + 2;
    C3DVector out = { 0, 0, 0 };
    d.OffsetOnlyAtStartup = true; d.GetPointAtIndex(out, 1, 0.0f); CHECK(out.X == 4 && out.Y == 5 && out.Z == 6); CHECK(g_assign == 0);
    d.OffsetOnlyAtStartup = false; d.GetPointAtIndex(out, 2, 0.0f);
    CHECK(out.X == 7 && out.Y == 8 && out.Z == 9); CHECK(g_assign == 1); CHECK(d.PreviousSpline.ControlPoints.m_begin == a); CHECK(d.PreviousSpline.Tension == 0.5f); CHECK(d.PreviousSpline.EndPointsConnected);
    d.PreviousSpline.ControlPoints.m_begin = b; d.PreviousSpline.ControlPoints.m_end = b + 3;
    d.GetPointAtIndex(out, 1, 0.0f); CHECK(out.X == 13 && out.Y == 14 && out.Z == 15); CHECK(g_assign == 1);
    if (fails == 0) printf("00b21090_TEST PASS\n"); else printf("00b21090_TEST FAIL\n");
    return fails;
}