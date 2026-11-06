#ifndef FABLE_ENGINE_CV_TEMPLEOFLIGHTSCRIPT_H
#define FABLE_ENGINE_CV_TEMPLEOFLIGHTSCRIPT_H
// Retail opt-in (chunk 788): NScript::CV_TempleOfLightScript, new header (no includer before chunk 788).
// Layout: Ego_r.pdb CV_TempleOfLightScript (struct_layouts_egor.tsv, 136 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): DonationTotal +0x48 .. QuestActivated +0x72, sizeof 0x74 (the
// landed Alloc 0x00EDEB40 news 0x74 bytes). Checked against retail: Main 0x00EDB190 reads QuestActivated +0x72, InsideTemple +0x5a,
// TempleIntroComplete +0x59, AttackedByHero +0x5b, writes WeaponWarningComplete +0x57 and MoralityInitial +0x64 (fstp).
// Retail methods: RegisterMain 0x00EDAE70 (slot 1), Main 0x00EDB190 (slot 2), Init 0x00EDAE20 (slot 3) of the script vtable
// 0x012F3C68 that the "V_TempleOfLight" Alloc 0x00EDEB40 stores; the threads Main spawns under their thread-name literals
// (Ego_r.pdb ?<Name>@CV_TempleOfLightScript@NScript@@QAEXXZ). The other members are the Ego_r.pdb name set.
// Nested entity scripts: the two classes Main binds (Ego_r.pdb ?_ECTOL_*@CV_TempleOfLightScript@NScript@@; the binding-name
// literal is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_TempleOfLightScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TempleOfLightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00EDAE70 (slot 1)
    virtual void Main();                                                 // 0x00EDB190 (slot 2)
    virtual void Init();                                                 // 0x00EDAE20 (slot 3)
    void DoMission();                                                    // 0x00EDC450
    void EndMission();                                                   // 0x00EDC640
    void WatchForAcolyteDeath(CScriptThing);
    void WatchForAcolyteExit();                                          // 0x00EDD060
    void WatchForAcolyteHit(CScriptThing);                               // 0x00EDCB80
    void WatchForDonationConditions();                                   // 0x00EDBC30
    void WatchForInsideTemple();                                         // 0x00EDBD80
    void WatchForLeaving();                                              // 0x00EDC5A0 (FableWin.pdb name set)
    void WatchForQuestInfoDisplayConditions();                           // 0x00EDBD10
    void WatchForTermination();                                          // 0x00EDBC80
    void WatchForWeaponUnsheathed();                                     // 0x00EDC010
    long DonationTotal;                                                  // +0x48
    long DonationThisVisit;                                              // +0x4c
    long DonationCounter;                                                // +0x50
    bool DonationInvalid;                                                // +0x54
    bool DonationActive;                                                 // +0x55
    bool DisplayQuestInfo;                                               // +0x56
    bool WeaponWarningComplete;                                          // +0x57
    bool WeaponUnsheathed;                                               // +0x58
    bool TempleIntroComplete;                                            // +0x59
    bool InsideTemple;                                                   // +0x5a
    bool AttackedByHero;                                                 // +0x5b
    bool AcolyteAttackMessageComplete;                                   // +0x5c
    bool AllAcolytesEscaped;                                             // +0x5d
    bool ReceivedReward;                                                 // +0x5e
    bool GivenCoolThing1;                                                // +0x5f
    bool GivenCoolThing2;                                                // +0x60
    bool AwardWeapon;                                                    // +0x61
    float MoralityInitial;                                               // +0x64
    float MoralityCurrent;                                               // +0x68
    float MoralityCumulative;                                            // +0x6c
    bool QuestFinished;                                                  // +0x70
    bool QuestComplete;                                                  // +0x71
    bool QuestActivated;                                                 // +0x72
    class CTOL_AcolyteGood;                                              // Alloc 0x00EDC1D0
    class CTOL_AltarGood;                                                // Alloc 0x00EDC2D0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TempleOfLightScript) == 0x74);
#ifdef FABLE_RETAIL_CV_TEMPLEOFLIGHTSCRIPT_ACOLYTEGOOD
// Retail opt-in (chunk 811): the TOL_AcolyteGood entity script. Ego_r.pdb layout (struct_layouts_egor.tsv, entity offsets equal
// retail's: CEntityScriptBase is 0x14 in both): CTOL_AcolyteGood 32 bytes (ParentClass +0x14, PMasterData +0x18,
// AcolytePrayerTimer +0x1c); the landed Init 0x00EDC720 and Main 0x00EDD170 use these offsets. Entity vtable 0x012F3E94 (0 scalar
// deleting dtor 0x00EDC2A0, 1 Main 0x00EDD170, 2 Init 0x00EDC720, 3 GetParentScript 0x00EDC270, 4 OnPersist 0x00EDC280,
// 5 OnPredicateFail 0x00EDC290) that Alloc 0x00EDC1D0 stores; Main / Init / OnPersist / OnPredicateFail are private
// (Ego_r.pdb and FableWin.pdb ?Main@CTOL_AcolyteGood@CV_TempleOfLightScript@NScript@@EAEXXZ and the other spellings).
#include "engine/CTimer.h"
namespace NScript {
class CV_TempleOfLightScript::CTOL_AcolyteGood : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_TempleOfLightScript&, CScriptDataBase*, const CScriptThing&); // 0x00EDC1D0
    virtual CScriptBase* GetParentScript() const;                        // 0x00EDC270
private:
    virtual void Main();                                                 // 0x00EDD170
    virtual void Init();                                                 // 0x00EDC720
    virtual void OnPersist(CPersistContext&);                            // 0x00EDC280
    virtual void OnPredicateFail();                                      // 0x00EDC290
protected:
    CV_TempleOfLightScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    CTimer AcolytePrayerTimer;                                           // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TempleOfLightScript::CTOL_AcolyteGood) == 0x20);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_TEMPLEOFLIGHTSCRIPT_H
