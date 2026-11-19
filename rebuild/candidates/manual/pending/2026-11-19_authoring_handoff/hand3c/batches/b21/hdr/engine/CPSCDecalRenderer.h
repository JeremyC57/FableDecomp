#ifndef FABLE_ENGINE_CPSCDECALRENDERER_H
#define FABLE_ENGINE_CPSCDECALRENDERER_H
#include "engine/CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
// Layout: Ego_r.pdb CPSCDecalRenderer (struct_layouts_egor.tsv, 100 bytes); retail accesses of the landed same-class rows
// (InterpolateTo 0x00B1BDA0, DatabaseCopy 0x00B1B810, WriteBinary 0x00B1BA10) and ReadBinary 0x00B1C880 / DatabaseSerialise 0x00B1E380.
// Virtuals: the overrides of the retail vtable 0x012A09BC (slot 13 PreparePrimitives 0x00B1F200, 14 InterpolateTo, 15 ReadBinary,
// 16 WriteBinary, 17 DatabaseSerialise, 18 DatabaseCopy) named as in FableWin.pdb.
#ifdef FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
#include "stlport/fable_containers.h"
namespace NParticleEngine { class CDecal; }
#endif
namespace NParticleEngine {
class CPSCDecalRenderer : public CPSCBase {
protected:
    float         DecalLifeSecs;                // +0x0c
    long          DecalBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long BlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long EndRenderSize : 11;           // +0x28
    unsigned long AnimationTimeSecs : 14;
    unsigned long SizeFadeMinimum : 7;
    unsigned long UseStartColour : 1;           // +0x2c
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned long FlickerBias : 8;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long FlickerSpeed : 12;            // +0x30
    unsigned char AlphaFadeEnable : 1;          // +0x34
    unsigned char SizeFadeEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1;
    unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;  // +0x35
    float         MaxPoolSize;                  // +0x38
    float         MaxPoolAlpha;                 // +0x3c
    float         MaxPoolLife;                  // +0x40
    float         PoolFrameIncreaseRate;        // +0x44
    float         StencilCubeWidth;             // +0x48
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle;   // +0x4c
    long          LandscapeDecalsChildPrimIndex;        // +0x54
    CEnginePrimitiveHandle MeshDecalsPrimHandle;        // +0x58
    long          MeshDecalsChildPrimIndex;             // +0x60
public:
#ifdef FABLE_RETAIL_CPSCDECALRENDERER_CTOR
    // Opt-in (hand3c, row 0x00B1DFD0): Ego_r.pdb 0x00ADE5E0 / FableWin.pdb 0x02F55400 ??0CPSCDecalRenderer@NParticleEngine@@QAE@XZ.
    // Declaration only, no layout change.
    CPSCDecalRenderer();
#endif
    virtual void ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&);
    virtual void PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
    virtual void InterpolateTo(const CPSCBase& other, const float t);
    virtual void ReadBinary(CDataInputStream& in);
    virtual void WriteBinary(CDataOutputStream& out) const;
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
    virtual void DatabaseCopy(const CPSCBase& other);
#ifdef FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
protected:
    // hand3c opt-in (row 0x00B1D550): Ego_r.pdb / FableWin.pdb ?DecalCollides@CPSCDecalRenderer@NParticleEngine@@IAE_NAAVCParticleSystem@2@
    // ABV?$_List_iterator@..PAVCDecal@NParticleEngine@@..@std@@MAAV45@AAM@Z (protected; FableWin 0x02F567F0; Ego_d.pdb parameter names).
    // Declaration only: no layout change.
    bool DecalCollides(CParticleSystem& system, const std::list<CDecal*>::iterator& it_decal, float decal_size,
                       std::list<CDecal*>::iterator& it_collison_decal, float& collison_decal_size);   // 0x00B1D550
#endif
#ifdef FABLE_RETAIL_CPSCDECALRENDERER_POOLDECAL
protected:
    // hand3c opt-in (row 0x00B1D790): Ego_r.pdb / FableWin.pdb ?PoolDecal@CPSCDecalRenderer@NParticleEngine@@IAE_NAAVCParticleSystem@2@
    // AAV?$_List_iterator@..PAVCDecal@NParticleEngine@@..@std@@@Z (protected; FableWin 0x02F56D80). Needs _DECALCOLLIDES (the forward
    // declarations). Declaration only: no layout change.
    bool PoolDecal(CParticleSystem& system, std::list<CDecal*>::iterator& it_decal);   // 0x00B1D790
#endif
};
}
FABLE_STATIC_ASSERT_AT(64, sizeof(NParticleEngine::CPSCDecalRenderer)==0x64);
#endif
