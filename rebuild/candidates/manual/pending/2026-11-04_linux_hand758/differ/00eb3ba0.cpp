// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero() -- turn a sheriff's guard on the hero: best enemy target, killable, damageable, pushable and moving in actions, then drop his own scripting handle and the parent script's handle for this guard (seh_Guards[ID])
// Grade: DIFFER(186v187) -- (try b, shared CV_MurderTwistFinalScript header): seh_Guards[ID] address `mov esi,[esi+30h] ; add esi,edx` v retail `mov edx ; lea esi,[eax+edx]`
// Leaf: ?AttackHero@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3BA0, 187 bytes (+ int3 padding), thiscall, plain `ret` / tail jump to Release.
// Identity: Ego_r.pdb / FableWin.pdb ?AttackHero@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ, the
//   last private member of CMurderTwistSheriffGuard (engine/CV_MurderTwistFinalScript.h); no vtable references 0x00EB3BA0, so
//   the catalog's "Init" label is propagated. Same body as the landed CMurderTwistSheriff::AttackHero 0x00EB1EC0 except the
//   last handle: the guard's ID (+0x30) indexes the parent's 16-byte seh_Guards vector (+0x7C).
// Callees: CGameScriptInterface slots 70 GetHero, 451 GiveThingBestEnemyTarget, 517 EntitySetAsKillable, 516
//   EntitySetAsDamageable, 844 SetIsPushableByHero (CScriptThing by value, copy inline), 529 EntitySetAsUseMovementInActions;
//   CScriptGameResourceObjectScriptedThingBase::IsValid const 0x00CD23B9 / Release 0x00CD2770.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CV_MurderTwistFinalScript.h"
// Retail expands the copy at this row's by-value argument site.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->SetIsPushableByHero(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    CScriptGameResourceObjectScriptedThingBase& handle = ParentClass->seh_Guards[ID];
    if (handle.IsValid())
        handle.Release();
}
