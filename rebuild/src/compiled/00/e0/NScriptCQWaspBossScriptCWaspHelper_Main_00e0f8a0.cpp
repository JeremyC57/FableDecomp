// NScript::CQ_WaspBossScript::CWaspHelper::Main() -- Wasp Menace helper villager: take Me under scripted control, follow the hero until within WB_WaspHelperCallsOutDistance and plead (long-time or plain "follow me" line in a paused movie sequence), lead the hero to WaspGuardMoveToPos (shout / wave when the hero strays more than 9 units behind, run or walk by distance, "go this way" every 8 s), wave the hero over at the gate, then loop at the entrance answering the hero and calling him over every 10 s
// Leaf: ?Main@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0F8A0, 4389 bytes, thiscall, plain `ret`, no EH frame. Literals "WaspGuardMoveToPos"
//   0x012E03A4, "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10" 0x012E0378, "TEXT_QST_072_HELPER_FOLLOW_ME_10" 0x012E0354,
//   "TEXT_QST_072_HELPER_OVER_HERE" 0x012E0334, "ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED" 0x0128F350,
//   "ST_OPINION_APPROVAL_WAVING_AT_DISTANCE" 0x0128F3B0, "TEXT_QST_072_HELPER_GO_THIS_WAY" 0x012E0314, "TEXT_QST_072_HELPER_THIS_WAY_10"
//   0x012E02F4, "STANDARD_WAVE" 0x012593EC, "ST_CALL_OVER" 0x01273434, "SCRIPT_NAME_HERO" 0x0125D1C8,
//   "TEXT_QST_072_HELPER_NOT_MOVED_10" 0x012E02D0, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10" 0x012E02A0, "" 0x0122D70E;
//   0.0f 0x0122DEDC.
// Identity (two sources): slot 1 (Main) of the entity vtable 0x012E0284 that the landed CWaspHelper::Alloc stores (0x00E0F7F6;
//   slot 0 scalar dtor 0x00E0F880, slot 2 Init 0x00E0F870, slot 3 GetParentScript 0x00E0F860, slot 4 OnPersist 0x00E12300);
//   Ego_r.pdb / FableWin.pdb ?Main@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ (private virtual); the WaspBoss seed-corpus Lua
//   port (refs/script_recovery/seed_corpus/sources/WaspBoss/FSE/WaspBoss/Entities/WaspHelper.lua) follows the same phases and literals.
// Layout: CWaspHelper (Ego_r 0x20) ParentClass +0x14, PMasterData +0x18, PleadedToHero +0x1c, LeadToRegion +0x1d, WavedOver +0x1e;
//   CQ_WaspBossScript (engine header, retail offsets) MissionSucceeded +0x48, SavedVillagerCount +0x50, ReachedWaspHelper +0x58;
//   pGlobalScriptDef WB_WaspHelperCallsOutDistance +0xe50.
// Callees: CIsThingAlive (vtable 0x012C32F0), CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   FollowThing 0x007E7320 / MoveToPosition 0x007E72F0 / ClearCommands 0x007E7360 / ClearAllActions 0x007E7400 / Speak 0x007E7390 /
//   PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490 / ~ 0x007E74D0, movie ctor 0x006E7B60 /
//   ~ 0x006E7B80, CTimer 0x00CD4450 / ~ 0x00CD4470 (GetTime and operator= expanded: pDSTGame 0x0143E8F8 slots 90 / 89), CCharString
//   0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF, GetDistanceBetweenThings 0x00CBE512 (__fastcall);
//   CGameScriptInterfaceBase slots 7, 8, 70, 72, 108, 264, 364, 365, 366, 370, 376, 379, 475, 844; CScriptThing slot 27
//   MsgIsTalkedToBy; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748.
// Shape: started from the landed CWaspVictim::Main 0x00E11670 (same TU family) verbatim; CTimer ctor / dtor same-TU noinline,
//   operator= and GetTime `__forceinline` (brief: established CTimer levers; plain inline GetTime stays a call at the
//   ReachedWaspHelper site); the lead-phase "hero further" test is the single expression `GetDistanceBetweenThings(Me, pos) <
//   GetDistanceBetweenThings(Game->GetHero(), pos)` (left operand first: retail loads Game before the first call). Every exit
//   cross-jumps to the shared ~CTimer / ~CScriptThing / ~STB tail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_WASPBOSSSCRIPT_WASPHELPER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CEntityScriptBase.h"
#include "engine/NScript_CQ_WaspBossScript.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE512
extern CGameScriptInterfaceBase* pDSTGame;
__declspec(noinline) CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
__declspec(noinline) CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
class CQ_WaspBossScript::CWaspHelper : public CEntityScriptBase {
public:
    CQ_WaspBossScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    PleadedToHero;              // +0x1c
    bool                    LeadToRegion;               // +0x1d
    bool                    WavedOver;                  // +0x1e
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Init();
    virtual void Main();
};
}

