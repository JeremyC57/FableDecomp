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
#ifdef FABLE_RETAIL_CTCBULLETTIME_CTOR
// Opt-in (hand3c3, row 0x0079D360 ??0CTCBulletTime@@QAE@AAVCThing@@@Z; needs _MEMBERS + _PLAYEFFECT): the constructor, and the three
// members it constructs in their Ego_r.pdb types in place of the byte arrays (same offsets and sizes, asserted below):
// DisplacementEffectPrimitive CEnginePrimitiveHandle +0x18 (0x0079D387 `call CBaseClass::CBaseClass` then vtable 0x0122F598),
// PItemRemoved / HitCreatureBlur CIntelligentPointer<CThing> +0x3C / +0x44 (vtable 0x01238C6C, SetPItem(0)).
#include "engine/CEnginePrimitiveHandle.h"
#endif
class CTCBulletTime : public CTCBase {
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
public: void CancelMove();
#endif

#if defined(FABLE_RETAIL_CTCBULLETTIME_CTOR) && !defined(FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23)
public: CTCBulletTime(CThing&);                                          // 0x0079D360
#endif
#ifdef FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23
 // Retail opt-in (Codex pass23): both original PDB virtual FrameUpdate and typed intelligent blur target.
public:CTCBulletTime(CThing&);virtual void FrameUpdate();
#endif
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
#ifdef FABLE_RETAIL_CTCBULLETTIME_CTOR
    CEnginePrimitiveHandle    DisplacementEffectPrimitive;   // +0x18
#else
    unsigned char             DisplacementEffectPrimitive[0x8]; // +0x18 CEnginePrimitiveHandle
#endif
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
#ifdef FABLE_RETAIL_CTCBULLETTIME_CTOR
    CIntelligentPointer<CThing> PItemRemoved;                // +0x3c
    CIntelligentPointer<CThing> HitCreatureBlur;             // +0x44
#else
    unsigned char             PItemRemoved[0x8];             // +0x3c CIntelligentPointer<CThing>
#ifdef FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23
    CIntelligentPointer<CThing> HitCreatureBlur;
#else
    unsigned char             HitCreatureBlur[0x8];          // +0x44 CIntelligentPointer<CThing>
#endif
#endif
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
FABLE_STATIC_ASSERT_AT(92, sizeof(CTCBulletTime)==0x58);
#ifdef FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#include <stddef.h>
FABLE_STATIC_ASSERT_AT(95, offsetof(CTCBulletTime, m_tpLoopingEffect) == 0x30);
FABLE_STATIC_ASSERT_AT(96, offsetof(CTCBulletTime, LoopSound) == 0x38);
FABLE_STATIC_ASSERT_AT(97, offsetof(CTCBulletTime, ScreenFilterKey) == 0x50);
FABLE_STATIC_ASSERT_AT(98, offsetof(CTCBulletTime, Pausable) == 0x54);
FABLE_STATIC_ASSERT_AT(99, offsetof(CTCBulletTime, Paused) == 0x55);
#ifdef FABLE_RETAIL_CTCBULLETTIME_CTOR
FABLE_STATIC_ASSERT_AT(100, offsetof(CTCBulletTime, DisplacementEffectPrimitive) == 0x18);
FABLE_STATIC_ASSERT_AT(101, offsetof(CTCBulletTime, PItemRemoved) == 0x3c);
FABLE_STATIC_ASSERT_AT(102, offsetof(CTCBulletTime, HitCreatureBlur) == 0x44);
#endif
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
#ifdef FABLE_RETAIL_CTCBULLETTIME_TICKS
    // NF378 add-only opt-in (rows 0x0079C5F0 / 0x0079C6D0): Ego_r.pdb / FableWin.pdb
    // ?GetBulletTimeTicksFromAbilityLevel@CTCBulletTime@@ABEJXZ and ?GetBulletTimeElapsedTicks@CTCBulletTime@@ABEJXZ (private const,
    // out of line). Declarations only: no layout change.
private:
    long GetBulletTimeTicksFromAbilityLevel() const;           // 0x0079C5F0
    long GetBulletTimeElapsedTicks() const;                    // 0x0079C6D0
public:
#endif
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

FABLE_STATIC_ASSERT_AT(141, sizeof(CTCBulletTime) == 0x58);
FABLE_STATIC_ASSERT_AT(142, offsetof(CTCBulletTime, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(143, offsetof(CTCBulletTime, f4) == 0x4);
FABLE_STATIC_ASSERT_AT(144, offsetof(CTCBulletTime, BulletTimeRemainingTicks) == 0xc);
FABLE_STATIC_ASSERT_AT(145, offsetof(CTCBulletTime, BulletTimeAbilityLevel) == 0x10);
FABLE_STATIC_ASSERT_AT(146, offsetof(CTCBulletTime, DisplacementMask) == 0x14);
FABLE_STATIC_ASSERT_AT(147, offsetof(CTCBulletTime, DisplacementEffectPrimitive) == 0x18);
FABLE_STATIC_ASSERT_AT(148, offsetof(CTCBulletTime, PDef_Object) == 0x20);
FABLE_STATIC_ASSERT_AT(149, offsetof(CTCBulletTime, m_nLevel) == 0x24);
FABLE_STATIC_ASSERT_AT(150, offsetof(CTCBulletTime, m_nTimeMultiplier) == 0x28);
FABLE_STATIC_ASSERT_AT(151, offsetof(CTCBulletTime, m_fDuration) == 0x2c);
FABLE_STATIC_ASSERT_AT(152, offsetof(CTCBulletTime, m_tpLoopingEffect) == 0x30);
FABLE_STATIC_ASSERT_AT(153, offsetof(CTCBulletTime, LoopSound) == 0x38);
FABLE_STATIC_ASSERT_AT(154, offsetof(CTCBulletTime, PItemRemoved) == 0x3c);
FABLE_STATIC_ASSERT_AT(155, offsetof(CTCBulletTime, HitCreatureBlur) == 0x44);
FABLE_STATIC_ASSERT_AT(156, offsetof(CTCBulletTime, RadialBlurKey) == 0x4c);
FABLE_STATIC_ASSERT_AT(157, offsetof(CTCBulletTime, ScreenFilterKey) == 0x50);
FABLE_STATIC_ASSERT_AT(158, offsetof(CTCBulletTime, Pausable) == 0x54);
FABLE_STATIC_ASSERT_AT(159, offsetof(CTCBulletTime, Paused) == 0x55);
FABLE_STATIC_ASSERT_AT(160, offsetof(CTCBulletTime, CutSceneCancel) == 0x56);

#endif // FABLE_RETAIL_CTCBULLETTIME
#endif // FABLE_ENGINE_CTCBULLETTIME_H
