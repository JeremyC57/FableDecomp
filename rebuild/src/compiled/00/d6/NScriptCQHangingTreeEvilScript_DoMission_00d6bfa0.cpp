// CQ_HangingTreeEvilScript::DoMission() -- start the mission-end and region watchers, wait for BowerstoneJail, then run the stage machine: stage 1 spawns the party at the spawn point and plays the introduction, then starts the mission timer and moves to stage 2, which waits for the HeroDualArea level (stage 6); finally play the conclusion and, unless failed, flag success
// Leaf: ?DoMission@CQ_HangingTreeEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6BFA0, length 1023 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012D1FE8), CScriptBase::AddSpawnedFunction
//   0x00CB7E50, CScriptBase::IsActiveThreadTerminating 0x00CB7940, WatchForMissionEnd 0x00D6C3A0, MonitorRegionPresence
//   0x00D6CDC0, SpawnParty 0x00D6C4A0, Introduction 0x00D6D080, Conclusion 0x00D6D670 (by retail order against the
//   Ego_r.pdb method order), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline ~CScriptThing); the timer service
//   g_pGameScriptInterface 0x0143E8F8 SetTimer (89); through the interface (+0x40): NewScriptFrame (7), IsLevelLoaded (11),
//   GetThingWithScriptName (72), DisplayQuestInfo (321), AddQuestInfoTimer (326).
// Data: pGlobalScriptDef 0x0143E90C HangingTreeEvilMissionTimer (+0xF58).
// Strings: "WatchForMissionEnd" 0x012D2328, "MonitorRegionPresence" 0x012D2310, "" 0x0122D70E, "BowerstoneJail" 0x012C7788,
//   "HTE_PartySpawnPoint" 0x012D2274, "HUD_CLOCK_ICON" 0x012CA7C8, "HeroDualArea" 0x012D2300.
// Layout (retail = Ego_r.pdb - 0x14): BanditsStarted +0x49, MissionSucceeded +0x4c, MissionFailed +0x4d, SpawnHere +0x4f,
//   SwitchingRegions +0x51, HTE_State +0x60, CurrentBandits +0x64, TimerGUIIndex +0x6c, MissionTimer +0xa4.
// Shape: spawn model of CQ_MinionCampScript::PhaseOne 0x00DA05D0; CScriptThing model of the landed
//   CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80 (inline destructor).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                  // out of line 0x004AA840
};
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CTimer;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
class CScriptDef { public:
    unsigned char _pad[0xf58];
    long HangingTreeEvilMissionTimer;                   // +0xF58
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool IntroCutscenePlayed;                           // +0x48
    bool BanditsStarted;                                // +0x49
    bool BanditKilled;                                  // +0x4a
    bool PartySuccessfullySetUp;                        // +0x4b
    bool MissionSucceeded;                              // +0x4c
    bool MissionFailed;                                 // +0x4d
    bool BanditsLeftRegion;                             // +0x4e
    bool SpawnHere;                                     // +0x4f
    bool AtWayPoint;                                    // +0x50
    bool SwitchingRegions;                              // +0x51
    unsigned char _pad52[0x5c - 0x52];
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    unsigned char _pad70[0xa4 - 0x70];
    CTimer MissionTimer;                                // +0xa4
    void DoMission();
    void WatchForMissionEnd();                          // 0x00D6C3A0
    void SpawnParty(long count, CScriptThing& spawnPoint);   // 0x00D6C4A0
    void MonitorRegionPresence();                       // 0x00D6CDC0
    void Introduction();                                // 0x00D6D080
    void Conclusion();                                  // 0x00D6D670
};
void CQ_HangingTreeEvilScript::DoMission()
{
    SpawnFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("WatchForMissionEnd", 0, this, &CQ_HangingTreeEvilScript::WatchForMissionEnd));
    SpawnFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorRegionPresence", 0, this, &CQ_HangingTreeEvilScript::MonitorRegionPresence));
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HTE_State = 1;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (HTE_State == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (SwitchingRegions) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (SpawnHere) {
                CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");
                SpawnParty(CurrentBandits, spawnPoint);
                Introduction();
            }
            while (!BanditsStarted) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            MissionTimer.SetTime(pGlobalScriptDef->HangingTreeEvilMissionTimer);
            TimerGUIIndex = Game->AddQuestInfoTimer(MissionTimer, "HUD_CLOCK_ICON", 1.0f);
            Game->DisplayQuestInfo(true);
            HTE_State = 2;
        }
        if (IsActiveThreadTerminating())
            return;
        while (HTE_State == 2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsLevelLoaded("HeroDualArea")) {
                if (IsActiveThreadTerminating())
                    return;
                HTE_State = 6;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    } while (HTE_State != 6);
    if (IsActiveThreadTerminating())
        return;
    Conclusion();
    if (MissionFailed)
        return;
    if (IsActiveThreadTerminating())
        return;
    MissionSucceeded = true;
}
}