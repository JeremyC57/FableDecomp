// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::AttackHero() -- the assassin turns on the hero: best enemy target, killable, damageable, pushable and moving in actions, clear his friends-with-everything flag, then drop his scripting handle
// Leaf: ?AttackHero@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// NF28 (2026-11-12, queue land1011ei): was landed as CMurderTwistSheriff::Init (propagated catalog label, local model). Identity: the only
//   caller is CMurderTwistAssassin::Main 0x00EB4093 (landed 0x00EB3F40 calls AttackHero() here); FableWin.pdb
//   ?AttackHero@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ (264 B, same hash as the AssassinGuard copy); the class is
//   engine/CV_MurderTwistFinalScript.h FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN (MyEntityHandle +0x1C).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB53A0, 166 bytes (+ int3 padding). __fastcall, `ret`.
// Callees (CGameScriptInterface virtuals, ghidra_out/cgsi_slots_aligned.tsv): GetHero 70 (+0x118) 0x00891CA0,
//   GiveThingBestEnemyTarget 451 (+0x70c) 0x008A0260, EntitySetAsDamageable 516 (+0x810) 0x0088EAD0, EntitySetAsKillable 517
//   (+0x814) 0x0088EB30, EntitySetAsUseMovementInActions 529 (+0x844) 0x0088EC20, SetIsPushableByHero 844 (+0xd30) 0x008A6DD0
//   (CScriptThing BY VALUE: inline CBaseClass vtable 0x01231710 then CScriptThing vtable 0x01238C8C stored into the argument area,
//   PImp copied with add-ref); the handle's IsValid 0x00CD23B9 / Release 0x00CD2770 (manifest labels
//   "C3DMeshInfo::HasPhysicsMesh" / "CMemoryDataOutputStream::Clear" are propagated; see landed CFH_BriarRose::Main 0x00D37330).
// Shape: the nested GetHero call fetches the interface vtable once (ebx); the by-value CScriptThing copy ctor is inline
//   (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY); SetFriendsWithEverythingFlag(false) (CScriptThing slot 67) before the handle release,
//   which is a tail jump.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::AttackHero()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->SetIsPushableByHero(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    Me.SetFriendsWithEverythingFlag(false);
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
}
