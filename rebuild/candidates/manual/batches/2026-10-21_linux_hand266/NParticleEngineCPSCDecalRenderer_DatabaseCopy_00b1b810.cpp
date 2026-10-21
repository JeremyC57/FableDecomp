// NParticleEngine::CPSCDecalRenderer::DatabaseCopy(const CPSCBase* other) -- copy another decal-renderer component's settings field by field: life, bank, the packed blend/fade/size/animation/flicker bit-fields, the colours, the pooling flags and limits, the stencil cube width
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1B810, length 512 bytes (509 + int3 padding), `ret 4`. No callees.
// Layout: Ego_r.pdb CPSCDecalRenderer (retail offsets equal the PDB): DecalLifeSecs +0x0c, DecalBankIndex +0x10, colours
//   +0x14/+0x18/+0x1c, ulong bit-field words +0x20 (BlendMode:3, BlendOp:3, Flags:10, FadeInEndInteger:10), +0x24
//   (FadeOutBeginInteger:10, StartRenderSize:11, AlphaFadeMinimum:7), +0x28 (EndRenderSize:11, AnimationTimeSecs:14,
//   SizeFadeMinimum:7), +0x2c (three Use*Colour bits, FlickerBias:8, FlickerMinAlphaInteger:10, FlickerMinSizeInteger:10),
//   +0x30 (FlickerSpeed:12), uchar flag bytes +0x34 (eight enables) / +0x35 (PoolIncreaseFrameEnable), pool limits
//   +0x38..+0x44, StencilCubeWidth +0x48.
// Shape: one bit-field assignment per setting in the editor's order (not declaration order); each compiles to VC7.1's
//   `xor ; and mask ; xor` merge, and a field ending at the top bit of its unit uses the `& ~mask ^ src` form.
namespace NParticleEngine {
struct CRGBColour { unsigned long Colour; };
struct CParticleSystem;
struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
};
struct CPSCDecalRenderer : CPSCBase {
    float         DecalLifeSecs;                        // +0x0c
    long          DecalBankIndex;                       // +0x10
    CRGBColour    StartColour;                          // +0x14
    CRGBColour    MidColour;                            // +0x18
    CRGBColour    EndColour;                            // +0x1c
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
    float         MaxPoolSize;                          // +0x38
    float         MaxPoolAlpha;                         // +0x3c
    float         MaxPoolLife;                          // +0x40
    float         PoolFrameIncreaseRate;                // +0x44
    float         StencilCubeWidth;                     // +0x48
    void DatabaseCopy(const CPSCBase* other);
};

void CPSCDecalRenderer::DatabaseCopy(const CPSCBase* other)
{
    const CPSCDecalRenderer* source = (const CPSCDecalRenderer*)other;
    DecalLifeSecs = source->DecalLifeSecs;
    DecalBankIndex = source->DecalBankIndex;
    ForceAnimationTime = source->ForceAnimationTime;
    AnimationTimeSecs = source->AnimationTimeSecs;
    StartRenderSize = source->StartRenderSize;
    EndRenderSize = source->EndRenderSize;
    BlendMode = source->BlendMode;
    BlendOp = source->BlendOp;
    FadeInEndInteger = source->FadeInEndInteger;
    FadeOutBeginInteger = source->FadeOutBeginInteger;
    AlphaFadeEnable = source->AlphaFadeEnable;
    AlphaFadeMinimum = source->AlphaFadeMinimum;
    SizeFadeEnable = source->SizeFadeEnable;
    SizeFadeMinimum = source->SizeFadeMinimum;
    UseStartColour = source->UseStartColour;
    UseMidColour = source->UseMidColour;
    UseEndColour = source->UseEndColour;
    StartColour = source->StartColour;
    MidColour = source->MidColour;
    EndColour = source->EndColour;
    Flags = source->Flags;
    FlickerEnable = source->FlickerEnable;
    FlickerSpeed = source->FlickerSpeed;
    FlickerBias = source->FlickerBias;
    FlickerMinAlphaInteger = source->FlickerMinAlphaInteger;
    FlickerMinSizeInteger = source->FlickerMinSizeInteger;
    PoolingEnable = source->PoolingEnable;
    PoolIncreaseAlphaEnable = source->PoolIncreaseAlphaEnable;
    PoolIncreaseSizeEnable = source->PoolIncreaseSizeEnable;
    PoolIncreaseLifeEnable = source->PoolIncreaseLifeEnable;
    MaxPoolSize = source->MaxPoolSize;
    MaxPoolAlpha = source->MaxPoolAlpha;
    MaxPoolLife = source->MaxPoolLife;
    PoolIncreaseFrameEnable = source->PoolIncreaseFrameEnable;
    PoolFrameIncreaseRate = source->PoolFrameIncreaseRate;
    StencilCubeWidth = source->StencilCubeWidth;
}
}
