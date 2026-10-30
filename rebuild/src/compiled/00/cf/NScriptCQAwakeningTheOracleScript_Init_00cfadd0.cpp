// NScript::CQ_AwakeningTheOracleScript::Init() -- reset the oracle quest's flags and counters and register its two quest regions, NorthernWastes2 and DemonDoor_Necropolis
// Leaf: ?Init@CQ_AwakeningTheOracleScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFADD0, 209 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the level-script vtable 0x012C78B0 (0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist), stored by the quest ctor 0x00CFAD30; ?Init@CQ_AwakeningTheOracleScript@NScript@@ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00cfadd0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slot 271 AddQuestRegion (the quest name temporary is built second, so it is the first argument).
// Data: "NorthernWastes2" @ 0x012C78DC, "Q_AwakeningTheOracle" @ 0x012C472C, "DemonDoor_Necropolis" @ 0x012C78C4.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. MissionSucceeded +0x48 .. GotA +0x70 (Ego_r.pdb +0x5c .. +0x84).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48 (Ego_r.pdb +0x5c)
    bool PlayedOpeningCS;                               // +0x49
    bool PlayedLostBayCS;                               // +0x4a
    bool PlayedNecroCS;                                 // +0x4b
    long TabletsCollected;                              // +0x4c
    bool SpawnedFinalSpotsStuff;                        // +0x50
    long PositionTaken;                                 // +0x54
    bool GivenDiggingSpotsRewards;                      // +0x58
    long GraveNumbersGivenOut;                          // +0x5c
    long FinalSummonersAlive;                           // +0x60
    bool DisplayedBadTabletMessage;                     // +0x64
    bool PlayedOpeningNecroCS;                          // +0x65
    long TotalSpawnedCreaturesAlive;                    // +0x68
    bool AutosavedInNW3;                                // +0x6c
    bool GotY;                                          // +0x6d
    bool GotM;                                          // +0x6e
    bool GotC;                                          // +0x6f
    bool GotA;                                          // +0x70
    virtual void Init();                                // public in Ego_r.pdb (?Init@...@UAEXXZ)
};
}

void NScript::CQ_AwakeningTheOracleScript::Init()
{
    MissionSucceeded = false;
    PlayedOpeningCS = false;
    PlayedLostBayCS = false;
    PlayedNecroCS = false;
    GraveNumbersGivenOut = 0;
    GotA = false;
    GotC = false;
    GotM = false;
    GotY = false;
    TabletsCollected = 0;
    PositionTaken = 0;
    SpawnedFinalSpotsStuff = false;
    GivenDiggingSpotsRewards = false;
    FinalSummonersAlive = 0;
    DisplayedBadTabletMessage = false;
    PlayedOpeningNecroCS = false;
    TotalSpawnedCreaturesAlive = 0;
    AutosavedInNW3 = false;
    Game->AddQuestRegion("Q_AwakeningTheOracle", "NorthernWastes2");
    Game->AddQuestRegion("Q_AwakeningTheOracle", "DemonDoor_Necropolis");
}