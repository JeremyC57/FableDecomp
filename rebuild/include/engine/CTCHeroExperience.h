#ifndef FABLE_ENGINE_CTCHEROEXPERIENCE_H
#define FABLE_ENGINE_CTCHEROEXPERIENCE_H
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE
// Retail opt-in (Codex pass 11 consolidation 2): the retail CTCHeroExperience model (row 0x00667D30 CThingCreatureBase::OnHit); see below.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER
#include "engine/CAnimSet.h"
#endif
#include "engine/CDefPointer.h"
#include "engine/CHeroCombatDef.h"
#include "engine/EHeroExperienceStatCategory.h"
#ifdef FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#include "engine/EHeroTrainableStatType.h"
#else
enum EHeroTrainableStatType;                                             // MS forward enum (chunk 735)
#endif
#include "engine/EHeroAbility.h"
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS
#include "stlport/fable_containers.h"
#endif
class CHeroCombatDef;class CThingCreatureBase;class CHeroExperienceDef;class CHeroStatsDef;
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#include "engine/CHeroStatsDef.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_MULTISTRIKE
#include "engine/CComboMultiplierData.h"
#include "engine/CWorld.h"
class CHitParameters;
#endif
class CTCHeroExperience:public CTCBase{
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
public: void ResetExperiencePickUpMultiplier();
#endif

#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_TOTALEXPERIENCE
public:
    // Ego_r.pdb / FableWin.pdb ?GetTotalExperience@CTCHeroExperience@@QBEJXZ (inline in retail).
    long GetTotalExperience() const { return TotalExperiencePoints; }
#endif
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
void AddExperience(long);
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
void AddSuccessfulBlockParry();void SetAsHit();void AddSuccessfulStrikeWithHitParams(const CHitParameters&);
#endif
public:
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_TOTALSTAT
 // Nested opt-in (agent1022, row 0x0078C210 CTCBerserk::CTCBerserk): Ego_r.pdb / FableWin.pdb
 // ?GetTotalStatLevel@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z (retail 0x006D6660) and
 // ?GetTotalStatMaxLevel@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z (retail 0x006D66E0), both landed. Declarations only.
 long GetTotalStatLevel(EHeroExperienceStatCategory category) const;     // 0x006D6660
 long GetTotalStatMaxLevel(EHeroExperienceStatCategory category) const;  // 0x006D66E0
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_MULTISTRIKE
    void AddSuccessfulMultistrikeHit(const CHitParameters& hit);
private:
    const CComboMultiplierData& GetActiveComboMultiplierData()const;
    float GetAttackVariationMultiplier()const;
    void UpdateComboMultiplier(bool increase);
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type) {
        AttackHistory.pop_front();
        AttackHistory.push_back(std::make_pair(type,GetWorld().GetFrame()));
    }
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_STRIKEOFTYPE
    // Nested opt-in (chunk 1016, row 0x006D8790): Ego_r.pdb / FableWin.pdb
    // ?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z (private; out of line at 0x006D8790, called by the
    // one-argument forwarder 0x006D8810 with (type, 0, false)). A declaration only: no layout change.
    void AddSuccessfulStrikeOfType(EDamageAttribute type,float damage,bool increase);   // 0x006D8790
#endif
public:
#endif
#ifdef FABLE_RETAIL_CREATEFISH_PASS29
    long GetFishingLevel() const { return FishingLevel; }
#endif
#ifdef FABLE_RETAIL_CREATOR_UPDATE_PASS29
    long GetRoughExperienceLevel() const;
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType(){return TCI_HERO_EXPERIENCE;}
    const CHeroCombatDef& PeekHeroCombatDef()const{return *PHeroCombatDef;}
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF
    // Retail opt-in (chunk 1004, row 0x005C90FC): FableWin.pdb ?GetPHeroCombatDef@CTCHeroExperience@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ,
    // inline in retail (the by-value copy of PHeroCombatDef +0x58: `mov esi,[esi+58h] ; inc [esi+4]`, 0x005C974C).
    CDefPointer<const CHeroCombatDef> GetPHeroCombatDef()const{return PHeroCombatDef;}
#endif
    void AddTrainableStatExperience(EHeroExperienceStatCategory,long);
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER
    // Retail opt-in (Codex pass 17): original public const getter; debug01eee710 and native006d8300 expansion.
    long GetExperiencePickUpMultiplier()const {return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam);}
private:
    long GetExperiencePickupMultiplierForParam(float param)const {
        float param_per_level=PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
        long multiplier=(long)(param/param_per_level+1.0);
        GFLimitLower(multiplier,1);
        return multiplier;
    }
