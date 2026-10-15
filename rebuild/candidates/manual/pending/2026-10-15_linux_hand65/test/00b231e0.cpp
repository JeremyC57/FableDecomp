#include <cstdio>
#include <cstdlib>
#include <new>
// DeleteSplineRandomOffsetData: null-safe; destroys the four C3DVector arrays (reverse order) and frees the object
static long g_frees; static void* g_freed[8]; static long g_deletes;
struct C3DVector { float X, Y, Z; };
template <class T> struct CArray {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CArray() { if (m_pFirst) { g_freed[g_frees++] = m_pFirst; std::free(m_pFirst); } }
};
struct CCardinalSpline { CArray<C3DVector> ControlPoints; float Tension; bool EndPointsConnected; };
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline; CCardinalSpline ThisSpline; CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup; bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits; CArray<C3DVector> SplinePosOffsetSpeed;
};
static void DeleteSplineRandomOffsetData(CSplineRandomOffsetData* data)
{
    if (data) {
        data->~CSplineRandomOffsetData();
        ::operator delete(data);
        ++g_deletes;
    }
}
int main()
{
    DeleteSplineRandomOffsetData(0);
    void* mem = ::operator new(sizeof(CSplineRandomOffsetData));
    CSplineRandomOffsetData* d = new (mem) CSplineRandomOffsetData;
    void* a = std::malloc(12); void* b = std::malloc(12); void* c = std::malloc(12); void* e = std::malloc(12);
    d->ThisSpline.ControlPoints.m_pFirst = (C3DVector*)a; d->PreviousSpline.ControlPoints.m_pFirst = (C3DVector*)b;
    d->SplinePosOffsetLimits.m_pFirst = (C3DVector*)c; d->SplinePosOffsetSpeed.m_pFirst = (C3DVector*)e;
    DeleteSplineRandomOffsetData(d);
    if (g_deletes != 1 || g_frees != 4 || g_freed[0] != e || g_freed[1] != c || g_freed[2] != b || g_freed[3] != a) { std::printf("00b231e0_TEST FAIL\n"); return 1; }
    std::printf("00b231e0_TEST PASS\n");
    return 0;
}
