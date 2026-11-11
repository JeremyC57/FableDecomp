// CTCAnimatingObject::PlayDefaultAnimation() -- drop the queued animations and play the appearance's first "DEFAULT" animation (type 6, mode 1, looping, percent 100) with no priority
// Leaf: ?PlayDefaultAnimation@CTCAnimatingObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8F30, 222 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?PlayDefaultAnimation@CTCAnimatingObject@@QAEXXZ.
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on the thing's base def read in place; result ignored),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CAnimationSet::GetAnim 0x005DC340, free 0x00BFEA14 (the
//   expanded STLport list clear), CAnimInit::Init 0x0070C050 (the inline argument constructor), CTCAnimatingObject::PlayAnimation(const
//   CAnimInit&, long) 0x007F8DC0, the def's virtual release (slot 1).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCAnimatingObject.h (Ego_r.pdb): QueuedAnims +0x14 (STLport list head node); CAppearanceDef Animation +0x34.
// Shape: the entry is fetched in its own statement (the string and filter temporaries die before the list clear); the def stays
//   in ebx for the release at the end.
// Name fix (agentNF11 2026-11-11): moved off the local CDefPointer / CAppearanceDef / CAnimationSet / CAnimInit / list / CThing /
// CTCAnimatingObject stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/CTCAnimatingObject.h).
// 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@CThingBaseDef@@..V?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DC340 GetAnim
// (was GetAnimation returning a pointer), 0x0070C050 CAnimInit::Init (was Set).
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
#define FABLE_RETAIL_STLP_LIST
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
#include "engine/CTCAnimatingObject.h"

void CTCAnimatingObject::PlayDefaultAnimation()
{
    CDefPointer<const CAppearanceDef> appearance;
    ParentThing->PeekBaseDef().GetSubDef(&appearance);
    const CAnimationEntry& entry = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
    QueuedAnims.clear();
    CAnimInit init(6, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, true, 0, 100);
    PlayAnimation(init, -1);
}
