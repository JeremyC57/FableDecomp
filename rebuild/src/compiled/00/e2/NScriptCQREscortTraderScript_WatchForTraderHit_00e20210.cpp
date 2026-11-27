// 00e20210 NScript::CQR_EscortTraderScript::WatchForTraderHit
// Leaf: ?WatchForTraderHit@CQR_EscortTraderScript@NScript@@QAEXVCScriptThing@@@Z
// Flags: none
// Inline-budget: The native timer assignment and comparison expand to the timer service calls at each use.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/NScript_CQR_EscortTraderScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
FABLE_FORCEINLINE bool CTimer::operator==(long time) const { return pDSTGame->GetTimer(TimerIndex)==time; }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
inline CScriptThing::~CScriptThing() {}
void NScript::CQR_EscortTraderScript::WatchForTraderHit(CScriptThing trader)
{
    TRetVoid _Fail_Val;
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        if (trader.IsAlive()) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            if (trader.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                trader.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game->GetHealth(trader) > DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    TraderHit = true;
                }
            }
            if (trader.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game->GetHealth(trader) > DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    CScriptThing Attacker = trader.MsgWhoHitMe();
                    if (Attacker.GetDefName() != Game->GetHero().GetDefName()) {
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        ++TraderHitCount;
                        if (TraderComplainTimer == 0) {
                            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                            long HLStyleConversationIndex = Game->AddNewConversation(trader, false, false);
                            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_053_COMPLAIN", false, trader, Game->GetHero());
                            TraderComplainTimer = pGlobalScriptDef->EscortTraderAttackComplaintTime;
                        }
                    }
                }
            }
        } else {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            return;
        }
    }
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
}