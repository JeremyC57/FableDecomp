#include <cstdio>
#include <stddef.h>
// BEGIN exact source snapshot SHA256 f51f899307c9befe7a509fff551e8d3005b300b86b981b6ad20c59aceb429b63
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
class CEnginePrimitive;
struct CEnginePrimitiveParticleDecalGroup;
class CIEngine;
class CBaseClass { public: virtual ~CBaseClass(); };
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass { public: CEnginePersistentPrimitiveBase* Data; };
// Original enum types; only storage and conversions are consumed by this leaf.
enum EEnginePrimitiveBlendMode { BlendModeZero=0 };
enum EEnginePrimitiveBlendOp { BlendOpZero=0 };
class CIEngine { public:
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
class CEngine : public CIEngine { public:
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,
        const ::CEnginePrimitive&, unsigned long);
};
namespace NParticleEngine {
class CParticleSystem { public:};
template<class A,class B> class CParticleUpdateParameters { public:};
class CPSCBase { public:
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
class CPSCDecalRenderer : public CPSCBase { public:
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

class CEnginePrimitive { public:
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

// END exact source snapshot
CBaseClass::~CBaseClass(){}
static bool validDescriptors=true;
static CEnginePrimitiveHandle* expectedOutput;
static NParticleEngine::CPSCDecalRenderer* activeRenderer;
static bool mutateFirst;
static int createCount;
static unsigned long returnedChild[2];
static bool capturedLandscape[2];
static unsigned long capturedBank[2], capturedMode[2], capturedOp[2], capturedFlags[2];
static float capturedHeight[2];

CEnginePrimitiveParticleDecalGroup::CEnginePrimitiveParticleDecalGroup(
    long bank, bool landscape, EEnginePrimitiveBlendMode mode, EEnginePrimitiveBlendOp op,
    float height, unsigned long flags)
{
    Type=0x1e;InterpolationSet=0;ThemeChannelDesc.ChannelIndex0=0;ThemeChannelDesc.ChannelIndex1=0;ThemeChannelDesc.ChannelBlendFactor=0;
    DecalBankIndex = bank;
    Flags = flags;
    BlendMode = mode;
    BlendOp = op;
    StencilCubeHeight = height;
    LandscapeDecal = landscape;
}
unsigned long CEngine::AddChildPrimitive(
    CEnginePrimitiveHandle& output, const ::CEnginePrimitive& primitive,
    unsigned long flags)
{
    const CEnginePrimitiveParticleDecalGroup& group =
        (const CEnginePrimitiveParticleDecalGroup&)primitive;
    validDescriptors=validDescriptors && (&output==expectedOutput) && flags==0 && group.Type==0x1e && group.InterpolationSet==0 && group.ThemeChannelDesc.ChannelIndex0==0 && group.ThemeChannelDesc.ChannelIndex1==0 && group.ThemeChannelDesc.ChannelBlendFactor==0;
    int i = createCount++;
    if (i < 2) {
        capturedLandscape[i] = group.LandscapeDecal;
        capturedBank[i] = group.DecalBankIndex;
        capturedMode[i] = group.BlendMode;
        capturedOp[i] = group.BlendOp;
        capturedHeight[i] = group.StencilCubeHeight;
        capturedFlags[i] = group.Flags;
    }
    if(mutateFirst && i==0){activeRenderer->DecalBankIndex=99;activeRenderer->Flags=1023;activeRenderer->BlendMode=1;activeRenderer->BlendOp=2;}
    (void)flags;
    return returnedChild[i < 2 ? i : 1];
}

int main()
{
    static NParticleEngine::CPSCDecalRenderer renderer;
    static NParticleEngine::CParticleSystem system;
    static NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> params;
    static CEnginePrimitiveHandle output;
    static CEngine engine;

    expectedOutput=&output;
    if(sizeof(CIEngine)!=12 || offsetof(CIEngine,Active)!=8) return 7;
    if(sizeof(CEnginePrimitive)!=12 || sizeof(CEnginePrimitiveParticleDecalGroup)!=36 || offsetof(CEnginePrimitiveParticleDecalGroup,Flags)!=0x10 || offsetof(CEnginePrimitiveParticleDecalGroup,LandscapeDecal)!=0x20 || sizeof(CEnginePrimitiveHandle)!=8) return 2;
    renderer.Initialised = false;
    if (renderer.Initialise(0, system, params, output)) {
        std::printf("00b1b710_TEST FAIL\n"); return 1;
    }
    renderer.Initialised = true;
    if (!renderer.Initialise(0, system, params, output) || createCount != 0) {
        std::printf("00b1b710_TEST FAIL\n"); return 1;
    }

    renderer.Initialised = false;
    renderer.DecalBankIndex = 27;
    renderer.Flags = 375;
    renderer.BlendMode = 5;
    renderer.BlendOp = 3;
    renderer.LandscapeDecalsChildPrimIndex = -1;
    renderer.MeshDecalsChildPrimIndex = -1;
    returnedChild[0] = 41;
    returnedChild[1] = 72;
    if (!renderer.Initialise(&engine, system, params, output) || createCount != 2 ||
        renderer.LandscapeDecalsChildPrimIndex != 41 || renderer.MeshDecalsChildPrimIndex != 72 ||
        !capturedLandscape[0] || capturedLandscape[1] ||
        capturedBank[0] != 27 || capturedBank[1] != 27 ||
        capturedMode[0] != 5 || capturedMode[1] != 5 ||
        capturedOp[0] != 3 || capturedOp[1] != 3 ||
        capturedHeight[0] != 375.0f || capturedHeight[1] != 375.0f ||
        capturedFlags[0] != 0 || capturedFlags[1] != 0) {
        std::printf("00b1b710_TEST FAIL\n"); return 1;
    }

    renderer.LandscapeDecalsChildPrimIndex = 2;
    renderer.MeshDecalsChildPrimIndex = -1;
    if (!renderer.Initialise(&engine, system, params, output) || createCount != 2) {
        std::printf("00b1b710_TEST FAIL\n"); return 1;
    }
    renderer.LandscapeDecalsChildPrimIndex=-1;renderer.MeshDecalsChildPrimIndex=2;
    if(!renderer.Initialise(&engine,system,params,output) || createCount!=2)return 4;
    renderer.MeshDecalsChildPrimIndex=-1;renderer.DecalBankIndex=0;renderer.Initialised=false;
    if(!renderer.Initialise(&engine,system,params,output) || createCount!=2)return 5;
    renderer.DecalBankIndex=27;renderer.Flags=375;renderer.BlendMode=5;renderer.BlendOp=3;
    renderer.UpdateStarted=true;activeRenderer=&renderer;mutateFirst=true;createCount=0;
    if(!renderer.Initialise(&engine,system,params,output) || createCount!=2 || !renderer.UpdateStarted ||
       capturedBank[0]!=27 || capturedBank[1]!=99 || capturedHeight[0]!=375.0f || capturedHeight[1]!=1023.0f ||
       capturedMode[1]!=5 || capturedOp[1]!=3)return 6;
    if(!validDescriptors)return 3;
    std::printf("00b1b710_TEST PASS\n");
    return 0;
}