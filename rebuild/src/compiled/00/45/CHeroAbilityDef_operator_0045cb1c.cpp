// CHeroAbilityDef::operator=(const CHeroAbilityDef&) -- memberwise copy: CParentDefClassBase part, the ability/stat/icon indices and offset, the text ids, the three per-level vectors, the priority/morality/aggressive fields and the dummy object; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CB1C, length 177 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886 (three times).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CHeroAbilityDef's vtable 0x01233974 is the 5-byte `jmp 0x0045CB1C` right before this body. PDB members
// +0x40.. sit at retail +0x3c.. (retail base 0x3c), and each retail vector is 12 bytes instead of 16.
// Shape: memberwise assignment in declaration order; the vectors through their out-of-line operator=,
// the C2DVector icon offset as its two floats (dword moves), the rest as dword/byte moves.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class C2DVector { public: float X; float Y; };

class CHeroAbilityDef : public CParentDefClassBase { public:  // vtable 0x01233974
    long        Ability;                     // +0x3c EHeroAbility
    long        Stat;                        // +0x40 EHeroExperienceStatCategory
    long        IconGraphicIndex;            // +0x44
    long        IconEffectIndex;             // +0x48
    long        IconActiveEffectIndex;       // +0x4c
    C2DVector   IconEffectOffset;            // +0x50
    long        Name;                        // +0x58
    long        Description;                 // +0x5c
    CLongVector LevDescription;              // +0x60
    long        MaxedOutDescription;         // +0x6c
    CLongVector StaminaCost;                 // +0x70
    long        StatExperienceGainedOnUse;   // +0x7c
    CLongVector ExperienceCostsToUpgrade;    // +0x80
    long        ABXYPriority;                // +0x8c
    float       MoralityCostFactor;          // +0x90
    bool        Aggressive;                  // +0x94
    long        DummyObject;                 // +0x98 CDefIndex
    CHeroAbilityDef& operator=(const CHeroAbilityDef& other);
};

CHeroAbilityDef& CHeroAbilityDef::operator=(const CHeroAbilityDef& other)
{
    CParentDefClassBase::operator=(other);
    Ability = other.Ability;
    Stat = other.Stat;
    IconGraphicIndex = other.IconGraphicIndex;
    IconEffectIndex = other.IconEffectIndex;
    IconActiveEffectIndex = other.IconActiveEffectIndex;
    IconEffectOffset = other.IconEffectOffset;
    Name = other.Name;
    Description = other.Description;
    LevDescription = other.LevDescription;
    MaxedOutDescription = other.MaxedOutDescription;
    StaminaCost = other.StaminaCost;
    StatExperienceGainedOnUse = other.StatExperienceGainedOnUse;
    ExperienceCostsToUpgrade = other.ExperienceCostsToUpgrade;
    ABXYPriority = other.ABXYPriority;
    MoralityCostFactor = other.MoralityCostFactor;
    Aggressive = other.Aggressive;
    DummyObject = other.DummyObject;
    return *this;
}