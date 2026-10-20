// Behaviour test for 0x00B22530 CPSCSplineBase::DatabaseCopy: the spline is copied; random-offset / pos-param data are
// created on demand (bound to this spline) and get the source's settings, or are released when the source has none.
#include <cstdio>

struct CArray { int count; int tag; CArray() : count(0), tag(0) {} CArray& operator=(const CArray& o) { count = o.count; tag = o.tag; return *this; } };
struct CCardinalSpline { CArray ControlPoints; float Tension; bool EndPointsConnected; };
static int g_destroyed;
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline; bool OffsetOnlyAtStartup, ResetEachFrame; CArray Limits, Speed;
    CSplineRandomOffsetData(CCardinalSpline* s) : InitialSpline(s), OffsetOnlyAtStartup(false), ResetEachFrame(false) {}
    ~CSplineRandomOffsetData() { ++g_destroyed; }
    CSplineRandomOffsetData& operator=(const CSplineRandomOffsetData& o) { ResetEachFrame = o.ResetEachFrame; OffsetOnlyAtStartup = o.OffsetOnlyAtStartup; Limits = o.Limits; Speed = o.Speed; return *this; }
};
struct CSplinePosParamData {
    CCardinalSpline* Spline; bool MaintainSplineShape; CArray SplinePosParam;
    CSplinePosParamData(CCardinalSpline* s) : Spline(s), MaintainSplineShape(false) {}
    ~CSplinePosParamData() { ++g_destroyed; }
    CSplinePosParamData& operator=(const CSplinePosParamData& o) { MaintainSplineShape = o.MaintainSplineShape; SplinePosParam = o.SplinePosParam; return *this; }
};
template <class T> struct CPtr {           // counted pointer reduced to sole ownership for the test
    T* Data;
    CPtr() : Data(0) {}
    void Reset(T* p) { delete Data; Data = p; }
    void Clear() { delete Data; Data = 0; }
};
struct CPSCSplineBase {
    CCardinalSpline Spline; CPtr<CSplineRandomOffsetData> SplineRandomOffsetData; CPtr<CSplinePosParamData> SplinePosParamData;
    void DatabaseCopy(const CPSCSplineBase* source)
    {
        Spline = source->Spline;
        if (source->SplineRandomOffsetData.Data) {
            if (!SplineRandomOffsetData.Data) SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
            *SplineRandomOffsetData.Data = *source->SplineRandomOffsetData.Data;
        } else SplineRandomOffsetData.Clear();
        if (source->SplinePosParamData.Data) {
            if (!SplinePosParamData.Data) SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
            *SplinePosParamData.Data = *source->SplinePosParamData.Data;
        } else SplinePosParamData.Clear();
    }
};
static int Fail(int n) { std::printf("00b22530_TEST FAIL %d\n", n); return 1; }
int main()
{
    CPSCSplineBase src, dst;
    src.Spline.ControlPoints.count = 4; src.Spline.Tension = 0.25f; src.Spline.EndPointsConnected = true;
    src.SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&src.Spline));
    src.SplineRandomOffsetData.Data->ResetEachFrame = true; src.SplineRandomOffsetData.Data->Limits.count = 4; src.SplineRandomOffsetData.Data->Speed.tag = 9;
    src.SplinePosParamData.Reset(new CSplinePosParamData(&src.Spline));
    src.SplinePosParamData.Data->MaintainSplineShape = true; src.SplinePosParamData.Data->SplinePosParam.count = 3;
    dst.DatabaseCopy(&src);
    if (dst.Spline.ControlPoints.count != 4 || dst.Spline.Tension != 0.25f || !dst.Spline.EndPointsConnected) return Fail(1);
    if (!dst.SplineRandomOffsetData.Data || dst.SplineRandomOffsetData.Data->InitialSpline != &dst.Spline || !dst.SplineRandomOffsetData.Data->ResetEachFrame
        || dst.SplineRandomOffsetData.Data->Limits.count != 4 || dst.SplineRandomOffsetData.Data->Speed.tag != 9) return Fail(2);
    if (!dst.SplinePosParamData.Data || dst.SplinePosParamData.Data->Spline != &dst.Spline || !dst.SplinePosParamData.Data->MaintainSplineShape || dst.SplinePosParamData.Data->SplinePosParam.count != 3) return Fail(3);
    CSplineRandomOffsetData* kept = dst.SplineRandomOffsetData.Data;
    dst.DatabaseCopy(&src);                                   // existing data reused
    if (dst.SplineRandomOffsetData.Data != kept || g_destroyed != 0) return Fail(4);
    src.SplineRandomOffsetData.Clear(); src.SplinePosParamData.Clear();
    dst.DatabaseCopy(&src);                                   // source has none: released
    if (dst.SplineRandomOffsetData.Data || dst.SplinePosParamData.Data || g_destroyed != 4) return Fail(5);
    std::printf("00b22530_TEST PASS\n");
    return 0;
}