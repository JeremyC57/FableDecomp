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
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES
    class CNOVI_AffairMan;                                               // Alloc 0x00DB0880
    class CNOVI_BarrelThug;                                              // Alloc 0x00DB6B40
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2
    class CNOVI_AffairWife;                                              // Alloc 0x00DB29A0
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_AFFAIRWOMAN
    class CNOVI_AffairWoman;                                             // Alloc 0x00DB1DB0
#endif
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
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES
// Retail opt-in (chunk 803): two more entity scripts. Ego_r.pdb layouts (struct_layouts_egor.tsv, entity offsets equal retail's:
// CEntityScriptBase is 0x14 in both): CNOVI_AffairMan 36 bytes (EncounterOver +0x1c, SaidFirstRangedComment +0x1d,
// HeroAgreedToKeepQuiet +0x1e, HeroSaidHeWouldReportMan +0x1f, BadgerIndex +0x20), CNOVI_BarrelThug 36 bytes (DoneIntro +0x1c,
// LastTimeSpoken +0x20); the landed Allocs 0x00DB0880 / 0x00DB6B40 allocate 0x24. Entity vtables 0x012D89EC (AffairMan: 0 scalar
// deleting dtor 0x00DB0930, 1 Main 0x00DB09E0, 2 Init 0x00DB0950, 3 GetParentScript 0x00DB0920) and 0x012D9234 (BarrelThug: 0
// 0x00DB6C40, 1 Main 0x00DB6C60, 2 Init 0x00DB6BF0, 3 GetParentScript 0x00DB6BE0); Main / Init are private (PDB EAEXXZ).
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_AffairMan : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DB0880
    virtual CScriptBase* GetParentScript() const;                        // 0x00DB0920
private:
    virtual void Main();                                                 // 0x00DB09E0
    virtual void Init();                                                 // 0x00DB0950
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool EncounterOver;                                                  // +0x1c
    bool SaidFirstRangedComment;                                         // +0x1d
    bool HeroAgreedToKeepQuiet;                                          // +0x1e
    bool HeroSaidHeWouldReportMan;                                       // +0x1f
    long BadgerIndex;                                                    // +0x20
};
class CQ_NewOakValeIntroScript::CNOVI_BarrelThug : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DB6B40
    virtual CScriptBase* GetParentScript() const;                        // 0x00DB6BE0
private:
    virtual void Main();                                                 // 0x00DB6C60
    virtual void Init();                                                 // 0x00DB6BF0
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool DoneIntro;                                                      // +0x1c
    long LastTimeSpoken;                                                 // +0x20
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan) == 0x24);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelThug) == 0x24);
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2
// Retail opt-in (chunk 806): two more entity scripts. Ego_r.pdb layouts (struct_layouts_egor.tsv, entity offsets equal retail's:
// CEntityScriptBase is 0x14 in both): CNOVI_AffairWife 32 bytes (GoingForHusband +0x1c, ForceFirstTimeSpeak +0x1d,
// SaidRunningLine +0x1e), CNOVI_TeddyGirl 32 bytes (DoneIntro +0x1c, FoundTeddy +0x1d, SpokeAboutFindingTeddy +0x1e, HeroHitMe
// +0x1f); retail Main 0x00DB2B10 / 0x00DAF080 read these offsets, the Allocs 0x00DB29A0 / 0x00DAEF50 allocate 0x20. Entity vtables
// 0x012D8C98 (AffairWife: 0 scalar deleting dtor 0x00DB2A50, 1 Main 0x00DB2B10, 2 Init 0x00DB2A70, 3 GetParentScript 0x00DB2A40)
// and 0x012D87D4 (TeddyGirl: 0 0x00DAF060, 1 Main 0x00DAF080, 2 Init 0x00DAF000, 3 GetParentScript 0x00DAEFF0); Main / Init are
// private (PDB EAEXXZ), GivenTeddy private (PDB ?GivenTeddy@CNOVI_TeddyGirl@CQ_NewOakValeIntroScript@NScript@@AAEXXZ, 0x00DB0600).
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_AffairWife : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DB29A0
    virtual CScriptBase* GetParentScript() const;                        // 0x00DB2A40
private:
    virtual void Main();                                                 // 0x00DB2B10
    virtual void Init();                                                 // 0x00DB2A70
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool GoingForHusband;                                                // +0x1c
    bool ForceFirstTimeSpeak;                                            // +0x1d
    bool SaidRunningLine;                                                // +0x1e
};
class CQ_NewOakValeIntroScript::CNOVI_TeddyGirl : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DAEF50
    virtual CScriptBase* GetParentScript() const;                        // 0x00DAEFF0