public:
#endif
    // Chunk 735 (CTCInventoryExperience rows 0x005729D8 / 0x00573792): the trainable-stat queries, out of line (Ego_r.pdb /
    // FableWin.pdb spellings): GetTrainableStatLevel 0x006D6520 (QBE), GetTrainableStatMaxLevel 0x006D6770 (QBE),
    // GetCostToUpgradeTrainableStat 0x006D67B0 (QBE), GetCategoryTrainableStatIsIn 0x006D6850 (QBE), GetStatExperience
    // 0x006D6510 (QBE), GetTrainableStatPossibleLevelUpAmount 0x006D6FA0 (QAE, the fraction by reference).
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
    bool IsToAutoBlock()const;
    bool IsBlockRecoilAnimToConserveAnimationHandedness()const;
    long GetTrainableStatLevel(EHeroTrainableStatType stat)const{return TrainableStatLevels[stat];}
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat)const {
        const std::vector<CAbilityTrackLevelDef>& abilities=PHeroStatsDef->StatLevels[stat].Abilities;
        long maxLevel=(abilities.end()-abilities.begin())-1;
        if(maxLevel<0)maxLevel=0;
        return maxLevel;
    }
#else
    long GetTrainableStatLevel(EHeroTrainableStatType)const;
    long GetTrainableStatMaxLevel(EHeroTrainableStatType)const;
#endif
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType)const;
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_CATEGORY_BODY
    // Retail opt-in (chunk 1014, row 0x006D6FA0 GetTrainableStatPossibleLevelUpAmount): the same-TU body of the out-of-line
    // GetCategoryTrainableStatIsIn (0x006D6850), which the row expands (`mov edx,[esi+3Ch]` off &StatLevels[stat]). Needs
    // FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS (the vector layout) and FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS.
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat)const{return PHeroStatsDef->StatLevels[stat].ExperienceStat;}
#else
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType)const;
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_GETSTATEXPERIENCE_BODY
    // hand4e opt-in (row 0x006D6FA0 GetTrainableStatPossibleLevelUpAmount): the same-TU body of ?GetStatExperience@CTCHeroExperience@@QBEJ
    // W4EHeroExperienceStatCategory@@@Z, out of line at 0x006D6510 (`mov eax,[ecx+18h] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*4] ; ret 4`) and
    // expanded by the row (0x006D6FC6..0x006D6FCE). Needs FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS (the vector layout).
    long GetStatExperience(EHeroExperienceStatCategory stat_category)const{return StatExperiencePoints[stat_category];}
#else
    long GetStatExperience(EHeroExperienceStatCategory)const;
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_SPENTONSTAT
    // NF125 opt-in (rows 0x006D64A0 self, 0x005724CD InitialiseExperienceSpendValues): Ego_r.pdb 0x00587C10 / FableWin.pdb 0x01EEFBE0
    // ?GetExperienceSpentOnStat@CTCHeroExperience@@QBEJW4EHeroTrainableStatType@@@Z (public const), out of line at retail 0x006D64A0 (Ego_r donor
    // order ForceSpendableExperienceTo / GetExperienceSpentOnStat = retail 0x006D6470 / 0x006D64A0; the body reads ExperienceSpentOnStats +0x44).
    long GetExperienceSpentOnStat(EHeroTrainableStatType)const;                // 0x006D64A0
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_FORCEEXPERIENCE
    // NF125 opt-in (rows 0x0056F4BA SExpSpendUndo::RevertExperiencePoints): Ego_r.pdb 0x00587BE0 / 0x00587E80, FableWin.pdb 0x01EEF510 /
    // 0x01EF5720 ?ForceSpendableExperienceTo@CTCHeroExperience@@QAEXJ@Z and
    // ?ForceTrainableStatExperienceTo@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z (public), out of line at retail 0x006D6470 /
    // 0x006D6830. Declarations only.
    void ForceSpendableExperienceTo(long);                                     // 0x006D6470
    void ForceTrainableStatExperienceTo(EHeroExperienceStatCategory,long);     // 0x006D6830
#endif
    long GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType,float&);
    // Chunk 735 (AddAbilityToList(PC)): ?GetAbilityPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroAbility@@AAM@Z 0x006D7B80.
    long GetAbilityPossibleLevelUpAmount(EHeroAbility,float&);
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT
    // Retail opt-in (chunk Q8, row 0x006D8980): Ego_r.pdb / FableWin.pdb ?UpgradeTrainableStat@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@@Z
    // (public, out of line at retail 0x006D8980) and the two private members it calls, out of line in retail:
    // ?UpdateAbilitiesForStat@CTCHeroExperience@@AAEXW4EHeroTrainableStatType@@_N@Z (0x006D8170) and
    // ?OnStatLevelChange@CTCHeroExperience@@AAEX_N@Z (0x006D83B0). Declarations only: no layout change.
    void UpgradeTrainableStat(EHeroTrainableStatType stat);              // 0x006D8980
