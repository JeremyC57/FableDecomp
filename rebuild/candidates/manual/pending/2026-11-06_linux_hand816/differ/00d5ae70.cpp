// NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main() -- the Guildmaster's archery (skill) lesson: comment while the hero walks up, then (repeatable) the start cut scene and the bow instructions, three practice targets with the bow controls ticked off and timed hints, the first-person (snipe) cut scene and instructions until the hero fires, the timed score run (disqualified by a guild warning), the graded result cut scene and the retake / continue question; afterwards walk back and create the two apprentices
// Grade: DIFFER(11636v11636) -- call sequence and size equal (a3 added the thread check retail has after the SkillRepeating loop); left: the stack-slot colouring of the scalars in [esp+0x44/0x48/0x4c] (retail commentCount 0x44, quest-info ticks 0x4c / 0x44 / 0x48, ours 0x48 / 0x48 / 0x4c / 0x44) and two comment-loop temporaries (the movie "" and the comment-line CCharString), the RemoveQuestInfoElement load order that follows, and later scalars at 0x44-0x4c
// Next idea: the spill-slot order: declaration order of the ticks (5 permutations), commentCount placement (3), fresh second tick set (2) were identical or worse; try the 815 twin's exact talk-block shape (else-if hit-whisper) to see whether the comment-loop temps move, then the ticks
// Leaf: ?Main@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D5AE70, 11636 bytes, thiscall, no EH frame, then a 7-entry jump table.
// Identity (proven, two sources): slot 1 of the entity vtable 0x012D09D8 that Alloc 0x00D5ABE0 stores (slot 0 the landed scalar
//   deleting dtor 0x00D5ACA0, slot 2 Init 0x00D5AC90, slot 3 GetParentScript 0x00D5AC80); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@EAEXXZ. Twin of the melee lesson 0x00D58490 (chunk 815).
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
#include "engine/CQ_GuildTrainingSkillScript.h"
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
#define GM_LINE(text) \
    conversation = Game->AddNewConversation(Me, false, false); \
    Game->AddPersonToConversation(conversation, Game->GetHero()); \
    Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero());
#define GM_WAIT_GAME_INFO \
    while (!Game->MsgIsGameInfoClickedPast()) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define GM_HINT(XBOX, PC) \
    if (T) return; \
    GM_LINE_START \
    if (Game->IsXbox()) { \
        if (T) return; \
        Game->AddLineToConversation(conversation, XBOX, false, Me, Game->GetHero()); \
    } \
    else { \
        if (T) return; \
        Game->AddLineToConversation(conversation, PC, false, Me, Game->GetHero()); \
    } \
    HelpTimer = 10;
#define GM_LINE_START \
    conversation = Game->AddNewConversation(Me, false, false); \
    Game->AddPersonToConversation(conversation, Game->GetHero());
#define GM_UNSHEATH_HINT \
    if (T) return; \
    GM_LINE("TEXT_QST_028_MAZE_BOW_UNSHEATH") \
    if (Game->IsXbox()) { \
        if (T) return; \
        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP"); \
        GM_WAIT_GAME_INFO \
    } \
    else { \
        if (T) return; \
        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC"); \
        GM_WAIT_GAME_INFO \
    } \
    HelpTimer = 15;

void NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    GM_START_SCRIPTING(Me, seh_me)
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), true);
    Game->SetIsPushableByHero(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    seh_me.MoveToPosition(Game->GetThingWithScriptName("M_SkillTeacherStand").GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    Game->SetPlayerUsingRangedDummies(true);
    CTimer CommentTimer;
    CommentTimer = 0;
    long commentCount = 0;
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
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                }
            }
            else {
                if (T) return;
                seh_me.ClearCommands();
                ParentClass.TutorialState = 3;
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
                Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", false, Me, Game->GetHero());
                break;
            case 1:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", false, Me, Game->GetHero());
                break;
            }
            commentCount = 1 - commentCount;
        }
    }
    if (T) return;
    PMasterData->SkillTrainingStarted = true;
    PMasterData->SkillRepeating = true;
    PMasterData->HeroTakingGuildTest = true;
    while (PMasterData->SkillRepeating) {
        NSF;
        if (T) return;
        ParentClass.TutorialState = 3;
        PMasterData->SkillRepeatKnown = false;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero)
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            ParentClass.TutorialState = 2;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_SKILL_START", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            PMasterData->SkillTrainingStarted = true;
            if (Game->IsXbox()) {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_RANGED");
            }
            else {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_RANGED");
            }
        }
        long counter = Game->AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0f);
        CTimer HelpTimer;
        HelpTimer = 15;
        PMasterData->SkillScore = 0;
        CScriptThing ring = Game->GetThingWithScriptName("ArcheryRing");
        long lastScore = PMasterData->SkillScore;
        Game->EntitySetTargetable(Me, false);
        long tick1;
        long tick2;
        long tick3;
        if (Game->IsXbox()) {
            if (T) return;
            tick1 = Game->AddQuestInfoTick("HUD_BLACK_BUTTON", false, 1.0f);
            tick2 = Game->AddQuestInfoTick("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0f);
            tick3 = Game->AddQuestInfoTick("HUD_CONTROLLER_X", false, 1.0f);
        }
        else {
            if (T) return;
            tick1 = Game->AddQuestInfoTick("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0f);
            tick2 = Game->AddQuestInfoTick("GAME_ACTION_LOCK_TARGET", false, 1.0f);
            tick3 = Game->AddQuestInfoTick("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0f);
        }
        bool ticksRemoved = false;
        Game->DisplayQuestInfo(true);
        HelpTimer = 5;
        long conversation = 0;
        while (PMasterData->SkillScore < 3) {
            NSF;
            if (T) return;
            if (Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") && Game->IsPlayerHoldingLockTargetButton() && Game->IsPlayerHoldingFireRangedWeaponButton()) {
                if (T) return;
                ticksRemoved = true;
                Game->RemoveQuestInfoElement(tick1);
                Game->RemoveQuestInfoElement(tick2);
                Game->RemoveQuestInfoElement(tick3);
            }
            else if (!ticksRemoved) {
                if (T) return;
                Game->UpdateQuestInfoTick(tick1, Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW"));
                Game->UpdateQuestInfoTick(tick2, Game->IsPlayerHoldingLockTargetButton());
                Game->UpdateQuestInfoTick(tick3, Game->IsPlayerHoldingFireRangedWeaponButton());
                if (HelpTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), ring, 6.0f) && !Game->IsConversationActive(conversation)) {
                    if (T) return;
                    if (!Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")) {
                        GM_HINT("TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10")
                    }
                    else if (!Game->IsPlayerHoldingLockTargetButton()) {
                        GM_HINT("TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20")
                    }
                    else if (!Game->IsPlayerHoldingFireRangedWeaponButton()) {
                        GM_HINT("TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30")
                    }
                }
            }
            if (lastScore < PMasterData->SkillScore) {
                if (T) return;
                lastScore = PMasterData->SkillScore;
            }
            else if (lastScore > PMasterData->SkillScore) {
                if (T) return;
                PMasterData->SkillScore++;
                if (!Game->IsConversationActive(conversation)) {
                    if (T) return;
                    GM_LINE("TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT")
                }
            }
            Game->UpdateQuestInfoCounter(counter, PMasterData->SkillScore, -1);
            if (PlayerNotWarned && IsDistanceBetweenThingsOver(Game->GetHero(), ring, 6.0f) && !Game->IsConversationActive(conversation)) {
                if (T) return;
                PlayerNotWarned = false;
                GM_LINE("TEXT_QST_028_MAZE_RING_OUT")
            }
        }
        if (T) return;
        if (!ticksRemoved) {
            if (T) return;
            Game->RemoveQuestInfoElement(tick1);
            Game->RemoveQuestInfoElement(tick2);
            Game->RemoveQuestInfoElement(tick3);
        }
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero)
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_SKILL_MOVE", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            ParentClass.TutorialState = 4;
            PMasterData->MovingDummiesNeeded = true;
            Game->CameraDefault();
            Game->RemoveQuestInfoElement(counter);
            if (Game->IsXbox()) {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_RANGED_FIRSTPERSON");
            }
            else {
                if (T) return;
                Game->DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC");
                GM_WAIT_GAME_INFO
                AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_RANGED_FIRSTPERSON");
            }
        }
        if (Game->IsXbox()) {
            if (T) return;
            tick1 = Game->AddQuestInfoTick("HUD_BLACK_BUTTON", false, 1.0f);
            tick2 = Game->AddQuestInfoTick("HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK", false, 1.0f);
            tick3 = Game->AddQuestInfoTick("HUD_CONTROLLER_X", false, 1.0f);
        }
        else {
            if (T) return;
            tick1 = Game->AddQuestInfoTick("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0f);
            tick2 = Game->AddQuestInfoTick("GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW", false, 1.0f);
            tick3 = Game->AddQuestInfoTick("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0f);
        }
        ticksRemoved = false;
        Game->DisplayQuestInfo(true);
        bool fired = false;
        while (!fired) {
            NSF;
            if (T) return;
            if (HelpTimer.GetTime() <= 0 && !Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") && IsDistanceBetweenThingsUnder(Game->GetHero(), ring, 6.0f)) {
                GM_UNSHEATH_HINT
            }
            if (Game->MsgOnHeroFiredRangedWeapon()) {
                if (T) return;
                fired = true;
            }
            if (Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") && Game->IsHeroInProjectileWeaponMode() && Game->IsPlayerHoldingFireRangedWeaponButton()) {
                if (T) return;
                ticksRemoved = true;
                Game->RemoveQuestInfoElement(tick1);
                Game->RemoveQuestInfoElement(tick2);
                Game->RemoveQuestInfoElement(tick3);
            }
            else if (!ticksRemoved) {
                if (T) return;
                Game->UpdateQuestInfoTick(tick1, Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW"));
                Game->UpdateQuestInfoTick(tick2, Game->IsHeroInProjectileWeaponMode());
                Game->UpdateQuestInfoTick(tick3, Game->IsPlayerHoldingFireRangedWeaponButton());
            }
        }
        if (T) return;
        if (!ticksRemoved) {
            if (T) return;
            Game->RemoveQuestInfoElement(tick1);
            Game->RemoveQuestInfoElement(tick2);
            Game->RemoveQuestInfoElement(tick3);
        }
        CTimer GameTimer;
        GameTimer = (long)pGlobalScriptDef->GUI_SkillTimer;
        PMasterData->SkillScore = 0;
        bool disqualified = false;
        long multiCounter = Game->AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0f);
        long arrowCounter = Game->AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0f);
        long timerInfo = Game->AddQuestInfoTimer(GameTimer, "HUD_CLOCK_ICON", 1.0f);
        Game->DisplayQuestInfo(true);
        Game->UpdateQuestInfoCounter(multiCounter, PMasterData->HighestSkillScore, -1);
        PMasterData->SkillTestOccuring = true;
        HelpTimer = 15;
        while (GameTimer.GetTime() > 0 && !disqualified) {
            NSF;
            if (T) return;
            if (PMasterData->GuildWarningOccuring) {
                if (T) return;
                disqualified = true;
            }
            Game->UpdateQuestInfoCounter(arrowCounter, PMasterData->SkillScore, -1);
            if (PMasterData->SkillScore > PMasterData->HighestSkillScore) {
                if (T) return;
                PMasterData->HighestSkillScore = PMasterData->SkillScore;
                Game->UpdateQuestInfoCounter(arrowCounter, PMasterData->HighestSkillScore, -1);
            }
            if (HelpTimer.GetTime() <= 0 && !Game->IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")) {
                GM_UNSHEATH_HINT
            }
            if (PlayerNotWarned && IsDistanceBetweenThingsOver(Game->GetHero(), Game->GetThingWithScriptName("ArcheryRing"), 6.0f)) {
                if (T) return;
                PlayerNotWarned = false;
                GM_LINE("TEXT_QST_028_MAZE_RING_OUT")
            }
        }
        if (T) return;
        PMasterData->HeroTakingGuildTest = false;
        PMasterData->SkillTestOccuring = false;
        while (!Game->IsHeroControlledByPlayer()) {
            NSF;
            if (T) return;
        }
        if (T) return;
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(multiCounter);
        Game->RemoveQuestInfoElement(arrowCounter);
        Game->RemoveQuestInfoElement(timerInfo);
        Game->EntitySetTargetable(Me, true);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero)
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            if (disqualified) {
                if (T) return;
                RunCutsceneMacro("CS_GUILD_SKILL_DISQUALIFIED", mymap, 0, 0, false, true);
            }
            else {
                if (T) return;
                float score = (float)PMasterData->SkillScore;
                long grade = 0;
                for (long i = 0; i < 7; ++i) {
                    grade = i;
                    if (pGlobalScriptDef->GUI_SkillGrades[i] <= score) {
                        if (T) return;
                        break;
                    }
                }
                std::map<CCharString,CCharString> textmap;
                switch (grade) {
                case 0: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_APLUS"; break;
                case 1: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_A"; break;
                case 2: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_B"; break;
                case 3: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_C"; break;
                case 4: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_D"; break;
                case 5: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_E"; break;
                case 6: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_F"; break;
                }
                RunCutsceneMacro("CS_GUILD_SKILL_WON_START", mymap, 0, &textmap, false, false);
            }
            ParentClass.TutorialState = 0;
            Game->RemoveQuestInfoElement(counter);
            Game->Pause(2.0f);
            Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true);
            long answer = Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer < 0) {
                NSF;
                if (T) return;
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
            }
            if (T) return;
            if (answer == 1) {
                if (T) return;
                RunCutsceneMacro("CS_GUILD_SKILL_CONTINUE", mymap, 0, 0, false, true);
                PMasterData->SkillRepeating = false;
            }
            else {
                if (T) return;
                RunCutsceneMacro("CS_GUILD_MELEE_REPEAT", mymap, 0, 0, false, false);
                PMasterData->MovingDummiesNeeded = false;
                PMasterData->SkillRepeating = true;
                PMasterData->SkillRepeatKnown = true;
                if (Game->IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", Game->GetHero())) {
                    if (T) return;
                    Game->TakeObjectFromHero("OBJECT_YEW_LONGBOW");
                }
                while (PMasterData->SkillDummyReset != true) {
                    NSF;
                    if (T) return;
                }
                if (T) return;
            }
            Game->FixMovieSequenceCamera(false);
        }
    }
    if (T) return;
    seh_me.MoveToPosition(Game->GetThingWithScriptName("M_GuildmasterMarker").GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    CScriptThing apprentice = Game->CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", Game->GetThingWithScriptName("SkillApprenticeMarker").GetPos(), "SkillApprentice", false);
    apprentice.SetToKillOnLevelUnload(false);
    Game->CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", Game->GetThingWithScriptName("BirdKillerMarker").GetPos(), "BirdKiller", false);
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "");
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->SetPlayerUsingRangedDummies(false);
}
