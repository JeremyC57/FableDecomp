// NScript::CV_TourGuideScript::CTourGuideGuide::Main() -- the guild tour guide: take control of himself, then each frame (until the followers are gone or the tour is over) walk the tour waypoint by waypoint, glance at a random follower, let a nearby follower overhear the location text, answer the hero, wait at each waypoint while the stand-still timer runs; at closing time walk to M_TG_ClosingTimeExit and fade out
// Grade: DIFFER(4356v4246) -- exit pads: retail has one common called pad (0x00EE6817: ~CScriptThing 0x004AA840, ~CTimer 0x00CD4470 x2, ~STB 0x007E74D0) plus two pads that expand DeregisterTimer (natural exit 0x00EE6475, 0x00EE65D9); ours calls ~CTimer everywhere (inline dtor: expanded at 6 pads); retail also expands the CTimer ctor (RegisterTimer slot 87) and keeps both indices in ebx/ebp, ours calls 0x00CD4450 (287 lines)
// Next idea: a `{ }` block that ends both timers before the closing-time block, or the closing-time block as a helper-free `break` to a post-loop tail (a5/a6 loop forms did not move it; a8 noinline dtor 302); else lane gap: per-exit inline budget (gotchas 1648/2099)
// Leaf: ?Main@CTourGuideGuide@CV_TourGuideScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE57B0, length 4246 bytes.
// Name: slot 1 of the entity vtable 0x012F511C (0 scalar deleting dtor 0x00EE4CD0, 2 Init 0x00EE4C60, 3 GetParentScript
//   0x00EE4C50) that the landed Alloc 0x00EE4BB0 stores; Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CTourGuideGuide@CV_TourGuideScript@NScript@@EAEXXZ.
// Callees: MoveToNextWaypoint 0x00EE6850, WatchForNoFollowers 0x00EE6A40 (spawned), IsDistanceBetweenThingsUnder 0x00CBE2FF,
//   IsDistanceFromThingToPositionOver 0x00CBE45C, FadeOutAndKillEntity 0x00CBE716, rand 0x00BFEB16, CTimer 0x00CD4450/0x00CD4470.
// Data: engine/CV_TourGuideScript.h opt-in FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE (saveWaypointIdx +0x1C, lastRandomSpeechIdx
//   +0x20); pGlobalScriptDef TourGuideOverhearDistance +0x8D4, TourGuideTalkDistance +0x8D8.
// Shape: the talk block is two nested ifs (retail T-checks between the distance test and MsgIsTalkedToBy, so no conditional
//   temporary flag word); the overheard block is one && test. Tried (12): see STATUS.tsv.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CV_TourGuideScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
extern "C" int __cdecl rand();

