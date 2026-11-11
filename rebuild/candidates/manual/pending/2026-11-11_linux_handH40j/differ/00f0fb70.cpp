// NScript::CQ_ArenaScript::Main() -- the arena quest main: objective, the fourteen entity bindings, the holding quest, the termination and crowd watchers, then the arena state machine (hall-of-heroes intro, cells intro with or without Cham, the cell pause, the walk out), the round loop (per-round objective, keeper lines, theme and cut-scene, then waves until the round is won or the hero leaves for the cells) and the Lady Grey / hall-of-heroes outro.
// Grade: DIFFER(18320v18144) -- 1458 lines: the special-member split now matches retail's counts (first 2 movie ctors / dtors and first 3 STB ctors expanded, the rest called) but the frame is 0x2D8 against retail's 0x2BC (28 bytes: the temporaries' slots differ from the first objective call on) and retail keeps 0 in edi through the binding block (cmp esi,edi / mov [esi+0x18],edi) where VC7.1 here keeps 2 (the temporary-flag bit) in edi; ~STB / ~Movie called at one exit copy more than retail
// Next idea: the 28-byte frame excess first (which block-scoped locals retail lets share a slot: the per-state cast maps, seh handles and the round block's strings), then the zero register follows from the constant-use counts H40j (1 grade): dropping the unused function-scope cast map is worse (1647 v 895; frame 0x2D8 v 0x2BC).
// Leaf: ?Main@CQ_ArenaScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0FB70, 18144 bytes incl. the 9-entry ArenaRound table 0x00F141DC and the 20-entry hero-title
//   table 0x00F14200, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): slot 2 (Main, at .rdata 0x012C77A0) of the CQ_ArenaScript level vtable 0x012C7798 (slot 0 scalar deleting dtor
//   0x00CFAC20, 1 RegisterMain 0x00CFAA50, 3 Init 0x00CFA700, 4 OnPersist 0x00CFAB10); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CQ_ArenaScript@NScript@@UAEXXZ.
// Data: engine/CQ_ArenaScript.h (ArenaState +0xa4, ArenaRound +0xa8, ArenaRoundWave +0xac, GoldMultiplier +0xb0, ChamLeaving +0xb4,
//   PlayerLeaving +0xcc, PlayerWon +0xcd, MissionSucceeded +0xe0, NewCrowdBaseLevel +0xfc, FinalBattleCS +0x104, Whisper +0x108,
//   WhisperNeededForCutscene +0x115, StopTimeIndex +0x118); CQ_SunnyvaleMasterData WhisperKilledByHero +0x74.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
extern "C" int __cdecl rand();                                          // 0x00BFEB16 (MSVCR71)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/EMusicSetType.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_ArenaScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> std::map<CCharString,CCharString>::map();
template<> std::map<CCharString,CCharString>::~map();
inline CScriptThing::~CScriptThing() { }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define AR_WAIT_LEVEL(NAME) \
    while (!Game->IsLevelLoaded(NAME)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AR_START(THING,HANDLE) \
    if (HANDLE.IsValid()) HANDLE.Release(); \
    while (!Game->StartScriptingEntity(THING,HANDLE,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AR_CELL_WHISPER \
    if (ArenaRound>2) { \
        if (T) return; \
        if (!Game->GetThingWithScriptName("CellWhisper").IsAlive()) { \
            if (T) return; \
            Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA",Game->GetThingWithScriptName("FlickPoint").GetPos(),"CellWhisper",false); \
        } \
    } \
    else if (Game->GetThingWithScriptName("CellWhisper").IsAlive()) { \
        if (T) return; \
        Game->RemoveThing(Game->GetThingWithScriptName("CellWhisper"),false,true); \
    }
#define AR_OBJECTIVE(TEXT) \
    Game->SetQuestCardObjective(Game->GetActiveQuestName(),TEXT,"Arena","KnotholeGlade");
#define AR_TITLE(N,TEXT) \
    case N: \
        args["$HEROTITLE"]=TEXT; \
        break;
void NScript::CQ_ArenaScript::Main()
{
    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_01")
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("Cham", this, &CCham::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("Flick", this, &CFlick::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("Shadow", this, &CShadow::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("Needle", this, &CNeedle::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("Roth", this, &CRoth::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("CellWhisper", this, &CCellWhisper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("ArenaCellDoorGuard", this, &CArenaCellDoorGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("ArenaCellDoorGuard2", this, &CArenaCellDoorGuard2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("SUMMONED_CREATURE", this, &CSUMMONED_CREATURE::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("ArenaEnemy", this, &CArenaEnemy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("ArenaSpawn", this, &CArenaSpawn::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("CagedBalverine", this, &CCagedBalverine::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("ArenaCellExitGuard", this, &CArenaCellExitGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaScript>("WhisperAlly", this, &CWhisperAlly::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!Game->IsQuestActive("Q_ArenaHoldingScript")) {
        if (T) return;
        Game->ActivateQuest("Q_ArenaHoldingScript");
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_ArenaScript>("WatchForTermination", 0, this, &CQ_ArenaScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_ArenaScript>("CrowdChecker", 0, this, &CQ_ArenaScript::CrowdChecker), CCharString(""));
    if (Game->GetHeroTitle()==0) {
        if (T) return;
        Game->GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER");
    }
    if (ArenaState==MAS_ON_THE_WAY) {
        if (T) return;
        AR_WAIT_LEVEL("ArenaHallOfHeroes")
        ArenaState=MAS_INTRO_HALL;
    }
    Game->SetTimeOfDay(12.0f);
    Game->SetTimeAsStopped(true,StopTimeIndex);
    Game->SetTeleportingAsActive(false);
    if (ArenaState==MAS_INTRO_HALL) {
        if (T) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("Q_Nav1"),false);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"]=seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            RunCutsceneMacro("CS_ARENA_HOH_INTRO",cast,0,0,false,true);
        }
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ArenaCellEntrance"),false);
        AR_WAIT_LEVEL("ArenaCells")
        Game->SetThingAsUsable(Game->GetThingWithScriptName("CellsExitToArena"),false);
        Game->SetThingAsUsable(Game->GetThingWithScriptName("CellsExitToHOH"),false);
        Game->SetThingPersistent(Game->GetThingWithScriptName("CellsExitToArena"),true);
        Game->SetThingPersistent(Game->GetThingWithScriptName("CellsExitToHOH"),true);
        Game->FadeScreenIn();
        ArenaState=MAS_CELLS_PRE_ARENA_INTRO;
    }
    if (ArenaState==MAS_CELLS_PRE_ARENA_INTRO) {
        if (T) return;
        {
            CScriptThing roth=Game->GetThingWithScriptName("Roth");
            CScriptGameResourceObjectScriptedThingBase seh_roth;
            AR_START(roth,seh_roth)
            CScriptThing flick=Game->GetThingWithScriptName("Flick");
            CScriptGameResourceObjectScriptedThingBase seh_flick;
            AR_START(flick,seh_flick)
            CScriptThing needle=Game->GetThingWithScriptName("Needle");
            CScriptGameResourceObjectScriptedThingBase seh_needle;
            AR_START(needle,seh_needle)
            CScriptThing cham=Game->GetThingWithScriptName("Cham");
            CScriptGameResourceObjectScriptedThingBase seh_cham;
            AR_START(cham,seh_cham)
            CScriptThing shadow=Game->GetThingWithScriptName("Shadow");
            CScriptGameResourceObjectScriptedThingBase seh_shadow;
            AR_START(shadow,seh_shadow)
            CScriptThing guard=Game->GetThingWithScriptName("ArenaCellDoorGuard2");
            CScriptGameResourceObjectScriptedThingBase seh_guard;
            AR_START(guard,seh_guard)
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            AR_START(Game->GetHero(),seh_hero)
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"]=seh_hero;
            cast["Roth"]=seh_roth;
            cast["Flick"]=seh_flick;
            cast["Cham"]=seh_cham;
            cast["Guard"]=seh_guard;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_ARENA_CELLS_INTRO",cast,0,0,false,true);
        }
        ChamLeaving=true;
        ArenaState=MAS_CELLS_PRE_ARENA_NORMAL;
    }
    CTimer timer;
    if (ArenaState==MAS_CELLS_PRE_ARENA_NORMAL) {
        if (T) return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_ARENA_OBJECTIVE_02","ArenaCells","KnotholeGlade");
        timer=60;
        while (timer.GetTime()>0) {
            NSF;
            if (T) return;
        }
        if (T) return;
        while (!Game->IsHeroControlledByPlayer()) {
            NSF;
            if (T) return;
        }
        if (T) return;
        ArenaState=MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE;
    }
    if (ArenaState==MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
        if (T) return;
        AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_03")
        {
            CScriptThing roth=Game->GetThingWithScriptName("Roth");
            CScriptGameResourceObjectScriptedThingBase seh_roth;
            AR_START(roth,seh_roth)
            CScriptThing flick=Game->GetThingWithScriptName("Flick");
            CScriptGameResourceObjectScriptedThingBase seh_flick;
            AR_START(flick,seh_flick)
            CScriptThing needle=Game->GetThingWithScriptName("Needle");
            CScriptGameResourceObjectScriptedThingBase seh_needle;
            AR_START(needle,seh_needle)
            CScriptThing shadow=Game->GetThingWithScriptName("Shadow");
            CScriptGameResourceObjectScriptedThingBase seh_shadow;
            AR_START(shadow,seh_shadow)
            CScriptThing guard=Game->GetThingWithScriptName("ArenaCellDoorGuard2");
            CScriptGameResourceObjectScriptedThingBase seh_guard;
            AR_START(guard,seh_guard)
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            AR_START(Game->GetHero(),seh_hero)
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"]=seh_hero;
            cast["Roth"]=seh_roth;
            cast["Flick"]=seh_flick;
            cast["Guard"]=seh_guard;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_ARENA_CELLS_INTRO_CHAMDEAD",cast,0,0,false,true);
        }
        ArenaState=MAS_CELLS_PRE_ARENA_NORMAL_GO;
    }
    if (ArenaState==MAS_CELLS_PRE_ARENA_NORMAL_GO) {
        if (T) return;
        AR_CELL_WHISPER
        while (ArenaState!=MAS_IN_ARENA) {
            NSF;
            if (T) return;
        }
        if (T) return;
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ArenaInsideHSP"),false);
        AR_WAIT_LEVEL("Arena")
        Game->MiniMapSetAsEnabled(false);
        PlayerLeaving=false;
        CScriptThing gate=Game->GetThingWithScriptName("ArenaHeroGate");
        Game->OpenDoor(gate);
        Game->SetThingPersistent(gate,true);
    }
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> unusedCast;
    if (ArenaState==MAS_CELLS_MID_ARENA) {
        if (T) return;
        AR_CELL_WHISPER
        while (ArenaState!=MAS_IN_ARENA) {
            NSF;
            if (T) return;
        }
        if (T) return;
        Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ArenaInsideHSP"),false);
        AR_WAIT_LEVEL("Arena")
        Game->MiniMapSetAsEnabled(false);
        GoldMultiplier=0;
        PlayerLeaving=false;
    }
    else {
        if (T) return;
        Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
    }
    AnimateCrowd();
    while (!PlayerWon) {
        NSF;
        if (T) return;
        while (!PlayerLeaving && !PlayerWon) {
            NSF;
            if (T) return;
            if (ArenaRoundWave==0) {
                if (T) return;
                Game->AutoSaveCheckPoint();
                WhisperNeededForCutscene=true;
                Game->SetThingAsUsable(Game->GetThingWithScriptName("ArenaMainExit"),false);
                Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ARENA_CentrePoint"),false);
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                CScriptGameResourceObjectScriptedThingBase seh_whisper;
                AR_START(Game->GetHero(),seh_hero)
                CCharString unused;
                CCharString cutscene("CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC");
                long choice=rand()%4;
                if (choice==1) {
                    if (T) return;
                    cutscene="CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2";
                }
                else if (choice==2) {
                    if (T) return;
                    cutscene="CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3";
                }
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
                cast["Hero"]=seh_hero;
                std::map<CCharString,CCharString> args;
                args["$THEME"]="NULL";
                EHeroTitle title=Game->GetHeroTitle();
                if (ArenaRound>2) {
                    if (T) return;
                    Whisper=Game->GetThingWithScriptName("WhisperAlly");
                    if (!Whisper.IsAlive()) {
                        if (T) return;
                        Whisper=Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA",Game->GetThingWithScriptName("ARENA_WhisperStartPoint").GetPos(),"WhisperAlly",false);
                    }
                }
                switch (ArenaRound) {
                case 0:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_01")
                    NewCrowdBaseLevel=0;
                    switch (title) {
                    AR_TITLE(1,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_01")
                    AR_TITLE(2,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_02")
                    AR_TITLE(3,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_03")
                    AR_TITLE(4,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_04")
                    AR_TITLE(5,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_05")
                    AR_TITLE(6,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_06")
                    AR_TITLE(7,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_07")
                    AR_TITLE(8,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_08")
                    AR_TITLE(9,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_09")
                    AR_TITLE(10,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_10")
                    AR_TITLE(11,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_11")
                    AR_TITLE(12,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_12")
                    AR_TITLE(13,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_13")
                    AR_TITLE(14,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_14")
                    AR_TITLE(15,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_15")
                    AR_TITLE(16,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_16")
                    AR_TITLE(17,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_17")
                    AR_TITLE(18,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_18")
                    AR_TITLE(19,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_19")
                    AR_TITLE(20,"TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_20")
                    }
                    cutscene="CS_ARENA_ROUND_1";
                    Game->OverrideMusic(GetFanfareMusic(rand()%10),false,false);
                    break;
                case 1:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_02")
                    NewCrowdBaseLevel=0;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_20";
                    cutscene="CS_ARENA_ROUND_BEGIN_GENERIC";
                    break;
                case 2:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_03")
                    NewCrowdBaseLevel=1;
                    cutscene="CS_ARENA_ROUND_WHISPER_INTRO";
                    args["$THEME"]="ENVIRONMENT_WITCHWOOD";
                    break;
                case 3:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_04")
                    NewCrowdBaseLevel=1;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_20";
                    args["$THEME"]="ENVIRONMENT_HAUNTED";
                    break;
                case 4:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_05")
                    NewCrowdBaseLevel=2;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_20";
                    args["$THEME"]="RESET";
                    break;
                case 5:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_06")
                    NewCrowdBaseLevel=2;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_20";
                    break;
                case 6:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_07")
                    NewCrowdBaseLevel=3;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_20";
                    break;
                case 7:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_08")
                    NewCrowdBaseLevel=3;
                    args["$SAY1"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_10";
                    args["$SAY2"]="TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_20";
                    args["$THEME"]="ENVIRONMENT_EOW_04";
                    Game->OverrideMusic(MUSIC_SET_BOSS,false,false);
                    break;
                case 8:
                    AR_OBJECTIVE("TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_09")
                    NewCrowdBaseLevel=3;
                    ArenaState=MAS_IN_ARENA_WHISPER;
                    cutscene="CS_ARENA_ROUND_BOTH_WINNERS";
                    args["$THEME"]="RESET";
                    break;
                }
                if (Whisper.IsAlive()) {
                    AR_START(Whisper,seh_whisper)
                    cast["Whisper"]=seh_whisper;
                }
                RunCutsceneMacro(cutscene,cast,0,&args,false,true);
                Game->StopOverrideMusic(false);
                if (cutscene=="CS_ARENA_ROUND_BOTH_WINNERS") {
                    if (T) return;
                    FinalBattleCS=true;
                }
                WhisperNeededForCutscene=false;
            }
            if (!PlayerWon) {
                if (T) return;
                if (ArenaState==MAS_IN_ARENA_WHISPER) {
                    if (T) return;
                    AR_WAIT_LEVEL("ArenaCells")
                    if (Game->GetThingWithScriptName("CellWhisper").IsAlive()) {
                        if (T) return;
                        Game->RemoveThing(Game->GetThingWithScriptName("CellWhisper"),false,true);
                    }
                    PlayerWon=true;
                }
                else {
                    if (T) return;
                    PlayWave();
                }
            }
        }
        if (T) return;
        if (PlayerLeaving) {
            if (T) return;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game->Pause(0.5f);
                Game->ResetToDefaultTheme(0.0f);
                Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ArenaCellEntrance2"),false);
            }
            AR_WAIT_LEVEL("ArenaCells")
            Game->FadeScreenIn();
            Game->MiniMapSetAsEnabled(true);
            ArenaState=MAS_CELLS_MID_ARENA;
            AR_CELL_WHISPER
            Game->AutoSaveCheckPoint();
            while (ArenaState!=MAS_IN_ARENA) {
                NSF;
                if (T) return;
            }
            if (T) return;
            Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ArenaInsideHSP"),false);
            Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
            AR_WAIT_LEVEL("Arena")
            AnimateCrowd();
            Game->MiniMapSetAsEnabled(false);
            GoldMultiplier=0;
        }
    }
    if (T) return;
    {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("",movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_roth;
        AR_START(Game->GetHero(),seh_hero)
        AR_START(Game->GetThingWithScriptName("Roth"),seh_roth)
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
        cast["Hero"]=seh_hero;
        cast["Roth"]=seh_roth;
        if (GetMasterData()->WhisperKilledByHero==true) {
            if (T) return;
            RunCutsceneMacro("CS_ARENA_CELLS_LADYGREY_TWO",cast,0,0,false,true);
        }
        else {
            if (T) return;
            RunCutsceneMacro("CS_ARENA_CELLS_LADYGREY_ONE",cast,0,0,false,true);
        }
    }
    CScriptThing entrance=Game->GetThingWithScriptName("CellsToHallOfHeroesEntrance");
    Game->EntityTeleportToThing(Game->GetHero(),entrance,false);
    Game->EntitySetFacingAngle(Game->GetHero(),entrance.GetAngleXY(),true);
    AR_WAIT_LEVEL("ArenaHallOfHeroes")
    CScriptGameResourceObjectScriptedThingBase seh_hero;
    Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
    {
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
        cast["Hero"]=seh_hero;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("",movie);
        RunCutsceneMacro("CS_ARENA_HOH_OUTRO",cast,0,0,false,true);
    }
    Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
    Game->ReturnAllConfiscatedItemsToHero();
    Game->MiniMapSetAsEnabled(true);
    MissionSucceeded=true;
}
