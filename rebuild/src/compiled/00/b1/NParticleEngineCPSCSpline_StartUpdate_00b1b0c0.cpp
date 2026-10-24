// NParticleEngine::CPSCSpline::StartUpdate(const CParticleUpdateParameters& params) -- vtable slot 8: per-frame spline set-up: refresh the spline data, mark the update started and advance the current spline position (bouncing between 0 and 1, or wrapping/clamping past the end)
// Leaf: ?StartUpdate@CPSCSpline@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B0C0, 232 bytes (+8 int3 padding).
// Name: FableWin.pdb ?StartUpdate@CPSCSpline@NParticleEngine@@UAEXABV?$CParticleUpdateParameters@KK@2@@Z; slot 8 of the
// vtable 0x012A0934 (rtti-vtables.tsv; slot 8 of CPSCOrbit is the landed StartUpdate 0x00B16D50; the landed
// CPSCSpline::GetUpdateParam 0x00B1B030 calls this slot when UpdateStarted is clear).
// Callees: CPSCSplineBase::UpdateSplineData(const CParticleUpdateParameters&) @ 0x00B215E0, the particle frame time
// getter @ 0x00AEAA00 (as in GetUpdateParam), CSplinePosParamData::CalcDistanceAlpha(params, float) @ 0x00B216F0.
// Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC (compiler literals).
// Layout: as the landed GetUpdateParam: UpdateStarted bit 0 of +8; the control points vector +0x10/+0x14
// (12-byte C3DVector, the `imul 2AAAAAABh ; sar edx,1` divide by 12); Spline.EndPointsConnected +0x20;
// SplinePosParamData (counted pointer, object word) +0x2C; SplineCurrentPosition +0x34, ReverseSpline +0x38,
// SplineBounce +0x39, ScaleSplineSpeed +0x3A, SplineAnimSpeed +0x3C.
// Shape: the step `frameTime * (speed / count)` (the count converted through the dead params slot, the
// step kept there by `fst`); the optional distance scaling is `alpha * step`; the bounce arm keeps the
// new position on the FPU and flips ReverseSpline (`sete` on the cached flag) at either end; the
// wrap arm stores the position through a float local and wraps by 1 (or clamps to 0) past 1. The
// SplinePosParamData call goes through the counted pointer's inline `operator->`: a plain pointer member
// loads the position argument into ecx and pushes it at once (retail: edx, pushed after the ecx load) and
// shifts the later sete/spill registers (al -> dl, ecx -> eax). `ret 4`.
struct C3DVector { float X, Y, Z; };
float GetParticleFrameTime_00aeaa00();      // 0x00AEAA00

namespace NParticleEngine {

template <class A, class B> struct CParticleUpdateParameters;
struct CParticleSystem;

struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

struct CPSCSplineBase : CPSCBase {
    struct CSplinePosParamData {
        float CalcDistanceAlpha(const CParticleUpdateParameters<unsigned long, unsigned long>& params, float pos);   // 0x00B216F0
    };
    float      SplineAnimSpeedBase;         // +0x0c
    C3DVector* m_controlFirst;              // +0x10 Spline.ControlPoints
    C3DVector* m_controlLast;               // +0x14
    C3DVector* m_controlEnd;                // +0x18
    float      Tension;                     // +0x1c
    bool       EndPointsConnected;          // +0x20
    void*      SplineRandomOffsetData[2];   // +0x24
    struct CPosParamPointer {               // CCountedPointer<CSplinePosParamData>
        CSplinePosParamData* Data;
        void*                Info;
        CSplinePosParamData* operator->() const { return Data; }
    };
    CPosParamPointer SplinePosParamData;    // +0x2c
    long GetControlPointCount() const { return m_controlLast - m_controlFirst; }
    void UpdateSplineData(const CParticleUpdateParameters<unsigned long, unsigned long>& params);   // 0x00B215E0
};

struct CPSCSpline : CPSCSplineBase {        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};

void CPSCSpline::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    UpdateSplineData(params);
    UpdateStarted = true;
    float frameTime = GetParticleFrameTime_00aeaa00();
    float step = frameTime * (SplineAnimSpeed / GetControlPointCount());
    if (ScaleSplineSpeed)
        step = SplinePosParamData->CalcDistanceAlpha(params, SplineCurrentPosition) * step;
    if (SplineBounce) {
        if (ReverseSpline)
            step = -step;
        float pos = step + SplineCurrentPosition;
        SplineCurrentPosition = pos;
        if (pos >= 1.0f || pos <= 0.0f)
            ReverseSpline = !ReverseSpline;
    } else {
        float pos = step + SplineCurrentPosition;
        SplineCurrentPosition = pos;
        if (pos > 1.0f) {
            if (EndPointsConnected)
                SplineCurrentPosition = pos - 1.0f;
            else
                SplineCurrentPosition = 0.0f;
        }
    }
}

} // namespace NParticleEngine