// NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main() -- the Guildmaster's will (spell) lesson: comment while the hero walks up, then (repeatable) the lightning cut scene and the zap instructions, the test cut scene once a dummy is hit, the timed will test (disqualified by a guild warning), the graded or disqualified result with the retake / continue question and the play-Whisper question (declining ends the guild training with the split video); afterwards walk back, create the will apprentice, hand out the woods-bandits quest and answer the end-of-training question
// Grade: DIFFER(15372v15084) -- 133 lines: VC7.1's inline budget at the exits (GOTCHAS chunk 809 class): the final loop's thread-exit pad expands ~seh_me where retail calls 0x007E74D0, so the movie-scope pad at 0x00D5E970 gets its own exit copy instead of jumping to the shared exit 0x00D61B0A, and the last movie ctor (final-loop talk branch) expands where retail calls 0x006E7B60 (ebx zero register in that block). /d2inlT70-72 (diagnostic only) leave 70 lines
// Next idea: a spelling that makes the final-loop talk block cost what retail's did (its movie ctor called, its pad sharing the exit): the GM_END_TRAINING macros' statement order, or the loop-top exit as retail's natural exit with the call
// Leaf: ?Main@CTheRealGuildmaster@CQ_GuildTrainingWillScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D5E0C0, 15084 bytes, thiscall, no EH frame, then the 7-entry grade jump table 0x00D61B90.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012D1070 (0x012D1074) that
//   Alloc 0x00D5DDE0 stores (slot 0 the landed scalar deleting dtor 0x00D5DEA0, slot 2 Init 0x00D5DE90, slot 3 GetParentScript
//   0x00D5DE80); Ego_r.pdb and FableWin.pdb both hold ?Main@CTheRealGuildmaster@CQ_GuildTrainingWillScript@NScript@@EAEXXZ.
//   Twin of the melee lesson 0x00D58490 (chunk 815) and the skill lesson 0x00D5AE70 (chunk 816).
// Data: engine/CQ_GuildTrainingWillScript.h (new, chunk 819); CQ_SunnyvaleMasterData WillTrainingStarted +0x9f, WillScore +0xac,
//   MeleeApprenticeNeededForCutscene +0xb0, GuildWarningOccuring +0xc0, WillTestOccuring +0xc2, HeroTakingGuildTest +0xc6;
//   pGlobalScriptDef GUI_WillGrades +0xECC, GUI_WillTimer +0xF08.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingWillScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
void FABLE_FASTCALL AddLogbookTutorialEntry(const CCharString&);     // 0x00CBE9EE
void FABLE_FASTCALL AddLogbookTutorialEntryPC(const CCharString&);   // 0x00CBEA81
inline CScriptThing::~CScriptThing() { }
inline void CScriptThing::SetToKillOnLevelUnload(bool on) { if (PImp.Data) PImp.Data->SetToKillOnLevelUnload(on); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define GM_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define GM_WAIT_GAME_INFO \
    while (!Game->MsgIsGameInfoClickedPast()) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define GM_SPEAK(target, text) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(target, text, GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define GM_ZAP_HELP \
    if (Game->IsXbox()) { \
        if (T) return; \
        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP"); \
        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST"); \
        GM_WAIT_GAME_INFO \
    } \
    else { \
        if (T) return; \
        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"); \
        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC"); \
        GM_WAIT_GAME_INFO \
    }
#define GM_CREATE_WILL_APPRENTICE \
    if (!Game->GetThingWithScriptName("WillApprentice").IsAlive()) { \
        if (T) return; \
        CScriptThing apprentice = Game->CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", Game->GetThingWithScriptName("WillApprenticeMarker").GetPos(), "WillApprentice", false); \
        apprentice.SetToKillOnLevelUnload(false); \
    }
#define GM_END_TRAINING_START \
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255)); \
    Game->Pause(1.0f); \
    Game->PlayAVIMovie("Data\\Video\\2_guild_split_2_comp.xmv"); \
    Game->ConfiscateAllHeroWeapons(); \
    if (Game->IsQuestActive("Q_GuildTrainingWoodsWill")) { \
        if (T) return; \
        Game->DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0); \
    }
#define GM_END_TRAINING_WORLD \
    Game->SetTimeOfDay(11.0f); \
    Game->ResetPlayerCreatureCombatMultiplier(); \
    Game->SetHeroAsTeenager(false); \
    Game->ChangeHeroHealthBy(1000.0f, true, false); \
    Game->RemoveThing(Game->GetThingWithScriptName("MeleeApprentice"), false, true); \
    Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", Game->GetThingWithScriptName("MeleeApprenticeMarker").GetPos(), "MeleeApprentice", false);