void NScript::CV_TourGuideScript::CTourGuideGuide::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer StandStillTimer;
    CTimer TalkToHeroTimer;
    TalkToHeroTimer = 0;
    CScriptThing ExitMarker;
    ParentClass->Initialise = true;
    ParentClass->TourGuideKilled = false;
    ParentClass->WaypointCounter = 0;
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->AllFollowersDead || ParentClass->TourFinished) {
            if (IsActiveThreadTerminating()) return;
            Game->ClearThingHasInformation(Me);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ExitMarker = Game->GetThingWithScriptName("M_TG_ClosingTimeExit");
            if (seh_me.IsValid() && ExitMarker.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                C3DVector ExitPos = ExitMarker.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), ExitPos, 3.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    seh_me.MoveToPosition(ExitPos, 1.0f, ENTITY_MOVE_WALK, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
            else {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
            }
            return;
        }
        if (ParentClass->Initialise) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (saveWaypointIdx != -1) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->WaypointCounter = saveWaypointIdx + 1;
                if (ParentClass->WaypointCounter >= TG_LOCATIONS_MAX) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->WaypointCounter = 0;
                }
                ParentClass->NextTourWaypoint = Game->GetThingWithScriptName(ParentClass->WaypointInfo[ParentClass->WaypointCounter].locMarker);
                saveWaypointIdx = -1;
            }
            else if (ParentClass->NextTourWaypoint.IsAlive() && ParentClass->WaypointCounter < TG_LOCATIONS_MAX) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->NextTourWaypoint = Game->GetThingWithScriptName(ParentClass->WaypointInfo[ParentClass->WaypointCounter].locMarker);
            }
            else {
                if (IsActiveThreadTerminating()) return;
                ParentClass->NextTourWaypoint = Game->GetThingWithScriptName("M_TG_LocationStart");
                ParentClass->WaypointCounter = 0;
            }
            ParentClass->MoveToNextWaypoint(ParentClass->NextTourWaypoint, seh_me);
            if (!ParentClass->SpawnedQuestFinishThread) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->AddSpawnedFunction(new CSpawnedFunc<CV_TourGuideScript>(CCharString("ParentClass.") + CCharString("WatchForNoFollowers"), 0, ParentClass, &CV_TourGuideScript::WatchForNoFollowers), CCharString(""));
                ParentClass->SpawnedQuestFinishThread = true;
            }
            ParentClass->Initialise = false;
            ParentClass->GuideSpokenToHeroThisWaypoint = false;
            ParentClass->OverheardTourGuideThisWaypoint = false;
        }
        if (rand() % 50 == 0) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing Follower = Game->GetRandomThingWithScriptName("TourGuideFollower");
            if (Follower.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                Game->EntityForceToLookAtThing(Me, Follower);
            }
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->TourGuideOverhearDistance) && !ParentClass->OverheardTourGuideThisWaypoint) {
            if (IsActiveThreadTerminating()) return;
            CCharString OverheardText = ParentClass->WaypointInfo[ParentClass->WaypointCounter].locTextOverheard;
            if (!ParentClass->TourGuideKilled) {
                if (IsActiveThreadTerminating()) return;
                CScriptThing Follower = Game->GetRandomThingWithScriptName("TourGuideFollower");
                if (Follower.IsAlive()) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Follower);
                    Game->AddLineToConversation(conversation, OverheardText, false, Me, Follower);
                }
            }
            ParentClass->OverheardTourGuideThisWaypoint = true;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->TourGuideTalkDistance)) {
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            CCharString SpeechText;
            if (!ParentClass->GuideSpokenToHeroThisWaypoint) {
                if (IsActiveThreadTerminating()) return;
                SpeechText = ParentClass->WaypointInfo[ParentClass->WaypointCounter].locTextRequested;
                ParentClass->GuideSpokenToHeroThisWaypoint = true;
            }
            else {
                if (IsActiveThreadTerminating()) return;
                long idx;
                for (;;) {
                    if (IsActiveThreadTerminating()) return;
                    idx = rand() % 5;
                    if (idx != lastRandomSpeechIdx) break;
                    Game->NewScriptFrame();
                }
                if (IsActiveThreadTerminating()) return;
                lastRandomSpeechIdx = idx;
                if (IsActiveThreadTerminating()) return;
                SpeechText = ParentClass->RandomGuideResponse[idx];
            }
            if (!ParentClass->TourGuideKilled) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, SpeechText, false, Me, Game->GetHero());
                TalkToHeroTimer = pGlobalScriptDef->TourGuideTalkToHeroTimer;
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass->MoveToNextWaypoint(ParentClass->NextTourWaypoint, seh_me);
            }
        }
        if (IsDistanceBetweenThingsUnder(Me, ParentClass->NextTourWaypoint, 2.0f)) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing Follower = Game->GetNearestWithScriptName(Me, "TourGuideFollower");
            if (Follower.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                seh_me.ClearCommands();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                Game->EntitySetFacingAngleTowardsThing(Me, Follower, false);
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            StandStillTimer = pGlobalScriptDef->TourGuideStandStillTimer;
            for (;;) {
                if (IsActiveThreadTerminating()) return;
                if (StandStillTimer.GetTime() == 0) break;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return;
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    seh_me.ClearCommands();
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    long idx;
                    for (;;) {
                        if (IsActiveThreadTerminating()) return;
                        idx = rand() % 5;
                        if (idx != lastRandomSpeechIdx) break;
                        Game->NewScriptFrame();
                    }
                    if (IsActiveThreadTerminating()) return;
                    lastRandomSpeechIdx = idx;
                    if (IsActiveThreadTerminating()) return;
                    CCharString SpeechText = ParentClass->RandomGuideResponse[idx];
                    if (!ParentClass->TourGuideKilled) {
                        if (IsActiveThreadTerminating()) return;
                        long conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, SpeechText, false, Me, Game->GetHero());
                    }
                }
                Game->NewScriptFrame();
            }
            if (IsActiveThreadTerminating()) return;
            if (IsActiveThreadTerminating()) return;
            ++ParentClass->WaypointCounter;
            ParentClass->Initialise = true;
        }
        Game->NewScriptFrame();
    }
}
