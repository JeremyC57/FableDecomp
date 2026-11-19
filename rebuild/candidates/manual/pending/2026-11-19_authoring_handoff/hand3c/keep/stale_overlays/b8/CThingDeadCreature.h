#ifndef FABLE_ENGINE_CTHINGDEADCREATURE_H
#define FABLE_ENGINE_CTHINGDEADCREATURE_H
#include "engine/CThingGameObject.h"
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#include "engine/CCountedPointer.h"
class CAnimFilterBase;
#endif
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_CREATE
#include "engine/CAnimFilterBase.h"
#endif
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// PAnimFilter, PKiller, BodyDieTimer, BodyReorienter, StartedDecay; native allocation527300.
class CThingDeadCreature:public CThingGameObject {
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_STATGETTYPE
public:
    // Opt-in (hand3c, row 0x00914650 CAIStateGroup_BirdStartled::IsRunnable): FableWin.pdb ?StatGetType@CThingDeadCreature@@SIJXZ
    // (0x01D3E930 `mov eax,0Dh`), inline in retail: CThingFilter_ScaresBirds compares CClass::Type with 13 in place (0x009147E0). Body only.
    static long FABLE_FASTCALL StatGetType() { return 13; }
#endif
protected:
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
    CCountedPointer<CAnimFilterBase> PAnimFilter; // +d0 (agentNF11: the typed member, same 8 bytes; Ego_r.pdb CThingDeadCreature)
#else
    unsigned char PAnimFilter[8]; // +d0 CCountedPointer<CAnimFilterBase>, ctor835c55/835c5b
#endif
    unsigned char PKiller[8]; // +d8 CIntelligentPointer<const CThingCreatureBase>, ctor835c4d/835c66
    long BodyDieTimer; // +e0, original member between the two known composites
    unsigned char BodyReorienter[4]; // +e4 boost::scoped_ptr<CThingBodyReorienter>, ctor835c6c
    bool StartedDecay; // +e8, ctor835c73
    unsigned char _pad_0xe9[7];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_GETANIMATIONFILTER
    // Opt-in (chunk 2053, row 0x0087F3A0): FableWin.pdb ?GetAnimationFilter@CThingDeadCreature@@QBEAAVCAnimFilterBase@@XZ (debug copy
    // 0x02878DD0, listed right after CCombatAction_KickThingOnGround::PerformAction: its first user; no Ego_r.pdb / Fable.exe copy: the
    // row reads the PAnimFilter object word +0xD0 in place, `mov eax,[ebp+0D0h] ; push eax`, 0x0087F686). Needs _ANIMATION.
    CAnimFilterBase& GetAnimationFilter() const { return *PAnimFilter; }
#endif
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_PEEKDEFINITION
    // Retail opt-in (agentNF9, row 0x00835B90): Ego_r.pdb ?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ (protected
    // const, returns a reference), out of line in retail at 0x00835B90 (called by OnCreate 0x00835FD0 and WorldUpdate 0x00836260).
    // Declaration only (CThingCreatureDef forward-declared).
protected:
    const class CThingCreatureDef& PeekDefinition() const;               // 0x00835B90
public:
#endif
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
    // Retail opt-in (agentNF11, rows 0x00835BD0 / 0x00835E30): Ego_r.pdb ?PreloadDeadAnimation@CThingDeadCreature@@IAEXXZ (0x00829AE0) and
    // ?SetupDeadAnimation@CThingDeadCreature@@IAEXXZ (0x00829E40), protected non-virtual; retail 0x00835BD0 / 0x00835E30 (both only called from
    // OnCreate 0x00835FD0). Declarations only.
protected:
    void PreloadDeadAnimation();                                         // 0x00835BD0
    void SetupDeadAnimation();                                           // 0x00835E30
public:
#endif
#if defined(FABLE_RETAIL_CTHINGDEADCREATURE_CREATE) && defined(FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION) && defined(FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT)
    // Retail opt-in (NF80, q10920: row 0x00835C80 Create): Ego_r.pdb ??0CThingDeadCreature@@QAE@XZ (out of line, 0x00835C80 calls
    // 0x00835C20; the landed Alloc 0x00527300 binds it), ?Create@CThingDeadCreature@@SIPAV1@JABVC3DVector@@JABVCCharString@@ABVCAnimFilterBase@@@Z
    // (public static __fastcall, 0x00835C80), ?SetAnimationFilter@CThingDeadCreature@@QAEXABVCAnimFilterBase@@@Z (public) and
    // ?ConstructFromParams@CThingDeadCreature@@IAE_NJABVC3DVector@@JABVCCharString@@@Z (protected); retail has no out-of-line copy of the
    // last two: Create expands them (0x00835CB0..0x00835CC5 the CThingGameObject::ConstructFromParams 0x008388D0 call with 0, 0, 0, true;
    // 0x00835D94..0x00835DA5 `call [edx+4]` = Clone, then CCountedPointer<CAnimFilterBase>::Reset 0x00836580). Needs engine/CAnimFilterBase.h.
public:
    CThingDeadCreature();                                                // 0x00835C20
    static CThingDeadCreature* __fastcall Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName,
                                                 const CAnimFilterBase& filter);   // 0x00835C80
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_SETANIMATIONFILTER_OUTOFLINE
    // NF82 opt-in (row 0x00835AF0): the same member declared only, for the row that owns its out-of-line copy (Ego_d.pdb places
    // CThingDeadCreature::SetAnimationFilter in thing_dead_creature.obj; retail 0x00835AF0 sits in that TU's range and is the body
    // below: `call [eax+4]` = Clone on the argument, then Reset 0x00836580 on this+0xD0 = PAnimFilter). Create still expands it.
    void SetAnimationFilter(const CAnimFilterBase& filter);              // 0x00835AF0
#else
    void SetAnimationFilter(const CAnimFilterBase& filter) { PAnimFilter.Reset(filter.Clone()); }
#endif
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName)
    {
        return CThingGameObject::ConstructFromParams(defIndex, position, a3, 0, scriptName, 0, 0, true);
    }
public:
#endif
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x00836670
public:
#endif
};
FABLE_STATIC_ASSERT_AT(54, sizeof(CThingDeadCreature)==0xf0);
#endif
