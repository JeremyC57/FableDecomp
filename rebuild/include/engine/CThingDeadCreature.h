#ifndef FABLE_ENGINE_CTHINGDEADCREATURE_H
#define FABLE_ENGINE_CTHINGDEADCREATURE_H
#include "engine/CThingGameObject.h"
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#include "engine/CCountedPointer.h"
class CAnimFilterBase;
#endif
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// PAnimFilter, PKiller, BodyDieTimer, BodyReorienter, StartedDecay; native allocation527300.
class CThingDeadCreature:public CThingGameObject {
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
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x00836670
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingDeadCreature)==0xf0);
#endif
