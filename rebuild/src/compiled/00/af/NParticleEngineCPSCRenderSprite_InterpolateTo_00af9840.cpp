// NParticleEngine::CPSCRenderSprite::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend the sprite renderer towards another one (indices, blend fields and Flags taken from the target past the halfway point; every packed setting lerped and rounded; the three colours through GFGetInterpolatedColour)
// Leaf: ?InterpolateTo@CPSCRenderSprite@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF9840, 2669 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ?InterpolateTo@CPSCRenderSprite@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCRenderSprite vtable
//   (the slot-14 InterpolateTo rows of CPSCSingleSprite 0x00B062A0 and CPSCLight 0x00B18C60 have the same signature and `ret 8`).
// Callees: GFGetInterpolatedColour 0x00A44250 (three colours, weight GFFloatToLongNear(t * 256)). No base-class call.
// Layout: retail CPSCRenderSprite (0x54 bytes, Ego_r.pdb names and offsets): two bank indices +0x0C/+0x10, three CRGBColour
//   +0x14..+0x1C, the packed settings as unsigned long bit-fields +0x20..+0x34 (FableWin.pdb field order; WriteBinary 0x00AF9620
//   writes them in this order), four one-bit flags in the byte +0x38, PrimHandle +0x3C, ChildPrimIndex +0x44, TrailPrimHandle +0x48,
//   ChildTrailPrimIndex +0x50. No shared CPSCRenderSprite / CPSCBase header yet: local model (header debt, NOTES.md).
// Shape: FableWin.pdb NParticleEngine::GFInterpolate overloads (`YIEEEM` unsigned char, `YIKKKM` unsigned long, CRGBColour) inline:
//   ((1 - t) * a + b * t) rounded to nearest; the CRGBColour overload is __forceinline (it holds the inline-asm helper).
// Retail-asm: 0x00AF99B4 `fistp dword ptr [esp+14h]` (and every later GFInterpolate rounding of this body) -- the engine's inline-asm
//   float-to-long helper GFFloatToLongNear; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`, so no C++ form gives the
//   bare dword fistp.
// Helper: the shared engine/GFFloatToLongNear.h (asm path + FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#include "fable_compiler.h"
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
namespace NParticleEngine {
inline unsigned char __fastcall GFInterpolate(unsigned char a, unsigned char b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
FABLE_FORCEINLINE CRGBColour __fastcall GFInterpolate(const CRGBColour& a, const CRGBColour& b, float t) { return GFGetInterpolatedColour(a, b, GFFloatToLongNear(t * 256.0f)); }
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
};
class CPSCRenderSprite : public CPSCBase {
public:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long TrailLengthInteger : 12;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;   // +0x28
    unsigned long NoCrossedSprites : 5;
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;         // +0x2c
    unsigned long EndRenderSize : 11;
    unsigned long FlickerBias : 8;
    unsigned long AnimationTimeSecs : 14;       // +0x30
    unsigned long SizeFadeMinimum : 7;
    unsigned long TrailWidth : 10;
    unsigned long FlickerSpeed : 12;            // +0x34
    unsigned long UseStartColour : 1;
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned char AlphaFadeEnable : 1;          // +0x38
    unsigned char SizeFadeEnable : 1;
    unsigned char FlickerEnable : 1;
    unsigned char ForceAnimationTime : 1;
    CEnginePrimitiveHandle PrimHandle;          // +0x3c
    long          ChildPrimIndex;               // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x48
    long          ChildTrailPrimIndex;          // +0x50
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCRenderSprite::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCRenderSprite& source = static_cast<const CPSCRenderSprite&>(other);
    SpriteBankIndex = t > 0.5f ? source.SpriteBankIndex : SpriteBankIndex;
    TrailBankIndex = t > 0.5f ? source.TrailBankIndex : TrailBankIndex;
    Flags = t > 0.5f ? source.Flags : Flags;
    BlendMode = t > 0.5f ? source.BlendMode : BlendMode;
    BlendOp = t > 0.5f ? source.BlendOp : BlendOp;
    TrailBlendMode = t > 0.5f ? source.TrailBlendMode : TrailBlendMode;
    TrailBlendOp = t > 0.5f ? source.TrailBlendOp : TrailBlendOp;
    ForceAnimationTime = GFInterpolate(ForceAnimationTime, source.ForceAnimationTime, t);
    AnimationTimeSecs = GFInterpolate(AnimationTimeSecs, source.AnimationTimeSecs, t);
    StartRenderSize = GFInterpolate(StartRenderSize, source.StartRenderSize, t);
    EndRenderSize = GFInterpolate(EndRenderSize, source.EndRenderSize, t);
    NoCrossedSprites = GFInterpolate(NoCrossedSprites, source.NoCrossedSprites, t);
    FadeInEndInteger = GFInterpolate(FadeInEndInteger, source.FadeInEndInteger, t);
    FadeOutBeginInteger = GFInterpolate(FadeOutBeginInteger, source.FadeOutBeginInteger, t);
    AlphaFadeEnable = GFInterpolate(AlphaFadeEnable, source.AlphaFadeEnable, t);
    AlphaFadeMinimum = GFInterpolate(AlphaFadeMinimum, source.AlphaFadeMinimum, t);
    SizeFadeEnable = GFInterpolate(SizeFadeEnable, source.SizeFadeEnable, t);
    SizeFadeMinimum = GFInterpolate(SizeFadeMinimum, source.SizeFadeMinimum, t);
    FlickerEnable = GFInterpolate(FlickerEnable, source.FlickerEnable, t);
    FlickerSpeed = GFInterpolate(FlickerSpeed, source.FlickerSpeed, t);
    FlickerBias = GFInterpolate(FlickerBias, source.FlickerBias, t);
    FlickerMinAlphaInteger = GFInterpolate(FlickerMinAlphaInteger, source.FlickerMinAlphaInteger, t);
    FlickerMinSizeInteger = GFInterpolate(FlickerMinSizeInteger, source.FlickerMinSizeInteger, t);
    UseStartColour = GFInterpolate(UseStartColour, source.UseStartColour, t);
    UseMidColour = GFInterpolate(UseMidColour, source.UseMidColour, t);
    UseEndColour = GFInterpolate(UseEndColour, source.UseEndColour, t);
    StartColour = GFInterpolate(StartColour, source.StartColour, t);
    MidColour = GFInterpolate(MidColour, source.MidColour, t);
    EndColour = GFInterpolate(EndColour, source.EndColour, t);
    TrailLengthInteger = GFInterpolate(TrailLengthInteger, source.TrailLengthInteger, t);
    TrailWidth = GFInterpolate(TrailWidth, source.TrailWidth, t);
}
}