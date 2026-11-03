#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPSCDECALRENDERER_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPSCDECALRENDERER_H
// New header (chunk 724 round 3): NParticleEngine::CPSCDecalRenderer (RTTI vtable 0x012A09BC), the decal particle renderer.
// Layout: Ego_r TPI CPSCDecalRenderer, size 0x64; retail offsets equal the PDB (no STL members): DecalLifeSecs +0x0C,
//   DecalBankIndex +0x10, Start/Mid/EndColour +0x14/+0x18/+0x1C, the unsigned long bit-field words +0x20..+0x30, the
//   unsigned char flag bits +0x34/+0x35, the pool floats +0x38..+0x44, StencilCubeWidth +0x48, the two primitive handles and
//   child indices +0x4C..+0x60 (the landed WriteBinary 0x00B1BA10 / DatabaseCopy 0x00B1B810 and the chunk 724 InterpolateTo
//   0x00B1BDA0 / ReadBinary 0x00B1C880 models, which keep a local copy of this layout).
// Needs FABLE_RETAIL_DEF_CLASS_BASE and FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE (engine/CEnginePrimitiveHandle.h).
#include "engine/NParticleEngine_CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
namespace NParticleEngine {
class CPSCDecalRenderer : public CPSCBase {
public:
    CPSCDecalRenderer();
    virtual ~CPSCDecalRenderer();
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
        const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);    // slot 13 0x00B1F200
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);          // slot 17 0x00B1E380
    virtual void ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&);                              // slot 29 0x00B1DBE0
protected:
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
};
} // namespace NParticleEngine
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCDecalRenderer) == 0x64);
#endif
