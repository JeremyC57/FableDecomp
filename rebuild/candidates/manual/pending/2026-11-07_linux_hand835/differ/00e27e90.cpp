// NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main() -- Maze in the Guild tells the hero about the trophy dealer: take him over, mark him on the mini-map and show the quest start screen; when the hero hits him (setup, hit-me and info cutscenes) or talks to him (setup and info cutscenes) give the trophy-dealer quest card, open the Witchwood teleporter, face him back, add the logbook entry, complete the quest and set PieceOver; then answer every later hit (on-hit and repeat lines) or talk (repeat line)
// Grade: DIFFER(3595v3595) -- 31 masked lines, nested-copy blocker: retail expands the CIsThingAlive/CIsThingConscious ctor (and dtor) but CALLS the nested CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 while expanding the by-value copies in the same function; VC7.1 on the lane makes the outer inline tree all-or-nothing
// Next idea: structural, not a budget (chunk 835: retail calls the nested copy at 100% of ~500 construction sites incl. tiny ones; /d2inlT, __forceinline, definition order, pragma regions other than inline_depth, /Ob1 /Os /Ot /Oa /Gy /GX /EHsc /Zi, a two-TU /GL+/LTCG build: none reproduce it). Only a region `#pragma inline_depth(0)` round the row-local predicate ctor/dtor definitions does (diagnostic: md 22; residue = a CALL to the nested CPredicateBase() ctor); forbidden in rows -- owner question in NOTES.md
// Leaf: ?Main@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E27E90, 3595 bytes. Identity: CGTDI_Maze vtable slot .rdata 0x012E20A8 -> ?Main@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ (Ego_r.pdb + FableWin.pdb). Layout: engine/CQS_GuardianTrophyDealerInfoScript.h (new; CGTDI_Maze 0x20, InitialAngle +0x1C). Callees: SetPredicate 0x00F35B10, ~CIsThingAlive 0x00E241B0, AddLogbookStoryEntry 0x00CBE87F (__fastcall), ScriptedThingBase operator= 0x008ABD10.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQS_GuardianTrophyDealerInfoScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
void FABLE_FASTCALL AddLogbookStoryEntry(long index);                   // 0x00CBE87F
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}

void NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), false, false);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid())
                seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["MAZE"] = seh_me;
            cast["HERO"] = seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_MAZE_TROPHY_INFO_SETUP", cast, 0, 0, false, false);
            RunCutsceneMacro("CS_MAZE_TROPHY_INFO_HITME", cast, 0, 0, false, true);
            RunCutsceneMacro("CS_MAZE_TROPHY_INFO", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            break;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid())
                seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["MAZE"] = seh_me;
            cast["HERO"] = seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_MAZE_TROPHY_INFO_SETUP", cast, 0, 0, false, false);
            RunCutsceneMacro("CS_MAZE_TROPHY_INFO", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            break;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIND_TROPHY_DEALER", "V_TrophyDealer", false);
    Game->SetTeleporterAsActive(Game->GetThingWithScriptName("WitchwoodTeleporter"), true);
    Game->ClearThingHasInformation(Me);
    Game->MiniMapRemoveMarker(Me);
    Game->EntitySetFacingAngle(Me, InitialAngle, false);
    AddLogbookStoryEntry(130);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    ParentClass.PieceOver = true;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_077_MAZE_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_077_MAZE_REPEAT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_077_MAZE_REPEAT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        Game->NewScriptFrame();
    }
}
