#include <cstdio>

// CPSCSplineBase::Initialise: on the first call only, re-offset the random spline (if any), hand the
// particle system to the position-parameter data and update it from the spline (if any), set
// Initialised; every call returns Initialised.
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

bool CPSCSplineBase::Initialise(void* owner, unsigned long flags, CParticleUpdateParameters* params, float time)
{
    if (!Initialised) {
        if (SplineRandomOffsetData.Data)
            SplineRandomOffsetData->OffsetSplinePositions();
        CSplinePosParamData* posParamData = SplinePosParamData.Data;
        if (posParamData) {
            posParamData->PParentParticleSystem = ParticleSystem;
            posParamData->Update(params, &Spline);
        }
        Initialised = true;
    }
    return Initialised;
}

int main()
{
    CParticleSystem ps; CParticleUpdateParameters params;
    CSplineRandomOffsetData offs; offs.m_offsetCalls = 0; offs.m_posCalls = 0;
    CSplinePosParamData pp; pp.PParentParticleSystem = 0; pp.m_updateCalls = 0; pp.m_transformCalls = 0; pp.m_lastParams = 0; pp.m_lastSpline = 0;
    CPSCSplineBase b;
    b.ParticleSystem = &ps; b.UpdateStarted = false; b.Initialised = false;
    b.SplineRandomOffsetData.Data = &offs; b.SplineRandomOffsetData.Info = 0;
    b.SplinePosParamData.Data = &pp; b.SplinePosParamData.Info = 0;
    bool r1 = b.Initialise(0, 0, &params, 0.0f);
    bool r2 = b.Initialise(0, 0, &params, 0.0f);
    bool ok = r1 && r2 && offs.m_offsetCalls == 1 && pp.m_updateCalls == 1 && pp.PParentParticleSystem == &ps
        && pp.m_lastParams == &params && pp.m_lastSpline == &b.Spline && b.Initialised;
    CPSCSplineBase e;                                   // no data objects: just marks itself initialised
    e.ParticleSystem = &ps; e.UpdateStarted = false; e.Initialised = false;
    e.SplineRandomOffsetData.Data = 0; e.SplinePosParamData.Data = 0;
    ok = ok && e.Initialise(0, 0, &params, 0.0f) && e.Initialised && !e.UpdateStarted;
    if (!ok) {
        std::printf("00b215a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b215a0_TEST PASS\n");
    return 0;
}
