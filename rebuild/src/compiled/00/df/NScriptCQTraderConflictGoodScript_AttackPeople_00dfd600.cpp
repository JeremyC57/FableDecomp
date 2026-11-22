// 00DFD600 NScript::CQ_TraderConflictGoodScript::AttackPeople() -- Run the Trader Conflict bandit attacks until escape, region transition, or termination.
// Leaf: ?AttackPeople@CQ_TraderConflictGoodScript@NScript@@QAEXXZ
// Flags: /d2inlT70
// Inline-budget: the two conversation exits use the shared timer destructor at 00DFDC8A.
// Inferred: the scoped exit at 00DFD84A reproduces the separately expanded timer cleanup at 00DFDC54.
// Identity: both commercial PDBs identify this member of the Trader Conflict level script.

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#include "fable_compiler.h"
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "engine/TRetVoid.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_containers.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing() { }
// Inline-budget: the empty handle assignment expands at 00DFD720. The temporary destructor is called at 00DFD763.
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { CScriptThing* implementation = PImp.Data; return implementation && implementation->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime() == time; }
// Inline-budget: both conversation branches reach the common SetTimer dispatch at 00DFDA46.
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
#include "engine/CQ_TraderConflictGoodScript.h"
void NScript::CQ_TraderConflictGoodScript::AttackPeople()
{
    TRetVoid _Fail_Val;
    UpdateLiveEnemies();
    long CreaturesAttacking = 0;
    long TotalCreaturesToAttack = AllCreatures.size();
    std::vector<CScriptThing> CurrentCreatures(AllCreatures);
    CTimer AttackCryDelay;
    CCharString NewRegionLoaded;
    while (!EnteredNewRegion && CurrentCreatures.size() > 0 && TradersReachedTeleporter < TradersFollowing) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        for (long i = 0; i < CurrentCreatures.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Trader = Game.GetNearestWithScriptName(CurrentCreatures[i], "TraderToRescue");
            if (!Game.IsEntityFollowingHero(Trader)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Trader = CScriptThing();
            }
            if (IsDistanceBetweenThingsUnder(CurrentCreatures[i], Game.GetHero(), 15.0f)
                || IsDistanceBetweenThingsUnder(CurrentCreatures[i], Trader, 15.0f)
                || Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Game.GetHero())
                || (Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Trader) && CurrentCreatures[i].GetName() != "TC_BanditGuard")) {
                if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    goto trader_cleanup;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
                if (IsDistanceBetweenThingsUnder(CurrentCreatures[i], Trader, 15.0f)
                    || (Game.IsThingAwareOfOtherThingInAnyWay(CurrentCreatures[i], Trader) && Trader.IsAlive())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveThingBestEnemyTarget(CurrentCreatures[i], Trader);
                    if (AttackCryDelay == 0) {
                        if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
                        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        long HLStyleConversationIndex = Game.AddNewConversation(CurrentCreatures[i], false, false);
                        Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                        Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", false, CurrentCreatures[i], Game.GetHero());
                        AttackCryDelay = 4;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveThingBestEnemyTarget(CurrentCreatures[i], Game.GetHero());
                    if (AttackCryDelay == 0) {
                        if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
                        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        long HLStyleConversationIndex = Game.AddNewConversation(CurrentCreatures[i], false, false);
                        Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                        Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B11_BANDIT_ATTACK_HERO", false, CurrentCreatures[i], Game.GetHero());
                        AttackCryDelay = 4;
                    }
                }
                Game.MiniMapAddMarker(CurrentCreatures[i], "HUD_ORB_RED_SMALL");
                for (std::vector<CScriptThing>::iterator it = CurrentCreatures.begin(); it != CurrentCreatures.end(); ++it) {
                    if (CurrentCreatures[i] == *it) {
                        CurrentCreatures.erase(it);
                        break;
                    }
                }
                ++CreaturesAttacking;
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (EnteredNewRegion) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        EnteredNewRegion = false;
    }
    return;
trader_cleanup:
    return _Fail_Val.Return();
}