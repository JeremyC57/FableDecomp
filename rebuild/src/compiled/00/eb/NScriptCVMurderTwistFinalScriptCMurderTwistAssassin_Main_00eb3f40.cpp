// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main() -- the assassin's entity script: with the alive predicate set and the
//   entity scripted at highest priority, note once whether the assassin trusts the hero, watch for trouble every frame while
//   AssassinState is 0 or 1, remind the hero to kill the sheriff every frame while it is 3, attack the hero if it is 2, then idle
//   until the thread terminates.
// Leaf: ?Main@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3F40, 381 bytes, thiscall, plain `ret`, no EH frame (private virtual: EAE).
// Identity: slot 1 of the entity vtable 0x012F00BC (only reference .rdata 0x012F00C0); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ; engine/CV_MurderTwistFinalScript.h declares it at 0x00EB3F40.
// Callees: CScriptThing copy ctor 0x004ABE90 (inside the inline CIsThingAlive ctor, vtable 0x012C32F0), CEntityScriptBase::SetPredicate
//   0x00F35B10, ~CIsThingAlive 0x00E241B0, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30,
//   CScriptGameResourceObjectScriptedThingBase::IsValid 0x00CD23B9 / Release 0x00CD2770, CV_MurderTwistFinalScript::DoesAssassinTrustHero
//   0x00EB03A0, DoWatchingForTrouble 0x00EB40C0, DoRemindingHeroToKillSheriff 0x00EB4F80, AttackHero 0x00EB53A0;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame (+0x1C), 8 StartScriptingEntity (+0x20).
// Layout: engine/CV_MurderTwistFinalScript.h opt-in FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN (ParentClass +0x14, MyEntityHandle
//   +0x1C; parent AssassinState +0x4C); Game +0x04 and Me +0x08 from CEntityScriptBase.
// Shape: generated entity-script code (TRetVoid _Fail_Val, a terminate check at every block start and after every wait loop).
//   Retail keeps two things the optimiser left behind, and the twin CMurderTwistAssassinGuard::Main 0x00EB55D0 has both too:
//   (1) `mov bl,al` at 0x00EB3FF4: the trust result is kept in a callee-saved register but never read. VC7.1 does that when the
//       value is tested by an if / else whose two arms compile to the same code (the arms are merged after register allocation);
//       it also explains loop A's second terminate check (0x00EB401C), the block-start check of the arm.
//   (2) `mov dword ptr [esp+0Ch],0` at 0x00EB3F4F: a conditional-destruction flag word (same slot and form as the one in
//       DoesAssassinTrustHero 0x00EB03B3 and CM_HiddenBooty::Main 0x00E9312C) whose every use folded away, i.e. the source had a
//       conditionally built class temporary inside a condition the optimiser (not the front end) proved false.
//   Inferred: RECONSTRUCTED DEAD CODE (behaves exactly as retail; owner rule 2026-11-12): the retail bytes prove (1) and (2) but cannot give the text of code that emitted no instructions.
//   The `if (trust && false && ...)` line and the duplicated arms below are the smallest spelling found that reproduces both; the
//   original wording of those script lines is unknown (see NOTES.md).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main()
{
    TRetVoid _Fail_Val;
    
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (MyEntityHandle.IsValid()) MyEntityHandle.Release();
    while (!Game->StartScriptingEntity(Me, MyEntityHandle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    bool trust = ParentClass->DoesAssassinTrustHero();
    if (trust && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { }
    while (ParentClass->AssassinState == ASSASSIN_STATE_0 || ParentClass->AssassinState == ASSASSIN_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (trust) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoWatchingForTrouble();
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoWatchingForTrouble();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass->AssassinState == ASSASSIN_STATE_3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoRemindingHeroToKillSheriff();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass->AssassinState == ASSASSIN_STATE_2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AttackHero();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}