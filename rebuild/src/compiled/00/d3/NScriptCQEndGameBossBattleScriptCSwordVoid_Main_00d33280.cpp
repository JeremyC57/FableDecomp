// NScript::CQ_EndGameBossBattleScript::CSwordVoid::Main() -- the Sword of Aeons choice in the Void: wait for the final decision, add the core quest orb marker on Me, then until the decision is made: whenever the hero is within 3 units and the re-ask timer has run out, ask the sword question (yes/no) and on "yes" take the hero under scripted control and play CS_JACK_BOSS_ENDING_GOOD inside a paused movie sequence
// Inline-budget: the inline ~CTimer is expanded at the in-loop thread-exit pads and called out of line (0x00CD4470) at the post-loop checks; `#pragma inline_depth(0)` round those `return` statements and the closing `return` (the landed 0x00D73F40 device; a region open at the closing brace also stops the handle's implicit ctor expanding its CCountedPointer ctor) reproduces that split.
// Leaf: ?Main@CSwordVoid@CQ_EndGameBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D33280, 1190 bytes (+ int3 padding), thiscall, plain `ret`; entity vtable 0x012CC6EC slot 1.
// Identity: registration "SwordVoid" 0x00D2F839 in CQ_EndGameBossBattleScript::RegisterMain 0x00D2F7F0, Alloc 0x00D2FCE0, vtable slot 1;
//   Ego_r.pdb / FableWin.pdb ?Main@CSwordVoid@CQ_EndGameBossBattleScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive (vtable 0x012C32F0) over CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF and RunCutsceneMacro 0x00CBFB7D (__fastcall),
//   CBaseObject() 0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / operator=
//   0x008ABD10, movie dtor 0x006E7B80, map<CCharString, CScriptGameResourceObjectScriptedThingBase> ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, ~CTimer 0x00CD4470, CCharString 0x0099EBF0 / 0x0099EAE0; Game slots 7, 8, 39 MsgIsQuestionAnsweredYesOrNo,
//   70 GetHero, 114 GiveHeroYesNoQuestion, 350 MiniMapAddMarker, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities; timers
//   87-90 through pDSTGame 0x0143E8F8. Literals "HUD_ORB_QUEST_CORE" 0x01258AB0, "TEXT_QST_039_SWORD_QUESTION" 0x012CCA5C,
//   "TEXT_OBJECT_HERO_ANSWER_YES" 0x012C2188, "TEXT_OBJECT_HERO_ANSWER_NO" 0x012C216C, "" 0x0122D70E, "Hero" 0x012C4570,
//   "CS_JACK_BOSS_ENDING_GOOD" 0x012CCA40.
// Data: ParentClass FinalDecision +0x49, FinalDecisionMade +0x4a, FinalCutscenePlayed +0x4d (engine/CQ_EndGameBossBattleScript.h);
//   PMasterData JackBossBattleResult +0x88 (engine/CQ_SunnyvaleMasterData.h).
// Shape: the final idle loop as the landed 0x00CFCCA0 / 0x00D43190 (`if (terminating) return; do NewScriptFrame(); while (!terminating)`):
//   the pre-check and the bottom test share one test, so retail enters the loop by `jmp` to its test after the in-loop exits.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CSWORDVOID
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
// Retail expands these at this row's sites: defined inline after the headers.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
using NScript::CIsThingAlive;

void NScript::CQ_EndGameBossBattleScript::CSwordVoid::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.FinalDecision) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    CTimer timer;
    timer = 2;
    while (!ParentClass.FinalDecisionMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        while (!((IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.0f) && timer.GetTime() <= 0) || ParentClass.FinalDecisionMade)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        if (ParentClass.FinalDecisionMade)
            continue;
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        timer = 10;
        Game->GiveHeroYesNoQuestion("TEXT_QST_039_SWORD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", false);
        long answer = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            answer = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        if (answer != 1)
            continue;
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        ParentClass.FinalDecisionMade = true;
        PMasterData->JackBossBattleResult = 2;
        CScriptGameResourceObjectScriptedThingBase hero;
        if (hero.IsValid())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        {
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"] = hero;
            RunCutsceneMacro("CS_JACK_BOSS_ENDING_GOOD", cast, 0, 0, false, true);
            ParentClass.FinalCutscenePlayed = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}