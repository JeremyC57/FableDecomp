// CHeroStatsDef::~CHeroStatsDef() -- the compiler-generated destructor: the members' destructors in reverse order (STLport vectors free their storage), then the out-of-line ~CParentDefClassBase 0x00430300 as the tail jump; no vtable store
// Leaf: ??1CHeroStatsDef@@UAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x0045619E. Identity: its only caller is the scalar deleting destructor in slot 0 of vtable 0x012339DC,
//   RTTI .?AVCHeroStatsDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??1CHeroStatsDef@@UAE@XZ.
// Callees: free 0x00BFEA14 (the STLport malloc allocator), CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CHeroStatsDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CHeroStatsDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CAbilityTrackDef.h"
#include "engine/CAppearanceModifierScalingDef.h"
class CPersistContext;

class CHeroStatsDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CHeroStatsDef();                                   // 0x0045619E
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long MaximumStamina;
    float NumberOfSecondsAfterUseStaminaStartsRefilling;
    float NumberOfSecondsAfterChangeHealthStartsRefilling;
    float NumberOfSecondsBetweenHealthRefills;
    long StaminaToRegainPerSecond;
    long HealthToRegainPerRefill;
    long RenownLossOnDeath;
    long MoneyLossOnDeath;
    long MaximumMoney;
    long MaximumExpToSpend;
    long ExpToSpendAtWhichToDisplayTutorial;
    float MinAge;
    float MaxAge;
    std::vector<CAppearanceModifierScalingDef> AppearanceModifierScaling;
    float AmountFatnessIncreasesPerPointOfOverEating;
    float NumberOfMinutesBetweenFatnessReductions;
    float AmountFatnessDecreasesPerReduction;
    float DrinkAffectMultiplierMultiplier;
    long MinVomitTime;
    long MaxVomitTime;
    long MinTimeBeforeVomiting;
    long MaxTimeBeforeVomiting;
    float PintsOfSobrietyPerSecondOfPuke;
    float PintsToDrunk;
    float PintsToVomit;
    float AlcoholAbsorbtionPerSecond;
    std::vector<long> RenownLevelCosts;
    std::vector<long> RenownLevelMaxFollowers;
    std::vector<long> RenownLevelMaxBoastingCrowd;
    long MaxMorality;
    long MaxMoralityAsChild;
    float MaxExtraMaxHitPoints;
    long MaxExtraMaxStaminaPoints;
    long MoralityChangeOnTheftFromHome;
    long MoralityChangeOnTheftFromShop;
    long MoralityChangeOnPicklock;
    long MoralityChangeOnVandalism;
    std::vector<float> TradeSkillForGuileLevel;
    std::vector<CAbilityTrackDef> StatLevels;
    std::vector<float> MaxHitPointsPerHealthLevel;
    std::vector<long> MaxStaminaPointsPerMagicPowerLevel;
    std::vector<float> SoundDistanceMultiplierPerStealthLevel;
    std::vector<float> VisibilityMultiplierPerStealthLevel;
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified
// call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting destructor's call does in retail.
void DestroyCHeroStatsDef(CHeroStatsDef* def)
{
    def->CHeroStatsDef::~CHeroStatsDef();
}