private:
    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent);   // 0x006D8170
    void OnStatLevelChange(bool silent);                                 // 0x006D83B0
public:
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_MODIFYLEVEL
    // Opt-in (chunk 1005, row 0x00576ED1 CTCInventoryExperience::UndoBuyStat): Ego_r.pdb / FableWin.pdb
    // ?ModifyTrainableStatLevel@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@J@Z, out of line at 0x006D6810.
    void ModifyTrainableStatLevel(EHeroTrainableStatType, long);
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE
    // NF61 opt-in (round 7, rows 0x005786B5 BuyStat / 0x005785B3 BuyAbility): Ego_r.pdb
    // ?SpendExperience@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@J@Z (out of line at 0x006D6F60) and
    // ?SpendExperience@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z (0x006D6590): the landed rows' Leafs. Declarations only.
    void SpendExperience(EHeroTrainableStatType, long);
    void SpendExperience(EHeroExperienceStatCategory, long);
#endif
    // Chunk 735 (row 0x00577AC0): ?GetExperiencePointsAvailableToSpend@CTCHeroExperience@@QBEJXZ (Ego_r.pdb / FableWin.pdb),
    // inline in retail (OpenInventoryInternalPC reads +0x14 in place).
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_ONHITSOUND
    // Opt-in (hand4a, row 0x00733B70 CTCCombat::PlayCombatStrikeSounds): Ego_r.pdb / FableWin.pdb ?GetOnHitSound@CTCHeroExperience@@QBE?AVCCharString@@XZ
    // (Ego_r 0x0058A7A0, FableWin 0x01EF13A0), out of line in retail at 0x006D8BF0 (hidden CCharString result). Declaration only.
    CCharString GetOnHitSound() const;                                   // 0x006D8BF0
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_BREAKDOORS
    // Nested opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): Ego_r.pdb / FableWin.pdb ?IsAbleToBreakDownDoors@CTCHeroExperience@@QBE_NXZ,
    // out of line in retail at 0x006D6370 (the landed row). Declaration only: no layout change.
    bool IsAbleToBreakDownDoors() const;                                 // 0x006D6370
#endif
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnCreate();
public: virtual void OnInitialActivate();
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public: virtual bool IsComponentSerialisable() const;
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public: virtual void Copy(const CTCBase &);
protected: virtual void OnSerialise(CPersistContext &,bool);
public: virtual ~CTCHeroExperience();
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_MULTISTRIKE
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;unsigned char StatExperiencePoints[12],TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];std::list<std::pair<EDamageAttribute,long> > AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#elif defined(FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS)
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;std::vector<long> StatExperiencePoints,TrainableStatLevels;float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#elif defined(FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS)
// NF125 opt-in (rows 0x006D64A0 / 0x006D6510 / 0x006D6520, the out-of-line stat getters): BLOCK_THRESHOLDS' vector layout without its inline
// GetTrainableStatLevel / GetTrainableStatMaxLevel bodies, and Ego_r.pdb ExperienceSpentOnStats as std::vector<long> (+0x44, retail STLport
// 12 bytes; 0x006D64A0 indexes `[eax+ecx*4]` off its _M_start). Needs stlport/fable_containers.h (included above).
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;std::vector<long> StatExperiencePoints,TrainableStatLevels;float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;std::vector<long> ExperienceSpentOnStats;unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#elif defined(FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE)
// Chunk 764 (row 0x006D7B80 GetAbilityPossibleLevelUpAmount): Ego_r.pdb StatExperiencePoints is std::vector<long> (+0x18, retail
// STLport 12 bytes; the row indexes `[ecx+eax*4]` off _M_start). Needs stlport/fable_containers.h (included above).
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;std::vector<long> StatExperiencePoints;unsigned char TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#else
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;unsigned char StatExperiencePoints[12],TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#endif
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_ONREGIONLOAD
public:
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?OnRegionLoad@CTCHeroExperience@@QAEXXZ (0x006D7570, called 0x004A49D9 on
    // the hero's TCI_HERO_EXPERIENCE component). Declaration only.
    void OnRegionLoad();                                                 // 0x006D7570
#endif // FABLE_RETAIL_CTCHEROEXPERIENCE_ONREGIONLOAD
#ifdef FABLE_RETAIL_CTCHEROEXPERIENCE_FRACTIONCOMPLETE
public:
    // Opt-in (w23 e15, row 0x006D63F0): Ego_r.pdb 0x00587B20 / FableWin.pdb 0x01EEE540
    // ?GetExperiencePickupMultiplierCurrentLevelFractionComplete@CTCHeroExperience@@QBEMXZ (public const; Ego_d.pdb tc_hero_experience.obj
    // right after GetRunningNumHits 0x006D63A0; one caller, CDrawCombatMultiplier::Draw 0x006482B0). Declaration only.
    float GetExperiencePickupMultiplierCurrentLevelFractionComplete() const;   // 0x006D63F0
#endif
};
FABLE_STATIC_ASSERT_AT(170, sizeof(CTCHeroExperience)==96);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCHeroExperience = 0x7c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCHeroExperience = 0x60.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;

