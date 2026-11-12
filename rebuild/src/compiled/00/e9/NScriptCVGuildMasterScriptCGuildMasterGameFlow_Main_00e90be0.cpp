// NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main() -- advance Guild Master game-flow dialogue
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Focal-site introduction and teleporter, guildmaster conversations and wizard quest-card choice.
// Leaf: ?Main@CGuildMasterGameFlow@CV_GuildMasterScript@NScript@@EAEXXZ
// Retail: 0x00E90BE0, 4914 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold
//   ?Main@CGuildMasterGameFlow@CV_GuildMasterScript@NScript@@EAEXXZ (private virtual); the address has one .rdata reference, slot 1
//   of the entity vtable 0x012EC184 that the landed CGuildMasterGameFlow::Alloc 0x00E908D0 stores (slot 0 ??_G 0x00E90980, slot 2 Init
//   0x00E909A0, slot 3 GetParentScript 0x00E90970, all landed).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_GUILDMASTERSCRIPT_GETMASTERDATA
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_GuildMasterScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EHeroAbility.h"
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
bool FABLE_FASTCALL IsDistanceFromThingToPositionUnder(const CScriptThing&, const C3DVector&, float); // 00CBE4B7

void NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (Game.IsQuestActive("Q_EndGameFocalSites")) {
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"] = seh_hero;
                mymap["GUILDDUDE"] = seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_FOCALSITEINTRO", mymap, 0, 0, false, true);
                Game.FixMovieSequenceCamera(false);
            }
            Game.SetTeleportingAsActive(false);
            Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES", "Q_EndGameFocalSites", false);
            Game.SetQuestCardObjective("Q_EndGameFocalSites", "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01", "", "");
            CScriptThing Effect = Game.CreateEffect("NEWTELEPORTER2", Game.GetThingWithScriptName("TeleporterResidue").GetPos(), "", 0.0f, false, false);
            Game.MiniMapAddMarker(Game.GetThingWithScriptName("TeleporterResidue"), "HUD_ORB_QUEST_CORE");
            while (Game.IsQuestActive("Q_EndGameFocalSites")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (IsDistanceFromThingToPositionUnder(Game.GetHero(), Game.GetThingWithScriptName("TeleporterResidue").GetPos(), 3.0f)) {
                    if (IsActiveThreadTerminating()) return;
                    Game.EntityTeleportToThing(Game.GetHero(), Game.GetThingWithScriptName("FocalSitesHSP"), false);
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), ParentClass.GetGuildMasterSpeech(), GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_081_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (GetMasterData()->PostSavePosition == EGP_WAITING_FOR_WIZARD_BATTLE) {
                if (IsActiveThreadTerminating()) return;
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long YNQAnswer;
                while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (YNQAnswer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false);
                    Game.TakeObjectFromHero("OBJECT_MAZE_JOURNAL");
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), ParentClass.GetGuildMasterSpeech(), GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
}