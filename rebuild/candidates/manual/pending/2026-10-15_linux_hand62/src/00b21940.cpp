// CPSCSplineBase::GetPointAtPos(CParticleUpdateParameters*, C3DVector*, float, float) -- point on the spline at a parametric position (through the random-offset data when present), then transformed by the position parameters if any are bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21940, length 82 bytes.
// Callees: CSplineRandomOffsetData::GetPointAtPos @ 0x00B20FD0, CCardinalSpline::GetSplinePosition @ 0x00A567A0,
// CSplinePosParamData::TransformFromPos @ 0x00B21850 (manifest "CEngineScreenEffectColourFilter::Render", propagated;
// it sits between CSplinePosParamData::Update 0x00B20BA0 and TransformFromIndex 0x00B218B0).
// Layout: Ego_r.pdb CPSCSplineBase (SplineAnimSpeed +0xc, CCardinalSpline Spline +0x10,
// CCountedPointer<CSplineRandomOffsetData> +0x28, CCountedPointer<CSplinePosParamData> +0x30) with the
// retail 12-byte CArray inside CCardinalSpline (0x14 bytes): the two counted pointers ({Data, Info}) sit
// at +0x24 and +0x2c. CSplinePosParamData is the landed model of 0x00B20B10 (CArray SplinePosParam +0xc).
// Shape: the floats are forwarded as dwords from callee-saved registers; the bound-parameter test
// is `Data && SplinePosParam.Size() > 0` (unsigned element count, `jbe`).
struct CBase {
    virtual ~CBase();
};

namespace NParticleEngine {
struct CParticleSystem;
struct CParticleUpdateParameters;           // CParticleUpdateParameters<unsigned long, unsigned long>

struct C3DVector {
    float X, Y, Z;
};

template<class T> struct CArray {           // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
};

struct CCardinalSpline {                    // 0x14 bytes in retail
    CArray<C3DVector> ControlPoints;        // +0x00
    float             Tension;              // +0x0c
    bool              EndPointsConnected;   // +0x10
    void GetSplinePosition(C3DVector* out, float pos);  // 0x00A567A0
};

struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

struct CSplineRandomOffsetData {
    void OffsetSplinePositions();                                       // 0x00B206B0
    void GetPointAtPos(C3DVector* out, float pos, float time);          // 0x00B20FD0
    void GetPointAtIndex(C3DVector* out, long index, float time);       // 0x00B21090
};

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;    // +0x00
    CCardinalSpline*      Spline;                   // +0x04
    bool                  MaintainSplineShape;      // +0x08
    CArray<unsigned long> SplinePosParam;           // +0x0c
    void Update(CParticleUpdateParameters* params, CCardinalSpline* spline);                // 0x00B20BA0
    void TransformFromPos(CParticleUpdateParameters* params, C3DVector* point, float pos);  // 0x00B21850
    void TransformFromIndex(CParticleUpdateParameters* params, C3DVector* point, long index);   // 0x00B218B0
};

struct CPSCBase : CBase {                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

struct CPSCSplineBase : CPSCBase {
    float                                     SplineAnimSpeed;        // +0x0c
    CCardinalSpline                           Spline;                 // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData; // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;     // +0x2c
    bool Initialise(void* owner, unsigned long flags, CParticleUpdateParameters* params, float time);
    void GetPointAtPos(CParticleUpdateParameters* params, C3DVector* out, float pos, float time);
    void GetPointAtIndex(CParticleUpdateParameters* params, C3DVector* out, long index, float time);
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
}
