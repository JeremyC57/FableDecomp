#ifndef FABLE_ENGINE_CQ_NEWOAKVALEINTROSCRIPT_H
#define FABLE_ENGINE_CQ_NEWOAKVALEINTROSCRIPT_H
// Retail opt-in (chunk 797): NScript::CQ_NewOakValeIntroScript, new header (no includer before chunk 797).
// Layout: Ego_r.pdb CQ_NewOakValeIntroScript (struct_layouts_egor.tsv, 320 bytes) at retail offsets PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after each of the eight STLport vectors (12 bytes; Ego_r's are 16): GoodDeedsPerformed
// +0x54, BadDeedsPerformed +0x58, lastVillagerSpeechIdx +0x98, VillagerSpeech* +0x9c..+0xfb, WhichBadDeedsPerformed +0xfc,
// TalkIntermittentTimer +0x104, WatchTimer +0x108; 0x10c bytes (the landed ctor 0x00DAAC00, AddBadDeed 0x00DAEA70,
// GetVillagerSpeechIndex 0x00DAEBF0 and CNOVI_Villager::Main 0x00DADF80 use these offsets; the factory 0x00DBEF70 allocates 0x10C).
// The PDB places the two C3DVectors at the unaligned offsets 138 / 153 (retail +0x76 / +0x85) while the longs stay 4-aligned:
// the class is modelled byte-packed with the PDB's alignment gaps written out (_pad49, _pad97, _pad101).
// Methods: Ego_r.pdb ?<Name>@CQ_NewOakValeIntroScript@NScript@@; AddBadDeed takes the nested enum EBadDeeds (its enumerator names
// are not in the PDB set: callers cast the value). Nested entity scripts forward-declared, CNOVI_Villager modelled.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_NewOakValeIntroScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CPersistContext;
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
#pragma pack(push, 1)
class CQ_NewOakValeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EBadDeeds { };
    CQ_NewOakValeIntroScript(CScriptDataBase*, CGameScriptInterfaceBase&);  // 0x00DAAC00
    virtual void RegisterMain();                                         // 0x00DAACE0
    virtual void Main();                                                 // 0x00DABAC0
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void AddBadDeed(EBadDeeds);                                          // 0x00DAEA70
    void AddGoodDeed();
    void AttackStuff();
    void DoMission();
    void ManageQuestCoreMarkers();
    void PostAttackStuff();
    void StartBarrelTimer();
    void WatchBarrels();
    void WatchForGotGold();
    bool VictimShake;                                                    // +0x48
    unsigned char _pad49[3];
    long StopTimeIndex;                                                  // +0x4c
    bool AttackOver;                                                     // +0x50
    bool DadFound;                                                       // +0x51
    bool DadFinishedIntro;                                               // +0x52
    bool DadOfferedRewards;                                              // +0x53
    long GoodDeedsPerformed;                                             // +0x54
    long BadDeedsPerformed;                                              // +0x58
    long GUIGoodDeedCounter;                                             // +0x5c
    long GUIBarrelCounter;                                               // +0x60
    long GUIBullyHealthCounter;                                          // +0x64
    long GuardsDealtWithBadDeeds;                                        // +0x68
    bool BullySubdued;                                                   // +0x6c
    bool BullyRanOff;                                                    // +0x6d
    bool GivenHeroTeddy;                                                 // +0x6e
    bool HeroAttackedVictim;                                             // +0x6f
    bool SpokeAboutFindingTeddy;                                         // +0x70
    bool HeroDiscoveredInfidelity;                                       // +0x71
    bool BarrelManLeftHeroInCharge;                                      // +0x72
    bool BarrelManSpokenToHeroOnReturn;                                  // +0x73
    bool BarrelBrokenInstantaneous;                                      // +0x74
    bool BarrelBrokenPersistent;                                         // +0x75
    C3DVector BarrelBrokenPos;                                           // +0x76
    bool InstructionGiven_Barrels;                                       // +0x82
    bool ReceiveKiss;                                                    // +0x83
    bool ReceiveHug;                                                     // +0x84
    C3DVector WarehouseMeetPoint;                                        // +0x85
    bool TeddyRuined;                                                    // +0x91
    bool GuardsSpokenOnce;                                               // +0x92
    bool TalkingToWoman;                                                 // +0x93
    bool GivenSweets;                                                    // +0x94
    bool GivenTheresaChocs;                                              // +0x95
    bool VictimComplainsAboutLosingTeddy;                                // +0x96
    unsigned char _pad97[1];
    long lastVillagerSpeechIdx;                                          // +0x98
    std::vector<CCharString> VillagerSpeechMaleGood;                     // +0x9c
    std::vector<CCharString> VillagerSpeechMaleBad;                      // +0xa8
    std::vector<CCharString> VillagerSpeechMaleBoth;                     // +0xb4
    std::vector<CCharString> VillagerSpeechMaleNeither;                  // +0xc0
    std::vector<CCharString> VillagerSpeechFemaleGood;                   // +0xcc
    std::vector<CCharString> VillagerSpeechFemaleBad;                    // +0xd8
    std::vector<CCharString> VillagerSpeechFemaleBoth;                   // +0xe4
    std::vector<CCharString> VillagerSpeechFemaleNeither;                // +0xf0
    bool WhichBadDeedsPerformed[5];                                      // +0xfc
    unsigned char _pad101[3];
    CTimer TalkIntermittentTimer;                                        // +0x104
    CTimer WatchTimer;                                                   // +0x108
    class CNOVI_LiveFather;                                              // Alloc 0x00DAC2C0
    class CNOVI_Theresa;                                                 // Alloc 0x00DAC420
    class CNOVI_Guard;                                                   // Alloc 0x00DAC580
    class CNOVI_Villager;                                                // Alloc 0x00DADE50
    class CNOVI_Bully;                                                   // Alloc 0x00DAEC60
    class CNOVI_Victim;                                                  // Alloc 0x00DAEDE0
    class CNOVI_TeddyGirl;                                               // Alloc 0x00DAEF50
    class COVI_DeadFather;
};
#pragma pack(pop)
// The villager entity script (Ego_r.pdb CNOVI_Villager, 32 bytes: ParentClass +0x14, PMasterData +0x18, HeroDidHitMe +0x1c; the
// landed Init 0x00DADF00 clears +0x1c). Entity vtable 0x012D8678 (landed Alloc 0x00DADE50): slot 1 Main 0x00DADF80, slot 2 Init.
class CQ_NewOakValeIntroScript::CNOVI_Villager : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DADE50
    virtual CScriptBase* GetParentScript() const;                        // 0x00DADEF0
private:
    virtual void Main();                                                 // 0x00DADF80
    virtual void Init();                                                 // 0x00DADF00
    long GetVillagerSpeechIndex(long);                                   // 0x00DAEBF0
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool HeroDidHitMe;                                                   // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript) == 0x10c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_Villager) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_NEWOAKVALEINTROSCRIPT_H
