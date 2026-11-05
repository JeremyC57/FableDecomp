// NScript::CQ_TraderConflictGoodScript::AttackPeople() -- the bandits of the Trader Conflict (good) quest attack: while the hero has not entered the new region, enemies remain and not every following trader has reached the teleporter, each frame find the first enemy near or aware of the hero or its nearest following trader, point it at that trader (or the hero), say the attack line once per four-second timer, mark it red on the mini map and drop it from the list; five script frames per pass.
// Grade: DIFFER(2456v2088) -- per-site inline decisions VC7.1 makes differently: retail calls ~CScriptThing (0x004AA840) out of line at the null-temporary and loop-end sites and expands it only at the break site, and merges the four trader-alive terminate returns into one exit block (0x00DFDC54: ~CScriptThing call, ~CCharString, inline ~CTimer, ~vector call) and the two timer-block returns into another (0x00DFDC8A, ~CTimer out of line); VC7.1 expands ~CScriptThing at the temporary and the first exits and emits a separate cleanup per return (+368 bytes), plus a different register choice in the hero arm.
// Next idea: tried (a) all shared-header inlines, (b) out-of-line vector::erase opt-in, (c) __forceinline CTimer::operator=, (d) a named null local, (e) out-of-line ~CScriptThing (main body matches to 0x00DFDABB but the break site then calls), (f) plain inline CScriptThing::operator= [best], (h) while + break head, (k) __forceinline GetTime, (n) const-ref enemy alias, (o) __forceinline ~CScriptThing (391 lines); the exit merge needs the dtor out of line at the returns but inline at the break site -- look for a retail spelling where the break-site object is a distinct inner-scope local (or the erase loop sits in a helper scope) so the budget lands on that site only.
// Leaf: ?AttackPeople@CQ_TraderConflictGoodScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFD600, 2088 bytes, thiscall, plain `ret`, no EH frame, frame 0x40.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?AttackPeople@CQ_TraderConflictGoodScript@NScript@@QAEXXZ; Ego_r orders UpdateLiveEnemies /
//   WatchForHittingEnemies / WatchForRegionTransitions / AttackPeople as retail 0x00DFC320 / 0x00DFC630 / (the caller, call at 0x00DFCDBD) /
//   0x00DFD600; the first call is the landed UpdateLiveEnemies 0x00DFC320; literals "TraderToRescue", "TC_BanditGuard",
//   "TEXT_QST_B11_BANDIT_ATTACK_TRADER" / "_HERO", "HUD_ORB_RED_SMALL".
// Callees: UpdateLiveEnemies 0x00DFC320; vector<CScriptThing> copy ctor 0x00DFDE30 / erase(iterator) 0x00CD3152 / ~vector 0x008AC970;
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; IsDistanceBetweenThingsUnder 0x00CBE2FF; ~CScriptThing 0x004AA840; CBaseClass()
//   0x0099A2D0 / ~CBaseClass 0x0099A2E0; ~CTimer 0x00CD4470; CCharString 0x0099EBF0 / ~ 0x0099EAE0 / the GetName comparison 0x0099E960; the
//   CGameScriptInterfaceBase slots through Game (+0x40) and pDSTGame 0x0143E8F8 (timer slots +0x15C / +0x160 / ...).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_containers.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
namespace NScript {
class CQ_TraderConflictGoodScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    std::vector<CScriptThing> AllCreatures;                 // +0x48
    long TradersFollowing;                                  // +0x54
    long TradersReachedTeleporter;                          // +0x58
    long BanditSecurityLinesSaid;                           // +0x5c
    bool FirstRegionDead;                                   // +0x60
    bool EnteredNewRegion;                                  // +0x61
    bool IntroDone;                                         // +0x62
    bool OutroStart;                                        // +0x63
    bool OutroDone;                                         // +0x64
    bool OpenedCage;                                        // +0x65
    bool CommentedOnBanditCostume;                          // +0x66
    bool MissionSucceeded;                                  // +0x67
    void UpdateLiveEnemies();
    void AttackPeople();
};
}
void NScript::CQ_TraderConflictGoodScript::AttackPeople()
{
    UpdateLiveEnemies();
    std::vector<CScriptThing> enemies(AllCreatures);
    CTimer conversationTimer;
    CCharString unused;
    while (!EnteredNewRegion && enemies.size() != 0 && TradersReachedTeleporter < TradersFollowing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        for (unsigned int i = 0; i < enemies.size(); ++i) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing trader = Game->GetNearestWithScriptName(enemies[i], "TraderToRescue");
            if (!Game->IsEntityFollowingHero(trader)) {
                if (IsActiveThreadTerminating()) return;
                trader = CScriptThing();
            }
            if (IsDistanceBetweenThingsUnder(enemies[i], Game->GetHero(), 15.0f)
                || IsDistanceBetweenThingsUnder(enemies[i], trader, 15.0f)
                || Game->IsThingAwareOfOtherThingInAnyWay(enemies[i], Game->GetHero())
                || (Game->IsThingAwareOfOtherThingInAnyWay(enemies[i], trader) && enemies[i].GetName() != "TC_BanditGuard")) {
                if (IsActiveThreadTerminating()) return;
                if (IsDistanceBetweenThingsUnder(enemies[i], trader, 15.0f)
                    || (Game->IsThingAwareOfOtherThingInAnyWay(enemies[i], trader) && trader.IsAlive())) {
                    if (IsActiveThreadTerminating()) return;
                    Game->GiveThingBestEnemyTarget(enemies[i], trader);
                    if (conversationTimer.GetTime() == 0) {
                        if (IsActiveThreadTerminating()) return;
                        long conversation = Game->AddNewConversation(enemies[i], false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", false, enemies[i], Game->GetHero());
                        conversationTimer = 4;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    Game->GiveThingBestEnemyTarget(enemies[i], Game->GetHero());
                    if (conversationTimer.GetTime() == 0) {
                        if (IsActiveThreadTerminating()) return;
                        long conversation = Game->AddNewConversation(enemies[i], false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_ATTACK_HERO", false, enemies[i], Game->GetHero());
                        conversationTimer = 4;
                    }
                }
                Game->MiniMapAddMarker(enemies[i], "HUD_ORB_RED_SMALL");
                for (std::vector<CScriptThing>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
                    if (enemies[i] == *it) {
                        enemies.erase(it);
                        break;
                    }
                }
                break;
            }
        }
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (EnteredNewRegion) {
        if (IsActiveThreadTerminating()) return;
        EnteredNewRegion = false;
    }
}
