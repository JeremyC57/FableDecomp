// NParticleEngine::CPSCDecalRenderer::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend the decal renderer towards another one (floats and the packed bit-field integers / flags lerped and rounded, bank / blend mode / blend op / flags taken from the target past the halfway point, the three colours through GFGetInterpolatedColour)
// Leaf: ?InterpolateTo@CPSCDecalRenderer@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1BDA0, 2770 bytes, `ret 8`.
// Retail-asm: 0x00B1BEAB `fistp dword ptr [esp+14h]` (and every later GFInterpolate rounding of this body) -- the engine's inline-asm
//   float-to-long helper; VC7.1 emits no bare dword fistp from C++.
// Helper (landing 2026-11-03): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm,
//   and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Identity: FableWin.pdb ?InterpolateTo@CPSCDecalRenderer@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the vtable 0x012A09BC
//   (RTTI CPSCDecalRenderer), as CPSCSpline::InterpolateTo 0x00B1AF50 is slot 14 of its vtable. The catalog label (unnamed slot) is replaced.
// Callees: GFGetInterpolatedColour(CRGBColour, CRGBColour, unsigned long) 0x00A44250 (__cdecl, FableWin
//   ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z), three times. Inlined: the NParticleEngine::GFInterpolate overloads of
//   FableWin.pdb (__fastcall: MMMM float, KKKM unsigned long, EEEM unsigned char, ?AVCRGBColour@@ABV2@0M colour -- the long one is
//   the landed out-of-line 0x00B05C20, `GFFloatToLongNear((1 - t) * a + b * t)`), whose rounding is the engine's inline-asm
//   GFFloatToLongNear (shared engine/GFFloatToLongNear.h, retail-asm helper: `fstp [esp+20h] ; fld [esp+20h] ; fistp [esp+14h]`).
// Constants: 1.0f 0x0122DED8, 0.5f 0x0122F59C, 2^32 0x0122DCB4 (unsigned -> float fix-up), 256.0f 0x0125B240.
// Layout: PDB (Ego_r) CPSCDecalRenderer, retail offsets equal the PDB (the landed WriteBinary 0x00B1BA10 / DatabaseCopy 0x00B1B810
//   model): DecalLifeSecs +0x0c, DecalBankIndex +0x10, Start/Mid/EndColour +0x14/+0x18/+0x1c, unsigned long bit-field words
//   +0x20..+0x30, unsigned char flag bits +0x34/+0x35, the pool floats +0x38..+0x44, StencilCubeWidth +0x48.
// Shape: `1 - t` is computed once; every unsigned bit field goes through the unsigned overload (`fild ; jge ; fadd 2^32`), every
//   unsigned char flag bit through the unsigned char one (`movzx ; fild`, the low byte of the rounded long stored back); `t * 256`
//   is computed once for the three colours. The colour arguments are by value through CRGBColour's copy constructor, which retail
//   builds on Initialise(r, g, b, a) (all four bytes read before any is stored; header engine/CRGBColour.h FABLE_RETAIL_CRGBCOLOUR,
//   overlay hdr/): with the plain member-wise copy every byte load interleaves with its store and the whole function's register
//   allocation shifts (648 diff lines before, RELOCATION_MATCH after). The colour overload is __forceinline (plain inline stays a call).
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250 ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z
namespace NParticleEngine {
inline float __fastcall GFInterpolate(float a, float b, float t) { return a * (1.0f - t) + b * t; }
inline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline unsigned char __fastcall GFInterpolate(unsigned char a, unsigned char b, float t) { return (unsigned char)GFFloatToLongNear((1.0f - t) * a + b * t); }
__forceinline CRGBColour __fastcall GFInterpolate(const CRGBColour& a, const CRGBColour& b, float t) { return GFGetInterpolatedColour(a, b, GFFloatToLongNear(t * 256.0f)); }
}
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
};
class CPSCDecalRenderer : public CPSCBase {
public:
    float DecalLifeSecs;                                // +0x0c
    long DecalBankIndex;                                // +0x10
    CRGBColour StartColour;                             // +0x14
    CRGBColour MidColour;                               // +0x18
    CRGBColour EndColour;                               // +0x1c
    unsigned long BlendMode : 3;                        // +0x20
    unsigned long BlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;             // +0x24
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long EndRenderSize : 11;                   // +0x28
    unsigned long AnimationTimeSecs : 14;
    unsigned long SizeFadeMinimum : 7;
    unsigned long UseStartColour : 1;                   // +0x2c
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned long FlickerBias : 8;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long FlickerSpeed : 12;                    // +0x30
    unsigned char AlphaFadeEnable : 1;                  // +0x34
    unsigned char SizeFadeEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1;
    unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;          // +0x35
    float MaxPoolSize;                                  // +0x38
    float MaxPoolAlpha;                                 // +0x3c
    float MaxPoolLife;                                  // +0x40
    float PoolFrameIncreaseRate;                        // +0x44
    float StencilCubeWidth;                             // +0x48
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle;   // +0x4c
    long LandscapeDecalsChildPrimIndex;                 // +0x54
    CEnginePrimitiveHandle MeshDecalsPrimHandle;        // +0x58
    long MeshDecalsChildPrimIndex;                      // +0x60
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCDecalRenderer::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCDecalRenderer& source = static_cast<const CPSCDecalRenderer&>(other);
    DecalLifeSecs = GFInterpolate(DecalLifeSecs, source.DecalLifeSecs, t);
    DecalBankIndex = t > 0.5f ? source.DecalBankIndex : DecalBankIndex;
    Flags = t > 0.5f ? source.Flags : Flags;
    BlendMode = t > 0.5f ? source.BlendMode : BlendMode;
    BlendOp = t > 0.5f ? source.BlendOp : BlendOp;
    ForceAnimationTime = GFInterpolate((unsigned char)ForceAnimationTime, (unsigned char)source.ForceAnimationTime, t);
    AnimationTimeSecs = GFInterpolate((unsigned long)AnimationTimeSecs, (unsigned long)source.AnimationTimeSecs, t);
    StartRenderSize = GFInterpolate((unsigned long)StartRenderSize, (unsigned long)source.StartRenderSize, t);
    EndRenderSize = GFInterpolate((unsigned long)EndRenderSize, (unsigned long)source.EndRenderSize, t);
    FadeInEndInteger = GFInterpolate((unsigned long)FadeInEndInteger, (unsigned long)source.FadeInEndInteger, t);
    FadeOutBeginInteger = GFInterpolate((unsigned long)FadeOutBeginInteger, (unsigned long)source.FadeOutBeginInteger, t);
    AlphaFadeEnable = GFInterpolate((unsigned char)AlphaFadeEnable, (unsigned char)source.AlphaFadeEnable, t);
    AlphaFadeMinimum = GFInterpolate((unsigned long)AlphaFadeMinimum, (unsigned long)source.AlphaFadeMinimum, t);
    SizeFadeEnable = GFInterpolate((unsigned char)SizeFadeEnable, (unsigned char)source.SizeFadeEnable, t);
    SizeFadeMinimum = GFInterpolate((unsigned long)SizeFadeMinimum, (unsigned long)source.SizeFadeMinimum, t);
    UseStartColour = GFInterpolate((unsigned long)UseStartColour, (unsigned long)source.UseStartColour, t);
    UseMidColour = GFInterpolate((unsigned long)UseMidColour, (unsigned long)source.UseMidColour, t);
    UseEndColour = GFInterpolate((unsigned long)UseEndColour, (unsigned long)source.UseEndColour, t);
    StartColour = GFInterpolate(StartColour, source.StartColour, t);
    MidColour = GFInterpolate(MidColour, source.MidColour, t);
    EndColour = GFInterpolate(EndColour, source.EndColour, t);
    FlickerEnable = GFInterpolate((unsigned char)FlickerEnable, (unsigned char)source.FlickerEnable, t);
    FlickerSpeed = GFInterpolate((unsigned long)FlickerSpeed, (unsigned long)source.FlickerSpeed, t);
    FlickerBias = GFInterpolate((unsigned long)FlickerBias, (unsigned long)source.FlickerBias, t);
    FlickerMinAlphaInteger = GFInterpolate((unsigned long)FlickerMinAlphaInteger, (unsigned long)source.FlickerMinAlphaInteger, t);
    FlickerMinSizeInteger = GFInterpolate((unsigned long)FlickerMinSizeInteger, (unsigned long)source.FlickerMinSizeInteger, t);
    PoolingEnable = GFInterpolate((unsigned char)PoolingEnable, (unsigned char)source.PoolingEnable, t);
    PoolIncreaseAlphaEnable = GFInterpolate((unsigned char)PoolIncreaseAlphaEnable, (unsigned char)source.PoolIncreaseAlphaEnable, t);
    PoolIncreaseSizeEnable = GFInterpolate((unsigned char)PoolIncreaseSizeEnable, (unsigned char)source.PoolIncreaseSizeEnable, t);
    PoolIncreaseLifeEnable = GFInterpolate((unsigned char)PoolIncreaseLifeEnable, (unsigned char)source.PoolIncreaseLifeEnable, t);
    MaxPoolSize = GFInterpolate(MaxPoolSize, source.MaxPoolSize, t);
    MaxPoolAlpha = GFInterpolate(MaxPoolAlpha, source.MaxPoolAlpha, t);
    MaxPoolLife = GFInterpolate(MaxPoolLife, source.MaxPoolLife, t);
    PoolIncreaseFrameEnable = GFInterpolate((unsigned char)PoolIncreaseFrameEnable, (unsigned char)source.PoolIncreaseFrameEnable, t);
    PoolFrameIncreaseRate = GFInterpolate(PoolFrameIncreaseRate, source.PoolFrameIncreaseRate, t);
    StencilCubeWidth = GFInterpolate(StencilCubeWidth, source.StencilCubeWidth, t);
}
}