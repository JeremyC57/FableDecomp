// NScript::CQ_HobbeCaveScript::CHobbeRoaming::Main() -- a roaming hobbe in the Hobbe Cave: take scripted control of Me; if the alarm is already raised, one time in four wait until the hero is near or noticed and call the alarm, else wait for the hero to be noticed, call the alarm and raise it; then attack a random hero follower (one time in three, when the nymph focal region is not loaded) or the hero, release control and, while the bandit sacrifice lives, flag "hobbes close by" once it comes within 10 units
// Leaf: ?Main@CHobbeRoaming@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91960, 1085 bytes (+ int3 padding), thiscall, plain `ret`; entity-script vtable slot 1 (Main).
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CHobbeRoaming@CQ_HobbeCaveScript@NScript@@EAEXXZ; the "HobbeRoaming" Alloc 0x00D8F4F0.
// Callees: CIsThingAlive (vtable 0x012C32F0) over the CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / dtor 0x007E74D0, ~vector<CScriptThing> 0x008AC970, rand 0x00BFEB16, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; Game slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   12 IsRegionLoaded, 70 GetHero, 72 GetThingWithScriptName, 231 GetFollowingEntityList, 451 GiveThingBestEnemyTarget,
//   689 PlayCriteriaSoundOnThing; CScriptThing slot 16 IsAwareOfHero (on Me) and slot 75 IsAlive (inline on the sacrifice).
//   Literals "HOBBE_ALARM_CALL" 0x012D59D0, "HobbeCaveNymphFocal", "BanditSacrifice" 0x012D5788.
// Data: ParentClass AlarmTriggered +0x4d, HobbesCloseby +0x6b (engine/CQ_HobbeCaveScript.h).
// Shape: the bandit-sacrifice loop is `while (sacrifice.IsAlive())` and the final idle loop is the landed 0x00CFCCA0 / 0x00D43190
//   form (`if (terminating) return; do NewScriptFrame(); while (!terminating)`): together they let both exits of the alive loop
//   jump straight to the idle loop's bottom test (0x00D91D1C / 0x00D91D28 -> 0x00D91D78), as retail. A `for (;;) { if (!alive)
//   break; ...}` alive loop keeps a duplicated entry test there (1101v1085).
#include <stdlib.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_HOBBECAVESCRIPT_HOBBEROAMING
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
// Retail expands these at this row's sites: defined inline after the headers.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // 0x004AB130 (slot 75)
using NScript::CIsThingAlive;

void NScript::CQ_HobbeCaveScript::CHobbeRoaming::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase hobbe;
    if (hobbe.IsValid())
        hobbe.Release();
    while (!Game->StartScriptingEntity(Me, hobbe, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->AlarmTriggered) {
        if (IsActiveThreadTerminating())
            return;
        if (rand() % 4 == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f) || Me.IsAwareOfHero())
                    break;
                Game->NewScriptFrame();
                if (!IsActiveThreadTerminating())
                    continue;
                return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->PlayCriteriaSoundOnThing(Me, "HOBBE_ALARM_CALL");
            if (IsActiveThreadTerminating())
                return;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        while (!ParentClass->AlarmTriggered) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.IsAwareOfHero()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Me, "HOBBE_ALARM_CALL");
                ParentClass->AlarmTriggered = true;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
    {
        std::vector<CScriptThing> followers;
        Game->GetFollowingEntityList(Game->GetHero(), followers);
        if (followers.size() != 0 && rand() % 3 == 0 && !Game->IsRegionLoaded("HobbeCaveNymphFocal")) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, followers[rand() % followers.size()]);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        }
    }
    hobbe.Release();
    CScriptThing sacrifice = Game->GetThingWithScriptName("BanditSacrifice");
    while (sacrifice.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass->HobbesCloseby && IsDistanceBetweenThingsUnder(Me, sacrifice, 10.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->HobbesCloseby = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}