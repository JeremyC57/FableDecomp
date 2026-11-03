// NParticleEngine::CPSCSingleSprite::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: the CPSCSplineBase part first, then blend the sprite settings towards another single sprite (floats lerped, long integers lerped and rounded, bank / mode / alignment indices and flags taken from the target past the halfway point, the three colours through GFGetInterpolatedColour)
// Leaf: ?InterpolateTo@CPSCSingleSprite@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B062A0, 1607 bytes, thiscall, `ret 8`.
// Retail-asm: 0x00B0650E `fistp dword ptr [esp+14h]` (and every later GFInterpolate rounding of this body) -- the engine's inline-asm
//   float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++. This row
//   carries no asm of its own.
// Identity: FableWin.pdb ?InterpolateTo@CPSCSingleSprite@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCSingleSprite vtable
//   0x0129FB84 (the CPSC family's slot 14, as 0x00B1BDA0 / 0x00B1AF50 / 0x00B18C60).
// Callees: CPSCSplineBase::InterpolateTo 0x00B22310 (the base part, called non-virtually first; FableWin
//   ?InterpolateTo@CPSCSplineBase@NParticleEngine@@UAEXABVCPSCBase@2@M@Z, slot 14 of the CPSCSplineBase vtable 0x012A0B14 -- NAME_FIXES:
//   reloc_bindings has ?InterpolateTo@CPSCSplineBase@NParticleEngine@@QAEXPBVCPSCBase@2@M@Z), GFGetInterpolatedColour 0x00A44250 (x3).
//   Inlined: the GFInterpolate overloads and GFInterpolateUpdateParams as in CPSCLight::InterpolateTo 0x00B18C60.
// Layout: PDB (Ego_r) CPSCSingleSprite with every offset 4 below the PDB (retail CPSCSplineBase is 0x34 bytes; the landed
//   WriteBinary 0x00B06060 model). StayWithEmitterIntFactor (+0x8C) and EnteringTimelineEnd (+0x97) are not interpolated.
// Shape: the members in the order retail touches them; MaxTrailLength's long lerp loads the operands straight from memory while
//   NoCrossedSprites / FadeIn / FadeOut go through frame copies -- the same inline body, VC7.1's choice.
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
namespace NParticleEngine {
inline float __fastcall GFInterpolate(float a, float b, float t) { return a * (1.0f - t) + b * t; }
inline long __fastcall GFInterpolate(long a, long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline bool __fastcall GFInterpolate(bool a, bool b, float t) { return t > 0.5f ? b : a; }
inline unsigned long __fastcall GFInterpolateUpdateParams(unsigned long a, unsigned long b, float t) { return t > 0.5f ? b : a; }
__forceinline CRGBColour __fastcall GFInterpolate(const CRGBColour& a, const CRGBColour& b, float t) { return GFGetInterpolatedColour(a, b, GFFloatToLongNear(t * 256.0f)); }
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
};
class CPSCSplineBase : public CPSCBase {
public:
    unsigned char _spline[0x28];                        // +0x0c SplineAnimSpeed, Spline, SplineRandomOffsetData, SplinePosParamData (retail 0x34)
    virtual void InterpolateTo(const CPSCBase& other, float t);   // 0x00B22310
};
class CPSCSingleSprite : public CPSCSplineBase {
public:
    long SpriteBankIndex;                               // +0x34
    CRGBColour StartColour;                             // +0x38
    CRGBColour MidColour;                               // +0x3c
    CRGBColour EndColour;                               // +0x40
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
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCSingleSprite::InterpolateTo(const CPSCBase& other, float t)
{
    CPSCSplineBase::InterpolateTo(other, t);
    const CPSCSingleSprite& source = static_cast<const CPSCSingleSprite&>(other);
    SpriteBankIndex = t > 0.5f ? source.SpriteBankIndex : SpriteBankIndex;
    PositionParam = GFInterpolateUpdateParams(PositionParam, source.PositionParam, t);
    ForceAnimationTime = GFInterpolate(ForceAnimationTime, source.ForceAnimationTime, t);
    AnimationTimeSecs = GFInterpolate(AnimationTimeSecs, source.AnimationTimeSecs, t);
    SelfIlluminating = GFInterpolate(SelfIlluminating, source.SelfIlluminating, t);
    StartRenderSize = GFInterpolate(StartRenderSize, source.StartRenderSize, t);
    EndRenderSize = GFInterpolate(EndRenderSize, source.EndRenderSize, t);
    BlendMode = t > 0.5f ? source.BlendMode : BlendMode;
    BlendOp = t > 0.5f ? source.BlendOp : BlendOp;
    TrailBlendMode = t > 0.5f ? source.TrailBlendMode : TrailBlendMode;
    TrailBlendOp = t > 0.5f ? source.TrailBlendOp : TrailBlendOp;
    FaceMe2D = GFInterpolate(FaceMe2D, source.FaceMe2D, t);
    FaceMe3D = GFInterpolate(FaceMe3D, source.FaceMe3D, t);
    CrossedSprites = GFInterpolate(CrossedSprites, source.CrossedSprites, t);
    SpriteAlignment = t > 0.5f ? source.SpriteAlignment : SpriteAlignment;
    RotateAroundCentre = GFInterpolate(RotateAroundCentre, source.RotateAroundCentre, t);
    NoCrossedSprites = GFInterpolate(NoCrossedSprites, source.NoCrossedSprites, t);
    InitialAngle = GFInterpolate(InitialAngle, source.InitialAngle, t);
    FadeInEndInteger = GFInterpolate(FadeInEndInteger, source.FadeInEndInteger, t);
    FadeOutBeginInteger = GFInterpolate(FadeOutBeginInteger, source.FadeOutBeginInteger, t);
    AlphaFadeEnable = GFInterpolate(AlphaFadeEnable, source.AlphaFadeEnable, t);
    AlphaFadeMinimum = GFInterpolate(AlphaFadeMinimum, source.AlphaFadeMinimum, t);
    SizeFadeEnable = GFInterpolate(SizeFadeEnable, source.SizeFadeEnable, t);
    SizeFadeMinimum = GFInterpolate(SizeFadeMinimum, source.SizeFadeMinimum, t);
    StartColour = GFInterpolate(StartColour, source.StartColour, t);
    MidColour = GFInterpolate(MidColour, source.MidColour, t);
    EndColour = GFInterpolate(EndColour, source.EndColour, t);
    UseStartColour = GFInterpolate(UseStartColour, source.UseStartColour, t);
    UseMidColour = GFInterpolate(UseMidColour, source.UseMidColour, t);
    UseEndColour = GFInterpolate(UseEndColour, source.UseEndColour, t);
    TrailBankIndex = t > 0.5f ? source.TrailBankIndex : TrailBankIndex;
    TrailWidth = GFInterpolate(TrailWidth, source.TrailWidth, t);
    MaxTrailLength = GFInterpolate(MaxTrailLength, source.MaxTrailLength, t);
    StayWithEmitter = GFInterpolate(StayWithEmitter, source.StayWithEmitter, t);
    UsePosition = GFInterpolate(UsePosition, source.UsePosition, t);
    UseSplinePoints = GFInterpolate(UseSplinePoints, source.UseSplinePoints, t);
}
}