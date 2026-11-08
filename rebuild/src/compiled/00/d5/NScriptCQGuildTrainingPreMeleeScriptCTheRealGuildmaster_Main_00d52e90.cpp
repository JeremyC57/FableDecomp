// NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main() -- the Guildmaster's pre-melee lesson: waits for the teleport, comments while the hero walks up, then the punch and stick cut scenes with seven counted dummy hits each, the smashed-dummy experience orb, the alarm cut scene and the woods quest card; afterwards he guards the woods exit (woods-won cut scene, end question, beetle comments) until the hero agrees to leave, then the hero sleeps
// Leaf: ?Main@CTheRealGuildmaster@CQ_GuildTrainingPreMeleeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D52E90, 11892 bytes (code to 0x00D55CF4, then the 4-entry comment switch table to 0x00D55D04),
//   thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012CF83C (slot 0 the landed
//   scalar deleting dtor 0x00D51EE0, slot 2 Init 0x00D51ED0, slot 3 GetParentScript 0x00D51EC0, all CTheRealGuildmaster); Ego_r.pdb
//   and FableWin.pdb both hold ?Main@CTheRealGuildmaster@CQ_GuildTrainingPreMeleeScript@NScript@@EAEXXZ.
// Model: the landed melee twin CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main 0x00D58490 (defines, includes, macros).
// Levers: a __forceinline CScriptThing::GetPos (the dummy position expands as retail); each dummy phase's quest counter in its
//   own block (its frame slot is reused by the last loop's line temporary, as retail) with one function-scope `long lastHits`.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingPreMeleeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
void FABLE_FASTCALL AddLogbookTutorialEntry(const CCharString&);     // 0x00CBE9EE
void FABLE_FASTCALL AddLogbookTutorialEntryPC(const CCharString&);   // 0x00CBEA81
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);   // 0x00704580
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline CScriptThing::~CScriptThing() { }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define GM_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define GM_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        GM_T; \
    } \
    GM_T
#define GM_LINE(conversation, text) \
    Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero())
#define GM_SAY(text) { \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        GM_LINE(conversation, text); \
    }
#define GM_WAIT_GAME_INFO \
    while (!Game->MsgIsGameInfoClickedPast()) { \
        Game->NewScriptFrame(); \
        GM_T; \
    } \
    GM_T
#define GM_SPEAK(text) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            GM_T; \
        } \
        GM_T; \
    }
