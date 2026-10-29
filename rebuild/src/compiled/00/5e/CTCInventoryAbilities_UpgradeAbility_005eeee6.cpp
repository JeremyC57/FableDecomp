// CTCInventoryAbilities::UpgradeAbility(EHeroAbility) -- raises an owned ability one level (up to its maximum) or has the hero learn it, then ages the hero by the def's per-upgrade amount
// Leaf: UpgradeAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EEEE6, 158 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: GetAbilityMaximumLevel(EHeroAbility) @ 0x005EDEA3 (its real body, as landed with 0x005EEF84, is defined
//          above under `auto_inline(off)`: VC7.1 then knows it leaves edx alone and keeps the entry address in edx
//          across the call), UpdateWillMorph() @ 0x005EE887, CThingPlayerCreature::LearnAbility(EHeroAbility, bool,
//          long, bool) @ 0x006AC690, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCBase::GetDefinitionManager() @ 0x006869D0, CTCHeroStats::SetAge(float) @ 0x0057B05F.
// Shape: the level is a value local (edi) read through `Abilities[ability]`, whose address VC keeps in edx; an owned
// ability (`> -1`) below its maximum is bumped by indexing again (`++Abilities[ability].Level` -> `inc dword ptr
// [edx]`; a reference local makes VC store edi + 1 instead); the hero-stats component comes
// from the parent's inline GetInterface(&stats, 4) with the key in the dead argument slot, used without a null
// test: `stats->SetAge(stats->Age + GetDefinitionManager()->PHeroAbilitiesDef->AgeIncreasePerUpgrade)`.
// Layout: retail CTCInventoryAbilities Abilities vector<pair<long, CDefPointer<const CHeroAbilityDef>>> +0x160
// (8-byte entries); retail CTCHeroStats Age +0x30; definition holder +0xD0 -> def +0xD0 the age step.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingPlayerCreature : public CThing { public:
    bool LearnAbility(long ability, bool notify, long level, bool updateMorph);   // 0x006AC690
};

class CTCHeroStats : public CTCHeroStatsDonor {
 public:
    void SetAge(float age);                          // 0x0057B05F
};

struct CHeroAbilitiesDef {
    unsigned char _pad_0x00[0xD0];
    float         AgeIncreasePerUpgrade;             // +0xD0
};

class CGameDefinitionManager { public:
    unsigned char      _pad_0x00[0xD0];
    CHeroAbilitiesDef* PHeroAbilitiesDef;            // +0xD0
};

struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
class CHeroAbilityDef { public:
    unsigned char _pad_0x00[0x80];
    CLongVector   ExperienceCostsToUpgrade;          // +0x80
};
struct SAbilityEntry { long Level; const CHeroAbilityDef* PDef; };
struct CAbilityEntryVector {
    SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage;
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
static const long MAX_ABILITY_LEVEL = 3;

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase { public:
    void*                 __vftable;                 // +0x00
    CThingPlayerCreature* ParentThing;               // +0x04 CTCBase
    unsigned char         _pad_0x08[0x158];
    CAbilityEntryVector   Abilities;                 // +0x160

    long GetAbilityMaximumLevel(long ability);       // 0x005EDEA3 (defined below)
    void UpdateWillMorph();                          // 0x005EE887
    void UpgradeAbility(long ability);
};

#pragma optimize("y", off)
#pragma auto_inline(off)
long CTCInventoryAbilities::GetAbilityMaximumLevel(long ability)
{
    if (ability != 0) {
        const CHeroAbilityDef* def = Abilities[ability].PDef;
        if (def != 0) {
            long level = def->ExperienceCostsToUpgrade.size() - 1;
            if (level < 0)
                level = 0;
            return Min(level, MAX_ABILITY_LEVEL);
        }
    }
    return 0;
}
#pragma auto_inline(on)

void CTCInventoryAbilities::UpgradeAbility(long ability)
{
    long level = Abilities[ability].Level;
    if (level > -1) {
        if (level < GetAbilityMaximumLevel(ability)) {
            ++Abilities[ability].Level;
            UpdateWillMorph();
        }
    } else {
        ParentThing->LearnAbility(ability, true, -1, true);
    }
    CTCHeroStats* stats;
    if (ParentThing->GetInterface(&stats, 4))
        stats->SetAge(stats->Age + GetDefinitionManager().PHeroAbilitiesDef->AgeIncreasePerUpgrade);
}