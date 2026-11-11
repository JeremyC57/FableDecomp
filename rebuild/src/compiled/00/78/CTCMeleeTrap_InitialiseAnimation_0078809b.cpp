// CTCMeleeTrap::InitialiseAnimation(const CCharString& name, bool loop) -- reset the thing's animator and start the appearance's first animation of the given name (type 6, update mode 1, looping as asked, priority 100), then pick up its strike events
// Leaf: ?InitialiseAnimation@CTCMeleeTrap@@AAEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078809B, 253 bytes, size-optimised TU, `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?InitialiseAnimation@CTCMeleeTrap@@AAEXABVCCharString@@_N@Z (private).
// Callees (PDB spellings): the forced-inline CThing::GetTC<CTCAnimationComplex> (bitset<274>::test 0x00410DE0, CVectorMap::LowerBound
//   0x0040F020, TCI_ANIMATION 0x5A; the absent arm passes the unset out-pointer), CTCAnimationComplex::GetAnimation 0x0070B460,
//   CAnimateThing::ClearAllAnims 0x0070EC30, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (result ignored), CAnimationSet::GetAnim
//   0x005DC340, CAnimInit::Init 0x0070C050 (the inline argument constructor), CAnimateThing::Init(const CAnimInit&) 0x0070D920 and
//   SetMainAnim(const CAnimInit&, long, long) 0x0070D580, CTCMeleeTrap::InitialiseStrikeAnimation(const CAnimationEntry&) 0x00787F29,
//   the defs' virtual release (slot 1).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: CTCBase ParentThing +0x04; CThing bitset +0x20, map +0x44, PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the thing def is taken through the by-value GetPBaseDef() (its reference added and dropped before GetSubDef).
// Name fix (agentNF12 2026-11-11): moved off the local CThing / CDefPointer / CThingBaseDef / CAppearanceDef / CAnimationSet / CAnimInit /
// CAnimateThing / CTCMeleeTrap stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/CTCMeleeTrap.h).
// 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x0070EC30
// ClearAllAnims (was Reset), 0x005DC340 GetAnim (was GetAnimation returning a pointer), 0x0070C050 CAnimInit::Init (was Set),
// 0x0070D920 CAnimateThing::Init (was a one-argument SetMainAnim), 0x00787F29 InitialiseStrikeAnimation private AAE (was QAE).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
#define FABLE_RETAIL_CANIMATETHING_INIT
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCMeleeTrap.h"

void CTCMeleeTrap::InitialiseAnimation(const CCharString& name, bool loop)
{
    CTCAnimationComplex* complex;
    ParentThing->GetTC(&complex);
    CAnimateThing& animation = complex->GetAnimation();
    animation.ClearAllAnims();
    const CThingBaseDef* thingDef = ParentThing->GetPBaseDef().Get();
    CDefPointer<const CAppearanceDef> appearance;
    thingDef->GetSubDef(&appearance);
    const CAnimationEntry& entry = appearance->Animation.GetAnim(name, 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, loop, 0, 100);
    animation.Init(init);
    animation.SetMainAnim(init, 1, 0);
    InitialiseStrikeAnimation(entry);
}
