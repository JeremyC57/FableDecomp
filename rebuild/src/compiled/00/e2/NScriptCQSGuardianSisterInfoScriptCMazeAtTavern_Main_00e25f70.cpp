// NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main() -- Maze in the Bowerstone tavern: live while alive, take scripted control, become an opinion source with a quest-core minimap marker and information, not pushable; each frame: wave the hero over once (ST_HELLO + TEXT_QST_027_MAZE_CALL_HERO_OVER_10) when he comes within GSI_MazeCallsOverDistance, face him until the guardian has spoken, answer a hit (not a heal-life spell) with TEXT_QST_027_MAZE_ON_HIT_10 under a paused speech movie and calm down; when talked to cast ME and HERO in CS_GUARDIANSISTER_BOWERSTONE, mark GuardianSpokeToHero, complete and deactivate the quest and remove herself
// Inline-budget: scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor (with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE, the landed predicate idiom of 0x00E27E90 / 0x00E5B3B0) -- retail expands the predicate ctor but CALLS its nested CScriptThing copy 0x004ABE90 (0x00E25F92) while it expands the by-value CScriptThing copy in the SetIsPushableByHero argument slot (0x00E260E2..0x00E26104; FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY). c2 trace without the pragma: nested site sc=28, by-value site sc=56, T=74, both EXPAND; retail expands the colder site, so no flag splits them (/d2inlT20..70 give 2028..2070, /d2inlS5..20 give 2070..2073, never the match). Ablation (each device removed in turn): no pragma 2070v2049, no NOVTABLE 2066v2049, no IMPLICIT_COPY 2031v2049, no FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR 2062v2049; the draft's four STB / MovieBase OUTOFLINE opt-ins were unneeded (RELOCATION_MATCH without them) and are dropped. Depth 0 and 1 both match; 0 is the smallest and the landed idiom. The family blocker of docs/pipeline/INLINE_DEPTH_REVISIT.md section 3.
// Leaf: ?Main@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25F70, 2049 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the CMazeAtTavern entity vtable stored by the landed Alloc 0x00E25C70 (Init 0x00E25D20,
//   GetParentScript 0x00E25D10, OnPredicateFail 0x00E25D30 landed); Ego_r.pdb ?Main@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive (vtable 0x012C32F0, dtor 0x00E241B0) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380, ScriptedThingBase ctor 0x007E72A0 (seh_hero; seh_me's
//   expanded) / operator= 0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / GetScriptThing 0x007E7490 /
//   Speak 0x007E7390 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80,
//   the cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, CCharString 0x0099EBF0 / ~ 0x0099EAE0, pGlobalScriptDef 0x0143E90C (GSI_MazeCallsOverDistance +0xC68),
//   NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748; CGameScriptInterfaceBase slots 7, 8, 70, 72, 108
//   RemoveThing, 264, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 350 MiniMapAddMarker, 360 / 361, 364-366, 370, 379, 452
//   ClearThingBestEnemyTarget, 475, 622 EntitySetAsOpinionSource, 655 GetActiveQuestName, 844 SetIsPushableByHero (by-value
//   CScriptThing, copy expanded); CScriptThing slots 6 GetPos, 21, 27, 41, 42, 67 SetFriendsWithEverythingFlag.
// Layout: engine/CQS_GuardianSisterInfoScript.h (NEW; Ego_r.pdb CMazeAtTavern 40 bytes = retail new size: ParentClass +0x14, PMasterData
//   +0x18, TalkCounter +0x1c, WavedOver +0x20, HitCount +0x24; parent 0x4C, GuardianSpokeToHero Ego_r +0x5c = retail +0x48).
// Source shape: Ego_d.pdb line table (lines 58..139) and locals (ExitDistance, TriggerDistance, seh_me, _Fail_Val, ExitPos;
//   HLStyleConversationIndex; speech_movie, PauseMyEntitiesPlease; seh_hero, speech_movie, PauseMyEntitiesPlease, mymap), written in the
//   generated quest-script text of the twin CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Main 0x00E26E30 (TRetVoid
//   returns, `for (;!false;Game->NewScriptFrame())` frame loop with its terminate test first). ExitPos (line 66) and ExitDistance (line
//   73, GSI_MazeRegionExitDistance +0xC6C) are dead locals: retail keeps only the GetPos call and no load of +0xC6C.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CQS_GuardianSisterInfoScript.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();              // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();             // 0x00CDBFB0

void NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetAsOpinionSource(Me, "OPINION_SOURCE_MAZE_BS_PUB");
    C3DVector ExitPos = Game->GetThingWithScriptName("M_MazeExit").GetPos();
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetIsPushableByHero(Me, false);
    long TriggerDistance = pGlobalScriptDef->GSI_MazeCallsOverDistance;
    long ExitDistance = pGlobalScriptDef->GSI_MazeRegionExitDistance;
    for (;!false;Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.GuardianSpokeToHero && !WavedOver && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, (float)TriggerDistance)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.PlayAnimation("ST_HELLO", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", false, Me, Game->GetHero());
            WavedOver = true;
        }
        if (!ParentClass.GuardianSpokeToHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_027_MAZE_ON_HIT_10", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++HitCount;
            Me.SetFriendsWithEverythingFlag(true);
            Game->ClearThingBestEnemyTarget(Me);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ME"] = seh_me;
                mymap["HERO"] = seh_hero;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                RunCutsceneMacro("CS_GUARDIANSISTER_BOWERSTONE", mymap, 0, 0, false, true);
            }
            ParentClass.GuardianSpokeToHero = true;
            Game->ClearThingHasInformation(Me);
            Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
            Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
            Game->RemoveThing(Me, false, true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}