// CTCAnimationComplex::OnCreate() -- reset the thing's animator and start the appearance's first "DEFAULT" animation as the main anim (type 6, mode 1, not looping, percent 100)
// Leaf: ?OnCreate@CTCAnimationComplex@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B4D0, 216 bytes (+ int3 padding), plain `ret`. Vtable 0x012650A4 slot 4
//   (.?AVCTCAnimationComplex@@); Ego_r.pdb / FableWin.pdb ?OnCreate@CTCAnimationComplex@@UAEXXZ.
// Callees (PDB spellings): CAnimateThing::ClearAllAnims 0x0070EC30, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CAnimationSet::GetAnim 0x005DC340, CAnimInit::Init 0x0070C050
//   (the inline argument constructor), CAnimateThing::SetMainAnim(const CAnimInit&, long, long) 0x0070D580, the def's virtual release.
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCAnimationComplex.h FULL (Ego_r.pdb, 0x10): PAnimation +0x0C; CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the base def read through a by-value GetPBaseDef() temporary that dies before the sub-def call (retail adds and drops the
//   reference, both count stores kept); the entry reference kept across the temporaries' destruction (retail `mov edi,eax ... mov edi,[edi]`).
// Name fix (agentNF11 2026-11-11): moved off the local CDefPointer / CAppearanceDef / CAnimationSet / CAnimInit / CAnimateThing /
// CThing / CTCAnimationComplex stand-ins onto the shared headers. 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ?GetSubDef@CThingBaseDef@@..V?$CDefPointer@VCAppearanceDef@@@@@Z), 0x0070EC30 ClearAllAnims (was ResetAnimations_0070ec30),
// 0x005DC340 GetAnim (was GetAnimation returning a pointer), 0x0070C050 CAnimInit::Init (was Set); the row's own symbol is the PDB
// public virtual UAEXXZ.
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
#define FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_ONCREATE
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

void CTCAnimationComplex::OnCreate()
{
    CAnimateThing* animation = PAnimation;
    animation->ClearAllAnims();
    const CThingBaseDef* base_def = ParentThing->GetPBaseDef().Get();
    CDefPointer<const CAppearanceDef> appearance;
    base_def->GetSubDef(&appearance);
    const CAnimationEntry& entry = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, false, 0, 100);
    animation->SetMainAnim(init, 1, 0);
}
