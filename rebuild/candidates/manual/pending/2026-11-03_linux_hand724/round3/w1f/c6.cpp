// NParticleEngine::CPSCDecalRenderer::PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
//   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) -- vtable slot 13.
// Retail 0x00B1F200, 3195 bytes, `ret 14h`, biased-ebp frame (`push ebp; lea ebp,[esp-64h]; sub esp,160h`).
// Leaf: ?PreparePrimitives@CPSCDecalRenderer@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#include <math.h>
#include "fable_string.h"
#include "engine/GFRoundFloatNear.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFastFloatToLong.h"

#include "engine/NParticleEngine_CPSCDecalRenderer.h"
#include "engine/CParticleSystem.h"
#include "engine/CDecal.h"
#include "engine/CIEngine.h"
#include "engine/C2DVector.h"

class CEnginePrimitiveThemeChannelDesc { public:            // 4 bytes (PDB; the landed CEnginePrimitiveParticleDecal ctor 0x00B1FE80)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                             // 12 bytes (PDB)
    long Type; long InterpolationSet; CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};
class CEnginePrimitiveParticleDecalGroup : public CEnginePrimitive { public:   // FableWin ??0CEnginePrimitiveParticleDecalGroup@@QAE@J_NW4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@MK@Z
    long DecalBankIndex; unsigned short Flags; long BlendMode; long BlendOp; float StencilCubeWidth; bool LandscapeDecals;
    CEnginePrimitiveParticleDecalGroup(long bank, bool landscape, long blendMode, long blendOp, float stencilCubeWidth, unsigned long flags)
        : CEnginePrimitive(0x1e), DecalBankIndex(bank), Flags((unsigned short)flags), BlendMode(blendMode), BlendOp(blendOp),
          StencilCubeWidth(stencilCubeWidth), LandscapeDecals(landscape) { }
};
class CEnginePrimitiveParticleDecal : public CEnginePrimitive { public:
    C3DVector WorldPos; C2DVector Size; CRGBColour Colour; long DecalFrame; CRightHandedSet RHSet;
    CEnginePrimitiveParticleDecal(long decalFrame, const C3DVector& worldPos, const C2DVector& size, const CRGBColour& colour,
        const CRightHandedSet& rhSet);                       // 0x00B1FE80
};
class CEnginePrimitivePrepareInfo { public:                  // Ego_r: Interpolate (CInterpolationInfo) +0, Engine +0x34
    unsigned char Interpolate[0x34]; CIEngine* Engine;
};
class CDecalBankFrameInfo { public: unsigned char _pad[6]; unsigned short Height; unsigned short Width; };   // unproven
class CDecalBankEntry { public: unsigned char _pad[0x28]; struct { unsigned char _pad[0xc]; CDecalBankFrameInfo* Frame; }* Data; unsigned long _pad2; };
class CDecalBankSource {                                    // the object behind global 0x013D2E08 (unproven identity)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual long GetNumberOfFrames(long bank);               // slot 20 (+0x50)
    unsigned char _pad04[0x1e0 - 4];
    std::vector<CDecalBankEntry> Entries;                    // +0x1e0 (0x2c-byte entries)
};
CDecalBankSource* GFGetPreMainMemoryUsed();                   // 0x00AEAAA0 (landed label; returns the global 0x013D2E08)
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
extern const float GFSinTable[];                             // 0x013CD550 (1024+1 entries, unproven name)
class CPerlinNoise { public: float GetGaussian2DNoise(const C2DVector&); };      // 0x00F36540
namespace NParticleEngine {
long GetParticleUpdateRate();                                // 0x00AEA9F0
float GetMaxDrawDistance();                                  // 0x00AEAA00 (landed label; 1/15)
class CParticleEmitterManager { public: unsigned char _pad[8]; CPerlinNoise Noise; };
CParticleEmitterManager& __fastcall GetParticleEmitterManager();   // 0x00AEAAF0
}
static __forceinline float CosineFade(float t)              // 0.5 - 0.5*table lerp (inline in retail)
{
    float scaled = t * 0.5f * 1024.0f;
    long index = GFFloatToLongNear(scaled - 0.5f);
    float frac = scaled - index;
    index &= 0x3ff;
    return 0.5f - ((1.0f - frac) * GFSinTable[index] + frac * GFSinTable[index + 1]) * 0.5f;
}

