// CTCCreatureHive::Open() -- play the hive's "OPEN_UP" appearance animation as the main animation (type 6, mode 1, not looping, percent 100) and start animating
// Leaf: ?Open@CTCCreatureHive@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D230, 243 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?Open@CTCCreatureHive@@QAEXXZ.
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, CAnimationSet::GetAnim 0x005DC340, CAnimInit::Init 0x0070C050 (the inline argument constructor),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the forced-inline GetTC<CTCAnimationComplex>, TCI_ANIMATION 0x5A),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::SetMainAnim(const CAnimInit&, long, long) 0x0070D580, the def's
//   virtual release (slot 1).
// Literal: "OPEN_UP" @ 0x0126AF84. Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCCreatureHive.h (Ego_r.pdb, 0x10): Animating +0x0c; CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the base def read in place (PeekBaseDef); the animation request built before the component lookup; the absent-component
//   branch passes the unset out-pointer (retail `mov ecx,[esp+14h]`); Animating set after SetMainAnim.
// Name fix (agentNF11 2026-11-11): moved off the local CDefPointer / CAppearanceDef / CAnimationSet / CAnimInit / CAnimateThing /
// CThing / CTCCreatureHive stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/CTCCreatureHive.h).
// 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@CThingBaseDef@@..V?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DC340
// GetAnim (was GetAnimation returning a pointer), 0x0070C050 CAnimInit::Init (was Set).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMATETHING_INIT
#define FABLE_RETAIL_CANIMFILTER
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
#include "engine/CTCCreatureHive.h"

void CTCCreatureHive::Open()
{
    CDefPointer<const CAppearanceDef> appearance;
    ParentThing->PeekBaseDef().GetSubDef(&appearance);
    long anim = appearance->Animation.GetAnim("OPEN_UP", 0, CAnimFilterNoGroup()).BankIndex;
    CAnimInit init(6, 0, anim, ANIM_UPDATE_FROM_SPEED, false, 0, 100);
    CTCAnimationComplex* complex;
    ParentThing->GetTC(&complex);
    CAnimateThing& animation = complex->GetAnimation();
    animation.SetMainAnim(init, 1, 0);
    Animating = true;
}
