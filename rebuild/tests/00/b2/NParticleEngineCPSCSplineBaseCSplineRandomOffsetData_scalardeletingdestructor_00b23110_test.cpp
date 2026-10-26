// Behaviour test (0x00b23110): deleting a CSplineRandomOffsetData (scalar deleting destructor) frees its four arrays and the object
#include <cstdio>
#include <stdlib.h>
#include <new>
static int g_live = 0, g_objects = 0;
class C3DVector { public: float X, Y, Z; };
template <class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) { }
    void Make(unsigned int n) { ++g_live; m_begin = (T*)malloc(n * sizeof(T)); m_end = m_capacity = m_begin + n; }
    ~CArray() { if (m_begin) { --g_live; free(m_begin); } }
};
class CCardinalSpline { public: CArray<C3DVector> ControlPoints; float Tension; bool EndPointsConnected; };
class CSplineRandomOffsetData {
public:
    CCardinalSpline*  InitialSpline;
    CCardinalSpline   ThisSpline;
    CCardinalSpline   PreviousSpline;
    bool              OffsetOnlyAtStartup;
    bool              ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    static void* operator new(size_t n) { ++g_objects; return malloc(n); }
    static void operator delete(void* p) { --g_objects; free(p); }
};
int main()
{
    CSplineRandomOffsetData* d = new CSplineRandomOffsetData;
    d->InitialSpline = 0;
    d->ThisSpline.ControlPoints.Make(2);
    d->PreviousSpline.ControlPoints.Make(2);
    d->SplinePosOffsetLimits.Make(2);
    d->SplinePosOffsetSpeed.Make(2);
    if (g_live != 4 || g_objects != 1) { std::printf("00b23110_TEST FAIL\n"); return 1; }
    delete d;
    if (g_live != 0 || g_objects != 0) { std::printf("00b23110_TEST FAIL\n"); return 1; }
    std::printf("00b23110_TEST PASS\n");
    return 0;
}