void NScript::CQ_WaspBossScript::CWaspHelper::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing move_to_pos = Game->GetThingWithScriptName("WaspGuardMoveToPos");
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (!PleadedToHero) {
        if (IsActiveThreadTerminating()) return;
        seh_me.FollowThing(Game->GetHero(), 1.0f, true);
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->WB_WaspHelperCallsOutDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->MissionSucceeded) {
            if (IsActiveThreadTerminating()) return;
            Game->RemoveThing(Me, false, true);
        }
        if (ParentClass->ReachedWaspHelper.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        } else {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_072_HELPER_FOLLOW_ME_10", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        PleadedToHero = true;
    }
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (!LeadToRegion) {
        if (IsActiveThreadTerminating()) return;
        bool shouted = false;
        bool running = false;
        CTimer say_timer;
        seh_me.ClearCommands();
        seh_me.MoveToPosition(move_to_pos.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
        while (!IsDistanceBetweenThingsUnder(Me, move_to_pos, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            bool hero_further = GetDistanceBetweenThings(Me, move_to_pos) < GetDistanceBetweenThings(Game->GetHero(), move_to_pos);
            if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 9.0f) && hero_further) {
                if (IsActiveThreadTerminating()) return;
                seh_me.ClearCommands();
                seh_me.ClearAllActions();
                if (!shouted) {
                    if (IsActiveThreadTerminating()) return;
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_OVER_HERE", false, Me, Game->GetHero());
                    Game->Pause(1.0f);
                    shouted = true;
                }
                if (say_timer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return;
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_OVER_HERE", false, Me, Game->GetHero());
                    seh_me.PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    Game->Pause(0.8f);
                    seh_me.PlayAnimation("ST_OPINION_APPROVAL_WAVING_AT_DISTANCE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    say_timer = 5;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 6.5f) && hero_further) {
                    if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 8.0f)) {
                        if (IsActiveThreadTerminating()) return;
                        if (running) {
                            if (IsActiveThreadTerminating()) return;
                            seh_me.ClearCommands();
                            seh_me.MoveToPosition(move_to_pos.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                            running = false;
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (!running) {
                        if (IsActiveThreadTerminating()) return;
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(move_to_pos.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                        running = true;
                    }
                }
                shouted = false;
                if (say_timer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_GO_THIS_WAY", false, Me, Game->GetHero());
                    say_timer = 8;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        LeadToRegion = true;
    }
    seh_me.ClearCommands();
    Game->SetIsPushableByHero(Me, false);
    if (!WavedOver) {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_THIS_WAY_10", false, Me, Game->GetHero());
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        seh_me.PlayAnimation("STANDARD_WAVE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetFacingAngleTowardsThing(Me, move_to_pos, false);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        seh_me.PlayAnimation("ST_CALL_OVER", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        WavedOver = true;
    }
    CTimer idle_timer;
    idle_timer = 10;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (ParentClass->SavedVillagerCount < 2) {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", false, Me, Game->GetHero());
            }
        }
        if (idle_timer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            idle_timer = 10;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (ParentClass->SavedVillagerCount < 2) {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", false, Me, Game->GetHero());
            }
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("STANDARD_WAVE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, move_to_pos, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("ST_CALL_OVER", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        Game->NewScriptFrame();
    }
}