// NParticleEngine::CPSCDecalRenderer::WriteBinary(CDataOutputStream*) -- serialize the decal renderer: life, bank, the three colours, the packed bit-field integers and flags, the pool floats and the stencil width
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1BA10, 610 bytes (vtable slot 16 of CPSCDecalRenderer, 0x012A09BC).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteFloat 0x00993EE0, WriteLong
//   0x00993EB0, WriteColour(const CRGBColour&) 0x00993F30, WriteULong 0x00993EA0 (propagated label "WriteCBYTE"),
//   WriteEBOOL 0x00993E30.
// Layout: PDB (Ego_r) CPSCDecalRenderer names/types; retail offsets equal the PDB: unsigned long bit-field words at
//   +0x20 (BlendMode:3 BlendOp:3 Flags:10 FadeInEndInteger:10), +0x24, +0x28, +0x2c (three colour flags, FlickerBias:8,
//   two 10-bit flicker integers), +0x30 FlickerSpeed:12; unsigned char flag bits at +0x34/+0x35.
// Shape: one stream call per member in the original's write order (the flicker integers are written alpha, size,
//   bias, speed, unlike their declaration order); VC7.1 picks byte/word loads for fields at byte boundaries.
#include "engine/CDataOutputStream.h"
class CRGBColour { public: union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                        // 0x00993E30
    void WriteLong(long value);                         // 0x00993EB0
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteColour(const CRGBColour& value);          // 0x00993F30
};
class CEnginePrimitiveHandle { public: long Index; long Generation; };
namespace NParticleEngine {
class CPSCDecalRenderer { public:
    unsigned char _base[0xc];                           // CPSCBase
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
    void WriteBinary(CDataOutputStream* out);
};
void CPSCDecalRenderer::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteFloat(DecalLifeSecs);
    out->WriteLong(DecalBankIndex);
    out->WriteColour(StartColour);
    out->WriteColour(MidColour);
    out->WriteColour(EndColour);
    out->WriteULong(BlendMode);
    out->WriteULong(BlendOp);
    out->WriteULong(Flags);
    out->WriteULong(FadeInEndInteger);
    out->WriteULong(FadeOutBeginInteger);
    out->WriteULong(StartRenderSize);
    out->WriteULong(AlphaFadeMinimum);
    out->WriteULong(EndRenderSize);
    out->WriteULong(AnimationTimeSecs);
    out->WriteULong(SizeFadeMinimum);
    out->WriteEBOOL(UseStartColour);
    out->WriteEBOOL(UseMidColour);
    out->WriteEBOOL(UseEndColour);
    out->WriteEBOOL(AlphaFadeEnable);
    out->WriteEBOOL(SizeFadeEnable);
    out->WriteEBOOL(ForceAnimationTime);
    out->WriteULong(FlickerMinAlphaInteger);
    out->WriteULong(FlickerMinSizeInteger);
    out->WriteULong(FlickerBias);
    out->WriteULong(FlickerSpeed);
    out->WriteEBOOL(FlickerEnable);
    out->WriteEBOOL(PoolingEnable);
    out->WriteEBOOL(PoolIncreaseAlphaEnable);
    out->WriteEBOOL(PoolIncreaseSizeEnable);
    out->WriteEBOOL(PoolIncreaseLifeEnable);
    out->WriteFloat(MaxPoolSize);
    out->WriteFloat(MaxPoolAlpha);
    out->WriteFloat(MaxPoolLife);
    out->WriteEBOOL(PoolIncreaseFrameEnable);
    out->WriteFloat(PoolFrameIncreaseRate);
    out->WriteFloat(StencilCubeWidth);
}
}