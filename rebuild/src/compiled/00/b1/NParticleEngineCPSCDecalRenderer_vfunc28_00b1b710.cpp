#define CIEngine CIEnginePdb
#include "engine/CIEngine.h"
#undef CIEngine
// Layout: donor has a flattened vptr; reproduce it with real virtual dispatch,
// preserving its 12-byte size and Active at +8. Slot26 is AddChildPrimitive.
// NParticleEngine::CPSCDecalRenderer::Initialise, retail 0x00B1B710 (250 bytes).
// PDB identity/signature recovered at slot 28: Initialise(CIEngine*, CParticleSystem&,
// const CParticleUpdateParameters<unsigned long,unsigned long>&, CEnginePrimitiveHandle&).
// CPSCDecalRenderer offsets follow struct_layouts_egor.tsv; the 36-byte engine
// descriptor is the distinct public CEnginePrimitiveParticleDecalGroup, not the
// 0xd8-byte internal group in the quarantined generated header.
struct CEnginePrimitive;
struct CEnginePrimitiveParticleDecalGroup;
struct CIEngine;
struct CBaseClass { virtual ~CBaseClass(); };
struct CEnginePersistentPrimitiveBase;
struct CEnginePrimitiveHandle : CBaseClass { CEnginePersistentPrimitiveBase* Data; };
// Original enum types; only storage and conversions are consumed by this leaf.
enum EEnginePrimitiveBlendMode { BlendModeZero=0 };
enum EEnginePrimitiveBlendOp { BlendOpZero=0 };
struct CIEngine {
    unsigned char Unknown04[4];
    bool Active;
    unsigned char Unknown09[3];
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual unsigned long AddChildPrimitive(
        CEnginePrimitiveHandle&, const ::CEnginePrimitive&, unsigned long) { return 0; }
};
typedef char CIEngineMatchesDonor[(sizeof(CIEngine)==sizeof(CIEnginePdb))?1:-1];
struct CEngine : CIEngine {
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,
        const ::CEnginePrimitive&, unsigned long);
};
namespace NParticleEngine {
struct CParticleSystem {};
template<class A,class B> struct CParticleUpdateParameters {};
struct CPSCBase {
    virtual void v00() {}
    void* ParticleSystem;                       // +0x04
    bool UpdateStarted : 1;                     // +0x08
    bool Initialised : 1;
    bool Initialise(CIEngine*, CParticleSystem&,
        const CParticleUpdateParameters<unsigned long, unsigned long>&,
        CEnginePrimitiveHandle&) {
        Initialised = true;
        return Initialised;
    }
};
struct CPSCDecalRenderer : CPSCBase {
    float DecalLifeSecs;                         // +0x0c
    long DecalBankIndex;                         // +0x10
    unsigned long StartColour, MidColour, EndColour; // +0x14..+0x1c
    unsigned long BlendMode : 3;                 // +0x20
    unsigned long BlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;       // +0x24
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long EndRenderSize : 11;             // +0x28
    unsigned long AnimationTimeSecs : 14;
    unsigned long SizeFadeMinimum : 7;
    unsigned long UseStartColour : 1;             // +0x2c
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned long FlickerBias : 8;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long FlickerSpeed : 12;              // +0x30
    unsigned char AlphaFadeEnable : 1;            // +0x34
    unsigned char SizeFadeEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1;
    unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;    // +0x35
    unsigned char _pad36[2];
    float MaxPoolSize, MaxPoolAlpha, MaxPoolLife, PoolFrameIncreaseRate, StencilCubeWidth;
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle; // +0x4c
    long LandscapeDecalsChildPrimIndex;          // +0x54
    CEnginePrimitiveHandle MeshDecalsPrimHandle; // +0x58
    long MeshDecalsChildPrimIndex;               // +0x60
    bool Initialise(CIEngine*, CParticleSystem&,
        const CParticleUpdateParameters<unsigned long, unsigned long>&,
        CEnginePrimitiveHandle&);
};
}

struct CEnginePrimitive {
    long Type;
    long InterpolationSet;
    struct { unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; } ThemeChannelDesc;
};
struct CEnginePrimitiveParticleDecalGroup : CEnginePrimitive {
    unsigned long DecalBankIndex;                // +0x0c
    unsigned short Flags;                        // +0x10
    unsigned char _pad12[2];
    EEnginePrimitiveBlendMode BlendMode;            // +0x14
    EEnginePrimitiveBlendOp BlendOp;                // +0x18
    float StencilCubeHeight;                     // +0x1c
    bool LandscapeDecal;                         // +0x20
    unsigned char _pad21[3];
    CEnginePrimitiveParticleDecalGroup(long bank, bool landscape,
        EEnginePrimitiveBlendMode blendMode, EEnginePrimitiveBlendOp blendOp, float stencilHeight,
        unsigned long flags);                   // retail 0x00B1B4D0, thiscall ret 24
};

namespace NParticleEngine {
bool CPSCDecalRenderer::Initialise(CIEngine* engine, CParticleSystem& system,
    const CParticleUpdateParameters<unsigned long, unsigned long>& params,
    CEnginePrimitiveHandle& output)
{
    if (engine) {
    EEnginePrimitiveBlendOp blendOp = (EEnginePrimitiveBlendOp)BlendOp;
    EEnginePrimitiveBlendMode blendMode = (EEnginePrimitiveBlendMode)BlendMode;
    if (DecalBankIndex != 0 && LandscapeDecalsChildPrimIndex == -1 &&
        MeshDecalsChildPrimIndex == -1) {
        unsigned long child = ((CEngine*)engine)->AddChildPrimitive(output,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, true, blendMode,
                blendOp, (float)Flags, 0), 0);
        LandscapeDecalsChildPrimIndex = child;

        child = ((CEngine*)engine)->AddChildPrimitive(output,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, false, blendMode,
                blendOp, (float)Flags, 0), 0);
        MeshDecalsChildPrimIndex = child;
    }
    CPSCBase::Initialise(engine, system, params, output);
    }
    return Initialised;
}
}