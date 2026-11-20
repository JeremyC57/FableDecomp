// NScript::CQ_TraderEscortScript::CDarkwoodTrader::SetBrainState(long state) -- remember the trader's brain state; on state 2
//   (following) make the trader follow the hero (and the hero's region), show the quest info and, by the trader's data string, add
//   its health bar (INFECTED: plain trader icon and start the incubation timer, the low infection time when the parent's
//   SavedNearEnd is set; SCARED: hat 01 icon and set scared; FRIENDLY: hat 02 icon)
// Leaf: ?SetBrainState@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@AAEXJ@Z
// Flags: /d2inlS22
// Inline-budget: /d2inlS22 -- the plain-inline CTimer::operator= below (the header only declares it) is expanded at every site, as retail does; without the flag it is called (DIFFER 759v778), S21 differs, S22 is the smallest that matches (landing nf194 sweep; replaces the agent's FABLE_FORCEINLINE, EXTRA_RULES inline preference 2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0A510, length 784 bytes, `ret 4`.
// Callees: IsActiveThreadTerminating 0x00F35B30 (CEntityScriptBase), the game script interface (Game +4) slots 70/229/321/325/480/496,
//   CScriptThing slot 3 GetDataString, CCharString 0x0099EBF0 / ~ 0x0099EAE0, Compare 0x00411570 (__fastcall), pDSTGame
//   0x0143E8F8 slot 89 SetTimer (CTimer::operator=, expanded), pGlobalScriptDef 0x0143E90C (TE_TraderToBalverineDelay +0xdf4 /
//   TE_TraderLowInfectionTime +0xdf8).
// Layout: engine/CQ_TraderEscortScript.h CDarkwoodTrader (Ego_r.pdb): ParentClass +0x14, Infected +0x1c, BrainState +0x20,
//   IncubationTime +0x2c, TraderHealthID +0x30; the parent's SavedNearEnd +0x5a.
// Identity (Naming NF194, 2026-11-20, q6529): Ego_r.pdb + FableWin.pdb ?SetBrainState@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@AAEXJ@Z
//   (the long argument stored at +0x20 = BrainState); CDarkwoodTrader::Main 0x00E07640 calls it and binds that spelling. Was the
//   propagated label CTraderScript::HandleTrader on local CTraderScript / interface / CEntityScriptBase models.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_SETBRAINSTATE
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }   // body as the same-TU CDarkwoodTrader::Main 0x00E07640

void NScript::CQ_TraderEscortScript::CDarkwoodTrader::SetBrainState(long state)
{
    BrainState = state;
    if (state != 2)
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->DisplayQuestInfo(true);
    if (Me.GetDataString() == "INFECTED") {
        if (IsActiveThreadTerminating())
            return;
        TraderHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER", 1.0f);
        Infected = 1;
        if (ParentClass.SavedNearEnd) {
            if (IsActiveThreadTerminating())
                return;
            IncubationTime = pGlobalScriptDef->TE_TraderLowInfectionTime;
        } else {
            if (IsActiveThreadTerminating())
                return;
            IncubationTime = pGlobalScriptDef->TE_TraderToBalverineDelay;
        }
    } else if (Me.GetDataString() == "SCARED") {
        if (IsActiveThreadTerminating())
            return;
        TraderHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_01", 1.0f);
        Game->EntitySetAsScared(Me, true);
    } else if (Me.GetDataString() == "FRIENDLY") {
        if (IsActiveThreadTerminating())
            return;
        TraderHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_02", 1.0f);
    }
}