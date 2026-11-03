// Grade: DIFFER(3698v3698) -- 11 diff lines: SizeFadeMinimum (+0x28 bits 25-31) store picks edx for the old word (and edx,1FFFFFFh after shl) where retail picks eax (1-byte shorter and), which shifts the following GFRoundFloatNear loop alignment padding (jmp+lea ebx vs lea esp); PoolIncreaseSizeEnable merge reads the new byte before [esi+34h] and stores after shr al,7 (register scheduling).
// NParticleEngine::CPSCDecalRenderer::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17
// Leaf: ?DatabaseSerialise@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CSymbolMap.h"
#include "engine/GFRoundFloatNear.h"
#include "engine/NParticleEngine_CPSCDecalRenderer.h"
#include "engine/NParticleEngine_CEmitterDatabaseSerialiseInfo.h"
#include "engine/NParticleEngine_CParticleEmitterManager.h"

namespace NParticleEngine {
void CPSCDecalRenderer::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    context.Transfer("DecalLifeSecs", DecalLifeSecs, 0.0f);
    if (context.IsSaving()) {
        CCharString spriteName;
        const CSymbolMap::CSymbolInfo* symbol = info.GraphicSymbols->FindFirstSymbolWithVal("EEngineGraphic", DecalBankIndex);
        if (symbol)
            spriteName = symbol->EntryName;
        context.Transfer("SpriteName", spriteName);
        {
            long spriteFlags = Flags;
            context.Transfer("SpriteFlags", spriteFlags);
            Flags = spriteFlags;
        }
    } else {
        CCharString spriteName;
        context.Transfer("SpriteName", spriteName);
        if (info.GraphicSymbols->SymbolExists(spriteName))
            DecalBankIndex = info.GraphicSymbols->GetSymbolSLONG(spriteName);
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
            context.Transfer("SpriteAlignment", spriteAlignment);
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
        float alphaFadeMinimum = GFRoundFloatNear((AlphaFadeMinimum * (1.0f / 127.0f)) * (1.0f - 0.0f) + 0.0f, 2);
        context.Transfer("AlphaFadeMinimum", alphaFadeMinimum, 1.0f);
        AlphaFadeMinimum = GFFloatToLongNear(((GFRoundFloatNear(alphaFadeMinimum, 2) - 0.0f) * (1.0f / (1.0f - 0.0f))) * 127.0f);
    }
    {
        float sizeFadeMinimum = GFRoundFloatNear((SizeFadeMinimum * (1.0f / 127.0f)) * (1.0f - 0.0f) + 0.0f, 2);
        context.Transfer("SizeFadeMinimum", sizeFadeMinimum, 1.0f);
        SizeFadeMinimum = GFFloatToLongNear(((GFRoundFloatNear(sizeFadeMinimum, 2) - 0.0f) * (1.0f / (1.0f - 0.0f))) * 127.0f);
    }
    {
        float animationTimeSecs = GFRoundFloatNear((AnimationTimeSecs * (1.0f / 16383.0f)) * (100.0f - 0.1f) + 0.1f, 2);
        context.Transfer("AnimationTimeSecs", animationTimeSecs, 1.0f);
        AnimationTimeSecs = GFFloatToLongNear(((GFRoundFloatNear(animationTimeSecs, 2) - 0.1f) * (1.0f / (100.0f - 0.1f))) * 16383.0f);
    }
    {
        float startRenderSize = GFRoundFloatNear((StartRenderSize * (1.0f / 2047.0f)) * (20.0f - 0.0f) + 0.0f, 2);
        context.Transfer("StartRenderSize", startRenderSize, 1.0f);
        StartRenderSize = GFFloatToLongNear(((GFRoundFloatNear(startRenderSize, 2) - 0.0f) * (1.0f / (20.0f - 0.0f))) * 2047.0f);
    }
    {
        float endRenderSize = GFRoundFloatNear((EndRenderSize * (1.0f / 2047.0f)) * (20.0f - 0.0f) + 0.0f, 2);
        context.Transfer("EndRenderSize", endRenderSize, 1.0f);
        EndRenderSize = GFFloatToLongNear(((GFRoundFloatNear(endRenderSize, 2) - 0.0f) * (1.0f / (20.0f - 0.0f))) * 2047.0f);
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
        float flickerSpeed = GFRoundFloatNear((FlickerSpeed * (1.0f / 4095.0f)) * (30.0f - 0.0f) + 0.0f, 2);
        context.Transfer("FlickerSpeed", flickerSpeed, 15.0f);
        FlickerSpeed = GFFloatToLongNear(((GFRoundFloatNear(flickerSpeed, 2) - 0.0f) * (1.0f / (30.0f - 0.0f))) * 4095.0f);
    }
    {
        float flickerBias = GFRoundFloatNear((FlickerBias * (1.0f / 255.0f)) * (1.0f - -1.0f) + -1.0f, 2);
        context.Transfer("FlickerBias", flickerBias, 0.0f);
        FlickerBias = GFFloatToLongNear(((GFRoundFloatNear(flickerBias, 2) - -1.0f) * (1.0f / (1.0f - -1.0f))) * 255.0f);
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