#define GM_PLAY_WHISPER_QUESTION \
    PMasterData->MeleeApprenticeNeededForCutscene = true; \
    CScriptThing apprentice = Game->GetThingWithScriptName("MeleeApprentice"); \
    CScriptGameResourceObjectScriptedThingBase seh_apprentice; \
    GM_START_SCRIPTING(apprentice, seh_apprentice) \
    mymap["HERO"] = seh_hero; \
    mymap["TEACHER"] = seh_me; \
    mymap["WHISPER"] = seh_apprentice; \
    RunCutsceneMacro("CS_GUILD_WILL_CONTINUE", mymap, 0, 0, false, true); \
    repeat = false; \
    Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true); \
    long play = Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (play < 0) { \
        NSF; \
        if (T) return; \
        play = Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return; \
    if (play != 1) { \
        if (T) return; \
        GM_SPEAK(Me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES") \
        if (seh_apprentice.IsValid()) seh_apprentice.Release(); \
    } \
    else { \
        if (T) return; \
        GM_SPEAK(Me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO") \
        GM_END_TRAINING_START \
        GM_END_TRAINING_WORLD \
        GM_CREATE_WILL_APPRENTICE \
        Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", ""); \
        AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES"); \
        Game->SetPlayerUsingWillDummies(false); \
        PMasterData->HeroTakingGuildTest = false; \
        ParentClass.TutorialState = 4; \
        ParentClass.TestFinished = true; \
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0); \
        for (;;) { \
            NSF; \
            if (T) return; \
        } \
    }
