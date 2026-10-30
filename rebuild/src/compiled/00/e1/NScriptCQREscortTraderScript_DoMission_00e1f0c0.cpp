// NScript::CQR_EscortTraderScript::DoMission() -- the escort-trader mission thread: unless the trader already exists, wait for the LookoutPoint region, take one script frame, then create CREATURE_TRADER_01 as "TraderToEscort" at the M_ET_TraderSpawnPoint marker and remember it; spawn the WatchForFailure thread, run EscortTrader and take a final script frame
// Leaf: ?DoMission@CQR_EscortTraderScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1F0C0, 525 bytes, thiscall, plain `ret`.
// Identity (proven): the landed NScript::CQR_EscortTraderScript::Main 0x00E1E110 spawns this body as the "DoMission" thread
//   (CSpawnedFunc<CQR_EscortTraderScript>::Func store at 0x00E1E399); Ego_r.pdb / FableWin.pdb name set
//   ?DoMission@CQR_EscortTraderScript@NScript@@QAEXXZ. The body itself spawns ?WatchForFailure@CQR_EscortTraderScript@NScript@@QAEXXZ
//   0x00E1F2D0 (landed) paired with the "WatchForFailure" literal, and calls EscortTrader 0x00E1F3A0 (landed). Catalog `sub_00e1f0c0`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, operator new 0x00BFEA1A,
//   CSpawnedFuncBase ctor 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50 / IsActiveThreadTerminating 0x00CB7940;
//   vtable CSpawnedFunc<CQR_EscortTraderScript> 0x012E12F4; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded,
//   72 GetThingWithScriptName, 91 CreateCreature; CScriptThing::GetPos inlined (imp slot 6, g_ZeroVector 0x0143E8E0).
// Literals: "LookoutPoint" 0x012C3394 (twice), "M_ET_TraderSpawnPoint" 0x012E15B8, "TraderToEscort" 0x012E14E4,
//   "CREATURE_TRADER_01" 0x012E15A4, "WatchForFailure" 0x012E1594, "" 0x0122D70E.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT); CQR_EscortTraderScript : CLevelScriptBase<CQ_SunnyvaleMasterData>
//   (Ego_r.pdb: first member at 0x5C, sizeof CLevelScriptBase<CQ_SunnyvaleMasterData> 0x5C; its entity scripts' PMasterData is
//   CQ_SunnyvaleMasterData*). Members at Ego_r.pdb offset - 0x14: TraderCreated +0x5E (Ego_r 0x72).
// Model: the landed CV_TalentlessBardScript::Main 0x00ED7830 (CSpawnedFunc / SpawnFunction, retail CSpawnedFuncBase through the
//   FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in) and CQGuildTrainingWoodsMeleeScript::CScorpionHome::Main 0x00D67270 (inline GetPos).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class C3DVector {
public:
    float X, Y, Z;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
__forceinline const C3DVector& CScriptThing::GetPos() const    // slot 6 0x004AA980, inlined here
{
    if (!PImp.Data)
        return g_ZeroVector;
    return PImp.Data->GetPos();
}

#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool HiredByTrader;                                 // +0x4a
    bool TraderAtDestination;                           // +0x4b
    bool TraderDepartedLost;                            // +0x4c
    bool GreetedHero;                                   // +0x4d
    bool TraderKilled;                                  // +0x4e
    bool TraderHit;                                     // +0x4f
    bool TraderFollowing;                               // +0x50
    bool TraderDeparted;                                // +0x51
    long BanditSpawnCount;                              // +0x54
    long BanditKillCount;                               // +0x58
    bool TraderIntroDone;                               // +0x5c
    bool DoneExpressions;                               // +0x5d
    bool TraderCreated;                                 // +0x5e
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
    void DoMission();
    void WatchForFailure();                             // 0x00E1F2D0
    void EscortTrader();                                // 0x00E1F3A0
};

void CQR_EscortTraderScript::DoMission()
{
    if (!TraderCreated) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("LookoutPoint")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        CScriptThing spawnPoint = Game->GetThingWithScriptName("M_ET_TraderSpawnPoint");
        Game->CreateCreature("CREATURE_TRADER_01", spawnPoint.GetPos(), "TraderToEscort", false);
        TraderCreated = true;
    }
    SpawnFunction(new CSpawnedFunc<CQR_EscortTraderScript>("WatchForFailure", 0, this, &CQR_EscortTraderScript::WatchForFailure));
    EscortTrader();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript