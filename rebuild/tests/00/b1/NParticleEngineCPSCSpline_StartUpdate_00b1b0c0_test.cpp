#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00b1b0c0.cpp) ----
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

// ---- stubs for the external callees ----

void NParticleEngine::CPSCBase::v00() {}

static float g_frameTime = 0.5f;
static int g_updates = 0;
static float g_alphaPos = -1.0f;
float GetParticleFrameTime_00aeaa00() { return g_frameTime; }
float NParticleEngine::CPSCSplineBase::CSplinePosParamData::CalcDistanceAlpha(const CParticleUpdateParameters<unsigned long, unsigned long>&, float pos) { g_alphaPos = pos; return 2.0f; }
void NParticleEngine::CPSCSplineBase::UpdateSplineData(const CParticleUpdateParameters<unsigned long, unsigned long>&) { ++g_updates; }

static void run()
{

    int paramsStorage = 0;
    const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& params =
        *reinterpret_cast<const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>*>(&paramsStorage);
    C3DVector points[4];
    NParticleEngine::CPSCSplineBase::CSplinePosParamData posData;
    NParticleEngine::CPSCSpline s;
    s.UpdateStarted = false;
    s.Initialised = false;
    s.m_controlFirst = points; s.m_controlLast = points + 4; s.m_controlEnd = points + 4;
    s.EndPointsConnected = false;
    s.SplinePosParamData.Data = &posData;
    s.SplinePosParamData.Info = 0;
    s.SplineAnimSpeed = 2.0f;                  // step = 0.5 * (2 / 4) = 0.25
    s.ScaleSplineSpeed = false;
    // bounce: forward past the end flips direction
    s.SplineBounce = true; s.ReverseSpline = false; s.SplineCurrentPosition = 0.875f;
    s.StartUpdate(params);
    CHECK(g_updates == 1 && s.UpdateStarted);
    CHECK(s.SplineCurrentPosition == 1.125f && s.ReverseSpline == true);
    s.StartUpdate(params);                     // reversed: back to 0.875, no flip
    CHECK(s.SplineCurrentPosition == 0.875f && s.ReverseSpline == true);
    s.SplineCurrentPosition = 0.125f;
    s.StartUpdate(params);                     // reaches 0: flips forward again
    CHECK(s.SplineCurrentPosition == -0.125f && s.ReverseSpline == false);
    // wrap: past the end restarts at 0, or wraps when the end points are connected
    s.SplineBounce = false; s.SplineCurrentPosition = 0.875f;
    s.StartUpdate(params);
    CHECK(s.SplineCurrentPosition == 0.0f);
    s.EndPointsConnected = true; s.SplineCurrentPosition = 0.875f;
    s.StartUpdate(params);
    CHECK(s.SplineCurrentPosition == 0.125f);
    s.SplineCurrentPosition = 0.5f;
    s.StartUpdate(params);
    CHECK(s.SplineCurrentPosition == 0.75f);
    // speed scaled by the position data's distance alpha (2): step 0.5
    s.ScaleSplineSpeed = true; s.SplineCurrentPosition = 0.25f;
    s.StartUpdate(params);
    CHECK(g_alphaPos == 0.25f && s.SplineCurrentPosition == 0.75f);
    CHECK(g_updates == 7);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b1b0c0_TEST FAIL\n"); return 1; }
    std::printf("00b1b0c0_TEST PASS\n");
    return 0;
}