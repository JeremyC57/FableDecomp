// CV_ArcheryCompetitionScript::WatchForOwnerKilled() -- once a contest is running, fetch the AC_Owner and wait for him to be alive; then, until the contest reaches state 2, watch his health: when he dies, stop the game timer, finish the quest, reset the contest, enable teleporting, sheathe the hero's weapons and make his will usable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E331F0, length 714 bytes, plain `ret`.
// Callees: CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of
//   line at the later returns), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0; the thing implementation's slot 75 IsAlive (+0x12c) behind the inlined null-safe
//   wrapper; SetTimer slot 89 through g_pGameScriptInterface 0x0143E8F8; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 72
//   GetThingWithScriptName (by value), 131 SetHeroWillAsUsable, 264 GetHealth, 506 EntitySheatheWeapons, 565
//   SetTeleportingAsActive. String "AC_Owner"; 0.0001f 0x0129BA3C.
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14 (retail CScriptBase): QuestFinished +0x51, GameTimeLeft (CTimer)
//   +0x6c, ContestState +0xa0. CScriptThing as the landed CV_TalentlessBardScript::WatchForAttack 0x00ED9790, with the
//   counted-pointer operator= of CQ_HangingTreeGoodScript::UpdateWayPoint 0x00D74E80.
// Shape: generated script code; the outer loop is `for (;;)` with the terminate return first (a `while` loop tail-merges
//   the first wait loop's inline-destructor exit into the loop exit); IsAlive is the `Data && Data->IsAlive()` form (the
//   if/return form tests the inlined result with `cmp al,bl`); a function-scope CScriptThing reassigned from the by-value lookup; ~CScriptThing is
//   __forceinline with `#pragma inline_depth(0)` around the returns that call the out-of-line copy.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;                   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ArcheryCompetitionScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x51 - 0x48];
    bool          QuestFinished;                            // +0x51
    unsigned char _pad52[0x6c - 0x52];
    CTimer        GameTimeLeft;                             // +0x6c
    unsigned char _pad70[0xa0 - 0x70];
    long          ContestState;                             // +0xa0
    void WatchForOwnerKilled();
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::WatchForOwnerKilled()
{
    CScriptThing owner;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (ContestState != 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        owner = Game->GetThingWithScriptName("AC_Owner");
        while (!owner.IsAlive()) {
            Game->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (ContestState == 2) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                break;
            }
            if (Game->GetHealth(owner) < 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                GameTimeLeft = 0;
                ContestState = 0;
                QuestFinished = true;
                Game->SetTeleportingAsActive(true);
                Game->EntitySheatheWeapons(Game->GetHero(), false);
                Game->SetHeroWillAsUsable(true);
                break;
            }
            Game->NewScriptFrame();
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}