// NScript::CQ_MinionCampScript::RunChantTimer() -- run Briar Rose's 72-second chant timer: while time is left (or the timer is paused) advance a frame, hold the timer at PausedTime while ChantTimerPaused (and once more when the pause ends), and show the elapsed time on the chant timer bar
// Leaf: ?RunChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA24F0, 276 bytes, thiscall; the trailing termination test is a tail `jmp`.
// Identity (proven): the row is referenced once, from 0x00DA1AD1 inside the landed CQ_MinionCampScript code, as the
//   CSpawnedFunc member pointer (+0x34) of a spawned thread (vtable 0x012D6CAC). Ego_r.pdb
//   ?RunChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ (0x00DC95D0) follows ?PauseChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ
//   (0x00DC9570, 0x60 bytes) exactly as retail follows the landed PauseChantTimer 0x00DA2490 (0x60 bytes); PauseChantTimer
//   records PausedTime and raises ChantTimerPaused, the two members this row consumes. Catalog label sub_00da24f0.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (Ego_r); the global script interface 0x0143E8F8 slots 89 SetTimer /
//   90 GetTimer (the inline CTimer operator= / operator>= / GetTime of Ego_r ??4CTimer@@QAEAAV0@J@Z, ??PCTimer@@QBE_NJ@Z,
//   ?GetTime@CTimer@@QBEJXZ); Game slots 7 NewScriptFrame and 332 UpdateQuestInfoBar.
// Layout: the landed PauseChantTimer 0x00DA2490 (retail = Ego_r - 0x14): ChantTimer +0x60, ChantTimerBar +0x64,
//   ChantTimerPaused +0x68, PausedTime +0x6c.
// Shape: the script WAIT/IF macro forms -- every arm opens with a termination test; `bool wasPaused` lives in bl; the
//   `||` loop test reads the timer first; -1.0f immediates for the two unused bar arguments. The arm order is
//   `if (!ChantTimerPaused) {..} else {..}` (retail lays the not-paused arm first; the positive test came out swapped, 272v276).
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface; no PDB data symbol)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    bool operator>=(long time) const { return GetTime() >= time; }
};
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    CTimer ChantTimer;                                  // +0x60
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    void RunChantTimer();
};

void CQ_MinionCampScript::RunChantTimer()
{
    ChantTimer = 72;
    bool wasPaused = false;
    while (ChantTimer >= 0 || ChantTimerPaused) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!ChantTimerPaused) {
            if (IsActiveThreadTerminating())
                return;
            if (wasPaused) {
                if (IsActiveThreadTerminating())
                    return;
                ChantTimer = PausedTime;
                wasPaused = false;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            ChantTimer = PausedTime;
            wasPaused = true;
        }
        Game->UpdateQuestInfoBar(ChantTimerBar, (float)(72 - ChantTimer.GetTime()), -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript