// NParticleEngine::CPSCLight::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend the light towards another one (floats lerped, the long fade / attenuation integers lerped and rounded, the flag bits and position parameter taken from the target past the halfway point, the four colours through GFGetInterpolatedColour, then the user points of the target's array)
// Leaf: ?InterpolateTo@CPSCLight@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18C60, 1903 bytes, thiscall, `ret 8`.
// Retail-asm: 0x00B18D8E `fistp dword ptr [esp+18h]` (and every later GFInterpolate rounding of this body) -- the engine's inline-asm
//   float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++. This row
//   carries no asm of its own.
// Identity: FableWin.pdb ?InterpolateTo@CPSCLight@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCLight vtable
//   0x012A0624, as the landed CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 / CPSCSpline::InterpolateTo 0x00B1AF50 are slot 14 of theirs.
// Callees: GFGetInterpolatedColour(CRGBColour, CRGBColour, unsigned long) 0x00A44250 (x4). Inlined: the NParticleEngine::GFInterpolate
//   overloads of FableWin.pdb (__fastcall MMMM float, JJJM long = the landed out-of-line 0x00B05C20 body, _N_N0M bool, CRGBColour,
//   C3DVector = the landed out-of-line 0x00AF3CF0 body) and GFInterpolateUpdateParams (KKKM; the select is inferred from the name,
//   one source: PositionParam is an update-parameter index).
// Constants: 0.5f 0x0122F59C, 1.0f 0x0122DED8, 256.0f 0x0125B240.
// Layout: PDB (Ego_r) CPSCLight (0x70), retail offsets equal the PDB (the landed WriteBinary / DatabaseCopy model); PrimHandle +0x58 (engine/CEnginePrimitiveHandle.h, untouched here),
//   LightUserPointsArray +0x60 (CArray<C3DVector>: a 12-byte STLport vector, engine/CArray.h).
// Shape: the members in the order retail touches them (not declaration order); the bool overload selects the byte before the bit is
//   extracted; `1 - t` and `t * 256` are each computed once; the point loop takes the target's count (signed /12 size) and VC7.1
//   unrolls it by four. Colour arguments go by value through engine/CRGBColour.h FABLE_RETAIL_CRGBCOLOUR (Initialise copy ctor).
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
namespace NParticleEngine {
inline float __fastcall GFInterpolate(float a, float b, float t) { return a * (1.0f - t) + b * t; }
inline long __fastcall GFInterpolate(long a, long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline bool __fastcall GFInterpolate(bool a, bool b, float t) { return t > 0.5f ? b : a; }
inline unsigned long __fastcall GFInterpolateUpdateParams(unsigned long a, unsigned long b, float t) { return t > 0.5f ? b : a; }
__forceinline CRGBColour __fastcall GFInterpolate(const CRGBColour& a, const CRGBColour& b, float t) { return GFGetInterpolatedColour(a, b, GFFloatToLongNear(t * 256.0f)); }
inline C3DVector __fastcall GFInterpolate(const C3DVector& a, const C3DVector& b, float t) { return a * (1.0f - t) + b * t; }
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
};
class CPSCLight : public CPSCBase {
public:
    long LightLifeTimer;                                // +0x0c
    long LightRespawnDelayTimer;                        // +0x10
    bool LightIsAlive;                                  // +0x14
    unsigned long LightPositionParam;                   // +0x18
    float LightLifeSecs;                                // +0x1c
    float LightRespawnDelaySecs;                        // +0x20
    float LightStartTime;                               // +0x24
    float LightTimelineSecs;                            // +0x28
    float LightStartRenderWorldRadius;                  // +0x2c
    float LightEndRenderWorldRadius;                    // +0x30
    long LightAttenuationFactor;                        // +0x34
    long LightFadeInEndInteger;                         // +0x38
    long LightFadeOutBeginInteger;                      // +0x3c
    float LightWorldRadiusFadeMinimum;                  // +0x40
    CRGBColour LightColourFadeMinimum;                  // +0x44
    bool LightWorldRadiusFadeEnable : 1;                // +0x48 bit 0
    bool LightUseLifeSecs : 1;
    bool LightEnabled : 1;
    bool LightRespawns : 1;
    bool LightColourFadeEnable : 1;
    bool LightUseStartColour : 1;
    bool LightUseMidColour : 1;
    bool LightUseEndColour : 1;
    bool LightUseFadeColour : 1;                        // +0x49 bit 0
    bool LightUseTimelineSecs : 1;
    bool StartedTimer : 1;
    CRGBColour LightStartColour;                        // +0x4c
    CRGBColour LightMidColour;                          // +0x50
    CRGBColour LightEndColour;                          // +0x54
    CEnginePrimitiveHandle PrimHandle;                  // +0x58
    CArray<C3DVector> LightUserPointsArray;             // +0x60
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCLight::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCLight& source = static_cast<const CPSCLight&>(other);
    LightEnabled = GFInterpolate(LightEnabled, source.LightEnabled, t);
    LightPositionParam = GFInterpolateUpdateParams(LightPositionParam, source.LightPositionParam, t);
    LightUseLifeSecs = GFInterpolate(LightUseLifeSecs, source.LightUseLifeSecs, t);
    LightLifeSecs = GFInterpolate(LightLifeSecs, source.LightLifeSecs, t);
    LightRespawns = GFInterpolate(LightRespawns, source.LightRespawns, t);
    LightRespawnDelaySecs = GFInterpolate(LightRespawnDelaySecs, source.LightRespawnDelaySecs, t);
    LightStartRenderWorldRadius = GFInterpolate(LightStartRenderWorldRadius, source.LightStartRenderWorldRadius, t);
    LightEndRenderWorldRadius = GFInterpolate(LightEndRenderWorldRadius, source.LightEndRenderWorldRadius, t);
    LightAttenuationFactor = GFInterpolate(LightAttenuationFactor, source.LightAttenuationFactor, t);
    LightFadeInEndInteger = GFInterpolate(LightFadeInEndInteger, source.LightFadeInEndInteger, t);
    LightFadeOutBeginInteger = GFInterpolate(LightFadeOutBeginInteger, source.LightFadeOutBeginInteger, t);
    LightWorldRadiusFadeEnable = GFInterpolate(LightWorldRadiusFadeEnable, source.LightWorldRadiusFadeEnable, t);
    LightWorldRadiusFadeMinimum = GFInterpolate(LightWorldRadiusFadeMinimum, source.LightWorldRadiusFadeMinimum, t);
    LightUseFadeColour = GFInterpolate(LightUseFadeColour, source.LightUseFadeColour, t);
    LightColourFadeEnable = GFInterpolate(LightColourFadeEnable, source.LightColourFadeEnable, t);
    LightColourFadeMinimum = GFInterpolate(LightColourFadeMinimum, source.LightColourFadeMinimum, t);
    LightUseStartColour = GFInterpolate(LightUseStartColour, source.LightUseStartColour, t);
    LightUseMidColour = GFInterpolate(LightUseMidColour, source.LightUseMidColour, t);
    LightUseEndColour = GFInterpolate(LightUseEndColour, source.LightUseEndColour, t);
    LightStartColour = GFInterpolate(LightStartColour, source.LightStartColour, t);
    LightMidColour = GFInterpolate(LightMidColour, source.LightMidColour, t);
    LightEndColour = GFInterpolate(LightEndColour, source.LightEndColour, t);
    LightStartTime = GFInterpolate(LightStartTime, source.LightStartTime, t);
    LightUseTimelineSecs = GFInterpolate(LightUseTimelineSecs, source.LightUseTimelineSecs, t);
    LightTimelineSecs = GFInterpolate(LightTimelineSecs, source.LightTimelineSecs, t);
    unsigned long count = source.LightUserPointsArray.size();
    for (unsigned long i = 0; i < count; ++i)
        LightUserPointsArray[i] = GFInterpolate(LightUserPointsArray[i], source.LightUserPointsArray[i], t);
}
}
