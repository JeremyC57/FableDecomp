// NParticleEngine::CPSCDecalRenderer::ReadBinary(CDataInputStream&) -- read the decal renderer back (the mirror of WriteBinary 0x00B1BA10)
// Leaf: ?ReadBinary@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Grade: DIFFER(3266v3266) -- 4 lines: the top 7-bit field SizeFadeMinimum (+0x28 bits 25-31) merge; retail `shl eax,19h ; and ecx,1FFFFFFh ; or eax,ecx ; mov [edi+28h],eax`, VC7.1 RTM `and ecx ; shl eax ; or ecx,eax ; mov [edi+28h],ecx` (or-operand order). Every other field, the inlined reads and ReadColour match. 10 shapes (ReadSLONG, named local, & 0x7f, (long), (unsigned char) cast, explicit Read(&v,4), CRGBColour retail branch, QFE note, pragma optimize("s"), alternative field type) leave it or worsen it.
// Agent724b (2026-11-03, task 3): 5 more shapes (r0 baseline 4 lines; r2 uchar temp 24; r3 block-scoped unsigned long temp 4; r4 ushort cast 24; r5 int cast 4). Probe TUs: VC7.1 RTM emits retail's `shl v; and old; or v,old` only when the RHS carries a conversion node (movzx/movsx/ftol: uchar/ushort/short/float source); any plain 32-bit RHS (field assign, manual mask merge, enum, long, signed field) gives `and old; shl v; or old,v`. Retail reads a plain 4-byte value here, so the lever is still unknown. 15 attempts over two agents.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READERS
#include "engine/CDataInputStream.h"
#include "engine/CRGBColour.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
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
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCDecalRenderer::ReadBinary(CDataInputStream& in)
{
    DecalLifeSecs = in.ReadFloat();
    DecalBankIndex = in.ReadSLONG();
    StartColour = in.ReadColour();
    MidColour = in.ReadColour();
    EndColour = in.ReadColour();
    BlendMode = in.ReadULONG();
    BlendOp = in.ReadULONG();
    Flags = in.ReadULONG();
    FadeInEndInteger = in.ReadULONG();
    FadeOutBeginInteger = in.ReadULONG();
    StartRenderSize = in.ReadULONG();
    AlphaFadeMinimum = in.ReadULONG();
    EndRenderSize = in.ReadULONG();
    AnimationTimeSecs = in.ReadULONG();
    SizeFadeMinimum = in.ReadULONG();
    UseStartColour = in.ReadEBOOL();
    UseMidColour = in.ReadEBOOL();
    UseEndColour = in.ReadEBOOL();
    AlphaFadeEnable = in.ReadEBOOL();
    SizeFadeEnable = in.ReadEBOOL();
    ForceAnimationTime = in.ReadEBOOL();
    FlickerMinAlphaInteger = in.ReadULONG();
    FlickerMinSizeInteger = in.ReadULONG();
    FlickerBias = in.ReadULONG();
    FlickerSpeed = in.ReadULONG();
    FlickerEnable = in.ReadEBOOL();
    PoolingEnable = in.ReadEBOOL();
    PoolIncreaseAlphaEnable = in.ReadEBOOL();
    PoolIncreaseSizeEnable = in.ReadEBOOL();
    PoolIncreaseLifeEnable = in.ReadEBOOL();
    MaxPoolSize = in.ReadFloat();
    MaxPoolAlpha = in.ReadFloat();
    MaxPoolLife = in.ReadFloat();
    PoolIncreaseFrameEnable = in.ReadEBOOL();
    PoolFrameIncreaseRate = in.ReadFloat();
    StencilCubeWidth = in.ReadFloat();
}
}
