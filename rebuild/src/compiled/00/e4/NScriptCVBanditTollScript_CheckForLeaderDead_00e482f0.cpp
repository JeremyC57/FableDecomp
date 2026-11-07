// NScript::CV_BanditTollScript::CheckForLeaderDead() -- bandit-toll leader watcher: waits for LeaderDead, completes the quest, and if the taunting bandit (the leader) is still standing has him shout the on-kill line to the hero, sets the bandits fleeing (faction unset) and waits until the taunting, toll and blocking bandits are all gone; then rewrites the three toll signs with their "gone" text tags and deactivates the quest
// Grade: RELOCATION_MATCH (chunk 844, after chunk 839's 11 attempts). Inline-split note: CScriptThing::operator= is a plain `inline` body (expanded by retail at the first loop re-assignment `TauntingBandit = GetNearestWithScriptName(...)` only, called 0x008AB980 at the TollBandit / BlockingBandit re-assignments); the single-site `do { ... } while (0);` around that first re-assignment is the lever -- VC7.1's inliner takes loop-nested call sites first, so the one-trip loop makes the budget-limited plain-inline operator= expand there and stay a call at the two later sites (plain inline without it expands none, `__forceinline` all three). Behaviour-identical (the loop runs once); review at landing.
// Same-type `reinterpret_cast<const CScriptThing&>` on the three inner wait loops (the landed CHTE_FreedomFighter::Main 0x00D6A560 form, owner-accepted 2026-11-07) keeps IsAlive virtual there as retail; IsAlive inline gives retail's 2 expanded + 4 called mix.
// Shape: the three IsAlive wait loops test the bandit locals through a same-type `reinterpret_cast<const CScriptThing&>` (the landed
//   CMapKeeper::Main 0x00DA91A0 / CHTE_FreedomFighter::Main 0x00D6A560 form): retail calls IsAlive through the vptr there, while the
//   loop-head tests on the value locals are devirtualized (the first two expanded inline, the rest `call 0x004AB130`).
// Leaf: ?CheckForLeaderDead@CV_BanditTollScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E482F0, 1627 bytes, thiscall, plain `ret`, no EH frame; frame 0x44, esi at entry, ebx/edi pushed
//   lazily after the LeaderDead wait.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?CheckForLeaderDead@CV_BanditTollScript@NScript@@QAEXXZ (engine/CV_BanditTollScript.h
//   declares it); the body reads LeaderDead +0x4e and writes BanditsFlee +0x4f / IsFactionSet +0x51 of the PDB CV_BanditTollScript
//   layout (retail = PDB - 0x14) and waits on the script names "TauntingBandit" / "TollBandit" / "BlockingBandit" that the landed
//   CM_TauntingBandit::Main 0x00E4B430, CM_TollBandit::Main 0x00E48A30 and CBlockingBandit::Main 0x00E49980 entity scripts register.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   ~CScriptThing 0x004AA840, CScriptThing::IsAlive 0x004AB130 (slot 75, +0x12c; expanded at the first two value sites), CScriptThing::
//   operator= 0x008AB980 (expanded at the first assignment, called at the other two), free 0x00BFE9BC (inside the expanded
//   CCountedPointer release); Game (+0x40) slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 77 GetNearestWithScriptName,
//   281 DeactivateQuestLater, 288 SetQuestAsCompleted, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation,
//   655 GetActiveQuestName, 714 SetReadableObjectTextTag.
// Literals: "TauntingBandit" 0x012E4C60, "TEXT_QST_011_BANDIT_ON_KILL_LEADER_10" 0x012E4D44, "TollBandit" 0x012E4C84, "BlockingBandit"
//   0x012E4C50, "TEXT_QST_011_SIGN_FAR_GONE" 0x012E4D28, "BT_Sign_Far" 0x012E4D1C, "TEXT_QST_011_SIGN_CLOSE_GONE" 0x012E4CFC,
//   "BT_Sign_Close" 0x012E4CEC, "TEXT_QST_011_SIGN_CLOSER_GONE" 0x012E4CCC, "BT_Sign_Closer" 0x012E4CBC.
// Layout (engine/CV_BanditTollScript.h, Ego_r.pdb - 0x14): Game +0x40, LeaderDead +0x4e, BanditsFlee +0x4f, IsFactionSet +0x51.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

void NScript::CV_BanditTollScript::CheckForLeaderDead()
{
    while (!LeaderDead) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    CScriptThing Leader = Game->GetNearestWithScriptName(Game->GetHero(), "TauntingBandit");
    if (Leader.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Leader, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_ON_KILL_LEADER_10", false, Leader, Game->GetHero());
        BanditsFlee = true;
        IsFactionSet = false;
        CScriptThing TauntingBandit = Game->GetNearestWithScriptName(Game->GetHero(), "TauntingBandit");
        while (TauntingBandit.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (reinterpret_cast<const CScriptThing&>(TauntingBandit).IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            do {
                TauntingBandit = Game->GetNearestWithScriptName(Game->GetHero(), "TauntingBandit");
            } while (0);
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing TollBandit = Game->GetNearestWithScriptName(Game->GetHero(), "TollBandit");
        while (TollBandit.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (reinterpret_cast<const CScriptThing&>(TollBandit).IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            TollBandit = Game->GetNearestWithScriptName(Game->GetHero(), "TollBandit");
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing BlockingBandit = Game->GetNearestWithScriptName(Game->GetHero(), "BlockingBandit");
        while (BlockingBandit.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (reinterpret_cast<const CScriptThing&>(BlockingBandit).IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            BlockingBandit = Game->GetNearestWithScriptName(Game->GetHero(), "BlockingBandit");
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Far"), "TEXT_QST_011_SIGN_FAR_GONE");
    Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Close"), "TEXT_QST_011_SIGN_CLOSE_GONE");
    Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BT_Sign_Closer"), "TEXT_QST_011_SIGN_CLOSER_GONE");
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}