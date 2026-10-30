// CQ_MinionCampScript::OnMinionKilled() -- a minion in the camp has just died (reached through CMC_Minion::OnPredicateFail's thunk @ 0x00D9F1A0, `mov ecx,[ecx+14h] ; jmp`): drop the live count, never below zero, and if the camp is below its maximum and the spawn timer has already run out, wind the timer up again so the next minion rises; every step bails out if the script thread is being torn down
// Leaf: OnMinionKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9F1B0, length 75 bytes (the catalogue row 00d9f1b0 also held
// 0x00D9F200, split via the skipped-row ledger, chunk 908). The manifest label CMC_Minion::OnPredicateFail is the
// thunk's name propagated onto this body; `this` is the CQ_MinionCampScript the thunk fetches from ParentClass (+0x14).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (non-virtual, `this` in ecx), the game-script
// interface global @ 0x0143E8F8 slot 90 GetTimer(long), and StartMinionSpawnTimer @ 0x00D9F200 as a tail jump.
// Shape: `dec [esi+50h] ; jns` is the pre-decrement test; the first terminating check still has ecx == this, the
// later ones reload it from esi; the tail call pops esi before the `jmp`.
// Names: the PDB gives no method names for CQ_MinionCampScript; OnMinionKilled and StartMinionSpawnTimer are
// descriptive. Field names and the 0x14 shift come from the PDB layout (NumberOfMinions, MaxNumberOfMinions,
// Min/MaxMinionSpawnPeriod, MinionSpawnTimer).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CScriptBase {                                // the quest script base: vptr first; the thread-state query is a
public:                                            // non-virtual member @ 0x00CB7940 (`this` in ecx)
    void* __vftable;                               // +0x00
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {   // Ego_r.pdb CQ_MinionCampScript (208 bytes): the retail fields sit
public:                                            // 0x14 below the PDB offsets (NumberOfMinions is PDB +0x64, retail +0x50)
    unsigned char _pad04[0x4c];                    // +0x04 .. +0x4f: Game, Me, quest state flags
    long          NumberOfMinions;                 // +0x50 minions alive in the camp right now
    long          MaxNumberOfMinions;              // +0x54 how many the camp keeps summoning up to
    float         MinMinionSpawnPeriod;            // +0x58 shortest wait before the next minion rises
    float         MaxMinionSpawnPeriod;            // +0x5c longest wait
    unsigned char _pad60[0x10];                    // +0x60 .. +0x6f: ChantTimer, ChantTimerBar, ChantTimerPaused, PausedTime
    CTimer        MinionSpawnTimer;                // +0x70 the game-script timer that paces summoning
    void OnMinionKilled();                         // 0x00D9F1B0
    void StartMinionSpawnTimer();                  // 0x00D9F200
};
} // namespace NScript
using NScript::CQ_MinionCampScript;

void NScript::CQ_MinionCampScript::OnMinionKilled()
{
    if (--NumberOfMinions < 0) {
        if (IsActiveThreadTerminating())
            return;
        NumberOfMinions = 0;
    }
    if (NumberOfMinions < MaxNumberOfMinions) {
        if (g_pGameScriptInterface->GetTimer(MinionSpawnTimer.TimerIndex) <= 0) {
            if (IsActiveThreadTerminating())
                return;
            StartMinionSpawnTimer();
        }
    }
}