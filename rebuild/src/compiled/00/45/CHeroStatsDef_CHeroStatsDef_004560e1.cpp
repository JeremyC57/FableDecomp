// CHeroStatsDef::CHeroStatsDef() -- definition default constructor: the CParentDefClassBase base, this vtable, and the ten vector members emptied (scalars left to the loader)
// Leaf: ??0CHeroStatsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004560E1, length 155 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x012339DC.
// Layout: Ego_r.pdb CHeroStatsDef with retail sizes -- the base is 0x3c bytes (PDB 0x40) and each retail vector
// is 12 bytes (PDB 16), so AppearanceModifierScaling sits at +0x70, RenownLevelCosts +0xac, RenownLevelMaxFollowers
// +0xb8, RenownLevelMaxBoastingCrowd +0xc4, TradeSkillForGuileLevel +0xf0, StatLevels +0xfc,
// MaxHitPointsPerHealthLevel +0x108, MaxStaminaPointsPerMagicPowerLevel +0x114,
// SoundDistanceMultiplierPerStealthLevel +0x120, VisibilityMultiplierPerStealthLevel +0x12c.
// Shape: the vtable store precedes the zero stores, so every zeroed triple is a member object with an inline
// constructor; a size TU (`#pragma optimize("s", on)`: each vector past +0x80 is addressed once with
// `lea ecx` instead of three disp32 stores). The STLport _Vector_base + _STLP_alloc_proxy model (as the landed CTCMultiArrow 0x007CC7D0)
// is the retail vector shape; one zero register; the first vector (+0x70, disp8) addressed off esi.
#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct SAllocator { SAllocator() {} };              // empty allocator (user constructor: no zero fill of the temporary)
template <class T> struct SAllocProxy : SAllocator { // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};
template <class T> struct CVectorBase {             // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};
template <class T> struct CRetailVector : CVectorBase<T> {   // retail (STLport) std::vector, 12 bytes
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
};
class CAppearanceModifierScalingDef;
class CAbilityTrackDef;

class CHeroStatsDef : public CParentDefClassBase { public:
    long   MaximumStamina;                                         // +0x3c
    float  NumberOfSecondsAfterUseStaminaStartsRefilling;          // +0x40
    float  NumberOfSecondsAfterChangeHealthStartsRefilling;        // +0x44
    float  NumberOfSecondsBetweenHealthRefills;                    // +0x48
    long   StaminaToRegainPerSecond;                               // +0x4c
    long   HealthToRegainPerRefill;                                // +0x50
    long   RenownLossOnDeath;                                      // +0x54
    long   MoneyLossOnDeath;                                       // +0x58
    long   MaximumMoney;                                           // +0x5c
    long   MaximumExpToSpend;                                      // +0x60
    long   ExpToSpendAtWhichToDisplayTutorial;                     // +0x64
    float  MinAge;                                                 // +0x68
    float  MaxAge;                                                 // +0x6c
    CRetailVector<CAppearanceModifierScalingDef> AppearanceModifierScaling;   // +0x70
    float  AmountFatnessIncreasesPerPointOfOverEating;             // +0x7c
    float  NumberOfMinutesBetweenFatnessReductions;                // +0x80
    float  AmountFatnessDecreasesPerReduction;                     // +0x84
    float  DrinkAffectMultiplierMultiplier;                        // +0x88
    long   MinVomitTime;                                           // +0x8c
    long   MaxVomitTime;                                           // +0x90
    long   MinTimeBeforeVomiting;                                  // +0x94
    long   MaxTimeBeforeVomiting;                                  // +0x98
    float  PintsOfSobrietyPerSecondOfPuke;                         // +0x9c
    float  PintsToDrunk;                                           // +0xa0
    float  PintsToVomit;                                           // +0xa4
    float  AlcoholAbsorbtionPerSecond;                             // +0xa8
    CRetailVector<long>  RenownLevelCosts;                         // +0xac
    CRetailVector<long>  RenownLevelMaxFollowers;                  // +0xb8
    CRetailVector<long>  RenownLevelMaxBoastingCrowd;              // +0xc4
    long   MaxMorality;                                            // +0xd0
    long   MaxMoralityAsChild;                                     // +0xd4
    float  MaxExtraMaxHitPoints;                                   // +0xd8
    long   MaxExtraMaxStaminaPoints;                               // +0xdc
    long   MoralityChangeOnTheftFromHome;                          // +0xe0
    long   MoralityChangeOnTheftFromShop;                          // +0xe4
    long   MoralityChangeOnPicklock;                               // +0xe8
    long   MoralityChangeOnVandalism;                              // +0xec
    CRetailVector<float> TradeSkillForGuileLevel;                  // +0xf0
    CRetailVector<CAbilityTrackDef> StatLevels;                    // +0xfc
    CRetailVector<float> MaxHitPointsPerHealthLevel;               // +0x108
    CRetailVector<long>  MaxStaminaPointsPerMagicPowerLevel;       // +0x114
    CRetailVector<float> SoundDistanceMultiplierPerStealthLevel;   // +0x120
    CRetailVector<float> VisibilityMultiplierPerStealthLevel;      // +0x12c
    virtual void v00();
    CHeroStatsDef();
};

CHeroStatsDef::CHeroStatsDef()
{
}