// CThingDeadCreature::SetupDeadAnimation() -- start the corpse's "STANDARD_DEAD" animation: a random one passing the creature's animation filter (else of any group) from its appearance def, played as the main animation and immediately finished, then a zero-time animation update, a frame-updated mark and a rendered-this-frame mark
// Leaf: ?SetupDeadAnimation@CThingDeadCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835E30, 392 bytes, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?SetupDeadAnimation@CThingDeadCreature@@IAEXXZ (protected; one caller, OnCreate 0x00835FD0).
// Callees (PDB spellings): CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the forced-inline GetTC<CTCAnimationComplex>,
//   TCI_ANIMATION 0x5A; the absent arm passes the unset out-pointer), CTCAnimationComplex::GetAnimation 0x0070B460,
//   CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::AnimExists 0x005DC2E0 / GetRandomAnim 0x005DCC00,
//   CThing::GetPWorldSeed 0x004C7AE0, CAnimateThing::ClearAllAnims 0x0070EC30, CAnimationEntry::GetAnimateThingFlags 0x005DC390,
//   CAnimInit::Init 0x0070C050 (the inline argument constructor), CAnimateThing::SetMainAnim 0x0070D580 / SetAsFinished 0x0070BE80 /
//   Update 0x0070E8B0, CThing::SetAsFrameUpdated 0x004C7C40 / SetAsRenderedThisFrame 0x004C7590, the def's virtual release.
// Globals: the static CCharString "STANDARD_DEAD" @ 0x013BAF40 (its dynamic initializer is landed at 0x0121F570).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: CThing bitset +0x20, map +0x44, PBaseDef +0x70; CThingDeadCreature PAnimFilter +0xD0 (CCountedPointer<CAnimFilterBase>, read
//   through its inline operator*); CAppearanceDef Animation +0x34.
// Shape: the entry's bank index is read before ClearAllAnims; the zero-time CAnimateThingUpdate is a temporary (three zero stores
//   after the push).
// Name fix (agentNF11 2026-11-11): moved off the local CThing / CDefPointer / CAppearanceDef / CAnimationSet / CAnimInit / CAnimateThing /
// CThingDeadCreature stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers. 0x0042B0A2 binds GetSubDef<CAppearanceDef>
// (was ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DCC00 GetRandomAnim (was GetRandomAnimation returning a
// pointer), 0x004C7AE0 GetPWorldSeed (was GetRandomSeed), 0x0070EC30 ClearAllAnims (was Reset), 0x0070C050 CAnimInit::Init (was Set),
// 0x0070E8B0 CAnimateThing::Update(const CAnimateThingUpdate&) (was UpdateFrameLogic(const CAnimFrameTime&)); the row's own symbol is
// the PDB protected IAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
#define FABLE_RETAIL_CANIMATETHING_SETASFINISHED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"

namespace { extern CCharString DEAD_ANIM_NAME; }                   // 0x013BAF40 ("STANDARD_DEAD")

void CThingDeadCreature::SetupDeadAnimation()
{
    CTCAnimationComplex* complex;
    GetTC(&complex);
    CAnimateThing& animation = complex->GetAnimation();
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    const CAnimationSet& animations = appearance->Animation;
    const CAnimationEntry* entry = 0;
    if (animations.AnimExists(DEAD_ANIM_NAME, *PAnimFilter))
        entry = &animations.GetRandomAnim(DEAD_ANIM_NAME, *GetPWorldSeed(), *PAnimFilter, -1);
    else if (animations.AnimExists(DEAD_ANIM_NAME, CAnimFilterNoGroup()))
        entry = &animations.GetRandomAnim(DEAD_ANIM_NAME, *GetPWorldSeed(), CAnimFilterNoGroup(), -1);
    if (entry) {
        long anim = entry->BankIndex;
        animation.ClearAllAnims();
        CAnimInit init(2, 0, anim, ANIM_UPDATE_FROM_SPEED, true, entry->GetAnimateThingFlags(), 100);
        animation.SetMainAnim(init, 5, 0);
        animation.SetAsFinished();
    }
    animation.Update(CAnimateThingUpdate());
    SetAsFrameUpdated();
    SetAsRenderedThisFrame(2);
}
