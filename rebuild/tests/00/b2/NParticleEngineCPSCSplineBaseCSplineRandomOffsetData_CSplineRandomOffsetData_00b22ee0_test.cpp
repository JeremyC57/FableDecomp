// Behaviour test (0x00b22ee0): ~CSplineRandomOffsetData frees the two offset arrays and both splines' control points (the initial spline is not owned)
#include <cstdio>
#include <stdlib.h>
static int g_live = 0;
struct C3DVector { float X, Y, Z; };
template <class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) { }
    void Make(unsigned int n) { ++g_live; m_begin = (T*)malloc(n * sizeof(T)); m_end = m_capacity = m_begin + n; }
    ~CArray() { if (m_begin) { --g_live; free(m_begin); } }
};
struct CCardinalSpline { CArray<C3DVector> ControlPoints; float Tension; bool EndPointsConnected; };
class CSplineRandomOffsetData {
public:
    ~CSplineRandomOffsetData() { }
    CCardinalSpline*  InitialSpline;
    CCardinalSpline   ThisSpline;
    CCardinalSpline   PreviousSpline;
    bool              OffsetOnlyAtStartup;
    bool              ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
};
int main()
{
    CCardinalSpline initial; initial.ControlPoints.Make(3);
    {
        CSplineRandomOffsetData d;
        d.InitialSpline = &initial;
        d.ThisSpline.ControlPoints.Make(3);
        d.PreviousSpline.ControlPoints.Make(3);
        d.SplinePosOffsetLimits.Make(3);
        if (g_live != 4) { std::printf("00b22ee0_TEST FAIL\n"); return 1; }
    }
    if (g_live != 1 || initial.ControlPoints.m_begin == 0) { std::printf("00b22ee0_TEST FAIL\n"); return 1; }
    std::printf("00b22ee0_TEST PASS\n");
    return 0;
}