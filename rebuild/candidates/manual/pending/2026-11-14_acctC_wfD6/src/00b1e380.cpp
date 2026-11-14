// NParticleEngine::CPSCDecalRenderer::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: save / load the decal renderer's emitter-database fields (decal graphic name through the symbol map, the legacy per-flag booleans when SpriteFlags is absent, every packed float unpacked, transferred and packed again)
// Grade: RELOCATION_MATCH
// Retail-asm: shared GFFloatToLongNear (GFRoundFloatNear / GFPackFloat / GFUnpackFloat) implements the original dword FPU rounding
//   (`fistp dword ptr` at 0x00B1E9CB, 0x00B1EA1D, 0x00B1EA37 and in every other pack / unpack); portable branch retained in the existing header; no new machine-code body.
// Opt-ins: FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT + FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT (existing GFPackFloat.h texts; the exact
//   pair the landed sibling CPSCEmitterGeneric::DatabaseSerialise 0x00B13530 defines). The grouped product adds a no-code tuple per pack:
//   that moves c2's 81-tuple scheduling cut so the PoolIncreaseSizeEnable merge (0x00B1F0D6, +0x34 bit 6) and the forwarded
//   PoolIncreaseLifeEnable read share retail's region (member load first, merged store before `shr al,7`).
// Leaf: ?DatabaseSerialise@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1E380, 3698 bytes, thiscall, `ret 8`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CPersistContext.h"
#include "engine/CSymbolMap.h"
#include "engine/CParticleEmitter.h"
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#define FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
#include "engine/GFPackFloat.h"
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND
#include "engine/CParticleEmitterManager.h"
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
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0
}
#include "engine/CPSCDecalRenderer.h"
namespace NParticleEngine {
void CPSCDecalRenderer::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    context.Transfer("DecalLifeSecs", DecalLifeSecs, 0.0f);
    if (context.IsSaving()) {
        CCharString spriteName;
        const CSymbolMap::CSymbolInfo* symbol = info.GraphicSymbols.FindFirstSymbolWithVal("EEngineGraphic", DecalBankIndex);
        if (symbol)
            spriteName = symbol->EntryName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        {
            long spriteFlags = Flags;
            context.Transfer("SpriteFlags", spriteFlags, 0L);
            Flags = spriteFlags;
        }
    } else {
        CCharString spriteName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        if (info.GraphicSymbols.SymbolExists(spriteName))
            DecalBankIndex = info.GraphicSymbols.GetSymbolSLONG(spriteName);
        else
            DecalBankIndex = 0;
        long spriteFlags = Flags;
        context.Transfer("SpriteFlags", spriteFlags, -1L);
        if (spriteFlags == -1) {
            bool legacyFlag;
            context.Transfer("FaceMe2D", legacyFlag, false);
            Flags = 0;
            context.Transfer("FaceMe3D", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 1 : 0);
            context.Transfer("CrossedSprites", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0x40 : 0);
            context.Transfer("RotateAroundCentre", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0x20 : 0);
            context.Transfer("SelfIlluminating", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0 : 0x80);
            long spriteAlignment = 0;
            context.Transfer("SpriteAlignment", spriteAlignment, 0L);
            Flags |= spriteAlignment;
        } else {
            Flags = spriteFlags;
        }
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
        bool poolingEnable = PoolingEnable;
        context.Transfer("PoolingEnable", poolingEnable, false);
        PoolingEnable = poolingEnable;
    }
    {
        bool poolIncreaseAlphaEnable = PoolIncreaseAlphaEnable;
        context.Transfer("PoolIncreaseAlphaEnable", poolIncreaseAlphaEnable, false);
        PoolIncreaseAlphaEnable = poolIncreaseAlphaEnable;
    }
    {
        bool poolIncreaseSizeEnable = PoolIncreaseSizeEnable;
        context.Transfer("PoolIncreaseSizeEnable", poolIncreaseSizeEnable, false);
        PoolIncreaseSizeEnable = poolIncreaseSizeEnable;
    }
    {
        bool poolIncreaseLifeEnable = PoolIncreaseLifeEnable;
        context.Transfer("PoolIncreaseLifeEnable", poolIncreaseLifeEnable, false);
        PoolIncreaseLifeEnable = poolIncreaseLifeEnable;
    }
    context.Transfer("MaxPoolSize", MaxPoolSize, 0.0f);
    context.Transfer("MaxPoolAlpha", MaxPoolAlpha, 0.0f);
    context.Transfer("MaxPoolLife", MaxPoolLife, 0.0f);
    {
        bool poolIncreaseFrameEnable = PoolIncreaseFrameEnable;
        context.Transfer("PoolIncreaseFrameEnable", poolIncreaseFrameEnable, false);
        PoolIncreaseFrameEnable = poolIncreaseFrameEnable;
    }
    context.Transfer("PoolFrameIncreaseRate", PoolFrameIncreaseRate, 0.0f);
    context.Transfer("StencilCubeWidth", StencilCubeWidth, 0.0f);
}
}