private:
    virtual void Main();                                                 // 0x00DAF080
    virtual void Init();                                                 // 0x00DAF000
    void GivenTeddy();                                                   // 0x00DB0600
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool DoneIntro;                                                      // +0x1c
    bool FoundTeddy;                                                     // +0x1d
    bool SpokeAboutFindingTeddy;                                         // +0x1e
    bool HeroHitMe;                                                      // +0x1f
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife) == 0x20);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl) == 0x20);
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER
// Retail opt-in (chunk 807): the two entity scripts the default body forward-declares. Ego_r.pdb layouts (struct_layouts_egor.tsv,
// entity offsets equal retail's: CEntityScriptBase is 0x14 in both): CNOVI_Victim 32 bytes (DisplayedGameInfo +0x1c, DoneThanks
// +0x1d), CNOVI_LiveFather 32 bytes (PenniesGiven +0x1c); retail Main 0x00DBCD60 / 0x00DB86B0 read these offsets. Entity vtables
// 0x012D87B8 (Victim: 0 scalar deleting dtor 0x00DAEE90, 1 Main 0x00DBCD60, 2 Init 0x00DAEEB0, 3 GetParentScript 0x00DAEE80) and
// 0x012D8388 (LiveFather: 0 0x00DAC370, 1 Main 0x00DB86B0, 2 Init 0x00DAC390, 3 GetParentScript 0x00DAC360); Main / Init are
// private (Ego_r.pdb ?Main@CNOVI_Victim@CQ_NewOakValeIntroScript@NScript@@EAEXXZ and the LiveFather / Init spellings).
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_Victim : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DAEDE0
    virtual CScriptBase* GetParentScript() const;                        // 0x00DAEE80
private:
    virtual void Main();                                                 // 0x00DBCD60
    virtual void Init();                                                 // 0x00DAEEB0
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool DisplayedGameInfo;                                              // +0x1c
    bool DoneThanks;                                                     // +0x1d
};
class CQ_NewOakValeIntroScript::CNOVI_LiveFather : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DAC2C0
    virtual CScriptBase* GetParentScript() const;                        // 0x00DAC360
private:
    virtual void Main();                                                 // 0x00DB86B0
    virtual void Init();                                                 // 0x00DAC390
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long PenniesGiven;                                                   // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_Victim) == 0x20);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather) == 0x20);
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_GUARD
// Retail opt-in (chunk 809): the NOVI_Guard entity script. Ego_r.pdb layout (struct_layouts_egor.tsv, entity offsets equal
// retail's: CEntityScriptBase is 0x14 in both): CNOVI_Guard 28 bytes (ParentClass +0x14, PMasterData +0x18); the landed Alloc
// 0x00DAC580 allocates 0x1c. Entity vtable 0x012D83C0 (0 scalar deleting dtor 0x00DAC630, 1 Main 0x00DAC760, 2 Init 0x00DAC650,
// 3 GetParentScript 0x00DAC620, all landed as CNOVI_Guard); Main / Init are private (Ego_r.pdb and FableWin.pdb
// ?Main@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ).
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_Guard : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DAC580
    virtual CScriptBase* GetParentScript() const;                        // 0x00DAC620
private:
    virtual void Main();                                                 // 0x00DAC760
    virtual void Init();                                                 // 0x00DAC650
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_Guard) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_AFFAIRWOMAN
// Retail opt-in (chunk 833): the NOVI_AffairWoman entity script. Ego_r.pdb layout (struct_layouts_egor.tsv, entity offsets equal
// retail's: CEntityScriptBase is 0x14 in both): CNOVI_AffairWoman 28 bytes (ParentClass +0x14, PMasterData +0x18); the landed Alloc
// 0x00DB1DB0 allocates 0x1c. Entity vtable 0x012D8C08 (read from retail .rdata): 0 scalar deleting dtor 0x00DB1E60, 1 Main 0x00DB1F00,
// 2 Init 0x00DB1E80, 3 GetParentScript 0x00DB1E50, 4 OnPersist / 5 OnPredicateFail inherited (CEntityScriptBase 0x00CDEBC0 /
// 0x00CDEBD0); Main / Init private (Ego_r.pdb ?Main@CNOVI_AffairWoman@CQ_NewOakValeIntroScript@NScript@@EAEXXZ).
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_AffairWoman : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_NewOakValeIntroScript&, CScriptDataBase*, const CScriptThing&); // 0x00DB1DB0
    virtual CScriptBase* GetParentScript() const;                        // 0x00DB1E50
private:
    virtual void Main();                                                 // 0x00DB1F00
    virtual void Init();                                                 // 0x00DB1E80
protected:
    CQ_NewOakValeIntroScript& ParentClass;                               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman) == 0x1c);
#endif
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript) == 0x10c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript::CNOVI_Villager) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_NEWOAKVALEINTROSCRIPT_H
