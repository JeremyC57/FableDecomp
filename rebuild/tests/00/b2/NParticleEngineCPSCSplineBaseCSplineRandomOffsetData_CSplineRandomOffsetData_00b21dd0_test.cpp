// Behaviour test for CSplineRandomOffsetData::CSplineRandomOffsetData(CCardinalSpline*) (0x00B21DD0).
#include <cstdio>
#include <cstdlib>
#include <new>
struct C3DVector { float X, Y, Z; C3DVector() {} };
template <class T> struct SRetailAllocProxy { T* _M_data; SRetailAllocProxy(T* data) : _M_data(data) {} };
static int g_assigns = 0;
template <class T> struct CArray {
    T* _M_start; T* _M_finish; SRetailAllocProxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CArray(const CArray& other) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { *this = other; }
    CArray& operator=(const CArray& other)
    {
        ++g_assigns; if (_M_start != 0 || _M_finish != 0) std::abort();   /* the copy ctor zeroes first */
        unsigned n = (unsigned)(other._M_finish - other._M_start);
        _M_start = (T*)std::malloc(sizeof(T) * (n + 1)); _M_finish = _M_start + n; _M_end_of_storage._M_data = _M_finish;
        for (unsigned i = 0; i < n; ++i) _M_start[i] = other._M_start[i];
        return *this;
    }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector())
    {
        (void)value; _M_start = (T*)std::malloc(sizeof(T) * (newSize + 1)); _M_finish = _M_start + newSize; _M_end_of_storage._M_data = _M_finish;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints; float Tension; bool EndPointsConnected;
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) {}
};
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline; CCardinalSpline ThisSpline; CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup; bool ResetEachFrame; CArray<C3DVector> SplinePosOffsetLimits; CArray<C3DVector> SplinePosOffsetSpeed;
    CSplineRandomOffsetData(CCardinalSpline* initialSpline);
};
CSplineRandomOffsetData::CSplineRandomOffsetData(CCardinalSpline* initialSpline)
    : InitialSpline(initialSpline), ThisSpline(*initialSpline), OffsetOnlyAtStartup(false), ResetEachFrame(false)
{
    SplinePosOffsetLimits.resize(InitialSpline->ControlPoints.size());
    SplinePosOffsetSpeed.resize(SplinePosOffsetLimits.size());
}
int main()
{
    CCardinalSpline init; init.Tension = 0.25f; init.EndPointsConnected = true;
    C3DVector pts[5]; for (int i = 0; i < 5; ++i) { pts[i].X = (float)i; pts[i].Y = 1; pts[i].Z = 2; }
    init.ControlPoints._M_start = pts; init.ControlPoints._M_finish = pts + 5;
    unsigned char raw[sizeof(CSplineRandomOffsetData)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    CSplineRandomOffsetData* d = new (raw) CSplineRandomOffsetData(&init);
    if (d->InitialSpline != &init || g_assigns != 1 || d->ThisSpline.ControlPoints.size() != 5 || d->ThisSpline.ControlPoints._M_start[3].X != 3.0f
        || d->ThisSpline.Tension != 0.25f || !d->ThisSpline.EndPointsConnected) { std::printf("00b21dd0_TEST FAIL\n"); return 1; }
    if (d->PreviousSpline.ControlPoints._M_start != 0 || d->PreviousSpline.ControlPoints._M_finish != 0 || d->PreviousSpline.ControlPoints._M_end_of_storage._M_data != 0
        || d->PreviousSpline.Tension != 0.5f || d->PreviousSpline.EndPointsConnected || d->OffsetOnlyAtStartup || d->ResetEachFrame) { std::printf("00b21dd0_TEST FAIL\n"); return 1; }
    if (d->SplinePosOffsetLimits.size() != 5 || d->SplinePosOffsetSpeed.size() != 5) { std::printf("00b21dd0_TEST FAIL\n"); return 1; }
    std::printf("00b21dd0_TEST PASS\n");
    return 0;
}