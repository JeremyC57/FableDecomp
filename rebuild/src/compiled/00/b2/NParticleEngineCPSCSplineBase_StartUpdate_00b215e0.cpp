// NParticleEngine::CPSCSplineBase::StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>& params) -- vtable slot 8: marks the component's update started, advances the random spline offsets and re-fits the position parameters to the spline (and to the offset copy when there is one)
// Leaf: ?StartUpdate@CPSCSplineBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B215E0, length 66 bytes, `ret 4`.
// Signature from Ego_r.pdb: ?StartUpdate@CPSCSplineBase@NParticleEngine@@UAEXABV?$CParticleUpdateParameters@KK@2@@Z;
//   slot 8 of the vtables @ 0x0129FB84 and 0x012A0B14 (absolute refs 0x0129FBA4 / 0x012A0B34) and called directly by the
//   landed CPSCSpline::StartUpdate @ 0x00B1B0C0 (which names it "UpdateSplineData").
// Callees: CSplineRandomOffsetData::Update() @ 0x00B21160, CSplinePosParamData::Update(const CParticleUpdateParameters&,
//   CCardinalSpline&) @ 0x00B20BA0 (twice).
// Layout (Ego_r.pdb, as the landed CPSCSpline relatives): CPSCBase vptr +0, ParticleSystem +4, UpdateStarted bit 0 of +8;
//   CPSCSplineBase SplineAnimSpeed +0x0C, CCardinalSpline Spline +0x10 (retail 0x14 bytes: a 12-byte control-point vector,
//   Tension, EndPointsConnected), CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData +0x24 (object word),
//   CCountedPointer<CSplinePosParamData> SplinePosParamData +0x2C; CSplineRandomOffsetData::ThisSpline +4.
// Shape: `or byte ptr [esi+8],1` is the one-bit UpdateStarted store; the counted pointers are tested through their object
//   word and dereferenced with an inline operator->; params is loaded once into edi for both Update calls, the second
//   passing `&SplineRandomOffsetData->ThisSpline` (`add eax,4`).
struct C3DVector { float X, Y, Z; };

class CCardinalSpline {
public:
    C3DVector* m_controlFirst;              // +0x00 ControlPoints (retail STLport vector, 12 bytes)
    C3DVector* m_controlLast;               // +0x04
    C3DVector* m_controlEnd;                // +0x08
    float      Tension;                     // +0x0C
    bool       EndPointsConnected;          // +0x10
};

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParameters;
class CParticleSystem;

template <class T> class CCountedPointer {
public:
    T*    Data;                             // +0x00
    void* Info;                             // +0x04 count block
    T* operator->() const { return Data; }
    operator bool() const { return Data != 0; }
};

class CPSCBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params);   // slot 8 (+0x20)
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

class CPSCSplineBase : public CPSCBase {
public:
    class CSplineRandomOffsetData {
    public:
        CCardinalSpline* InitialSpline;     // +0x00
        CCardinalSpline  ThisSpline;        // +0x04
        void Update();                      // 0x00B21160
        CCardinalSpline& GetThisSpline() { return ThisSpline; }
    };
    class CSplinePosParamData {
    public:
        void Update(const CParticleUpdateParameters<unsigned long, unsigned long>& params, CCardinalSpline& spline);   // 0x00B20BA0
    };

    float                                    SplineAnimSpeed;          // +0x0C
    CCardinalSpline                          Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;       // +0x2C

    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};

void CPSCSplineBase::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    UpdateStarted = true;
    if (SplineRandomOffsetData)
        SplineRandomOffsetData->Update();
    if (SplinePosParamData) {
        SplinePosParamData->Update(params, Spline);
        if (SplineRandomOffsetData)
            SplinePosParamData->Update(params, SplineRandomOffsetData->GetThisSpline());
    }
}

}   // namespace NParticleEngine