// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero() -- the guard turns on the hero: best enemy target,
// Grade: DIFFER(186v187) -- register choice only, at the guard-handle tail: retail `mov edx,[esi+30h] ; mov eax,[ecx+7Ch] ; shl edx,4 ; lea esi,[eax+edx]`, ours `mov esi,[esi+30h] ; mov edx,[ecx+7Ch] ; shl esi,4 ; add esi,edx` (the index temp coalesces with the handle in esi). Everything else matches.
// Tried (42 grades + 5 probes, none moved it): named reference / named pointer / &v[ID] / begin()+ID / ID+begin() / ID[begin()] / &front()+ID / &v[0]+ID / begin()[ID] / iterator += ID / (long)(int) index / index local / id+parent locals (ID first) / one pointer reused for both handles / handle declared at the top / block scopes / early return / inline helpers (ReleaseIfValid(H&), (parent,id), accessor taking this) / Game as the PDB reference (ORIGINAL_MEMBERS, kept) / /G7 /Oa /Ow /Ob1 /EHsc /Gy- / optimize("s") / QFE-4035 compiler. Mechanism found: the retail lea pattern (index edx, base eax) appears exactly when the source writes seh_Guards[ID] twice and c2 CSEs it across IsValid -- which our compiler only does with IsValid's real body visible (noinline), and then it also keeps ecx across the call (179v187), which retail does not. Next: find what gives c2 IsValid's mod/ref without its register summary.
// killable, damageable, pushable and moving in actions; then drop his own scripting handle and his slot in the parent's guard
// handle array.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3BA0, 187 bytes (+ int3 padding). __thiscall, `ret`.
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
// Layout: engine/CV_MurderTwistFinalScript.h (Ego_r.pdb names): seh_Guards +0x7c (std::vector of 16-byte handles), guard ID +0x30.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero()
{
    Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
    Game.EntitySetAsKillable(Me, true, true);
    Game.EntitySetAsDamageable(Me, true);
    Game.SetIsPushableByHero(Me, true);
    Game.EntitySetAsUseMovementInActions(Me, true);
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    CScriptGameResourceObjectScriptedThingBase& handle = ParentClass->seh_Guards[ID];
    if (handle.IsValid())
        handle.Release();
}
