// NScript::CV_HauntedHouseScript::Main() -- the haunted-house quest main: until Q_TraderEscort is done, raise the red forcefield barrier (two effects at PreBarrierHere, cellar door unusable, DoorToLock hidden, candles off) whenever the house loads, re-create the forcefields if they die, and drop the barrier when the house unloads; then remove the forcefields, bind the four entity scripts, spawn CheckRegionLeaving, and until the hero is inside keep the candles off while the house is loaded; light the candles, wait for the cellar door to open, then put them out
// Grade: DIFFER(5171v5321) -- SDIFF 514: inline-budget residue: retail declines the IsAlive wrappers of forcefield1/2 in the re-create test (virtual slot-75 calls), keeps the final T TRetVoid::Return call (0x00E95380) apart from the prelude exit cascades, and expands the natural-end CScriptThing and vector<CScriptThing> dtors that this build calls
// Next idea: model the exit cascades first (a6 without the final T matched the prelude cascades); then the end-of-function dtor expansion order (vector candles, the three things)
// Leaf: ?Main@CV_HauntedHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93EB0, 5321 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CV_HauntedHouseScript@NScript@@UAEXXZ; the address has
//   one .rdata reference, slot 2 of the script vtable 0x012ECB94 that the landed Alloc 0x00E96DD0 stores (slot 1 RegisterMain 0x00E93DA0,
//   slot 3 Init 0x00E93D80, slot 4 OnPersist 0x00E93E60, all landed).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/TRetVoid.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { if (PImp.IsNull()) return false; return PImp->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CSpawnedFunc<NScript::CV_HauntedHouseScript>::CSpawnedFunc(const CCharString&, unsigned long, NScript::CV_HauntedHouseScript*, void (NScript::CV_HauntedHouseScript::*)());   // 0x00E95F10
template <> CEntityScriptBinding<NScript::CV_HauntedHouseScript>::CEntityScriptBinding(const CCharString&, NScript::CV_HauntedHouseScript*, TAllocFunc, unsigned long);   // 0x00E95390 (ICF: one body for every T)

void NScript::CV_HauntedHouseScript::Main()
{
    TRetVoid _Fail_Val;
    bool barrierUp = false;
    CScriptThing preBarrier;
    CScriptThing forcefield1;
    CScriptThing forcefield2;
    std::vector<CScriptThing> candles;
    while (!Game->IsQuestCompleted("Q_TraderEscort")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!barrierUp && Game->IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            preBarrier = Game->GetThingWithScriptName("PreBarrierHere");
            forcefield1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", preBarrier.GetPos(), "", 0.0f, false, false);
            forcefield2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", preBarrier.GetPos(), "", 0.0f, false, false);
            CScriptThing cellarDoor = Game->GetThingWithScriptName("HH_CellarDoor");
            Game->SetThingAsUsable(cellarDoor, false);
            CScriptThing doorToLock = Game->GetThingWithScriptName("DoorToLock");
            Game->EntitySetAsDrawable(doorToLock, false);
            Game->GetAllThingsWithScriptName("CandleToLight", candles);
            for (unsigned int i = 0; i < candles.size(); ++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetLightAsOn(candles[i], false);
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            barrierUp = true;
        }
        if (preBarrier.IsAlive() && !forcefield1.IsAlive() && !forcefield2.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            forcefield1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", preBarrier.GetPos(), "", 0.0f, false, false);
            forcefield2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", preBarrier.GetPos(), "", 0.0f, false, false);
        }
        if (barrierUp && !Game->IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            barrierUp = false;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (forcefield1.IsAlive() && forcefield2.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveThing(forcefield1, false, true);
        Game->RemoveThing(forcefield2, false, true);
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_UndeadHere", this, &CM_UndeadHere::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("HHUndead", this, &CHHUndead::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_QuestControl", this, &CM_QuestControl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_HeroInsideHouse", this, &CM_HeroInsideHouse::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_HauntedHouseScript>("CheckRegionLeaving", 0, this, &CV_HauntedHouseScript::CheckRegionLeaving), CCharString(""));
    while (!InHouse) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game->IsRegionLoaded("HauntedHouse")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GetAllThingsWithScriptName("CandleToLight", candles);
        for (unsigned int i = 0; i < candles.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetLightAsOn(candles[i], false);
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (Game->IsRegionLoaded("HauntedHouse") && !InHouse) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (unsigned int i = 0; i < candles.size(); ++i) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetLightAsOn(candles[i], true);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!CellarDoorOpen) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (unsigned int j = 0; j < candles.size(); ++j) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetLightAsOn(candles[j], false);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
