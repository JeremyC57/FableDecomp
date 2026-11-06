#ifndef FABLE_ENGINE_CQ_AWAKENINGTHEORACLESCRIPT_H
#define FABLE_ENGINE_CQ_AWAKENINGTHEORACLESCRIPT_H
// Retail opt-in (chunk 787): NScript::CQ_AwakeningTheOracleScript, new header (no includer before chunk 787).
// Layout: Ego_r.pdb CQ_AwakeningTheOracleScript (struct_layouts_egor.tsv, 184 bytes) at retail offsets = PDB - 0x14 up to GotA (retail
// CLevelScriptBase is 0x48), then 12-byte STLport vector / maps (PDB 16): DiggingPoints +0x74, cs_args +0x80, cs_chars +0x8c, sizeof 0x98 --
// the offsets the landed ctor 0x00CFAD30 builds and Init 0x00CFADD0 writes (MissionSucceeded +0x48 .. GotA +0x70).
// Methods (Ego_r.pdb): Init 0x00CFADD0, Main 0x00CFB080 (slot 2), WatchForAliveTablets 0x00CFF110 (?WatchForAliveTablets@...@QAEXXZ, spawned by
// Main under its thread-name literal), PlayCutscene(CCharString, bool) 0x00CFF9A0 (?PlayCutscene@...@QAEXVCCharString@@_N@Z, landed).
// Nested entity scripts: the eleven classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_AwakeningTheOracleScript@NScript@@SI...; the binding
// name is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_AwakeningTheOracleScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();                                                 // 0x00CFB080
    virtual void Init();                                                 // 0x00CFADD0
    void WatchForAliveTablets();                                         // 0x00CFF110
    void PlayCutscene(CCharString, bool);                                // 0x00CFF9A0
    bool MissionSucceeded;                                               // +0x48
    bool PlayedOpeningCS;                                                // +0x49
    bool PlayedLostBayCS;                                                // +0x4a
    bool PlayedNecroCS;                                                  // +0x4b
    long TabletsCollected;                                               // +0x4c
    bool SpawnedFinalSpotsStuff;                                         // +0x50
    long PositionTaken;                                                  // +0x54
    bool GivenDiggingSpotsRewards;                                       // +0x58
    long GraveNumbersGivenOut;                                           // +0x5c
    long FinalSummonersAlive;                                            // +0x60
    bool DisplayedBadTabletMessage;                                      // +0x64
    bool PlayedOpeningNecroCS;                                           // +0x65
    long TotalSpawnedCreaturesAlive;                                     // +0x68
    bool AutosavedInNW3;                                                 // +0x6c
    bool GotY;                                                           // +0x6d
    bool GotM;                                                           // +0x6e
    bool GotC;                                                           // +0x6f
    bool GotA;                                                           // +0x70
    std::vector<CScriptThing> DiggingPoints;                             // +0x74
    std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CCharString> > > cs_args;   // +0x80
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CScriptGameResourceObjectScriptedThingBase> > > cs_chars;   // +0x8c
    class COracleTablet;                                                 // Alloc 0x00CFBB20
    class CFinalTabletSarcophagus;                                       // Alloc 0x00CFBC00
    class CNecropolisGhost;                                              // Alloc 0x00CFBCE0
    class CNecropolisGate;                                               // Alloc 0x00CFC700
    class CLostBayCutsceneTrigger;                                       // Alloc 0x00CFC7D0
    class CChunkCollectionIndicator;                                     // Alloc 0x00CFC8A0
    class CNecropolisEnemy;                                              // Alloc 0x00CFCA60
    class CScytheBeforeGettingTablets;                                   // Alloc 0x00CFCDC0
    class CATO_MagicBarrier;                                             // Alloc 0x00CFD5D0
    class CFinalSummoner;                                                // Alloc 0x00CFDCA0
    class CPretendGuard;                                                 // Alloc 0x00CFCBD0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AwakeningTheOracleScript) == 0x98);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_AWAKENINGTHEORACLESCRIPT_H
