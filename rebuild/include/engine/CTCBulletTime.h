#ifndef FABLE_ENGINE_CTCBULLETTIME_H
#define FABLE_ENGINE_CTCBULLETTIME_H
#ifdef FABLE_RETAIL_CTCBULLETTIME
// Retail opt-in (Codex pass 11 consolidation 2): the retail CTCBulletTime model (row 0x00667D30 CThingCreatureBase::OnHit); see below.
#include "engine/CTCBase.h"
class CThingCreatureBase;
// TPI0x2f400 original CTCBase; full fields BulletTimeRemainingTicks..CutSceneCancel.
#ifdef FABLE_RETAIL_CTCBULLETTIME_MEMBERS
// Opt-in (chunk 898, row 0x0079CF30 CTCBulletTime::Pause): the Ego_r.pdb member names of the generated branch below in place of
// the 0x4c-byte pad (same offsets, asserted below), the looping effect as its real CIntelligentPointer<NParticleEngine::CParticleEmitter>
// (needs FABLE_RETAIL_CINTELLIGENTPOINTER), Pause (0x0079CF30 ?Pause@CTCBulletTime@@QAEXXZ) and the private StopSound (0x0079CEE0
// ?StopSound@CTCBulletTime@@AAEXK@Z).
#include "engine/CIntelligentPointer.h"
class CDefPointeeBase;
struct CEnginePrimitive2DSprite;
namespace NParticleEngine { class CParticleEmitter; }
#endif
#ifdef FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
// Opt-in (wf11 row 0x0079CC80 CTCBulletTime::PlayEffect): PDef typed as its Ego_r.pdb CDefPointer<CSpecialAbilitiesBulletTimeDef const>
// (same +0x20 slot) and the private PlayEffect (Ego_r/FableWin ?PlayEffect@CTCBulletTime@@AAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@PAVCThingCreatureBase@@J@Z).
#include "engine/CDefPointer.h"
#include "fable_string.h"
class CSpecialAbilitiesBulletTimeDef;
class CThing;
#endif
class CTCBulletTime : public CTCBase {
#ifdef FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#ifdef FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
private:
    void PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType);   // 0x0079CC80
#endif
public:
    void Pause();                                                        // 0x0079CF30
private:
    void StopSound(unsigned long sound);                                 // 0x0079CEE0
#ifdef FABLE_RETAIL_CTCBULLETTIME_UNPAUSE
    // Opt-in (wf17 row 0x0079D7E0): Ego_r.pdb ?UnPause@CTCBulletTime@@QAEXXZ and the private
    // ?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z (retail 0x0079CE80).
    unsigned long PlaySoundW(const CCharString& name, bool loop);        // 0x0079CE80
public:
    void UnPause();                                                      // 0x0079D7E0
private:
#endif
public:                                                  // members public as in the generated branch
    long                      BulletTimeRemainingTicks;      // +0x0c
    long                      BulletTimeAbilityLevel;        // +0x10
    CEnginePrimitive2DSprite* DisplacementMask;              // +0x14
    unsigned char             DisplacementEffectPrimitive[0x8]; // +0x18 CEnginePrimitiveHandle
#ifdef FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
    CDefPointer<const CSpecialAbilitiesBulletTimeDef> PDef;  // +0x20
#else
    CDefPointeeBase*          PDef_Object;                   // +0x20 CDefPointer<CSpecialAbilitiesBulletTimeDef const>::Object
#endif
    long                      m_nLevel;                      // +0x24
    long                      m_nTimeMultiplier;             // +0x28
    float                     m_fDuration;                   // +0x2c
    CIntelligentPointer<NParticleEngine::CParticleEmitter> m_tpLoopingEffect;   // +0x30
    unsigned long             LoopSound;                     // +0x38
    unsigned char             PItemRemoved[0x8];             // +0x3c CIntelligentPointer<CThing>
    unsigned char             HitCreatureBlur[0x8];          // +0x44 CIntelligentPointer<CThing>
    void*                     RadialBlurKey;                 // +0x4c
    void*                     ScreenFilterKey;               // +0x50
    bool                      Pausable;                      // +0x54
    bool                      Paused;                        // +0x55
    bool                      CutSceneCancel;                // +0x56
    unsigned char             _pad_0x57[0x1];                // +0x57
