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
    long GetStatExperience(EHeroExperienceStatCategory)const;
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
    // Chunk 735 (row 0x00577AC0): ?GetExperiencePointsAvailableToSpend@CTCHeroExperience@@QBEJXZ (Ego_r.pdb / FableWin.pdb),
    // inline in retail (OpenInventoryInternalPC reads +0x14 in place).
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
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
#elif defined(FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE)
// Chunk 764 (row 0x006D7B80 GetAbilityPossibleLevelUpAmount): Ego_r.pdb StatExperiencePoints is std::vector<long> (+0x18, retail
// STLport 12 bytes; the row indexes `[ecx+eax*4]` off _M_start). Needs stlport/fable_containers.h (included above).
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;std::vector<long> StatExperiencePoints;unsigned char TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#else
private:long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;unsigned char StatExperiencePoints[12],TrainableStatLevels[12];float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;bool AbleToGainExperience;unsigned long Pickups;unsigned char ExperienceSpentOnStats[12];unsigned long AttackHistory;CDefPointer<const CHeroExperienceDef> PDef;CDefPointer<const CHeroCombatDef> PHeroCombatDef;CDefPointer<const CHeroStatsDef> PHeroStatsDef;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroExperience)==96);
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

FABLE_STATIC_ASSERT(sizeof(CTCHeroExperience) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, FishingLevel) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, TotalExperiencePoints) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperiencePointsAvailableToSpend) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, StatExperiencePoints) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, TrainableStatLevels) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceMultiplierParam) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, LastExperienceMultiplierParam) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceAttenuationTimer) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, AbleToGainExperience) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, Pickups) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, ExperienceSpentOnStats) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, AttackHistory) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PDef_Object) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PHeroCombatDef_Object) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCHeroExperience, PHeroStatsDef) == 0x5c);

#endif // FABLE_RETAIL_CTCHEROEXPERIENCE
#endif // FABLE_ENGINE_CTCHEROEXPERIENCE_H
