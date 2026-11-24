// NScript::CQ_RansomVictimScript::CRansomHelper::Main() -- Ransom Victim, the ransom helper at the cave: wait for the hero (or the helper start); when hit turn bandit; play CS_RANSOM_CAVE_EXTERIOR_1 (father's guard alive) or _2 and open the demon door; then until the ransommer dies: walk home after being hit, answer the hero (_RESCUED_VICTIM_10 / _YET_TO_PAY_10); finally turn bandit
// Grade: RELOCATION_MATCH (B24A11, stock flags)
// Leaf: ?Main@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE9690, 3412 bytes, thiscall, plain `ret` (three epilogues: the hit-forever exit 0x00DE9A93, the shared 0x00DE9D0C, the loop tail after the backward jmp).
// Identity (proven, two sources): slot 1 (Main) of the CRansomHelper entity vtable 0x012DD1E4 stored by its Alloc 0x00DE1D00; Ego_r.pdb and
//   FableWin.pdb ?Main@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ (FableWin 0x0388CEA0).
// Model: the landed sibling CVictimsBrotherHelper::Main 0x00DEB280 (same script: defines, includes, CTimer bodies, movie COMDAT ctor/dtor
//   0x006E7B60 / 0x006E7B80). Fields (engine/CQ_RansomVictimScript.h, Ego_r.pdb): RescuedVictim +0x4B, RansommerDied +0x4C, RansomHelperStarted
//   +0x4D, RansomHelperSpoken +0x4E, RansomHelpersAttacked +0x50, ToldGuardToWaitOutside +0x51. Callees: PlayCutscene 0x00DE6730,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceFromThingToPositionUnder 0x00CBE4B7, CTimer 0x00CD4450 / ~ 0x00CD4470 (GetTime / = expanded
//   over pDSTGame 0x0143E8F8 slots 89 / 90), operator+ 0x0099F690 / 0x0099F600, CCharString::operator const char* 0x0099E4C0; Game slots 7, 8,
//   70, 72, 264, 370, 379, 425 OpenDoor, 477 SetThingPersistent, 601 EntitySetInFaction, 844 SetIsPushableByHero; Me slots 3 GetDataString,
//   7 GetHomePos, 21 / 41 / 42 hit tests, 27 MsgIsTalkedToBy, 75 IsAlive.
// Locals (Ego_d.pdb records + FableWin 0x0388CEA0): function scope _Fail_Val (TRetVoid), seh_me, ReturnToCaveTimer, BeenHit; four blocks
//   {speech_movie, PauseMyEntitiesPlease}; every terminate exit calls TRetVoid::Return (FableWin 0x017EA518 at each).
// CTimer bodies: the real header-family text (B24B SHARED_FORM, FableWin-hash-exact) is visible in the TU, as in retail's object
//   (register_scripts.obj holds the quest scripts and the CTimer bodies). Retail still CALLS the ctor / dtor (0x00DE9C6E / 0x00DE9D07), but
//   with the bodies visible c2 knows `this` does not escape, so ReturnToCaveTimer is live only from its construction and shares its frame
//   slot with the first hit test's second CCharString temporary (esp+0x14), as retail; with the ctor / dtor only declared the timer is live
//   in the whole function and every hit-test temporary moves (46 hunks).
// Inline-budget: retail expands CTimer::operator= / GetTime / operator== (pDSTGame slots 89 / 90 inline at 0x00DE9E69 / 0x00DE9E91); at the
//   stock threshold operator== is called (4 hunks) unless the three are FABLE_FORCEINLINE (landed CTimer spelling, 99 rows).
// Inline-budget: #pragma inline_depth(0) round the row-local CIsThingAlive ctor (+ FABLE_RETAIL_CPREDICATEBASE_NOVTABLE): retail CALLS the
//   nested CScriptThing copy 0x004ABE90 inside the expanded CIsThingAlive ctor (0x00DE96B9) -- the chunk H40 predicate lever (landed
//   0x00D82640 / 0x00D8BCD0, staged 0x00ED8680); the empty ~CIsThingAlive body (Ego_r 0x00E241B0) is the header's own contract, also
//   under inline_depth(0) (landed 0x00D973D0 form) so the emitted ??_GCIsThingAlive copy is the family copy (30 bytes, = staged 0x00ED8680
//   and landed 0x00D973D0); Main still expands it at SetPredicate as retail (0x00DE96D1).
// Pick-any (every emitted copy v Fable.exe, relocations masked): EQ ??0/??1CTimer, ??1 / ??_G ScriptedThingBase (0x007E74D0 / 0x007E7520,
//   the implicit dtor -- no OUTOFLINE_DTOR opt-in), ??_GCBaseClass, ??_GCScriptGameResourceObjectBase and the reloc_bindings set; NEQ only
//   ??_GCScriptThing / ??_GCIsThingAlive / ??1CIsThingAlive (vtable-only, not leaf-referenced), byte-identical to 0x00ED8680's and landed rows'.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
#include "engine/EScriptAIPriority.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#include "engine/fable_script_text.h"
bool FABLE_FASTCALL IsDistanceFromThingToPositionUnder(const CScriptThing&, const C3DVector&, float);   // 0x00CBE4B7
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE bool CTimer::operator==(long time) const { return GetTime() == time; }
void NScript::CQ_RansomVictimScript::CRansomHelper::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    while (!ParentClass.RansomHelperStarted && !IsDistanceBetweenThingsUnder(Me,Game->GetHero(),10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.RansomHelpersAttacked=true;
            Game->SetIsPushableByHero(Me,true);
            Game->EntitySetInFaction(Me,"FACTION_BANDITS");
            if (seh_me.IsValid()) seh_me.Release();
            while (true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    if (!ParentClass.RansomHelperStarted) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.RansomHelperStarted=true;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game->GetThingWithScriptName("VictimsFatherGuard").IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_1","NULL");
            ParentClass.ToldGuardToWaitOutside=true;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_2","NULL");
        }
        Game->OpenDoor(Game->GetThingWithScriptName("DemonDoorDoor"));
        Game->SetThingPersistent(Game->GetThingWithScriptName("DemonDoorDoor"),true);
        ParentClass.RansomHelperSpoken=true;
    }
    while (!ParentClass.RansomHelperSpoken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer ReturnToCaveTimer;
    bool BeenHit=false;
    while (!ParentClass.RansommerDied) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.RansomHelpersAttacked=true;
            break;
        }
        if (Me.MsgIsHitBy("")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            ReturnToCaveTimer=8;
            BeenHit=true;
        }
        if (BeenHit && ReturnToCaveTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(Me.GetHomePos(),1.0f,(EScriptEntityMoveType)1,false,true);
            }
            if (IsDistanceFromThingToPositionUnder(Me,Me.GetHomePos(),2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                BeenHit=false;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !BeenHit) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_RESCUED_VICTIM_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_YET_TO_PAY_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetIsPushableByHero(Me,true);
    Game->EntitySetInFaction(Me,"FACTION_BANDITS");
    if (seh_me.IsValid()) seh_me.Release();
    while (true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