#define GM_REPEAT_ANSWER \
    long answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer < 0) { \
        NSF; \
        if (T) return; \
        answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return; \
    if (answer == 1) { \
        if (T) return; \
        GM_PLAY_WHISPER_QUESTION \
    } \
    else { \
        if (T) return; \
        Game->SetHeroWillEnergyLevel(1.0f); \
        mymap["TEACHER"] = seh_me; \
        RunCutsceneMacro("CS_GUILD_MELEE_REPEAT", mymap, 0, 0, false, false); \
        repeat = true; \
    }

void NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    GM_START_SCRIPTING(Me, seh_me)
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), true);
    Game->SetThingHasInformation(Me, false, false, false);
    Game->SetPlayerUsingWillDummies(true);
    Game->SetIsPushableByHero(Me, false);
    CTimer HelpTimer;
    CTimer CommentTimer;
    CommentTimer = 0;
    long commentCount = 0;
    Game->EntitySetTargetingType(Me, (ETargetingType)0x1a);
    if (!ParentClass.TestFinished) {
        if (T) return;
        seh_me.MoveToPosition(Game->GetThingWithScriptName("M_WillTeacherStand").GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
        while (ParentClass.TutorialState == 1) {
            NSF;
            if (T) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                if (PMasterData->HeroTakingGuildTest) {
                    if (T) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    seh_me.ClearCommands();
                    GM_SPEAK(Game->GetHero(), "TEXT_QST_028_GUILDMASTER_WILL_NOT_START")
                }
                else {
                    if (T) return;
                    ParentClass.TutorialState = 2;
                }
            }
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0 && !seh_me.IsPerformingScriptTask()) {
                if (T) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                CommentTimer = 10;
                switch (commentCount) {
                case 0:
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", false, Me, Game->GetHero());
                    break;
                case 1:
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", false, Me, Game->GetHero());
                    break;
                }
                commentCount = 1 - commentCount;
            }
        }
        if (T) return;
        PMasterData->HeroTakingGuildTest = true;
        bool repeat = true;
        while (repeat) {
            NSF;
            if (T) return;
            ParentClass.TutorialState = 2;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                GM_START_SCRIPTING(Game->GetHero(), seh_hero)
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"] = seh_hero;
                mymap["TEACHER"] = seh_me;
                Game->GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, true);
                PMasterData->WillTrainingStarted = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GUILD_WILL_LIGHTNING", mymap, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
            }
            if (Game->IsXbox()) {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_USINGSPELLS");
            }
            else {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_USINGSPELLS");
            }
            PMasterData->WillScore = 0;
            CTimer FaceTimer;
            FaceTimer = 0;
            bool talked = false;
            while (ParentClass.TutorialState == 2) {
                NSF;
                if (T) return;
                while (!talked && PMasterData->WillScore == 0) {
                    NSF;
                    if (T) return;
                    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                        if (T) return;
                        talked = true;
                    }
                    if (FaceTimer.GetTime() <= 0) {
                        if (T) return;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                        FaceTimer = 2;
                    }
                }
                if (T) return;
                if (PMasterData->WillScore != 0) {
                    if (T) return;
                    ParentClass.TutorialState = 3;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    GM_START_SCRIPTING(Game->GetHero(), seh_hero)
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"] = seh_hero;
                    mymap["TEACHER"] = seh_me;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_GUILD_WILL_TEST", mymap, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                }
                if (talked) {
                    if (T) return;
                    talked = false;
                    if (Game->IsXbox()) {
                        if (T) return;
                        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP");
                        GM_WAIT_GAME_INFO
                    }
                    else {
                        if (T) return;
                        Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC");
                        GM_WAIT_GAME_INFO
                    }
                }
            }
            if (T) return;
            if (Game->IsXbox()) {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST");
                GM_WAIT_GAME_INFO
            }
            else {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC");
                GM_WAIT_GAME_INFO
            }
            EHeroAbility spell;
            while (!Game->MsgOnHeroCastSpell(spell)) {
                NSF;
                if (T) return;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (T) return;
                    GM_ZAP_HELP
                }
            }
            if (T) return;
            CTimer TestTimer;
            TestTimer = (long)pGlobalScriptDef->GUI_WillTimer;
            ParentClass.TutorialState = 3;
            PMasterData->WillScore = 0;
            CTimer NoWillTimer;
            NoWillTimer = 0;
            long counter = Game->AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0f);
            long timerInfo = Game->AddQuestInfoTimer(TestTimer, "HUD_CLOCK_ICON", 1.0f);
            Game->DisplayQuestInfo(true);
            talked = false;
            FaceTimer = 0;
            while (TestTimer.GetTime() > 0 && !talked) {
                NSF;
                if (T) return;
                if (FaceTimer.GetTime() <= 0) {
                    if (T) return;
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    FaceTimer = 2;
                }
                PMasterData->WillTestOccuring = true;
                if (PMasterData->GuildWarningOccuring) {
                    if (T) return;
                    talked = true;
                }
                if (Game->GetHeroWillEnergy() == 0 && NoWillTimer.GetTime() <= 0) {
                    if (T) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", false, Me, Game->GetHero());
                    NoWillTimer = 8;
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (T) return;
                    GM_ZAP_HELP
                }
                Game->UpdateQuestInfoCounter(counter, PMasterData->WillScore, -1);
            }
            if (T) return;
            PMasterData->WillTestOccuring = false;
            while (!Game->IsHeroControlledByPlayer()) {
                NSF;
                if (T) return;
            }
            if (T) return;
            Game->DisplayQuestInfo(false);
            Game->RemoveQuestInfoElement(counter);
            Game->RemoveQuestInfoElement(timerInfo);
            ParentClass.TutorialState = 0;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                GM_START_SCRIPTING(Game->GetHero(), seh_hero)
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                Game->FixMovieSequenceCamera(true);
                if (talked) {
                    if (T) return;
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    mymap["HERO"] = seh_hero;
                    mymap["TEACHER"] = seh_me;
                    RunCutsceneMacro("CS_GUILD_WILL_DISQUALIFIED", mymap, 0, 0, false, true);
                    Game->Pause(2.0f);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true);
                    GM_REPEAT_ANSWER
                }
                else {
                    if (T) return;
                    float score = (float)PMasterData->WillScore;
                    long grade = 0;
                    for (long i = 0; i < 7; ++i) {
                        grade = i;
                        if (pGlobalScriptDef->GUI_WillGrades[i] <= score) {
                            if (T) return;
                            break;
                        }
                    }
                    std::map<CCharString,CCharString> textmap;
                    switch (grade) {
                    case 0: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"; break;
                    case 1: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"; break;
                    case 2: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"; break;
                    case 3: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"; break;
                    case 4: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"; break;
                    case 5: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"; break;
                    case 6: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"; break;
                    }
                    mymap["HERO"] = seh_hero;
                    mymap["TEACHER"] = seh_me;
                    RunCutsceneMacro("CS_GUILD_WILL_WON_START", mymap, 0, 0, false, false);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    RunCutsceneMacro("CS_GUILD_WILL_WON", mymap, 0, &textmap, false, false);
                    Game->Pause(2.0f);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true);
                    GM_REPEAT_ANSWER
                }
                Game->FixMovieSequenceCamera(false);
                PMasterData->MeleeApprenticeNeededForCutscene = false;
            }
        }
        if (T) return;
    }
    Game->EntitySetTargetingType(Me, (ETargetingType)0x3a);
    PMasterData->HeroTakingGuildTest = false;
    ParentClass.TutorialState = 4;
    ParentClass.TestFinished = true;
    seh_me.MoveToPosition(Game->GetThingWithScriptName("M_GuildmasterMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
    GM_CREATE_WILL_APPRENTICE
    if (!ParentClass.BanditsDefeated) {
        if (T) return;
        Game->ActivateQuest("Q_GuildTrainingWoodsWill");
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false);
        Game->SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false);
        Game->SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "GuildWoods", "");
    }
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "");
    AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES");
    Game->SetPlayerUsingWillDummies(false);
    for (;; NSF) {
        if (T) return;
        if (!ParentClass.BanditsDefeated && !Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
            if (T) return;
            ParentClass.BanditsDefeated = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer = Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer < 0) {
                NSF;
                if (T) return;
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
            }
            if (T) return;
            if (answer == 1) {
                if (T) return;
                GM_END_TRAINING_START
                Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
                GM_END_TRAINING_WORLD
            }
            else {
                if (T) return;
                GM_SPEAK(Game->GetHero(), "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO")
                seh_me.MoveToPosition(Game->GetThingWithScriptName("M_GuildmasterMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
            }
        }
    }
}
