// CPSCSplineBase::Initialise(void*, unsigned long, CParticleUpdateParameters*, float) -- once: re-offset the random spline, hand the particle system to the position-parameter data and update it from the spline; returns the Initialised bit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B215A0, length 62 bytes.
// Callees: CSplineRandomOffsetData::OffsetSplinePositions @ 0x00B206B0, CSplinePosParamData::Update @ 0x00B20BA0.
// Identity: slot 28 of NParticleEngine::CPSCSpline's vtable 0x012A0934 (and of the sibling vtable at
// 0x012A0B84); the CPSCBase default for that slot (0x00AF2AB0) just sets Initialised and returns it,
// with the same four arguments (`ret 10h`). Only the third (the update parameters) is used here.
// Layout: Ego_r.pdb CPSCSplineBase (SplineAnimSpeed +0xc, CCardinalSpline Spline +0x10,
// CCountedPointer<CSplineRandomOffsetData> +0x28, CCountedPointer<CSplinePosParamData> +0x30) with the
// retail 12-byte CArray inside CCardinalSpline (0x14 bytes): the two counted pointers ({Data, Info}) sit
// at +0x24 and +0x2c. CSplinePosParamData is the landed model of 0x00B20B10 (CArray SplinePosParam +0xc).
// Shape: the position-parameter data pointer is a named local (it stays in ecx across the member
// store and becomes `this` of Update; reading the counted pointer twice re-loads it after the store);
// the bit-field result is `mov al ; shr al,1 ; and al,1`.
struct CBase {
    virtual ~CBase();
};

namespace NParticleEngine {
class CParticleSystem;
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

class CPSCBase : public CBase { public:                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

class CPSCSplineBase : public CPSCBase { public:
    float                                     SplineAnimSpeed;        // +0x0c
    CCardinalSpline                           Spline;                 // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData; // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;     // +0x2c
    bool Initialise(void* owner, unsigned long flags, CParticleUpdateParameters* params, float time);
    void GetPointAtPos(CParticleUpdateParameters* params, C3DVector* out, float pos, float time);
    void GetPointAtIndex(CParticleUpdateParameters* params, C3DVector* out, long index, float time);
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
}