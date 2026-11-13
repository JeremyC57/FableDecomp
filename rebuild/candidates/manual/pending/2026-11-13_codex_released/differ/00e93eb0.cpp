// NScript::CV_HauntedHouseScript::Main() -- the haunted-house quest main: until Q_TraderEscort is done, raise the red forcefield barrier (two effects at PreBarrierHere, cellar door unusable, DoorToLock hidden, Candles off) whenever the house loads, re-create the forcefields if they die, and drop the barrier when the house unloads; then remove the forcefields, bind the four entity scripts, spawn CheckRegionLeaving, and until the hero is inside keep the Candles off while the house is loaded; light the Candles, wait for the cellar door to open, then put them out
// Grade: DIFFER(5173v5321) -- First 21 of 22 true cleanup paths agree in callee choices; final cancellation expands all three Things versus native two expanded plus called FXBarrier. FXBarrier/Forcefield1 slots are swapped and one vector-cleanup placement differs. 59 strict / 4 loose hunks.
// Inline-budget: Native E950CC calls vector/Thing cleanup; guarded depth0 applies only to that post-cellar cancellation. Default T74 gives native callee choices at the first21 exits.
// Remaining: final cancellation expands one extra Thing destructor; cleanup placement and two object slots differ. No byte-match claim.
// Leaf: ?Main@CV_HauntedHouseScript@NScript@@UAEXXZ
// Retail: 0x00E93EB0, 5321 bytes, thiscall.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CV_HauntedHouseScript@NScript@@UAEXXZ; the address has
//   one .rdata reference, slot 2 of the script vtable 0x012ECB94 that the landed Alloc 0x00E96DD0 stores (slot 1 RegisterMain 0x00E93DA0,
//   slot 3 Init 0x00E93D80, slot 4 OnPersist 0x00E93E60, all landed).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
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
// Original called root constructor99A2D0 is visible; root destructor retains its original declaration.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CBaseClass::CBaseClass() {}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { CBaseClass::operator=(other); PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline bool CScriptThing::IsDead() const { return !IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CSpawnedFunc<NScript::CV_HauntedHouseScript>::CSpawnedFunc(const CCharString&, unsigned long, NScript::CV_HauntedHouseScript*, void (NScript::CV_HauntedHouseScript::*)());   // 0x00E95F10
template <> CEntityScriptBinding<NScript::CV_HauntedHouseScript>::CEntityScriptBinding(const CCharString&, NScript::CV_HauntedHouseScript*, TAllocFunc, unsigned long);   // 0x00E95390 (ICF: one body for every T)

void NScript::CV_HauntedHouseScript::Main()
{
    TRetVoid _Fail_Val;
    bool DoneThisBefore = false;
    CScriptThing FXBarrier;
    CScriptThing Forcefield1;
    CScriptThing Forcefield2;
    std::vector<CScriptThing> Candles;
    while (!Game.IsQuestCompleted("Q_TraderEscort")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!DoneThisBefore && Game.IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            FXBarrier = Game.GetThingWithScriptName("PreBarrierHere");
            Forcefield1 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", FXBarrier.GetPos(), "", 0.0f, false, false);
            Forcefield2 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", FXBarrier.GetPos(), "", 0.0f, false, false);
            CScriptThing CellarDoor = Game.GetThingWithScriptName("HH_CellarDoor");
            Game.SetThingAsUsable(CellarDoor, false);
            CScriptThing LockableDoor = Game.GetThingWithScriptName("DoorToLock");
            Game.EntitySetAsDrawable(LockableDoor, false);
            Game.GetAllThingsWithScriptName("CandleToLight", Candles);
            if (!false) {
                for (long i = 0; i < Candles.size(); ++i) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntitySetLightAsOn(Candles[i], false);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoneThisBefore = true;
        }
        if (FXBarrier.IsAlive() && Forcefield1.IsDead() && Forcefield2.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Forcefield1 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", FXBarrier.GetPos(), "", 0.0f, false, false);
            Forcefield2 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", FXBarrier.GetPos(), "", 0.0f, false, false);
        }
        if (DoneThisBefore && !Game.IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoneThisBefore = false;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Forcefield1.IsAlive() && Forcefield2.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Forcefield1, false, true);
        Game.RemoveThing(Forcefield2, false, true);
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_UndeadHere", this, &CM_UndeadHere::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("HHUndead", this, &CHHUndead::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_QuestControl", this, &CM_QuestControl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_HeroInsideHouse", this, &CM_HeroInsideHouse::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_HauntedHouseScript>("CheckRegionLeaving", 0, this, &CV_HauntedHouseScript::CheckRegionLeaving), CCharString(""));
    while (!InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game.IsRegionLoaded("HauntedHouse")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.GetAllThingsWithScriptName("CandleToLight", Candles);
        if (!false) {
            for (long i = 0; i < Candles.size(); ++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetLightAsOn(Candles[i], false);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (Game.IsRegionLoaded("HauntedHouse") && !InHouse) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!false) {
        for (long i = 0; i < Candles.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetLightAsOn(Candles[i], true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!CellarDoorOpen) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    if (!false) {
        for (long i = 0; i < Candles.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetLightAsOn(Candles[i], false);
        }
    }
    if (IsActiveThreadTerminating()) {
        void (TRetVoid::*ReturnFunction)()=&TRetVoid::Return;
        (_Fail_Val.*ReturnFunction)();
        return;
    }
}
