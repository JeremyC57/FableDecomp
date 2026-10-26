// Behaviour test for NParticleEngine::CPSCSplineBase::InterpolateTo (0x00B22310): tension is lerped, the control points are
// interpolated from copies of both arrays, and each optional data block is interpolated when both sides have it; otherwise,
// only past t = 0.5, the own block is released (random offset: only when the other has none; position parameters: likewise)
// or a copy of the other's block is created bound to this spline.
#include <cstdio>
#include <vector>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
typedef std::vector<C3DVector> CPointArray;
static CPointArray GFInterpolateArray(const CPointArray& a, const CPointArray& b, float t, const C3DVector& def)
{
    CPointArray r;
    for (unsigned int i = 0; i < a.size() || i < b.size(); ++i) {
        C3DVector p = i < a.size() ? a[i] : def, q = i < b.size() ? b[i] : def;
        r.push_back(C3DVector(p.X + (q.X - p.X) * t, p.Y + (q.Y - p.Y) * t, p.Z + (q.Z - p.Z) * t));
    }
    return r;
}
class CCardinalSpline { public:
    CPointArray ControlPoints;
    float       Tension;
    CPointArray GetControlPoints() const { return ControlPoints; }
};
static int g_live = 0;
template <class T> class CCountedPointer { public:             // single-owner stand-in for the counted pointer
    T* Data;
    CCountedPointer() : Data(0) {}
    ~CCountedPointer() { Clear(); }
    T* operator->() const { return Data; }
    void Reset(T* object) { Clear(); Data = object; }
    void Clear() { if (Data) { delete Data; --g_live; } Data = 0; }
};
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    float Amount;
    int Interpolations;
    CSplineRandomOffsetData(CCardinalSpline* s) : InitialSpline(s), Amount(0), Interpolations(0) { ++g_live; }
    void InterpolateTo(const CSplineRandomOffsetData& other, float t) { Amount += (other.Amount - Amount) * t; ++Interpolations; }
    CSplineRandomOffsetData& operator=(const CSplineRandomOffsetData& o) { Amount = o.Amount; return *this; }
};
struct CSplinePosParamData {
    CCardinalSpline* Spline;
    bool MaintainSplineShape;
    int Interpolations;
    CSplinePosParamData(CCardinalSpline* s) : Spline(s), MaintainSplineShape(false), Interpolations(0) { ++g_live; }
    void InterpolateTo(const CSplinePosParamData&, float) { ++Interpolations; }
    CSplinePosParamData& operator=(const CSplinePosParamData& o) { MaintainSplineShape = o.MaintainSplineShape; return *this; }
};
struct CPSCBase { virtual ~CPSCBase() {} };
struct CPSCSplineBase : CPSCBase {
    CCardinalSpline                          Spline;
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;
    void InterpolateTo(const CPSCBase* other, float t);
};

void CPSCSplineBase::InterpolateTo(const CPSCBase* other, float t)
{
    const CPSCSplineBase* source = (const CPSCSplineBase*)other;
    Spline.Tension = (1.0f - t) * Spline.Tension + t * source->Spline.Tension;
    Spline.ControlPoints = GFInterpolateArray(Spline.GetControlPoints(), source->Spline.GetControlPoints(), t, C3DVector(0.0f, 0.0f, 0.0f));

    if (SplineRandomOffsetData.Data && source->SplineRandomOffsetData.Data)
        SplineRandomOffsetData->InterpolateTo(*source->SplineRandomOffsetData.Data, t);
    else if (SplineRandomOffsetData.Data) {
        if (t > 0.5f)
            SplineRandomOffsetData.Clear();
    } else if (source->SplineRandomOffsetData.Data) {
        if (t > 0.5f) {
            SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
            *SplineRandomOffsetData.Data = *source->SplineRandomOffsetData.Data;
        }
    }

    if (SplinePosParamData.Data && source->SplinePosParamData.Data)
        SplinePosParamData->InterpolateTo(*source->SplinePosParamData.Data, t);
    else if (t > 0.5f) {
        if (SplinePosParamData.Data)
            SplinePosParamData.Clear();
        else if (source->SplinePosParamData.Data) {
            SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
            *SplinePosParamData.Data = *source->SplinePosParamData.Data;
        }
    }
}

int main()
{
    int ok = 1;
    CPSCSplineBase a, b;
    a.Spline.Tension = 0.0f;
    b.Spline.Tension = 1.0f;
    a.Spline.ControlPoints.push_back(C3DVector(0, 0, 0));
    b.Spline.ControlPoints.push_back(C3DVector(4, 8, 12));
    b.Spline.ControlPoints.push_back(C3DVector(4, 4, 4));

    // t = 0.25: lerped tension and points; b's data blocks are not copied below the halfway point.
    b.SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&b.Spline));
    b.SplineRandomOffsetData->Amount = 2.0f;
    b.SplinePosParamData.Reset(new CSplinePosParamData(&b.Spline));
    b.SplinePosParamData->MaintainSplineShape = true;
    a.InterpolateTo(&b, 0.25f);
    ok &= a.Spline.Tension == 0.25f && a.Spline.ControlPoints.size() == 2;
    ok &= a.Spline.ControlPoints[0].X == 1 && a.Spline.ControlPoints[0].Y == 2 && a.Spline.ControlPoints[0].Z == 3;
    ok &= a.Spline.ControlPoints[1].X == 1;
    ok &= a.SplineRandomOffsetData.Data == 0 && a.SplinePosParamData.Data == 0;

    // t = 0.75: both blocks are created as copies bound to a's spline.
    a.InterpolateTo(&b, 0.75f);
    ok &= a.SplineRandomOffsetData.Data && a.SplineRandomOffsetData->Amount == 2.0f && a.SplineRandomOffsetData->InitialSpline == &a.Spline;
    ok &= a.SplinePosParamData.Data && a.SplinePosParamData->MaintainSplineShape && a.SplinePosParamData->Spline == &a.Spline;

    // Both present: interpolated, not replaced.
    b.SplineRandomOffsetData->Amount = 4.0f;
    a.InterpolateTo(&b, 0.5f);
    ok &= a.SplineRandomOffsetData->Amount == 3.0f && a.SplineRandomOffsetData->Interpolations == 1;
    ok &= a.SplinePosParamData->Interpolations == 1;

    // Other has none: kept at t <= 0.5, released past it.
    b.SplineRandomOffsetData.Clear();
    b.SplinePosParamData.Clear();
    a.InterpolateTo(&b, 0.5f);
    ok &= a.SplineRandomOffsetData.Data != 0 && a.SplinePosParamData.Data != 0;
    a.InterpolateTo(&b, 0.6f);
    ok &= a.SplineRandomOffsetData.Data == 0 && a.SplinePosParamData.Data == 0 && g_live == 0;

    printf("00b22310_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}