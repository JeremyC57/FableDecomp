// NScript::CQ_AwakeningTheOracleScript::OnPersist(CPersistContext& context) -- persist the oracle quest's cutscene flags (PlayedLostBayCS, PlayedOpeningCS, PlayedNecroCS, default false), tablet count and grave numbers (default 0), the digging-spot and final-spot flags, the bad-tablet message, the opening necro cutscene and the NW3 autosave
// Leaf: ?OnPersist@CQ_AwakeningTheOracleScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFAF70, 269 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the level-script vtable 0x012C78B0 (0 dtor 0x00D005C0, 1 RegisterMain
//   0x00CFAEB0, 2 Main 0x00CFB080, 3 Init 0x00CFADD0 (landed), 4 OnPersist), stored by the quest ctor 0x00CFAD30 (landed);
//   the row is referenced only from that slot; ?OnPersist@CQ_AwakeningTheOracleScript@NScript@@UAEXAAVCPersistContext@@@Z
//   is in Ego_r.pdb and FableWin.pdb; the ten keys are the member names.
// Label: the manifest's "sub_00cfaf70" is a gap-scan placeholder.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0 (Ego_r.pdb ??$Transfer@_N@ / ??$Transfer@J@
//   CPersistContext@@, the spellings the landed CQ_ArenaScript::OnPersist 0x00CFAB10 binds); keys 0x012C79CC .. 0x012C7908.
// Shape: ten Transfer calls in retail order (PlayedLostBayCS first, not member order), each default a `false` / `0L`
//   temporary in the one frame slot (the zero kept in ebx). MissionSucceeded, PositionTaken, FinalSummonersAlive,
//   TotalSpawnedCreaturesAlive and the GotY/M/C/A letters are not persisted; no base OnPersist call.
// Layout: engine/CLevelScriptBase.h (retail CScriptBase 0x40 bytes, Game +0x40, master data +0x44); the quest's
//   members 0x14 below their Ego_r.pdb offsets: PlayedOpeningCS +0x49 .. AutosavedInNW3 +0x6c.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <bool> 0x004045C0, <long> 0x00410BE0
};
// CScriptBase / CLevelScriptBase<T> are the shared retail NScript models (FABLE_RETAIL_NSCRIPT opt-in: CScriptBase 0x40,
//   vtable 0x012C1648, OnPersist slot 4; CLevelScriptBase Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
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
    virtual void OnPersist(CPersistContext& context);
};
}

void NScript::CQ_AwakeningTheOracleScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PlayedLostBayCS", PlayedLostBayCS, false);
    context.Transfer("PlayedOpeningCS", PlayedOpeningCS, false);
    context.Transfer("PlayedNecroCS", PlayedNecroCS, false);
    context.Transfer("TabletsCollected", TabletsCollected, 0L);
    context.Transfer("SpawnedFinalSpotsStuff", SpawnedFinalSpotsStuff, false);
    context.Transfer("GivenDiggingSpotsRewards", GivenDiggingSpotsRewards, false);
    context.Transfer("GraveNumbersGivenOut", GraveNumbersGivenOut, 0L);
    context.Transfer("DisplayedBadTabletMessage", DisplayedBadTabletMessage, false);
    context.Transfer("PlayedOpeningNecroCS", PlayedOpeningNecroCS, false);
    context.Transfer("AutosavedInNW3", AutosavedInNW3, false);
}