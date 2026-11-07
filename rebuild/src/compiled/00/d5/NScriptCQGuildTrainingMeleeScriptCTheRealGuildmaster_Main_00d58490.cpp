// NScript::CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main() -- the Guildmaster's melee lesson: comment while the hero walks up and whisper's early hits, then (repeatable) the attack cut scene and seven counted attacks, the block cut scene and five counted blocks with the blocking instructions, the battle with Thunder and Whisper graded A+..F, and the retake / continue question; afterwards remove Whisper, walk back and create the two apprentices
// Leaf: ?Main@CTheRealGuildmaster@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D58490, 9560 bytes, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012D030C that Alloc 0x00D565C0
//   stores (0x00D565F6); slot 0 the landed scalar deleting dtor 0x00D56680, slot 2 Init 0x00D56670, slot 3 GetParentScript 0x00D56660;
//   Ego_r.pdb and FableWin.pdb both hold ?Main@CTheRealGuildmaster@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ.
// Callees: StartScriptingEntity / NewScriptFrame / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490 /
//   MoveToPosition 0x007E72F0 / ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (ctor 0x007E72A0 where VC stops expanding it),
//   movie ctor 0x006E7B60 / dtor 0x006E7B80 (expanded at the first site), CScriptThing ~ 0x004AA840 (expanded at the first temporary),
//   cast map ctor 0x00CDBF70 / operator[] 0x00CD3D2E / ~ 0x00CDBFB0, CScriptThing operator= 0x008ABD10 (STB assign), text map ctor
//   0x009AC2D0 / operator[] 0x009AC700 / ~ 0x009AC310, CCharString operator=(const char*) 0x0099EFE0, RunCutsceneMacro 0x00CBFB7D,
//   AddLogbookTutorialEntry 0x00CBE9EE / AddLogbookTutorialEntryPC 0x00CBEA81, IsDistanceBetweenThingsUnder 0x00CBE2FF, CTimer ctor
//   0x00CD4450 / ~ 0x00CD4470 where VC stops expanding them; interface slots through Game and pDSTGame 0x0143E8F8; pGlobalScriptDef
//   0x0143E90C (GUI_MeleeGrades +0xEB4, GUI_MinHealth +0xED8).
// Data: engine/CQ_GuildTrainingMeleeScript.h (new, chunk 815; CQ_SunnyvaleMasterData::HeroTakingGuildTest +0xC6).
// Shape: generated entity script (T-check at the start of every nested block and after every WAIT loop). The split expansions
//   (first ~CScriptThing temporary, first movie / STB / CTimer special members expanded, later ones called) are VC7.1's own inline
//   budget with plain-inline definitions: a row-local inline ~CScriptThing reproduces it. The two end healths are named locals (the
//   order retail loads GUI_MeleeGrades before the x87 score).
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
#include "engine/CQ_GuildTrainingMeleeScript.h"
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
#include "engine/TRetVoid.h"
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
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define GM_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define GM_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        GM_T; \
    } \
    GM_T
#define GM_SAY(text) { \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero()); \
    }
#define GM_HEAL_HERO \
    if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) { \
        GM_T; \
        GM_SAY("TEXT_QST_028_MAZE_HEAL_HERO") \
        Game->ChangeHeroHealthBy(1000.0f, true, false); \
    }
#define GM_WAIT_GAME_INFO \
    while (!Game->MsgIsGameInfoClickedPast()) { \
        Game->NewScriptFrame(); \
        GM_T; \
    } \
    GM_T

