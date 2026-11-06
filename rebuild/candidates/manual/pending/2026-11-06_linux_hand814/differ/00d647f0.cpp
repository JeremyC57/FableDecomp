// NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main() -- the departure maze's final sparring partner: opening cut-scene, then three rounds (melee, bow, lightning) in which seven hits with the right weapon beat him, wrong-weapon hits are healed with a scolding line, and a win cut-scene ends the mission
// Grade: DIFFER(7405v7389) -- register-masked SDIFF 10: in the phase-2 (bow round) lightning branch retail evicts ebx (the round counter, reloaded from [esp+0x1c]) to cache the Game vtable and keeps the conversation handle in ebp, VC7.1 here spills the conversation to [esp+0x14] (+16 bytes); the rest of the 316 lines are the timer / text-temporary frame-slot order that follows; every call, string and relocation matches
// Next idea: a phase-2 spelling that changes which value VC7.1 evicts (per-phase counters fixed phase 1; const / block-scoped counters, a blocked line and declaration moves were inert)
// Leaf: ?Main@CFinalMaze@CQ_GuildTrainingWoodsDepartureScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D647F0, 7389 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CFinalMaze@CQ_GuildTrainingWoodsDepartureScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012D16C0 that the landed CFinalMaze::Alloc
//   0x00D62270 stores (slot 0 ??_G 0x00D62340, 2 Init 0x00D62320, 3 GetParentScript 0x00D62310, 5 OnPredicateFail 0x00D62330).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EHeroAbility.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingWoodsDepartureScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CTimer.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();      // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();     // 0x00CDBFB0
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#define FM_T if (IsActiveThreadTerminating()) return;
#define FM_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        FM_T \
    } \
    FM_T
#define FM_LINE(TEXT) \
    conversation=Game->AddNewConversation(Me,false,false); \
    Game->AddPersonToConversation(conversation,Game->GetHero()); \
    Game->AddLineToConversation(conversation,TEXT,false,Me,Game->GetHero());
#define FM_GOODHIT(CNT) \
    FM_T \
    if (++BeenHit==7) { \
        FM_T \
        NotBeaten=false; \
        Game->ModifyThingHealth(Me,1000.0f,false); \
    } \
    Game->UpdateQuestInfoCounter(CNT,BeenHit,-1); \
    if (timer.GetTime()<=0 && !Game->IsConversationActive(conversation)) { \
        FM_T \
        if (rand()%2==0) { \
            FM_T \
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT") \
        } \
        else { \
            FM_T \
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING") \
        } \
        timer=5; \
    }
#define FM_MELEE (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_IRON_LONGSWORD") || Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_IRON_KATANA"))
#define FM_BOW (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_YEW_LONGBOW") || Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_YEW_CROSSBOW"))
#define FM_LIGHTNING Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL,"SCRIPT_NAME_HERO")
#define FM_INTRO(TEXT) \
    { \
        CScriptGameResourceObjectMovieBase movie; \
        Game->StartMovieSequence("",movie); \
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
        Game->FixMovieSequenceCamera(true); \
        Game->Pause(0.5f); \
        Game->EntitySetAsDrawable(Game->GetHero(),false); \
        Game->CameraUseCameraPoint(Game->GetThingWithScriptName("CAM_RC_MAZE"),Me,-1.0f,0,-1); \
        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
            seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
            while (seh_me.IsPerformingScriptTask()) { \
                Game->NewScriptFrame(); \
                FM_T \
            } \
            FM_T \
        } \
        Game->EntitySetAsDrawable(Game->GetHero(),true); \
        Game->FixMovieSequenceCamera(false); \
    }
#define FM_CUTSCENE(NAME) \
    { \
        CScriptGameResourceObjectScriptedThingBase seh_hero; \
        if (seh_hero.IsValid()) seh_hero.Release(); \
        while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) { \
            Game->NewScriptFrame(); \
            FM_T \
        } \
        FM_T \
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast; \
        cast["HERO"]=seh_hero; \
        cast["MAZE"]=seh_me; \
        CScriptGameResourceObjectMovieBase movie; \
        Game->StartMovieSequence("",movie); \
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
        Game->FixMovieSequenceCamera(true); \
        RunCutsceneMacro(NAME,cast,0,0,false,true); \
        Game->FixMovieSequenceCamera(false); \
    }
void NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    FM_T
    CScriptGameResourceObjectScriptedThingBase seh_me;
    FM_START
    NotFighting=true;
    NotBeaten=true;
    BeenHit=0;
    Game->EntitySetAsKillable(Me,false,true);
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    Game->EntitySetAllowBossPhaseChanges(Me,false);
    FM_CUTSCENE("CS_GUILD_DEPARTURE_MAZE_START")
    Game->EntitySetInFaction(Me,"FACTION_MONSTERS");
    long counter=Game->AddQuestInfoCounter("HUD_QUEST_ICON_MAZE",7,1.0f);
    Game->DisplayQuestInfo(true);
    if (seh_me.IsValid()) seh_me.Release();
    Game->EntitySetBossPhase(Me,0);
    Game->EntitySetAsDamageable(Game->GetHero(),false);
    Game->UpdateQuestInfoCounter(counter,BeenHit,-1);
    CTimer timer;
    timer=0;
    Game->EntityWillTeleportToArea(Me,Me.GetPos(),5.0f,20.0f);
    Game->CacheMusicSet((EMusicSetType)47);
    long conversation=0;
    while (NotBeaten) {
        Game->NewScriptFrame();
        FM_T
        if (FM_MELEE) {
            FM_GOODHIT(counter)
        }
        else if (FM_BOW && !Game->IsConversationActive(conversation)) {
            FM_T
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW")
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
        else if (FM_LIGHTNING) {
            FM_T
            if (!Game->IsConversationActive(conversation)) {
                FM_T
                FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING")
            }
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
    }
    FM_T
    Game->RemoveQuestInfoElement(counter);
    Game->DisplayQuestInfo(false);
    BeenHit=0;
    NotBeaten=true;
    FM_START
    FM_INTRO("TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST")
    if (seh_me.IsValid()) seh_me.Release();
    Game->EntitySetBossPhase(Me,1);
    long counter2=Game->AddQuestInfoCounter("HUD_QUEST_ICON_MAZE",7,1.0f);
    Game->DisplayQuestInfo(true);
    Game->UpdateQuestInfoCounter(counter2,BeenHit,-1);
    Game->EntityWillTeleportToArea(Me,Me.GetPos(),5.0f,20.0f);
    while (NotBeaten) {
        Game->NewScriptFrame();
        FM_T
        if (FM_MELEE && !Game->IsConversationActive(conversation)) {
            FM_T
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE")
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
        else if (FM_BOW) {
            FM_GOODHIT(counter2)
        }
        else if (FM_LIGHTNING) {
            FM_T
            if (!Game->IsConversationActive(conversation)) {
                FM_T
                FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING")
            }
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
    }
    FM_T
    Game->RemoveQuestInfoElement(counter2);
    Game->DisplayQuestInfo(false);
    BeenHit=0;
    NotBeaten=true;
    FM_START
    FM_INTRO("TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST")
    if (seh_me.IsValid()) seh_me.Release();
    Game->EntitySetBossPhase(Me,2);
    long counter3=Game->AddQuestInfoCounter("HUD_QUEST_ICON_MAZE",7,1.0f);
    Game->DisplayQuestInfo(true);
    Game->UpdateQuestInfoCounter(counter3,BeenHit,-1);
    Game->EntityWillTeleportToArea(Me,Me.GetPos(),5.0f,20.0f);
    while (NotBeaten) {
        Game->NewScriptFrame();
        FM_T
        if (FM_MELEE && !Game->IsConversationActive(conversation)) {
            FM_T
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE")
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
        else if (FM_BOW && !Game->IsConversationActive(conversation)) {
            FM_T
            FM_LINE("TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW")
            Game->ModifyThingHealth(Me,1000.0f,false);
        }
        else if (FM_LIGHTNING) {
            FM_GOODHIT(counter3)
        }
    }
    FM_T
    Game->RemoveQuestInfoElement(counter3);
    Game->DisplayQuestInfo(false);
    FM_START
    seh_me.ClearCommands();
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    FM_CUTSCENE("CS_GUILD_DEPARTURE_MAZE_WIN")
    ParentClass->MissionSucceeded=true;
    Game->ModifyThingHealth(Me,1000.0f,false);
    Game->RemoveThing(Me,false,true);
    Game->EntitySetAsDamageable(Game->GetHero(),true);
}
