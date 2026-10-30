// CQ_MinionCampScript::StartMinionSpawnTimer() -- set the camp's minion spawn timer to a random wait between the min and max spawn periods (whole seconds), so the next summoning comes at an unpredictable moment
// Leaf: StartMinionSpawnTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9F200, length 69 bytes (second piece of catalogue row 00d9f1b0,
// split via the skipped-row ledger, chunk 908). Called only as the tail jump of OnMinionKilled @ 0x00D9F1B0.
// Callees: rand @ 0x00BFEB16, __ftol2 @ 0x00BFEA70 (the `(long)` cast), the game-script interface global
// @ 0x0143E8F8 slot 89 SetTimer(long timer, long time).
// Shape: rand() is called first (its result is stored to the one frame word and `fild`ed), the interface and its
// vtable are loaded into edi/ebx across the x87 work, `(max - min) * r + min` is `fld max ; fsub min ; fmul st(1) ;
// fadd min`, the constant 0x0129C81C is 1.0f / RAND_MAX (3.0518509e-05) folded from the source, and the random
// factor stays on the FPU stack until the `fstp st(0)` after the cast.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped). Same bytes.
#include <stdlib.h>
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

void NScript::CQ_MinionCampScript::StartMinionSpawnTimer()
{
    float r = rand() * (1.0f / RAND_MAX);                 // 0..1, kept on the FPU stack until the call is made
    g_pGameScriptInterface->SetTimer(MinionSpawnTimer.TimerIndex,
        (long)((MaxMinionSpawnPeriod - MinMinionSpawnPeriod) * r + MinMinionSpawnPeriod));
}