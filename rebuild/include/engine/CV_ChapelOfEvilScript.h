#ifndef FABLE_ENGINE_CV_CHAPELOFEVILSCRIPT_H
#define FABLE_ENGINE_CV_CHAPELOFEVILSCRIPT_H
// Retail opt-in (chunk 788): NScript::CV_ChapelOfEvilScript, new header (no includer before chunk 788).
// Layout: Ego_r.pdb CV_ChapelOfEvilScript (struct_layouts_egor.tsv, 164 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): DonationTotal +0x48 .. TortureSound +0x88, TortureType +0x8c,
// sizeof 0x90 (the landed Alloc 0x00E6E230 news 0x90 bytes and the ctor 0x00E68F70 builds the CCharString at +0x88).
// Checked against retail: Main 0x00E693F0 reads QuestActivated +0x6e, InsideChapel +0x59, ChapelIntroComplete +0x58,
// AttackedByHero +0x5a, writes WeaponWarningComplete +0x56 and MoralityInitial +0x60 (fstp).
// Retail methods: RegisterMain 0x00E69030 (slot 1), Main 0x00E693F0 (slot 2), Init 0x00E68FA0 (slot 3) of the script vtable
// 0x012E8080 that the "V_ChapelOfEvil" Alloc 0x00E6E230 stores; the threads Main spawns under their thread-name literals
// (Ego_r.pdb ?<Name>@CV_ChapelOfEvilScript@NScript@@QAEXXZ). The other members are the Ego_r.pdb name set.
// Nested entity scripts: the three classes Main binds (Ego_r.pdb ?_ECCOE_*@CV_ChapelOfEvilScript@NScript@@; the binding-name
// literal is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*, fable_types.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_ChapelOfEvilScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CScriptGameResourceObjectScriptedThingBase;
enum ChapelOfEvilTortureType { };                                        // Ego_r.pdb enum (enumerators not recovered)
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E69030 (slot 1)
    virtual void Main();                                                 // 0x00E693F0 (slot 2)
    virtual void Init();                                                 // 0x00E68FA0 (slot 3)
    void DoMission();                                                    // 0x00E6A7F0
    void EndMission();                                                   // 0x00E6A9E0
    void GetMethodOfTorture();                                           // 0x00E6DA40
    void GetTortureSoundEffect(ESex);                                    // 0x00E6DE10
    bool GetVictimIsBandit(CCharString);                                 // 0x00E6E120
    bool GetVictimIsGuard(CCharString);                                  // 0x00E6E050
    bool GetVictimIsTrader(CCharString);                                 // 0x00E6E1B0
    void TeleportAcolytes(const CScriptThing&);                          // 0x00E6B900
    void TortureVictim(CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, CScriptGameResourceObjectScriptedThingBase&); // 0x00E6DA80
    void WatchForAcolyteDeath(CScriptThing);
    void WatchForAcolyteHit(CScriptThing);                               // 0x00E6AF20
    void WatchForExpressions();                                          // 0x00E6A000
    void WatchForInsideChapel();                                         // 0x00E6A0B0
    void WatchForLeaving();                                              // 0x00E6A940
    void WatchForQuestInfoDisplayConditions();                           // 0x00E69F90
    void WatchForRegionEntry();                                          // 0x00E6B750
    void WatchForTermination();                                          // 0x00E69F00
    void WatchForWeaponUnsheathed();                                     // 0x00E6A340
    long DonationTotal;                                                  // +0x48
    long DonationThisVisit;                                              // +0x4c
    long DonationCounter;                                                // +0x50
    bool DonationInvalid;                                                // +0x54
    bool DisplayQuestInfo;                                               // +0x55
    bool WeaponWarningComplete;                                          // +0x56
    bool WeaponUnsheathed;                                               // +0x57
    bool ChapelIntroComplete;                                            // +0x58
    bool InsideChapel;                                                   // +0x59
    bool AttackedByHero;                                                 // +0x5a
    bool AcolyteAttackMessageComplete;                                   // +0x5b
    bool ReceivedReward;                                                 // +0x5c
    bool GivenCoolThing1;                                                // +0x5d
    bool GivenCoolThing2;                                                // +0x5e
    bool AwardWeapon;                                                    // +0x5f
    float MoralityInitial;                                               // +0x60
    float MoralityCurrent;                                               // +0x64
    float MoralityCumulative;                                            // +0x68
    bool QuestFinished;                                                  // +0x6c
    bool QuestComplete;                                                  // +0x6d
    bool QuestActivated;                                                 // +0x6e
    long TortureCount;                                                   // +0x70
    bool VictimTortured;                                                 // +0x74
    long VictimHealth;                                                   // +0x78
    ESex VictimGender;                                                   // +0x7c
    bool VictimNightTime;                                                // +0x80
    bool VictimSunday;                                                   // +0x81
    bool VictimIsGuard;                                                  // +0x82
    bool VictimIsBandit;                                                 // +0x83
    bool VictimIsTrader;                                                 // +0x84
    CCharString TortureSound;                                            // +0x88
    ChapelOfEvilTortureType TortureType;                                 // +0x8c
    class CCOE_AcolyteEvil;                                              // Alloc 0x00E6A500
    class CCOE_AltarEvil;                                                // Alloc 0x00E6A6E0
    class CCOE_Victim;                                                   // Alloc 0x00E6A600
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ChapelOfEvilScript) == 0x90);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_CHAPELOFEVILSCRIPT_H
