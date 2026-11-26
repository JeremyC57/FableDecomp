#ifndef FABLE_ENGINE_CPSCRENDERSPRITE_H
#define FABLE_ENGINE_CPSCRENDERSPRITE_H
#include "engine/CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
// Layout: both original PDB types; native AFCC30 accesses and constructor neighbours.
namespace NParticleEngine {
class CPSCRenderSprite : public CPSCBase {
protected:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long TrailLengthInteger : 12;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;   // +0x28
    unsigned long NoCrossedSprites : 5;
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;         // +0x2c
    unsigned long EndRenderSize : 11;
    unsigned long FlickerBias : 8;
    unsigned long AnimationTimeSecs : 14;       // +0x30
    unsigned long SizeFadeMinimum : 7;
    unsigned long TrailWidth : 10;
    unsigned long FlickerSpeed : 12;            // +0x34
    unsigned long UseStartColour : 1;
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned char AlphaFadeEnable : 1;          // +0x38
    unsigned char SizeFadeEnable : 1;
    unsigned char FlickerEnable : 1;
    unsigned char ForceAnimationTime : 1;
    CEnginePrimitiveHandle PrimHandle;          // +0x3c
    long          ChildPrimIndex;               // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x48
    long          ChildTrailPrimIndex;          // +0x50
public:
#ifdef FABLE_RETAIL_CPSCRENDERSPRITE_BOUNDING_PASS24
    // Retail opt-in (Codex pass 24): FableWin and slot 19 in the CPSCRenderSprite vtable at 0x00AFAE50.
    virtual bool AddToBoundingSphere(C3DBoundingSphere&, const float, CParticleSystem&, const CParticleUpdateParameters<unsigned long,unsigned long>&);
#endif
#ifdef FABLE_RETAIL_CPSCRENDERSPRITE_ALLOC
    // Opt-in (NF462, ruling r460d; the engine/NParticleEngine_CPSCOrbit.h pattern): the class factory and the members a TU needs to
    // `new` the class. Ego_r.pdb + FableWin.pdb: ??0CPSCRenderSprite@NParticleEngine@@QAE@XZ (0x00AFB7C0, out of line),
    // ?Alloc@CPSCRenderSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ (0x00AF7D10, public static __fastcall; RegisterPSCs 0x00AF8170 binds it)
    // and the ten overrides of CPSCBase's pure virtuals (DatabaseClone 0x00AFE580 = vtable 0x0129F67C slot 2, its only retail reference). Declarations only: the
    // virtuals override CPSCBase slots, no new slot, no layout change.
    CPSCRenderSprite();
    static CPSCBase* FABLE_FASTCALL Alloc();
    virtual CPSCBase* DatabaseClone() const;
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&,const float);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
#endif
    virtual void ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&);
    virtual void PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
};
}
FABLE_STATIC_ASSERT_AT(51, sizeof(NParticleEngine::CPSCRenderSprite)==0x54);
#endif