#else
private: unsigned char _pad_0x0c[0x4c];
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BULLET_TIME; } void PlayCreatureOnHitEffect(CThingCreatureBase*);
    void ImmediateCancel();                                              // 0x0079D020 ?ImmediateCancel@CTCBulletTime@@QAEXXZ (Ego_r + FableWin)
    // chunk 755 (row 0x006A02B0): Ego_r.pdb / FableWin.pdb ?GetTimeMultiplier@CTCBulletTime@@QBEMXZ, out of line at 0x0079C740.
    float GetTimeMultiplier() const;                                     // 0x0079C740
#ifdef FABLE_RETAIL_CTCBULLETTIME_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0079DC80
public:
#endif
#ifdef FABLE_RETAIL_CTCBULLETTIME_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void OnKill();                                               // slot 14, 0x0079D7C0 (tail call to FinalCleanup)
private:
    void FinalCleanup();                                                 // 0x0079D2D0 ?FinalCleanup@CTCBulletTime@@AAEXXZ
public:
#endif // FABLE_RETAIL_CTCBULLETTIME_VSLOTS2
};
FABLE_STATIC_ASSERT(sizeof(CTCBulletTime)==0x58);
#ifdef FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#include <stddef.h>
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_tpLoopingEffect) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, LoopSound) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, ScreenFilterKey) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Pausable) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Paused) == 0x55);
#endif

#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBulletTime = 0x58.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;
struct CEnginePrimitive2DSprite;

#pragma pack(push, 1)
class CTCBulletTime {
public:
    void*                     __vftable;                     // +0x00 vptr, or first dword of a flattened base subobject
    long                      f4;                            // +0x04 retail-only long (previous header)
    unsigned char             _pad_0x08[0x4];                // +0x08
    long                      BulletTimeRemainingTicks;      // +0x0c
    long                      BulletTimeAbilityLevel;        // +0x10
    CEnginePrimitive2DSprite* DisplacementMask;              // +0x14
    unsigned char             DisplacementEffectPrimitive[0x8]; // +0x18 CEnginePrimitiveHandle
    CDefPointeeBase*          PDef_Object;                   // +0x20 CDefPointer<CSpecialAbilitiesBulletTimeDef_const_>::Object
    long                      m_nLevel;                      // +0x24
    long                      m_nTimeMultiplier;             // +0x28
    float                     m_fDuration;                   // +0x2c
    unsigned char             m_tpLoopingEffect[0x8];        // +0x30 CIntelligentPointer<NParticleEngine::CParticleEmitter>
    unsigned long             LoopSound;                     // +0x38
    unsigned char             PItemRemoved[0x8];             // +0x3c CIntelligentPointer<CThing>
    unsigned char             HitCreatureBlur[0x8];          // +0x44 CIntelligentPointer<CThing>
    void*                     RadialBlurKey;                 // +0x4c
    void*                     ScreenFilterKey;               // +0x50
    bool                      Pausable;                      // +0x54
    bool                      Paused;                        // +0x55
    bool                      CutSceneCancel;                // +0x56
    unsigned char             _pad_0x57[0x1];                // +0x57
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBulletTime) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, f4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, BulletTimeRemainingTicks) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, BulletTimeAbilityLevel) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, DisplacementMask) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, DisplacementEffectPrimitive) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, PDef_Object) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_nLevel) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_nTimeMultiplier) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_fDuration) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, m_tpLoopingEffect) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, LoopSound) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, PItemRemoved) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, HitCreatureBlur) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, RadialBlurKey) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, ScreenFilterKey) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Pausable) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, Paused) == 0x55);
FABLE_STATIC_ASSERT(offsetof(CTCBulletTime, CutSceneCancel) == 0x56);

#endif // FABLE_RETAIL_CTCBULLETTIME
#endif // FABLE_ENGINE_CTCBULLETTIME_H
