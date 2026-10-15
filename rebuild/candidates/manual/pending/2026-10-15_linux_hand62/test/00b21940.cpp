#include <cstdio>

// CPSCSplineBase::GetPointAtPos: the random-offset data supplies the point when present, else the
// cardinal spline; the position parameters transform it only when they exist AND hold parameters.
struct CParticleSystem { int m_id; };
struct CParticleUpdateParameters { int m_id; };
struct C3DVector { float X, Y, Z; };
template<class T> struct CArray {
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
};
static int g_order = 0;
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
    int               m_positionCalls;
    void GetSplinePosition(C3DVector* out, float pos) { ++m_positionCalls; out->X = pos; out->Y = 0.0f; out->Z = 0.0f; }
};
template<class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CSplineRandomOffsetData {
    int m_offsetCalls, m_posCalls;
    void OffsetSplinePositions() { ++m_offsetCalls; }
    void GetPointAtPos(C3DVector* out, float pos, float time) { ++m_posCalls; out->X = pos; out->Y = time; out->Z = 1.0f; }
};
struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;
    CCardinalSpline*      Spline;
    bool                  MaintainSplineShape;
    CArray<unsigned long> SplinePosParam;
    int                   m_updateCalls, m_transformCalls;
    CParticleUpdateParameters* m_lastParams;
    CCardinalSpline*      m_lastSpline;
    void Update(CParticleUpdateParameters* params, CCardinalSpline* spline) { ++m_updateCalls; m_lastParams = params; m_lastSpline = spline; }
    void TransformFromPos(CParticleUpdateParameters* params, C3DVector* point, float pos) { ++m_transformCalls; m_lastParams = params; point->Y += 100.0f; }
};
struct CPSCSplineBase {
    CParticleSystem*                         ParticleSystem;
    bool                                     UpdateStarted : 1;
    bool                                     Initialised : 1;
    float                                    SplineAnimSpeed;
    CCardinalSpline                          Spline;
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;
    bool Initialise(void* owner, unsigned long flags, CParticleUpdateParameters* params, float time);
    void GetPointAtPos(CParticleUpdateParameters* params, C3DVector* out, float pos, float time);
};

void CPSCSplineBase::GetPointAtPos(CParticleUpdateParameters* params, C3DVector* out, float pos, float time)
{
    if (SplineRandomOffsetData.Data)
        SplineRandomOffsetData->GetPointAtPos(out, pos, time);
    else
        Spline.GetSplinePosition(out, pos);
    if (SplinePosParamData.Data && SplinePosParamData->SplinePosParam.Size() > 0)
        SplinePosParamData->TransformFromPos(params, out, pos);
}

int main()
{
    CParticleUpdateParameters params;
    unsigned long ids[2] = { 1, 2 };
    CSplineRandomOffsetData offs; offs.m_offsetCalls = 0; offs.m_posCalls = 0;
    CSplinePosParamData pp; pp.m_updateCalls = 0; pp.m_transformCalls = 0; pp.m_lastParams = 0;
    pp.SplinePosParam.m_pData = ids; pp.SplinePosParam.m_pLast = ids; pp.SplinePosParam.m_pEnd = ids + 2;   // empty
    CPSCSplineBase b;
    b.Spline.m_positionCalls = 0;
    b.SplineRandomOffsetData.Data = 0; b.SplinePosParamData.Data = &pp;
    C3DVector v;
    b.GetPointAtPos(&params, &v, 0.5f, 2.0f);           // spline, no transform (no parameters)
    bool ok = b.Spline.m_positionCalls == 1 && offs.m_posCalls == 0 && pp.m_transformCalls == 0 && v.X == 0.5f && v.Y == 0.0f;
    pp.SplinePosParam.m_pLast = ids + 2;
    b.SplineRandomOffsetData.Data = &offs;
    b.GetPointAtPos(&params, &v, 0.75f, 2.0f);          // random offsets, then transformed
    ok = ok && offs.m_posCalls == 1 && b.Spline.m_positionCalls == 1 && pp.m_transformCalls == 1
        && pp.m_lastParams == &params && v.X == 0.75f && v.Y == 102.0f;
    b.SplinePosParamData.Data = 0;
    b.GetPointAtPos(&params, &v, 0.25f, 1.0f);          // no position data at all
    ok = ok && offs.m_posCalls == 2 && pp.m_transformCalls == 1 && v.Y == 1.0f;
    if (!ok) {
        std::printf("00b21940_TEST FAIL\n");
        return 1;
    }
    std::printf("00b21940_TEST PASS\n");
    return 0;
}
