// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Main() -- a Murder Twist sheriff guard's entity script: alive
//   predicate on Me, melee-guard brain, scripted control at highest priority (retried each frame), registers himself and his handle
//   in the parent quest's Guards / seh_Guards slot ID, then runs his state machine on the parent's SheriffState / AssassinState
//   (ask hero to speak to boss, remind to kill assassin, ask again, congratulate, say thank you, or attack the hero), then idles
//   until the thread terminates.
// Leaf: ?Main@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB2140, 658 bytes, thiscall, plain `ret`, no EH frame (private virtual: EAE); slot 1 of the
//   entity vtable 0x012EFEE8 (only reference .rdata 0x012EFEEC). Ego_r.pdb names it (egor_pdb_names.tsv + egor_signatures.tsv).
// Callees: CScriptThing copy ctor 0x004ABE90 (inside the inline CIsThingAlive ctor, vtable 0x012C32F0), CEntityScriptBase::SetPredicate
//   0x00F35B10, ~CIsThingAlive 0x00E241B0, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptGameResourceObjectScriptedThingBase::IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   operator= 0x008ABD10, CScriptThing::operator= 0x008AB980, DoAskingHeroToSpeakToBoss 0x00EB23E0, DoRemindingHeroToKillAssassin
//   0x00EB2900, DoAskingHeroToSpeakToBossAgain 0x00EB2DC0, DoCongratulatingHero 0x00EB3280, DoSayingThankYouToHero 0x00EB3740,
//   AttackHero 0x00EB3BA0; CGameScriptInterfaceBase slots 7 NewScriptFrame (+0x1C), 8 StartScriptingEntity (+0x20),
//   589 SetCreatureBrain (+0x934).
// Layout: engine/CV_MurderTwistFinalScript.h (ParentClass +0x14, MyEntityHandle +0x1C, ID +0x30; parent SheriffState +0x48,
//   AssassinState +0x4C, Guards +0x60, seh_Guards +0x7C). AssassinState is compared with 4 (one past the header's named
//   enumerators; the landed CMurderTwistSheriff::Main 0x00EAEF30 compares the same value).
// Shape: generated entity-script code (TRetVoid _Fail_Val, a terminate check at every block start and after every wait loop).
//   Retail keeps a dead `mov dword ptr [esp+8],0` at 0x00EB214A: a conditional-destruction flag word (GOTCHAS line 230; same
//   form as CMurderTwistAssassin::Main 0x00EB3F40) whose every use folded away.
//   Inferred: RECONSTRUCTED DEAD CODE (behaves exactly as retail; owner rule 2026-11-12): the retail bytes prove the folded flag
//   but cannot give the text of code that emitted no instructions; the `dead && false && ...` line before the first state loop is
//   the spelling (and position) that reproduces it -- at the top or after the Guards registration the register choice differs.
// Flags: none
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetCreatureBrain(Me, "BRAIN_MELEE_GUARD");
    if (MyEntityHandle.IsValid()) MyEntityHandle.Release();
    while (!Game->StartScriptingEntity(Me, MyEntityHandle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass->Guards[ID] = Me;
    ParentClass->seh_Guards[ID] = MyEntityHandle;
    bool dead = false;
    if (dead && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { }
    while (ParentClass->SheriffState == SHERIFF_STATE_0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoAskingHeroToSpeakToBoss();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass->AssassinState == ASSASSIN_STATE_0 && ParentClass->SheriffState != SHERIFF_STATE_2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoRemindingHeroToKillAssassin();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass->AssassinState != (EAssassinState)4 && ParentClass->SheriffState != SHERIFF_STATE_2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoAskingHeroToSpeakToBossAgain();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass->AssassinState == (EAssassinState)4 && ParentClass->SheriffState == SHERIFF_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoCongratulatingHero();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass->SheriffState == SHERIFF_STATE_3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DoSayingThankYouToHero();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass->SheriffState == SHERIFF_STATE_2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AttackHero();
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}