void NScript::CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    GM_START_SCRIPTING(Me, seh_me);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), true);
    Game->SetIsPushableByHero(Me, false);
    CScriptThing MeleeOpponent = Game->GetThingWithScriptName("MeleeOpponent");
    Game->SetThingHasInformation(Me, false, false, false);
    CTimer CommentTimer;
    CommentTimer = 0;
    long commentCount = 0;
    while (ParentClass.TutorialState == 1) {
        Game->NewScriptFrame();
        GM_T;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            GM_T;
            ParentClass.TutorialState = 2;
        } else if ((ParentClass.EarlyHitWhisper && ParentClass.WhisperArrived) || Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            GM_T;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    GM_T;
                }
                GM_T;
            }
            ParentClass.TutorialState = 2;
        }
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0 && !seh_me.IsPerformingScriptTask()) {
            GM_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            CommentTimer = 10;
            switch (commentCount) {
            case 0:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", false, Me, Game->GetHero());
                commentCount = 1;
                break;
            case 1:
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", false, Me, Game->GetHero());
                break;
            }
        }
    }
    GM_T;
    PMasterData->HeroTakingGuildTest = true;
    ParentClass.WhisperStopWalking = true;
    Game->SetPlayerCreatureOnlyTarget(MeleeOpponent);
    while (ParentClass.MeleeRepeating == true) {
        Game->NewScriptFrame();
        GM_T;
        ParentClass.TutorialState = 2;
        ParentClass.MeleeRepeatKnown = false;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero);
            CScriptGameResourceObjectScriptedThingBase seh_whisper;
            GM_START_SCRIPTING(MeleeOpponent, seh_whisper);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            mymap["WHISPER"] = seh_whisper;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_MELEE_ATTACK", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
        }
        ParentClass.GenericTutorialCounter = 0;
        Game->DisplayQuestInfo(true);
        long counter = Game->AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0f);
        ParentClass.TutorialState = 3;
        CTimer HelpTimer;
        HelpTimer = 15;
        while (ParentClass.GenericTutorialCounter < 7) {
            Game->NewScriptFrame();
            GM_T;
            if (HelpTimer.GetTime() <= 0) {
                GM_T;
                GM_SAY("TEXT_QST_028_MAZE_HELP_ATTACK")
                HelpTimer = 15;
            }
            Game->UpdateQuestInfoCounter(counter, ParentClass.GenericTutorialCounter, -1);
            GM_HEAL_HERO
        }
        GM_T;
        Game->ClearAllRumbles();
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(counter);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero);
            CScriptGameResourceObjectScriptedThingBase seh_whisper;
            GM_START_SCRIPTING(MeleeOpponent, seh_whisper);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            mymap["WHISPER"] = seh_whisper;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_MELEE_BLOCK", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
        }
        ParentClass.GenericTutorialCounter = 0;
        if (Game->IsXbox()) {
            GM_T;
            Game->DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK");
            GM_WAIT_GAME_INFO;
            AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_BLOCKING");
        } else {
            GM_T;
            Game->DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC");
            GM_WAIT_GAME_INFO;
            AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_BLOCKING");
        }
        ParentClass.TutorialState = 4;
        counter = Game->AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0f);
        Game->DisplayQuestInfo(true);
        while (ParentClass.GenericTutorialCounter < 5) {
            Game->NewScriptFrame();
            GM_T;
            Game->UpdateQuestInfoCounter(counter, ParentClass.GenericTutorialCounter, -1);
            GM_HEAL_HERO
        }
        GM_T;
        Game->ClearAllRumbles();
        ParentClass.TutorialState = 5;
        CScriptThing Thunder = Game->CreateCreature("CREATURE_RIVAL_HERO_THUNDER", Game->GetThingWithScriptName("SkillApprenticeMarker").GetPos(), "MeleeThunder", false);
        Game->EntitySetAppearanceMorphSeed(Thunder, 1);
        CScriptGameResourceObjectScriptedThingBase seh_thunder;
        GM_START_SCRIPTING(Thunder, seh_thunder);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero);
            CScriptGameResourceObjectScriptedThingBase seh_whisper;
            GM_START_SCRIPTING(MeleeOpponent, seh_whisper);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            mymap["THUNDER"] = seh_thunder;
            mymap["WHISPER"] = seh_whisper;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GUILD_MELEE_BATTLE", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            Game->ChangeHeroHealthBy(1000.0f, true, false);
            Game->ModifyThingHealth(MeleeOpponent, 1000.0f, false);
            ParentClass.TutorialState = 6;
        }
        HeroStanding = true;
        WhisperStanding = true;
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(counter);
        Game->DisplayQuestInfo(true);
        Game->AddQuestInfoBarHealth(Game->GetThingWithScriptName("MeleeOpponent"), CRGBColour(255, 255, 255, 255), "HUD_WHISPER_ICON", 1.0f);
        float heroStartHealth = Game->GetHealth(Game->GetHero());
        float whisperStartHealth = Game->GetHealth(MeleeOpponent);
        while (HeroStanding && WhisperStanding) {
            Game->NewScriptFrame();
            GM_T;
            if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
                GM_T;
                HeroStanding = false;
                GM_SAY("TEXT_QST_028_MAZE_FIGHT_OVER")
            } else if (Game->GetHealth(Game->GetThingWithScriptName("MeleeOpponent")) < pGlobalScriptDef->GUI_MinHealth) {
                GM_T;
                WhisperStanding = false;
                GM_SAY("TEXT_QST_028_MAZE_FIGHT_OVER")
            }
        }
        GM_T;
        Game->ClearAllRumbles();
        float heroEndHealth = Game->GetHealth(Game->GetHero());
        float whisperEndHealth = Game->GetHealth(MeleeOpponent);
        float score = (whisperStartHealth - whisperEndHealth) - (heroStartHealth - heroEndHealth);
        long grade = 0;
        for (long i = 0; i < 7; ++i) {
            grade = i;
            if (pGlobalScriptDef->GUI_MeleeGrades[i] <= score) {
                GM_T;
                break;
            }
        }
        Game->ResetPlayerCreatureOnlyTarget();
        ParentClass.TutorialState = 7;
        Game->DisplayQuestInfo(false);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            GM_START_SCRIPTING(Game->GetHero(), seh_hero);
            CScriptGameResourceObjectScriptedThingBase seh_whisper;
            GM_START_SCRIPTING(MeleeOpponent, seh_whisper);
            Game->RemoveQuestInfoElement(counter);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TEACHER"] = seh_me;
            mymap["THUNDER"] = seh_thunder;
            mymap["WHISPER"] = seh_whisper;
            std::map<CCharString,CCharString> textmap;
            switch (grade) {
            case 0: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS"; break;
            case 1: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A"; break;
            case 2: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B"; break;
            case 3: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C"; break;
            case 4: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D"; break;
            case 5: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E"; break;
            case 6: textmap["$GRADE"] = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F"; break;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            if (WhisperStanding) {
                GM_T;
                RunCutsceneMacro("CS_GUILD_MELEE_BATTLE_LOST", mymap, 0, &textmap, false, true);
            } else {
                GM_T;
                RunCutsceneMacro("CS_GUILD_MELEE_BATTLE_WON", mymap, 0, &textmap, false, true);
            }
            Game->ChangeHeroHealthBy(1000.0f, true, false);
            Game->EntitySetInFaction(MeleeOpponent, "FACTION_HERO");
            Me.SetFriendsWithEverythingFlag(true);
            Game->RemoveThing(Game->GetThingWithScriptName("MeleeThunder"), false, true);
            Game->Pause(2.0f);
            Game->GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true);
            long answer = Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer < 0) {
                Game->NewScriptFrame();
                GM_T;
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
            }
            GM_T;
            if (answer == 1) {
                GM_T;
                RunCutsceneMacro("CS_GUILD_MELEE_CONTINUE", mymap, 0, 0, false, true);
                ParentClass.MeleeRepeating = false;
                ParentClass.MeleeRepeatKnown = true;
            } else {
                GM_T;
                RunCutsceneMacro("CS_GUILD_MELEE_REPEAT", mymap, 0, 0, false, false);
                ParentClass.MeleeRepeating = true;
                ParentClass.MeleeRepeatKnown = true;
                if (Game->IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", Game->GetHero())) {
                    GM_T;
                    Game->TakeObjectFromHero("OBJECT_IRON_LONGSWORD");
                }
            }
            Game->FixMovieSequenceCamera(false);
        }
        while (ParentClass.MeleeOpponentReset != true) {
            Game->NewScriptFrame();
            GM_T;
        }
        GM_T;
    }
    GM_T;
    PMasterData->HeroTakingGuildTest = false;
    Game->RemoveThing(Game->GetThingWithScriptName("MeleeOpponent"), false, true);
    seh_me.MoveToPosition(Game->GetThingWithScriptName("M_GuildmasterMarker").GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", Game->GetThingWithScriptName("MeleeApprenticeMarker").GetPos(), "MeleeApprentice", false);
    Game->CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", Game->GetThingWithScriptName("CombatApprenticeMarker").GetPos(), "CombatApprentice", false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "");
}