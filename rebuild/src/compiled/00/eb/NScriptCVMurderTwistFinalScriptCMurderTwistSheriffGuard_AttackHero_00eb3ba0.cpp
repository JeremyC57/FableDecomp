// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero() -- the guard turns on the hero: best enemy target,
// killable, damageable, pushable and moving in actions; then drop his own scripting handle and his slot in the parent's guard
// handle array.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3BA0, 187 bytes (+ int3 padding). __fastcall, `ret`.
// Leaf: ?AttackHero@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Identity: the catalog label "CMurderTwistSheriff::Init" is propagated. The only caller is 0x00EB23B2 inside
//   CMurderTwistSheriffGuard::Main (0x00EB2140); the indexed handle uses retail +0x30, which is CMurderTwistSheriffGuard::ID in
//   Ego_r.pdb (CMurderTwistSheriff has no index there); Ego_r.pdb / FableWin.pdb list the guard's private AttackHero right after
//   DoSayingThankYouToHero (retail 0x00EB3740), the function before this one. Same body shape as the landed
//   CMurderTwistSheriff::AttackHero 0x00EB1EC0.
// Callees (CGameScriptInterfaceBase virtuals): GetHero 70 (+0x118), GiveThingBestEnemyTarget 451 (+0x70c), EntitySetAsKillable
//   517 (+0x814), EntitySetAsDamageable 516 (+0x810), SetIsPushableByHero 844 (+0xd30, CScriptThing by value: inline copy ctor),
//   EntitySetAsUseMovementInActions 529 (+0x844); CScriptGameResourceObjectScriptedThingBase::IsValid 0x00CD23B9 / Release
//   0x00CD2770 (tail jump).
// Shape: the guard handle is spelled seh_Guards[ID] twice (IsValid, then Release). IsValid is an inline member (its retail body
//   0x00CD23B9 `xor eax,eax ; cmp [ecx+8],eax ; setne al` sits unaligned among the COMDATs = an emitted inline, not a TU function);
//   its real body is defined here as an inline (select-any COMDAT) that this TU calls out of line (__declspec(noinline)). c2 then
//   knows IsValid stores nothing and CSEs the second &seh_Guards[ID] across the call (index edx, base eax, `lea esi,[eax+edx]`),
//   but, the COMDAT being replaceable at link, it does not use IsValid's register summary (ecx is reloaded after the call).
//   A plain out-of-line (non-inline) noinline body gives both (keeps ecx, 179 bytes); a declaration only gives no CSE (186 bytes).
// Layout: engine/CV_MurderTwistFinalScript.h (Ego_r.pdb names): seh_Guards +0x7c (std::vector of 16-byte handles), guard ID +0x30.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CGameScriptInterfaceBase.h"

// Real body of the retail COMDAT 0x00CD23B9 (PImp.Data != 0); inline, emitted out of line in this TU.
inline __declspec(noinline) bool CScriptGameResourceObjectScriptedThingBase::IsValid() const
{
    return PImp.Data != 0;
}

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero()
{
    Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
    Game.EntitySetAsKillable(Me, true, true);
    Game.EntitySetAsDamageable(Me, true);
    Game.SetIsPushableByHero(Me, true);
    Game.EntitySetAsUseMovementInActions(Me, true);
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    if (ParentClass->seh_Guards[ID].IsValid())
        ParentClass->seh_Guards[ID].Release();
}