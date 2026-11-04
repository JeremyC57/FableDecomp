// NParticleEngine::CPSCSingleSprite::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float) -- vtable slot 20: the spline base's cost when spline points are used; a sprite adds its crossed-sprite polygons and the (system-scaled) larger of the two render sizes squared; a trail adds its length and the scaled width x length
// Grade: DIFFER(428v429) -- try b dl=2: everything matches except the CrossedSprites bit test -- retail `mov al,[esi+93h] ; test al,20h`, ours `test byte ptr [esi+93h],20h` (ternary/if/accessor/unsigned-char-bitfield/(bool)/long-local forms all give test byte ptr) (best try l; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?GetComponentCost@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
// Flags: none
// Retail-asm: 0x00B05CDA + 0x00B05D34 `fistp dword ptr` -- engine/GFFloatToLongNear.h (GFFloatToLongNear inside the
//   RoundToDecimals quantiser of the landed CPSCRenderSprite::GetComponentCost 0x00AF90C0: the scaled size stored to the
//   helper's by-value copy, reloaded, bare dword fistp). VC7.1 `(long)x` calls __ftol2, `/QIfist` stores `fistp qword`,
//   `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05C60, 0x1AD bytes (padded to 432), `ret 10h`.
// Name: Ego_r.pdb ?GetComponentCost@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z.
// Callees: CPSCSplineBase::GetComponentCost 0x00B202C0 (Ego_r.pdb, same signature), __ftol2 0x00BFEA70, CParticleSystem::
//   ScaleParticleDimensions(float&) const 0x00AF8F80 (declared only: another TU's body).
// Layout: the landed CPSCSingleSprite::InterpolateTo 0x00B062A0 model (Ego_r.pdb members, retail offsets).
// Constants: 4294967296.0f 0x0122DCB4, 1/2047 0x0129F65C, 20.0f 0x0122F160, 1.0f 0x0122DED8, 10.0f 0x01237F0C, 2.0f 0x0122DCDC.
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {

class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};
class C3DVector { public:
    float X, Y, Z;
};
class CParticleSystem { public:
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;
    bool          ScaleParticles : 1;                // bit 7
    float         GlobalAlpha;                       // +0x54
    float         FadeAlpha;                         // +0x58
    float         NormalisedTime;                    // +0x5c
    C3DVector     Scale;                             // +0x60
    __declspec(noinline) void ScaleParticleDimensions(float& size) const   // 0x00AF8F80 (same-TU body, the landed one)
    {
        if (ScaleParticles) {
            float a = Scale.X > Scale.Y ? Scale.X : Scale.Y;
            size *= a > Scale.Z ? a : Scale.Z;
        }
    }
};
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float) = 0;
};
class CPSCSplineBase : public CPSCBase {
public:
    unsigned char _spline[0x28];                        // +0x0c SplineAnimSpeed, Spline, SplineRandomOffsetData, SplinePosParamData
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float);   // 0x00B202C0
};
class CPSCSingleSprite : public CPSCSplineBase {
public:
    long SpriteBankIndex;                               // +0x34
    unsigned long StartColour;                          // +0x38 CRGBColour
    unsigned long MidColour;                            // +0x3c
    unsigned long EndColour;                            // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;                              // +0x48
    float EndRenderSize;                                // +0x4c
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;                              // +0x54
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;                           // +0x60
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;                              // +0x68
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;                                       // +0x74
    long TrailBlendMode;                                // +0x78
    long TrailBlendOp;                                  // +0x7c
    long TrailBankIndex;                                // +0x80
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;                                  // +0x91
    bool UseEndColour;                                  // +0x92
    bool SelfIlluminating : 1;                          // +0x93 bit 0
    bool AlphaFadeEnable : 1;
    bool RotateAroundCentre : 1;
    bool FaceMe2D : 1;
    bool FaceMe3D : 1;
    bool CrossedSprites : 1;
    bool SizeFadeEnable : 1;
    bool ForceAnimationTime : 1;
    bool StayWithEmitter : 1;                           // +0x94 bit 0
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float);
};

static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return GFFloatToLongNear(scaled) / scale;
}

void CPSCSingleSprite::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float lightCount)
{
    if (UseSplinePoints)
        CPSCSplineBase::GetComponentCost(cost, system, particleCount, lightCount);
    if (SpriteBankIndex) {
        float start = (float)(unsigned long)StartRenderSize * (1.0f / 2047.0f);
        float startSize = RoundToDecimals(start * 20.0f, 2);
        float end = (float)(unsigned long)EndRenderSize * (1.0f / 2047.0f);
        float endSize = RoundToDecimals(end * 20.0f, 2);
        float size = startSize > endSize ? startSize : endSize;
        system.ScaleParticleDimensions(size);
        long crossed = CrossedSprites;
        float polys = crossed ? (float)(NoCrossedSprites * 2) : 2.0f;
        float area = size * size;
        cost.PolyCount += polys;
        cost.PixelCount += area;
        cost.RenderCost += area + polys;
    }
    if (TrailBankIndex) {
        float length = (float)(MaxTrailLength * 16 - 16);
        float width = TrailWidth;
        system.ScaleParticleDimensions(width);
        float area = width * length;
        cost.PolyCount += length;
        cost.PixelCount += area;
        cost.RenderCost += area + length;
    }
}

} // namespace NParticleEngine
