// NParticleEngine::CPSCLight::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: set the light primitive for this frame (start / mid / end colour blended by life fraction, faded by the system alpha and the fade-in / fade-out ramps, radius from the start / end radii) or clear it when the light is off or dead
// Grade: DIFFER(2546v2516)
// Leaf: ?PreparePrimitives@CPSCLight@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B19C60, 2516 bytes, thiscall, `ret 14h`.
// Identity: FableWin.pdb ?PreparePrimitives@CPSCLight@NParticleEngine@@...; slot 13 of the CPSCLight vtable 0x012A0624 (the landed
//   CPSCUpdateNormal::PreparePrimitives 0x00B0C910 is slot 13 of its own, same signature). Catalog label "vfunc_13".
// Callees: GetTickRate 0x00AEA9F0, CPSCLight::SmoothFadeValue 0x00B1AD90, CParticleSystem::ScaleParticleDimensions(float&) 0x00AF8F80,
//   CPSCBase::GetCurrentPosition 0x00AF2640, CParticleSystem::GetSystemTransform 0x00AF2250, CEnginePrimitiveLight ctor 0x00642000,
//   CIEngine slot 23 SetPersistentPrimitive, CEnginePrimitiveHandle::Clear inline (slot 3 NotifyReleased + counted Reset).
// Residue: frame 0xA8 vs retail 0xA4 -- retail packs lifeFraction / radiusScale / spotRadius into one slot [esp+1Ch] and the
//   white / mid temps, radiusFraction and the final colour into [esp+18h], radius at [esp+80h] beside the matrix; the arg-push lea
//   is scheduled differently around the radius sum. 14 graded variants (a1..a14, LOG.tsv).
// Retail-asm: 0x00B1A4DA `fistp dword ptr [esp+20h]` (the four GetRGBColourClamped roundings) -- the engine's inline-asm
//   float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++.
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLight.h"
#include "engine/CIEngine.h"
#include "engine/CParticleSystem.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
namespace NParticleEngine {
long __fastcall GetTickRate();                          // 0x00AEA9F0
template <class K, class V> class CParticleUpdateParameters;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) = 0;
    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params, const unsigned long& param,
                                 float time, bool local, unsigned long flags) const;   // 0x00AF2640
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
    virtual void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
    float SmoothFadeValue(float value) const;           // 0x00B1AD90
};
void CPSCLight::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                  const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    if (LightEnabled && LightIsAlive) {
        float tickRate = (float)GetTickRate();
        float fadeInEnd = LightFadeInEndInteger * 0.001f;
        float fadeOutBegin = LightFadeOutBeginInteger * 0.001f;
        CRGBFloatColour startColour(LightUseStartColour ? LightStartColour : LightUseMidColour ? LightMidColour
                                    : LightUseEndColour ? LightEndColour : CRGBColour(255, 255, 255, 255));
        CRGBFloatColour endColour(LightUseEndColour ? LightEndColour : LightUseMidColour ? LightMidColour
                                  : LightUseStartColour ? LightStartColour : CRGBColour(255, 255, 255, 255));
        CRGBFloatColour midColour(LightUseMidColour ? LightMidColour : (startColour * 0.5f + endColour * 0.5f).GetRGBColour());
        float lifeFraction = 0.0f;
        float radiusFraction = 0.0f;
        if (LightUseLifeSecs) {
            lifeFraction = (LightLifeTimer + time) / (tickRate * LightLifeSecs);
            if (lifeFraction < 0.0f) lifeFraction = 0.0f;
            else if (lifeFraction > 1.0f) lifeFraction = 1.0f;
            radiusFraction = lifeFraction;
            if (radiusFraction < 0.0f) radiusFraction = 0.0f;
            else {
                float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                if (radiusFraction > limit) radiusFraction = limit;
            }
        }
        CRGBFloatColour colour;
        if (lifeFraction < 0.5f)
            colour = startColour.Interpolate(midColour, lifeFraction * 2.0f);
        else
            colour = midColour.Interpolate(endColour, (lifeFraction - 0.5f) * 2.0f);
        float alpha = system.GetGlobalAlpha();
        if (lifeFraction < fadeInEnd)
            alpha *= SmoothFadeValue(lifeFraction / fadeInEnd);
        if (lifeFraction > fadeOutBegin)
            alpha *= SmoothFadeValue((1.0f - lifeFraction) / (1.0f - fadeOutBegin));
        float radiusScale = 1.0f;
        if (alpha < 1.0f) {
            if (LightColourFadeEnable) {
                colour *= (CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f) - LightColourFadeMinimum) * CRGBFloatColour(alpha, alpha, alpha, 1.0f) + LightColourFadeMinimum;
                CRGBFloatColour minimum(LightColourFadeMinimum);
                if (!(colour.R > minimum.R)) colour.R = minimum.R;
                if (!(colour.G > minimum.G)) colour.G = minimum.G;
                if (!(colour.B > minimum.B)) colour.B = minimum.B;
            }
            if (LightWorldRadiusFadeEnable)
                radiusScale = (1.0f - LightWorldRadiusFadeMinimum) * alpha + LightWorldRadiusFadeMinimum;
        }
        system.ScaleParticleDimensions(radiusScale);
        float attenuation = (LightAttenuationFactor + 1) * 0.001f;
        float innerStart = attenuation * LightStartRenderWorldRadius;
        float start = LightStartRenderWorldRadius;
        float radius = ((LightEndRenderWorldRadius - start) * radiusFraction + start) * radiusScale;
        float spotRadius = ((attenuation * LightEndRenderWorldRadius - innerStart) * radiusFraction + innerStart) * radiusScale;
        C3DVector position = GetCurrentPosition(params, LightPositionParam, time, false, 0);
        system.TransformPosIfWorld(position);
        CRGBColour lightColour = colour.GetRGBColourClamped();
        CIEngine* engine = info.Engine;
        engine->SetPersistentPrimitive(PrimHandle, CEnginePrimitiveLight(0, position, lightColour, radius, spotRadius,
                                            (EEngineInterpolationSet)0, 0xffffffff), 0);
    } else
        PrimHandle.Clear();
}
}
