// CTCClock::OnInitialActivate() -- reset the last-updated time to 0.5 and start the clock's "CLOCKHAND_TURN" appearance animation as the main animation (type 0, looping, mode 1, percent 100)
// Leaf: ?OnInitialActivate@CTCClock@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732850, 247 bytes (+ int3 padding), plain `ret`. Vtable 0x01267044 slot 6
//   (.?AVCTCClock@@); Ego_r.pdb / FableWin.pdb ?OnInitialActivate@CTCClock@@UAEXXZ.
// Callees (PDB spellings): CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the forced-inline GetTC<CTCAnimationComplex>,
//   TCI_ANIMATION 0x5A), CTCAnimationComplex::GetAnimation 0x0070B460, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2,
//   CAnimationSet::GetAnim 0x005DC340, CAnimInit::Init 0x0070C050 (the inline argument constructor), CAnimateThing::Init(const CAnimInit&)
//   0x0070D920 (Ego_r.pdb / FableWin.pdb ?Init@CAnimateThing@@QAEXABVCAnimInit@@@Z: Ego_r donor order SetMainAnim 0x00577F00 / Init
//   0x00578140 = retail SetMainAnim 0x0070D580 / 0x0070D920, whose body forwards (init, 1, 0) to SetMainAnim), the def's virtual
//   release (slot 1).
// Globals: the static CCharString "CLOCKHAND_TURN" @ 0x013BAD3C (its dynamic initializer is landed at 0x0121ED40).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCClock.h (Ego_r.pdb, 0x14): PDef +0x0c, LastTimeUpdated +0x10; CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the base def read through a by-value GetPBaseDef() temporary that dies before the sub-def call (retail adds and drops the
//   reference, both count stores kept, then calls GetSubDef on the raw pointer); the absent-component branch passes the unset
//   out-pointer (retail `mov ecx,[esp+10h]`).
// Name fix (agentNF11 2026-11-11): moved off the local CDefPointer / CThingBaseDef / CAppearanceDef / CAnimationSet / CAnimInit /
// CAnimateThing / CThing / CTCClock stand-ins onto the shared headers (new engine/CTCClock.h). 0x0042B0A2 binds GetSubDef<CAppearanceDef>
// (was ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DC340 GetAnim (was GetAnimation returning a pointer),
// 0x0070C050 CAnimInit::Init (was Set), 0x0070D920 CAnimateThing::Init (was a one-argument SetMainAnim); the row's own symbol is the
// PDB public virtual UAEXXZ.
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
#include "engine/CTCClock.h"

extern CCharString g_ClockHandTurnName;                      // 0x013BAD3C ("CLOCKHAND_TURN")

void CTCClock::OnInitialActivate()
{
    LastTimeUpdated = 0.5f;
    CTCAnimationComplex* complex;
    ParentThing->GetTC(&complex);
    CAnimateThing& animation = complex->GetAnimation();
    const CThingBaseDef* base_def = ParentThing->GetPBaseDef().Get();
    CDefPointer<const CAppearanceDef> appearance;
    base_def->GetSubDef(&appearance);
    const CAnimationEntry& entry = appearance->Animation.GetAnim(g_ClockHandTurnName, 0, CAnimFilterNoGroup());
    CAnimInit init(0, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, true, 0, 100);
    animation.Init(init);
}
