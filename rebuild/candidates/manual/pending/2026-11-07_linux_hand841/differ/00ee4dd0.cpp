// NScript::CV_TourGuideScript::CTourGuideFollower::Main() -- a guild-tour follower: take scripted control of Me and follow the TourGuideGuide; each frame, while the guide lives and the tour runs, answer the hero (once the stand-still timer has run out) with a random male / female follower response and follow the guide again; once the guide is killed or the tour is over, clear Me's information, retake control, walk to M_TG_ClosingTimeExit and fade out (or just idle when there is no exit)
// Grade: DIFFER(1925v1946) -- per-exit ~CTimer split: the closing block's IsPerformingScriptTask wait-loop exit takes a pad that expands DeregisterTimer where retail sends it to the common pad calling ~CTimer 0x00CD4470, which also moves the two expanded-timer pads after the closing block (91 lines)
// Next idea: cool that one wait-loop exit without touching the StartScripting retry exit before it (f4 break form and f6 retry for(;;) did not); otherwise a function-wide inline budget park
// Leaf: ?Main@CTourGuideFollower@CV_TourGuideScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE4DD0, 1946 bytes, thiscall, plain `ret` (five epilogue copies), frame 0x54.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CTourGuideFollower@CV_TourGuideScript@NScript@@EAEXXZ; the single .rdata
//   reference is slot 1 of the entity vtable 0x012F5160 (slot 0 scalar dtor 0x00EE4DB0, slot 2 Init 0x00EE4DA0, slot 3 GetParentScript
//   0x00EE4D90, the class's landed labels).
// Callees: CCharString default ctor 0x0099E4B0 / ~ 0x0099EAE0 / operator= 0x0099EFB0 / (const char*, long) 0x0099EBF0, CTimer inline over
//   pDSTGame 0x0143E8F8 (slots 87-90; the one called dtor 0x00CD4470), CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase IsValid
//   0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / FollowThing 0x007E7320 / ClearCommands 0x007E7360 / GetScriptThing 0x007E7490 /
//   MoveToPosition 0x007E72F0 / IsPerformingScriptTask 0x007E7450, CScriptThing operator= 0x008AB980 / ~ 0x004AA840 / IsAlive 0x004AB130
//   (CBaseClass 0x0099A2D0 / ~ 0x0099A2E0 inline), IsActiveThreadTerminating 0x00F35B30, rand 0x00BFEB16,
//   IsDistanceFromThingToPositionOver 0x00CBE45C, FadeOutAndKillEntity 0x00CBE716 (__fastcall); CGameScriptInterfaceBase slots 7, 8,
//   70, 72, 361, 364-366, 475, 513.
// Strings: "TourGuideGuide" 0x012F510C, "SCRIPT_NAME_HERO" 0x0125D1C8, "M_TG_ClosingTimeExit" 0x012F517C.
// Layout: engine/CV_TourGuideScript.h (opt-in FABLE_RETAIL_CV_TOURGUIDESCRIPT_FOLLOWER): ParentClass pointer +0x14 (TourGuideKilled
//   +0x120, TourFinished +0x123, RandomFollowerResponseM +0x13c, RandomFollowerResponseF +0x150); pGlobalScriptDef 0x0143E90C
//   TourGuideFollowerDistance +0x8d0, TourGuideFollowerStandStillTimer +0x8e4.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CV_TOURGUIDESCRIPT_FOLLOWER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CV_TourGuideScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;                                                                            // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
extern "C" int __cdecl rand();

void NScript::CV_TourGuideScript::CTourGuideFollower::Main()
{
    CCharString text;
    CTimer timer;
    timer = 0;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guide = Game->GetThingWithScriptName("TourGuideGuide");
    CScriptThing exit_marker;
    if (!ParentClass->TourGuideKilled) {
        if (IsActiveThreadTerminating())
            return;
        seh_me.FollowThing(guide, pGlobalScriptDef->TourGuideFollowerDistance, true);
    }
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass->TourGuideKilled && !ParentClass->TourFinished) {
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && timer.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return;
                seh_me.ClearCommands();
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                CCharString speech;
                long idx = rand() % 5;
                ESex sex = Game->EntityGetSex(Me);
                if (sex == SEX_MALE) {
                    if (IsActiveThreadTerminating())
                        return;
                    text = ParentClass->RandomFollowerResponseM[idx];
                }
                else if (sex == SEX_FEMALE) {
                    if (IsActiveThreadTerminating())
                        return;
                    text = ParentClass->RandomFollowerResponseF[idx];
                }
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero());
                timer = pGlobalScriptDef->TourGuideFollowerStandStillTimer;
                seh_me.FollowThing(guide, pGlobalScriptDef->TourGuideFollowerDistance, true);
            }
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            Game->ClearThingHasInformation(Me);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                if (!seh_me.IsPerformingScriptTask())
                    break;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            exit_marker = Game->GetThingWithScriptName("M_TG_ClosingTimeExit");
            if (seh_me.IsValid() && exit_marker.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                C3DVector exit_pos = exit_marker.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), exit_pos, 3.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    seh_me.MoveToPosition(exit_pos, 1.0f, ENTITY_MOVE_WALK, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                if (seh_me.IsValid())
                    seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            return;
        }
    }
}