namespace NParticleEngine {
// TU-local stand-in for the inline test retail expands (outer size, inner size > 0, then the id kept while `setge` tests it).
static __forceinline bool GetFirstPrimID(const CDecal& decal, long& primID)
{
    if (decal.PrimIDList.size() && decal.PrimIDList[0].size() > 0) {
        primID = decal.PrimIDList[0][0];
        return primID >= 0;
    }
    return false;
}
void CPSCDecalRenderer::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
    const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle& handle)
{
    if (!DecalBankIndex) {
        ClearAllPrimitives(info.Engine, handle);
        return;
    }
    float alphaFadeMinimum = GFRoundFloatNear(AlphaFadeMinimum * (1.0f / 127.0f), 2);
    float sizeFadeMinimum = GFRoundFloatNear(SizeFadeMinimum * (1.0f / 127.0f), 2);
    float animationTimeSecs = GFRoundFloatNear((AnimationTimeSecs * (1.0f / 16383.0f)) * (100.0f - 0.1f) + 0.1f, 2);
    float startRenderSize = GFRoundFloatNear((StartRenderSize * (1.0f / 2047.0f)) * 20.0f, 2);
    float endRenderSize = GFRoundFloatNear((EndRenderSize * (1.0f / 2047.0f)) * 20.0f, 2);
    float flickerSpeed = GFRoundFloatNear((FlickerSpeed * (1.0f / 4095.0f)) * 30.0f, 2);
    float flickerBias = GFRoundFloatNear((FlickerBias * (1.0f / 255.0f)) * 2.0f - 1.0f, 2);
    GetParticleUpdateRate();
    float frameTime = GetMaxDrawDistance();
    CDecalBankSource* bank = GFGetPreMainMemoryUsed();
    float fadeInEnd = FadeInEndInteger * 0.001f;
    float fadeOutBegin = FadeOutBeginInteger * 0.001f;
    float fadeAlpha = system.GetFadeAlpha();
    float inverseAnimationTime = 1.0f / animationTimeSecs;
    float inverseFadeIn = 1.0f / fadeInEnd;
    float inverseFadeOut = 1.0f / (1.0f - fadeOutBegin);
    float flickerMinAlpha = FlickerMinAlphaInteger * 0.001f;
    float flickerMinSize = FlickerMinSizeInteger * 0.001f;
    float flickerRange = fabs(flickerBias) + 0.5f;
    float flickerCentre = flickerBias + 0.5f;
    float flickerStep = frameTime * flickerSpeed;
    float alphaFadeScale, alphaFadeBase;
    if (AlphaFadeEnable) { alphaFadeScale = 1.0f - alphaFadeMinimum; alphaFadeBase = alphaFadeMinimum; }
    else { alphaFadeScale = 0.0f; alphaFadeBase = 1.0f; }
    float sizeFadeScale, sizeFadeBase;
    if (SizeFadeEnable) { sizeFadeScale = 1.0f - sizeFadeMinimum; sizeFadeBase = sizeFadeMinimum; }
    else { sizeFadeScale = 0.0f; sizeFadeBase = 1.0f; }
    long blendMode = BlendMode;
    long blendOp = BlendOp;
    long numberOfFrames = 0;
    float numberOfFramesF = 0.0f;
    float aspect = 0.0f;
    CPerlinNoise& noise = GetParticleEmitterManager().Noise;
    float flickerAlphaRange = 1.0f - flickerMinAlpha;
    float flickerSizeRange = 1.0f - flickerMinSize;
    if (DecalBankIndex) {
        numberOfFrames = bank->GetNumberOfFrames(DecalBankIndex);
        numberOfFramesF = (float)numberOfFrames;
        CDecalBankEntry* entry = (unsigned long)DecalBankIndex < bank->Entries.size() ? &bank->Entries[DecalBankIndex] : 0;
        CDecalBankFrameInfo* frame = entry->Data->Frame;
        aspect = (float)frame->Width / (long)frame->Height;
        CEnginePrimitiveParticleDecalGroup landscape(DecalBankIndex, true, blendMode, blendOp, StencilCubeWidth, Flags);
        info.Engine->SetChildPrimitive(handle, LandscapeDecalsChildPrimIndex, LandscapeDecalsPrimHandle, landscape, 0);
        CEnginePrimitiveParticleDecalGroup mesh(DecalBankIndex, false, blendMode, blendOp, StencilCubeWidth, Flags);
        info.Engine->SetChildPrimitive(handle, MeshDecalsChildPrimIndex, MeshDecalsPrimHandle, mesh, 0);
    }
    long lastFrame = numberOfFrames - 1;
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 0x80);
    colours[0].A = (unsigned char)(colours[0].A * fadeAlpha);
    colours[1].A = (unsigned char)(colours[1].A * fadeAlpha);
    colours[2].A = (unsigned char)(colours[2].A * fadeAlpha);
    std::list<CDecal*>& decals = system.GetDecalList();
    for (std::list<CDecal*>::iterator it = decals.begin(); it != decals.end(); ++it) {
        CDecal* decal = *it;
        float age = decal->Lifetimer + time;
        long maxLife = decal->MaxLife;
        if (maxLife > -1 && decal->Active) {
            float lifeFraction = decal->MinLife > age / maxLife ? decal->MinLife : (age / maxLife > 1.0f ? 1.0f : age / maxLife);
            decal->MinLife = lifeFraction;
            unsigned long colourPos = GFFloatToLongNear(lifeFraction * 511.0f);
            CRGBColour colour = GFGetInterpolatedColour(colours[colourPos >> 8], colours[(colourPos >> 8) + 1], colourPos & 0xff);
            float globalAlpha = system.GetGlobalAlpha();
            if (lifeFraction < fadeInEnd)
                globalAlpha *= CosineFade(lifeFraction * inverseFadeIn);
            if (lifeFraction > fadeOutBegin)
                globalAlpha *= CosineFade((1.0f - lifeFraction) * inverseFadeOut);
            float alphaFactor = globalAlpha * alphaFadeScale + alphaFadeBase;
            float sizeFactor = globalAlpha * sizeFadeScale + sizeFadeBase;
            if (FlickerEnable) {
                C2DVector noisePos(age * flickerStep + decal->FlickerTimeOffset, decal->FlickerIndex);
                float flicker = noise.GetGaussian2DNoise(noisePos) * flickerRange + flickerCentre;
                if (flicker < 0.0f) flicker = 0.0f; else if (flicker > 1.0f) flicker = 1.0f;
                alphaFactor *= flicker * flickerAlphaRange + flickerMinAlpha;
                sizeFactor *= flicker * flickerSizeRange + flickerMinSize;
            }
            if (PoolingEnable && PoolIncreaseAlphaEnable) {
                float maxAlpha = MaxPoolAlpha * 255.0f;
                float alpha = ((decal->AlphaFactor[0] - decal->AlphaFactor[1]) * time + decal->AlphaFactor[1]) * colour.A * alphaFactor;
                colour.A = (unsigned char)GFFloatToLongNear(std::min(alpha, maxAlpha));
            } else
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * alphaFactor);
            if (colour.A) {
                float frameFraction = lifeFraction;
                if (ForceAnimationTime)
                    frameFraction = (float)fmod((double)(inverseAnimationTime * frameTime * age), 1.0);
                long frame;
                if (PoolingEnable && PoolIncreaseFrameEnable)
                    frame = GFFastFloatToLong(decal->GraphicFrame);
                else
                    frame = GFFloatToLongFloor(frameFraction * numberOfFramesF);
                frame = std::min(frame, lastFrame);
                C2DVector size;
                if (PoolingEnable && PoolIncreaseSizeEnable) {
                    float poolSize = ((decal->SizeFactor[0] - decal->SizeFactor[1]) * time + decal->SizeFactor[1])
                        * ((endRenderSize - startRenderSize) * frameFraction + startRenderSize) * sizeFactor;
                    size.X = std::min(poolSize, MaxPoolSize);
                } else
                    size.X = ((endRenderSize - startRenderSize) * frameFraction + startRenderSize) * sizeFactor;
                size.Y = size.X * aspect;
                system.ScaleParticleDimensions(size);
                if (size.X > 0.0f && size.Y > 0.0f) {
                    CEnginePrimitiveHandle* parent = decal->LandscapeOnlyDecal ? &LandscapeDecalsPrimHandle : &MeshDecalsPrimHandle;
                    long primID;
                    if (GetFirstPrimID(*decal, primID)) {
                        info.Engine->SetChildPrimitive(*parent, primID,

                            CEnginePrimitiveParticleDecal(frame, decal->Position, size, colour, decal->RHSet), 0);
                    } else {
                        unsigned long id = info.Engine->AddChildPrimitive(*parent,
                            CEnginePrimitiveParticleDecal(frame, decal->Position, size, colour, decal->RHSet), 0);
                        if (!decal->PrimIDList.size()) decal->PrimIDList.resize(1, CArray<long>());
                        if (!decal->PrimIDList[0].size()) decal->PrimIDList[0].resize(1, 0);
                        decal->PrimIDList[0][0] = id;
                        if (!decal->ParentHandleList.size()) decal->ParentHandleList.resize(1);
                        decal->ParentHandleList[0] = *parent;

                    }
                }
            }
        } else if (maxLife < age)
            system.ClearDecal(it);
    }
}
} // namespace NParticleEngine