#pragma pack(push, 1)
class CTCHeroExperience {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _base_0x04[0x8];                        // +0x04 flattened base-class subobject(s)
    long             FishingLevel;                           // +0x0c
    long             TotalExperiencePoints;                  // +0x10
    long             ExperiencePointsAvailableToSpend;       // +0x14
    unsigned char    StatExperiencePoints[0xc];              // +0x18 vector<long,std::allocator<long>_>
    unsigned char    TrainableStatLevels[0xc];               // +0x24 vector<long,std::allocator<long>_>
    float            ExperienceMultiplierParam;              // +0x30
    float            LastExperienceMultiplierParam;          // +0x34
    long             ExperienceAttenuationTimer;             // +0x38
    bool             AbleToGainExperience;                   // +0x3c
    unsigned char    _pad_0x3d[0x3];                         // +0x3d
    unsigned char    Pickups[0x4];                           // +0x40 list<std::pair<long,long>,std::allocator<std::pair<long,long>_>_>
    unsigned char    ExperienceSpentOnStats[0xc];            // +0x44 vector<long,std::allocator<long>_>
    unsigned char    AttackHistory[0x4];                     // +0x50 list<std::pair<EDamageAttribute,long>,std::allocator<std::pair<EDamageAttribute,long>_>_>
    CDefPointeeBase* PDef_Object;                            // +0x54 CDefPointer<CHeroExperienceDef_const_>::Object
    CDefPointeeBase* PHeroCombatDef_Object;                  // +0x58 CDefPointer<CHeroCombatDef_const_>::Object
    unsigned char    PHeroStatsDef[0x4];                     // +0x5c CDefPointer<CHeroStatsDef_const_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(207, sizeof(CTCHeroExperience) == 0x60);
FABLE_STATIC_ASSERT_AT(208, offsetof(CTCHeroExperience, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(209, offsetof(CTCHeroExperience, FishingLevel) == 0xc);
FABLE_STATIC_ASSERT_AT(210, offsetof(CTCHeroExperience, TotalExperiencePoints) == 0x10);
FABLE_STATIC_ASSERT_AT(211, offsetof(CTCHeroExperience, ExperiencePointsAvailableToSpend) == 0x14);
FABLE_STATIC_ASSERT_AT(212, offsetof(CTCHeroExperience, StatExperiencePoints) == 0x18);
FABLE_STATIC_ASSERT_AT(213, offsetof(CTCHeroExperience, TrainableStatLevels) == 0x24);
FABLE_STATIC_ASSERT_AT(214, offsetof(CTCHeroExperience, ExperienceMultiplierParam) == 0x30);
FABLE_STATIC_ASSERT_AT(215, offsetof(CTCHeroExperience, LastExperienceMultiplierParam) == 0x34);
FABLE_STATIC_ASSERT_AT(216, offsetof(CTCHeroExperience, ExperienceAttenuationTimer) == 0x38);
FABLE_STATIC_ASSERT_AT(217, offsetof(CTCHeroExperience, AbleToGainExperience) == 0x3c);
FABLE_STATIC_ASSERT_AT(218, offsetof(CTCHeroExperience, Pickups) == 0x40);
FABLE_STATIC_ASSERT_AT(219, offsetof(CTCHeroExperience, ExperienceSpentOnStats) == 0x44);
FABLE_STATIC_ASSERT_AT(220, offsetof(CTCHeroExperience, AttackHistory) == 0x50);
FABLE_STATIC_ASSERT_AT(221, offsetof(CTCHeroExperience, PDef_Object) == 0x54);
FABLE_STATIC_ASSERT_AT(222, offsetof(CTCHeroExperience, PHeroCombatDef_Object) == 0x58);
FABLE_STATIC_ASSERT_AT(223, offsetof(CTCHeroExperience, PHeroStatsDef) == 0x5c);

#endif // FABLE_RETAIL_CTCHEROEXPERIENCE
#endif // FABLE_ENGINE_CTCHEROEXPERIENCE_H
