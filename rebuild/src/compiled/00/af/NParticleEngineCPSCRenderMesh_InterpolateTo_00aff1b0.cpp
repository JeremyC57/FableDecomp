// NParticleEngine::CPSCRenderMesh::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend every render-mesh parameter towards another component at t
// Leaf: ?InterpolateTo@CPSCRenderMesh@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFF1B0, 3630 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb / Ego_r.pdb ?InterpolateTo@CPSCRenderMesh@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCRenderMesh
//   vtable 0x0129F96C (rtti-vtables.tsv), the same slot as the landed CPSCSingleSprite / CPSCDecalRenderer InterpolateTo rows.
// Shape (as the landed CPSCDecalRenderer::InterpolateTo 0x00B1BDA0): index members (BankIndex, TrailBankIndex, blend modes / ops) and
//   RenderSizeParam (GFInterpolateUpdateParams) take the other component's value when t > 0.5; every other bitfield (packed sizes,
//   fade / flicker integers, the bool flags) blends with GFInterpolate ((1 - t) * a + b * t rounded with GFFloatToLongNear), the
//   colours with GFGetInterpolatedColour 0x00A44250 at weight round(t * 256). Members in the CPSCRenderMesh order (Ego_r.pdb, 0x64).
// Callees: GFGetInterpolatedColour 0x00A44250 (cdecl, colours by value through the CRGBColour copy constructor).
// Header debt: no shared CPSCBase / CPSCRenderMesh header; the class is modelled locally (Ego_r.pdb member names and offsets).
// Retail-asm: 0x00AFF22D `fistp dword ptr [esp+14h]` (and 32 more, one per rounded member) -- GFFloatToLongNear, shared helper
//   engine/GFFloatToLongNear.h; VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives fistp qword).
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
inline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline unsigned long __fastcall GFInterpolateUpdateParams(unsigned long a, unsigned long b, float t) { return t > 0.5f ? b : a; }
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
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;                      // +0x48
    CEnginePrimitiveHandle PrimHandle;                  // +0x4c
    long ChildPrimIndex;                                // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;             // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCRenderMesh::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCRenderMesh& source = static_cast<const CPSCRenderMesh&>(other);
    BankIndex = t > 0.5f ? source.BankIndex : BankIndex;
    StartRenderSizeX = GFInterpolate((unsigned long)StartRenderSizeX, (unsigned long)source.StartRenderSizeX, t);
    StartRenderSizeY = GFInterpolate((unsigned long)StartRenderSizeY, (unsigned long)source.StartRenderSizeY, t);
    StartRenderSizeZ = GFInterpolate((unsigned long)StartRenderSizeZ, (unsigned long)source.StartRenderSizeZ, t);
    EndRenderSizeX = GFInterpolate((unsigned long)EndRenderSizeX, (unsigned long)source.EndRenderSizeX, t);
    EndRenderSizeY = GFInterpolate((unsigned long)EndRenderSizeY, (unsigned long)source.EndRenderSizeY, t);
    EndRenderSizeZ = GFInterpolate((unsigned long)EndRenderSizeZ, (unsigned long)source.EndRenderSizeZ, t);
    UseRenderSizeParam = GFInterpolate((unsigned long)UseRenderSizeParam, (unsigned long)source.UseRenderSizeParam, t);
    RenderSizeParam = GFInterpolateUpdateParams(RenderSizeParam, source.RenderSizeParam, t);
    UseStartColour = GFInterpolate((unsigned long)UseStartColour, (unsigned long)source.UseStartColour, t);
    UseMidColour = GFInterpolate((unsigned long)UseMidColour, (unsigned long)source.UseMidColour, t);
    UseEndColour = GFInterpolate((unsigned long)UseEndColour, (unsigned long)source.UseEndColour, t);
    StartColour = GFInterpolate(StartColour, source.StartColour, t);
    MidColour = GFInterpolate(MidColour, source.MidColour, t);
    EndColour = GFInterpolate(EndColour, source.EndColour, t);
    CentredOnPos = GFInterpolate((unsigned long)CentredOnPos, (unsigned long)source.CentredOnPos, t);
    FadeInEndInteger = GFInterpolate((unsigned long)FadeInEndInteger, (unsigned long)source.FadeInEndInteger, t);
    FadeOutBeginInteger = GFInterpolate((unsigned long)FadeOutBeginInteger, (unsigned long)source.FadeOutBeginInteger, t);
    AlphaFadeEnable = GFInterpolate((unsigned long)AlphaFadeEnable, (unsigned long)source.AlphaFadeEnable, t);
    AlphaFadeMinimum = GFInterpolate((unsigned long)AlphaFadeMinimum, (unsigned long)source.AlphaFadeMinimum, t);
    SizeFadeEnable = GFInterpolate((unsigned long)SizeFadeEnable, (unsigned long)source.SizeFadeEnable, t);
    SizeFadeMinimum = GFInterpolate((unsigned long)SizeFadeMinimum, (unsigned long)source.SizeFadeMinimum, t);
    FlickerEnable = GFInterpolate((unsigned long)FlickerEnable, (unsigned long)source.FlickerEnable, t);
    FlickerSpeed = GFInterpolate((unsigned long)FlickerSpeed, (unsigned long)source.FlickerSpeed, t);
    FlickerBias = GFInterpolate((unsigned long)FlickerBias, (unsigned long)source.FlickerBias, t);
    FlickerMinAlphaInteger = GFInterpolate((unsigned long)FlickerMinAlphaInteger, (unsigned long)source.FlickerMinAlphaInteger, t);
    FlickerMinSizeInteger = GFInterpolate((unsigned long)FlickerMinSizeInteger, (unsigned long)source.FlickerMinSizeInteger, t);
    TrailBankIndex = t > 0.5f ? source.TrailBankIndex : TrailBankIndex;
    TrailLengthInteger = GFInterpolate((unsigned long)TrailLengthInteger, (unsigned long)source.TrailLengthInteger, t);
    TrailWidth = GFInterpolate((unsigned long)TrailWidth, (unsigned long)source.TrailWidth, t);
    TrailUseStartColour = GFInterpolate((unsigned long)TrailUseStartColour, (unsigned long)source.TrailUseStartColour, t);
    TrailUseMidColour = GFInterpolate((unsigned long)TrailUseMidColour, (unsigned long)source.TrailUseMidColour, t);
    TrailUseEndColour = GFInterpolate((unsigned long)TrailUseEndColour, (unsigned long)source.TrailUseEndColour, t);
    TrailStartColour = GFInterpolate(TrailStartColour, source.TrailStartColour, t);
    TrailMidColour = GFInterpolate(TrailMidColour, source.TrailMidColour, t);
    TrailEndColour = GFInterpolate(TrailEndColour, source.TrailEndColour, t);
    BlendMode = t > 0.5f ? source.BlendMode : BlendMode;
    BlendOp = t > 0.5f ? source.BlendOp : BlendOp;
    TrailBlendMode = t > 0.5f ? source.TrailBlendMode : TrailBlendMode;
    TrailBlendOp = t > 0.5f ? source.TrailBlendOp : TrailBlendOp;
}
}