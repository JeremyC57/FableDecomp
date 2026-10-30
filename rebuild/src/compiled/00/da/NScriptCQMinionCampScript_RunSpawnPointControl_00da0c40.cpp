// CQ_MinionCampScript::RunSpawnPointControl() -- script thread for mission stage 3: each frame, while there are fewer minions than the maximum and the spawn timer has run out, pick the spawn point (a random one on the side holding the furthest spawn point), restart the spawn timer with a random period between the minimum and maximum (unless this is the last minion) and flag a spawn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0C40, length 322 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?RunSpawnPointControl@CQ_MinionCampScript@NScript@@QAEXXZ.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, FindFurthestSpawnPoint(const list<CScriptThing>&,
// const CScriptThing*&, float&) 0x00DA0D90 (manifest label "CScriptBase_RunActiveSpawnPoints" is propagated),
// GetRandomSpawnPoint(const list<CScriptThing>&, CScriptThing&) 0x00DA0E40, rand 0x00BFEB16, _ftol 0x00BFEA70; the
// CTimer helpers through the global interface 0x0143E8F8 (GetTimer slot 90, SetTimer slot 89); the quest's interface
// (+0x40) NewScriptFrame (slot 7).
// Data: 1/32767 0x0129C81C.
// Layout: retail = Ego_r.pdb - 0x14 (STLport lists are one node pointer): NumberOfMinions +0x50, MaxNumberOfMinions
// +0x54, MinMinionSpawnPeriod +0x58, MaxMinionSpawnPeriod +0x5C, MinionSpawnTimer +0x70, CurrentStage +0x74,
// LeftSpawnPoints +0x90, RightSpawnPoints +0x94, ShouldSpawn +0x98, SpawnPointToSpawn +0x9C.
// Shape: `while (CurrentStage == 3)` rotated; both FindFurthestSpawnPoint calls share the zeroed out-pointer and the
// -1 distance; the two GetRandomSpawnPoint arms tail-merge; the random period keeps the scaled rand() on the x87
// stack through the interpolation (`fmul st(1)` ... `fstp st(0)`); the right-side test is written `if (!Find..)` so
// the left arm falls through; CScriptThing is modelled 4-aligned (a char[12] body shifts SpawnPointToSpawn to +0x99).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CScriptThing { public: void* _body[3]; };                // 12 bytes, 4-aligned
template <class T> struct list { void* _M_node; };      // STLport list (retail 4 bytes)
extern "C" int __cdecl rand();                          // 0x00BFEB16

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long seconds) { g_pGameScriptInterface->SetTimer(TimerIndex, seconds); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x50 - 0x48];
    long                  NumberOfMinions;              // +0x50 (PDB +0x64)
    long                  MaxNumberOfMinions;           // +0x54
    float                 MinMinionSpawnPeriod;         // +0x58
    float                 MaxMinionSpawnPeriod;         // +0x5c
    unsigned char         _pad60[0x70 - 0x60];
    CTimer                MinionSpawnTimer;             // +0x70
    long                  CurrentStage;                 // +0x74
    unsigned char         _pad78[0x90 - 0x78];
    list<CScriptThing>    LeftSpawnPoints;              // +0x90
    list<CScriptThing>    RightSpawnPoints;             // +0x94
    bool                  ShouldSpawn;                  // +0x98
    CScriptThing          SpawnPointToSpawn;            // +0x9c
    bool FindFurthestSpawnPoint(const list<CScriptThing>& points, const CScriptThing*& furthest, float& distance);   // 0x00DA0D90
    void GetRandomSpawnPoint(const list<CScriptThing>& points, CScriptThing& point);                                  // 0x00DA0E40
    void RunSpawnPointControl();
};
void CQ_MinionCampScript::RunSpawnPointControl()
{
    while (CurrentStage == 3) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (NumberOfMinions < MaxNumberOfMinions && MinionSpawnTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            const CScriptThing* furthest = 0;
            float distance = -1.0f;
            FindFurthestSpawnPoint(LeftSpawnPoints, furthest, distance);
            if (!FindFurthestSpawnPoint(RightSpawnPoints, furthest, distance)) {
                if (IsActiveThreadTerminating())
                    return;
                GetRandomSpawnPoint(LeftSpawnPoints, SpawnPointToSpawn);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                GetRandomSpawnPoint(RightSpawnPoints, SpawnPointToSpawn);
            }
            if (NumberOfMinions < MaxNumberOfMinions - 1) {
                if (IsActiveThreadTerminating())
                    return;
                float t = rand() * (1.0f / 32767.0f);
                MinionSpawnTimer = (long)(MinMinionSpawnPeriod + (MaxMinionSpawnPeriod - MinMinionSpawnPeriod) * t);
            }
            ShouldSpawn = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}