#define GM_END_QUESTION \
    Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true); \
    long answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer < 0) { \
        Game->NewScriptFrame(); \
        GM_T; \
        answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    GM_T; \
    if (answer == 1) { \
        GM_T; \
        Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255)); \
        Game->Pause(1.0f); \
        Game->PlayAVIMovie("Data\\Video\\2_guild_split_1_comp.xmv"); \
        waiting = false; \
    } else { \
        GM_T; \
        GM_SPEAK("TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO") \
        seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_GUARD").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true); \
        facedHero = false; \
    }

void NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass.GuildmasterTeleport) {
        Game->NewScriptFrame();
        GM_T;
    }
    GM_T;
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "");
    CScriptGameResourceObjectScriptedThingBase seh_me;
    GM_START_SCRIPTING(Me, seh_me);
    Game->EntitySetAsKillable(Me, false, true);
    Game->SetIsPushableByHero(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_MeleeTeacherStand"), false);
    CTimer CommentTimer;
    CommentTimer = 0;
    long commentCount = 0;
    bool waiting = true;
    while (waiting) {
        Game->NewScriptFrame();
        GM_T;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            GM_T;
            waiting = false;
        }
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0) {
            GM_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            CommentTimer = 5;
            switch (commentCount) {
            case 0:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST");
                seh_me.PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                commentCount = 1;
                break;
            case 1:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND");
                commentCount = 2;
                break;
            case 2:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD");
                commentCount = 3;
                break;
            case 3:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH");
                commentCount = 2;
                break;
            }
        }
    }
    GM_T;
    ParentClass.WhisperStopFollowing = true;
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        GM_START_SCRIPTING(Game->GetHero(), seh_hero);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["TEACHER"] = seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_PUNCH", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        Game->RemoveThing(Game->GetThingWithScriptName("PreMeleeWhisper"), false, true);
    }
    if (Game->IsXbox()) {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_LOCKINGON");
        AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_PUNCHING");
    } else {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_LOCKINGON");
        AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_PUNCHING");
    }
    CTimer HelpTimer;
    long lastHits;
    HelpTimer = 10;
    ParentClass.PreMeleeMode = 1;
    ParentClass.DummyHits = 0;
    {
    long counter = Game->AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0f);
    Game->DisplayQuestInfo(true);
    lastHits = 0;
    while (ParentClass.DummyHits < 7) {
        Game->NewScriptFrame();
        GM_T;
        Game->UpdateQuestInfoCounter(counter, ParentClass.DummyHits, -1);
        if (lastHits != ParentClass.DummyHits) {
            GM_T;
            lastHits = ParentClass.DummyHits;
            HelpTimer = 10;
        }
        if (HelpTimer.GetTime() <= 0) {
            GM_T;
            GM_SAY("TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY")
            if (Game->IsXbox()) {
                GM_T;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP");
                GM_WAIT_GAME_INFO;
            } else {
                GM_T;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC");
                GM_WAIT_GAME_INFO;
            }
            HelpTimer = 10;
        }
    }
    GM_T;
    Game->RemoveQuestInfoElement(counter);
    Game->DisplayQuestInfo(false);
    }
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        GM_START_SCRIPTING(Game->GetHero(), seh_hero);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["TEACHER"] = seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_STICK", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    if (Game->IsXbox()) {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_MELEE");
    } else {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_MELEE");
    }
    ParentClass.PreMeleeMode = 2;
    ParentClass.DummyHits = 0;
    HelpTimer = 10;
    {
    long counter = Game->AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0f);
    Game->DisplayQuestInfo(true);
    lastHits = 0;
    while (ParentClass.DummyHits < 7) {
        Game->NewScriptFrame();
        GM_T;
        Game->UpdateQuestInfoCounter(counter, ParentClass.DummyHits, -1);
        if (lastHits != ParentClass.DummyHits) {
            GM_T;
            lastHits = ParentClass.DummyHits;
            HelpTimer = 10;
        }
        if (HelpTimer.GetTime() <= 0) {
            GM_T;
            GM_SAY("TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE")
            if (Game->IsXbox()) {
                GM_T;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP");
                GM_WAIT_GAME_INFO;
            } else {
                GM_T;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC");
                GM_WAIT_GAME_INFO;
            }
            HelpTimer = 10;
        }
    }
    GM_T;
    Game->RemoveQuestInfoElement(counter);
    Game->DisplayQuestInfo(false);
    }
    CScriptThing orb;
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        GM_START_SCRIPTING(Game->GetHero(), seh_hero);
        CScriptThing dummy = Game->GetThingWithScriptName("PreMeleeDummy");
        C3DVector pos = dummy.GetPos();
        CScriptThing smash = Game->CreateEffect("SMASH_DUMMY_01", pos, "", 0.0f, false, false);
        FadeOutAndKillEntity(dummy, 1.0f, true, true);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["TEACHER"] = seh_me;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_PASSED_SETUP", mymap, 0, 0, false, false);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        orb = Game->CreateExperienceOrb(pos, 1);
        Game->EntitySetCutsceneBehaviour(orb, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
        RunCutsceneMacro("CS_GUILD_PREMELEE_PASSED", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    if (Game->IsXbox()) {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_EXPERIENCEORBS");
    } else {
        GM_T;
        Game->DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC");
        GM_WAIT_GAME_INFO;
        AddLogbookTutorialEntryPC("TEXT_QST_LOG_HERO_EXPERIENCEORBS");
    }
    CTimer RepeatTimer;
    RepeatTimer = 10;
    while (orb.IsAlive()) {
        Game->NewScriptFrame();
        GM_T;
        if (RepeatTimer.GetTime() <= 0) {
            GM_T;
            GM_SAY("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10")
            RepeatTimer = 10;
        }
    }
    GM_T;
    Game->Pause(0.5f);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        GM_START_SCRIPTING(Game->GetHero(), seh_hero);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["TEACHER"] = seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_ALARM", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false);
    Game->SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "");
    Game->KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false);
    Game->NewScriptFrame();
    GM_T;
    Game->NewScriptFrame();
    GM_T;
    Game->NewScriptFrame();
    GM_T;
    Game->NewScriptFrame();
    GM_T;
    Game->NewScriptFrame();
    GM_T;
    if (Game->DisplayTutorial((ETutorialCategory)28)) {
        GM_T;
        while (!Game->MsgIsTutorialClickedPast()) {
            Game->NewScriptFrame();
            GM_T;
        }
        GM_T;
    }
    Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("TheRealGuildmaster"));
    Game->MiniMapAddMarker(Game->GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL");
    seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_GUARD").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
    waiting = true;
    long beetleComment = 0;
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "");
    bool facedHero = false;
    while (waiting) {
        Game->NewScriptFrame();
        GM_T;
        if (PMasterData->ScorpionsDestroyedCutscenePlayed && !WoodsEndPlayed) {
            GM_T;
            WoodsEndPlayed = true;
            Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("TheRealGuildmaster"));
            Game->MiniMapAddMarker(Game->GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE");
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["GUARD"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_MELEE_WOODSWON", mymap, 0, 0, false, true);
            GM_END_QUESTION
            Game->FixMovieSequenceCamera(false);
        } else if (PMasterData->ScorpionsDestroyedCutscenePlayed && Game->IsQuestActive("Q_GuildTrainingWoodsMelee")) {
            GM_T;
            if (seh_me.IsValid()) seh_me.Release();
            while (Game->IsQuestActive("Q_GuildTrainingWoodsMelee") != false) {
                Game->NewScriptFrame();
                GM_T;
            }
            GM_T;
            GM_START_SCRIPTING(Me, seh_me);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            GM_SPEAK("TEXT_QST_028_GUILDMASTER_PREMELEE_END")
            GM_END_QUESTION
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            GM_T;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            seh_me.ClearCommands();
            if (!PMasterData->ScorpionsDestroyedCutscenePlayed) {
                GM_T;
                GM_SPEAK("TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD")
            } else {
                GM_T;
                GM_SPEAK("TEXT_QST_028_GUILDMASTER_PREMELEE_END")
                GM_END_QUESTION
            }
        }
        if (waiting && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0 && !seh_me.IsPerformingScriptTask()) {
            GM_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            CommentTimer = 10;
            if (PMasterData->ScorpionsDestroyedCutscenePlayed) {
                GM_T;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST");
            } else {
                GM_T;
                switch (beetleComment) {
                case 1:
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST");
                    break;
                case 2:
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    GM_LINE(conversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND");
                    break;
                }
                beetleComment = 1 - beetleComment;
            }
        }
        if (!facedHero) {
            GM_T;
            if (!seh_me.IsPerformingScriptTask()) {
                GM_T;
                facedHero = true;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            }
        }
    }
    GM_T;
    ParentClass.HeroSleeps = true;
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->SetTimeOfDay(11.0f);
    Game->ChangeHeroHealthBy(1000.0f, true, false);
    Game->ResetPlayerCreatureCombatMultiplier();
}