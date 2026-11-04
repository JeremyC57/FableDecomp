// NParticleEngine::CPSCRenderSprite::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: save / load the sprite renderer's emitter-database fields (graphic names through the symbol map, the legacy per-flag booleans when SpriteFlags is absent, every packed float unpacked, transferred and packed again)
// Leaf: ?DatabaseSerialise@CPSCRenderSprite@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFBCE0, 3920 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ?DatabaseSerialise@CPSCRenderSprite@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z;
//   slot 17 of the CPSCRenderSprite vtable (slot 17 DatabaseSerialise of CPSCAttractor 0x00B18110 and CPSCSpline 0x00B1B2E0, landed,
//   same `ret 8`); catalog "vfunc_17". The 40 field names are retail's strings (0x0129F954 "EEngineGraphic" .. 0x0129F6FC "TrailWidth").
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / <float> 0x00410620 / <CCharString> 0x004109A0 / <long> 0x00410BE0 /
//   <CRGBColour> 0x006420F0; CCharString ctor 0x0099E4B0, (const char*, long) 0x0099EBF0, copy 0x0099EC30, dtor 0x0099EAE0,
//   operator= 0x0099EFB0; CSymbolMap::FindFirstSymbolWithVal 0x009E50C0, GetSymbolSLONG 0x009E5120, SymbolExists 0x009E51C0 (overlay
//   engine/CSymbolMap.h, FABLE_RETAIL_CSYMBOLMAP); NParticleEngine::GetParticleEmitterManager 0x00AEAAF0 (the default blend mode / op
//   accessors inline: 2 and 0).
// Shape: IsSaving() picks the branch (Mode 1 or 3). Each member is transferred through a local copy in its own block (retail reuses one
//   frame slot per type); the legacy flags share one bool. Packed floats: GFUnpackFloat / GFPackFloat from engine/GFPackFloat.h (FableWin
//   ?GFUnpackFloat@@YIMKKMMK@Z / ?GFPackFloat@@YIKMKMMK@Z, out-of-line copies 0x00AF8DC0 / 0x00AF8D50); AlphaFadeMinimum and
//   SizeFadeMinimum unpack as GFRoundFloatNear(x / 127.0f, 2) (the GFUnpackFloat spelling adds an `fxch` at 0x00AFC5A2).
// Layout: retail CPSCRenderSprite as in InterpolateTo 0x00AF9840 (local model; no shared header yet).
// Retail-asm: 0x00AFC5B0 `fistp dword ptr [esp+1Ch]` (and every GFRoundFloatNear / GFPackFloat rounding of this body) -- the engine's
//   inline-asm float-to-long helper GFFloatToLongNear; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`.
// Helper: the shared engine/GFFloatToLongNear.h (asm path + FABLE_PORTABLE C++ twin), reached through engine/GFPackFloat.h; this row
//   carries no asm of its own.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CPersistContext.h"
#include "engine/CSymbolMap.h"
#include "engine/CParticleEmitter.h"
#include "engine/GFPackFloat.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
namespace NParticleEngine {
class CParticleEmitterManager { public:
    const long GetDefaultBlendMode() const { return 2; }
    const long GetDefaultBlendOp() const { return 0; }
    const long GetDefaultAlignmentMode() const { return 0; }
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) = 0;
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
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
void CPSCRenderSprite::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    if (context.IsSaving()) {
        CCharString spriteName;
        const CSymbolInfo* symbol = info.GraphicSymbols.FindFirstSymbolWithVal("EEngineGraphic", SpriteBankIndex);
        if (symbol)
            spriteName = symbol->EntryName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        CCharString trailName;
        symbol = info.GraphicSymbols.FindFirstSymbolWithVal("EEngineGraphic", TrailBankIndex);
        if (symbol)
            trailName = symbol->EntryName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        {
            long flags = Flags;
            context.Transfer("SpriteFlags", flags, 0L);
            Flags = flags;
        }
    } else {
        CCharString spriteName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        if (info.GraphicSymbols.SymbolExists(spriteName))
            SpriteBankIndex = info.GraphicSymbols.GetSymbolSLONG(spriteName);
        else
            SpriteBankIndex = 0;
        CCharString trailName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        if (info.GraphicSymbols.SymbolExists(trailName))
            TrailBankIndex = info.GraphicSymbols.GetSymbolSLONG(trailName);
        else
            TrailBankIndex = 0;
        long flags = Flags;
        context.Transfer("SpriteFlags", flags, -1L);
        if (flags == -1) {
            bool flag;
            context.Transfer("FaceMe2D", flag, false);
            Flags = 0;
            context.Transfer("FaceMe3D", flag, false);
            Flags |= flag ? 1 : 0;
            context.Transfer("CrossedSprites", flag, false);
            Flags |= flag ? 0x40 : 0;
            context.Transfer("RotateAroundCentre", flag, false);
            Flags |= flag ? 0x20 : 0;
            context.Transfer("SelfIlluminating", flag, false);
            Flags |= flag ? 0 : 0x80;
            long alignment = 0;
            context.Transfer("SpriteAlignment", alignment, 0L);
            Flags |= alignment;
        } else
            Flags = flags;
    }
    {
        bool forceAnimationTime = ForceAnimationTime;
        context.Transfer("ForceAnimationTime", forceAnimationTime, false);
        ForceAnimationTime = forceAnimationTime;
    }
    {
        bool useStartColour = UseStartColour;
        context.Transfer("UseStartColour", useStartColour, true);
        UseStartColour = useStartColour;
    }
    {
        bool useMidColour = UseMidColour;
        context.Transfer("UseMidColour", useMidColour, false);
        UseMidColour = useMidColour;
    }
    {
        bool useEndColour = UseEndColour;
        context.Transfer("UseEndColour", useEndColour, false);
        UseEndColour = useEndColour;
    }
    context.Transfer("StartColour", StartColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("MidColour", MidColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("EndColour", EndColour, CRGBColour(255, 255, 255, 255));
    {
        long blendMode = BlendMode;
        context.Transfer("BlendMode", blendMode, GetParticleEmitterManager().GetDefaultBlendMode());
        BlendMode = blendMode;
    }
    {
        long blendOp = BlendOp;
        context.Transfer("BlendOp", blendOp, GetParticleEmitterManager().GetDefaultBlendOp());
        BlendOp = blendOp;
    }
    {
        long trailBlendMode = TrailBlendMode;
        context.Transfer("TrailBlendMode", trailBlendMode, GetParticleEmitterManager().GetDefaultBlendMode());
        TrailBlendMode = trailBlendMode;
    }
    {
        long trailBlendOp = TrailBlendOp;
        context.Transfer("TrailBlendOp", trailBlendOp, GetParticleEmitterManager().GetDefaultBlendOp());
        TrailBlendOp = trailBlendOp;
    }
    {
        long noCrossedSprites = NoCrossedSprites;
        context.Transfer("NoCrossedSprites", noCrossedSprites, 0L);
        NoCrossedSprites = noCrossedSprites;
    }
    {
        long fadeInEndInteger = FadeInEndInteger;
        context.Transfer("FadeInEndInteger", fadeInEndInteger, 0L);
        FadeInEndInteger = fadeInEndInteger;
    }
    {
        long fadeOutBeginInteger = FadeOutBeginInteger;
        context.Transfer("FadeOutBeginInteger", fadeOutBeginInteger, 0L);
        FadeOutBeginInteger = fadeOutBeginInteger;
    }
    {
        bool alphaFadeEnable = AlphaFadeEnable;
        context.Transfer("AlphaFadeEnable", alphaFadeEnable, false);
        AlphaFadeEnable = alphaFadeEnable;
    }
    {
        bool sizeFadeEnable = SizeFadeEnable;
        context.Transfer("SizeFadeEnable", sizeFadeEnable, false);
        SizeFadeEnable = sizeFadeEnable;
    }
    {
        bool flickerEnable = FlickerEnable;
        context.Transfer("FlickerEnable", flickerEnable, false);
        FlickerEnable = flickerEnable;
    }
    {
        long flickerMinAlphaInteger = FlickerMinAlphaInteger;
        context.Transfer("FlickerMinAlphaInteger", flickerMinAlphaInteger, 0L);
        FlickerMinAlphaInteger = flickerMinAlphaInteger;
    }
    {
        long flickerMinSizeInteger = FlickerMinSizeInteger;
        context.Transfer("FlickerMinSizeInteger", flickerMinSizeInteger, 0L);
        FlickerMinSizeInteger = flickerMinSizeInteger;
    }
    {
        long trailLengthInteger = TrailLengthInteger;
        context.Transfer("TrailLengthInteger", trailLengthInteger, 0L);
        TrailLengthInteger = trailLengthInteger;
    }
    {
        float alphaFadeMinimum = GFRoundFloatNear(AlphaFadeMinimum / 127.0f, 2);
        context.Transfer("AlphaFadeMinimum", alphaFadeMinimum, 1.0f);
        AlphaFadeMinimum = GFPackFloat(alphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    }
    {
        float sizeFadeMinimum = GFRoundFloatNear(SizeFadeMinimum / 127.0f, 2);
        context.Transfer("SizeFadeMinimum", sizeFadeMinimum, 1.0f);
        SizeFadeMinimum = GFPackFloat(sizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    }
    {
        float animationTimeSecs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
        context.Transfer("AnimationTimeSecs", animationTimeSecs, 1.0f);
        AnimationTimeSecs = GFPackFloat(animationTimeSecs, 14, 0.1f, 100.0f, 2);
    }
    {
        float startRenderSize = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
        context.Transfer("StartRenderSize", startRenderSize, 1.0f);
        StartRenderSize = GFPackFloat(startRenderSize, 11, 0.0f, 20.0f, 2);
    }
    {
        float endRenderSize = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
        context.Transfer("EndRenderSize", endRenderSize, 1.0f);
        EndRenderSize = GFPackFloat(endRenderSize, 11, 0.0f, 20.0f, 2);
    }
    {
        float flickerSpeed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
        context.Transfer("FlickerSpeed", flickerSpeed, 15.0f);
        FlickerSpeed = GFPackFloat(flickerSpeed, 12, 0.0f, 30.0f, 2);
    }
    {
        float flickerBias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
        context.Transfer("FlickerBias", flickerBias, 0.0f);
        FlickerBias = GFPackFloat(flickerBias, 8, -1.0f, 1.0f, 2);
    }
    {
        float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
        context.Transfer("TrailWidth", trailWidth, 0.5f);
        TrailWidth = GFPackFloat(trailWidth, 10, 0.0f, 10.0f, 2);
    }
}
}
