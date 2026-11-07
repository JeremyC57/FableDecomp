#ifndef FABLE_ENGINE_CQ_TENTACLEKRAKENBOSSFIGHTSCRIPT_H
#define FABLE_ENGINE_CQ_TENTACLEKRAKENBOSSFIGHTSCRIPT_H
// Retail opt-in (chunk 853): NScript::CQ_TentacleKrakenBossFightScript, new header (no includer before chunk 853; the landed rows of this
// quest model the class locally).
// Layout: Ego_r.pdb CQ_TentacleKrakenBossFightScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets PDB - 0x14 (retail CLevelScriptBase
// is 0x48; no STLport container in the class): MissionSucceeded +0x48, MissionFailed +0x49, KrakenAutoSaveCheckPoint +0x4a, TheKraken
// (CScriptThing) +0x4c; 0x58 bytes. Checked against retail: the landed DoMission 0x00DF4E00 reads +0x48..+0x4a, the landed ??_G 0x00DF5C60
// destroys the CScriptThing at +0x4c, PhaseOne 0x00DF5000 assigns +0x4c (PImp +0x50 / +0x54) and writes MasterScriptData->MotherDodgingKraken.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>.
// Methods (Ego_r.pdb ?<Name>@CQ_TentacleKrakenBossFightScript@NScript@@): Init 0x00DF4130, RegisterMain 0x00DF4150, OnPersist 0x00DF4210,
// Main 0x00DF4240 (landed rows), DoMission 0x00DF4E00, WatchForLeaving 0x00DF4F80, PhaseOne 0x00DF5000 (QAEXXZ).
// Nested entity scripts (landed Alloc rows): CKrakenMinion 0x00DF4520, CTheKraken 0x00DF4730, CM_Experience 0x00DF4820, forward-declared only.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_TentacleKrakenBossFightScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TentacleKrakenBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00DF4150
    virtual void Main();                                                 // 0x00DF4240
    virtual void Init();                                                 // 0x00DF4130
    void DoMission();                                                    // 0x00DF4E00
    void WatchForLeaving();                                              // 0x00DF4F80
    void PhaseOne();                                                     // 0x00DF5000
    bool MissionSucceeded;                                               // +0x48
    bool MissionFailed;                                                  // +0x49
    bool KrakenAutoSaveCheckPoint;                                       // +0x4a
    CScriptThing TheKraken;                                              // +0x4c
    class CKrakenMinion;                                                 // Alloc 0x00DF4520
    class CTheKraken;                                                    // Alloc 0x00DF4730
    class CM_Experience;                                                 // Alloc 0x00DF4820
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TentacleKrakenBossFightScript) == 0x58);
#endif
#endif
