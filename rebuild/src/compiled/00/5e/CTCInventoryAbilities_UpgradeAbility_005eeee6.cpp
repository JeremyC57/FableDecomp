// CTCInventoryAbilities::UpgradeAbility(EHeroAbility) -- raises an owned ability one level (up to its maximum) or has the hero learn it, then ages the hero by the def's per-upgrade amount
// Leaf: UpgradeAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EEEE6, 158 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: GetAbilityMaximumLevel(EHeroAbility) @ 0x005EDEA3 (its real body, as landed with 0x005EEF84, is defined
//          above under `auto_inline(off)`: VC7.1 then knows it leaves edx alone and keeps the entry address in edx
//          across the call), UpdateWillMorph() @ 0x005EE887, CThingPlayerCreature::LearnAbility(EHeroAbility, bool,
//          long, bool) @ 0x006AC690, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCBase::GetDefinitions() @ 0x006869D0, CTCHeroStats::SetAge(float) @ 0x0057B05F.
// Shape: the level is a value local (edi) read through `Abilities[ability]`, whose address VC keeps in edx; an owned
// ability (`> -1`) below its maximum is bumped by indexing again (`++Abilities[ability].Level` -> `inc dword ptr
// [edx]`; a reference local makes VC store edi + 1 instead); the hero-stats component comes
// from the parent's inline GetInterface(&stats, 4) with the key in the dead argument slot, used without a null
// test: `stats->SetAge(stats->Age + GetDefinitions()->PHeroAbilitiesDef->AgeIncreasePerUpgrade)`.
// Layout: retail CTCInventoryAbilities Abilities vector<pair<long, CDefPointer<const CHeroAbilityDef>>> +0x160
// (8-byte entries); retail CTCHeroStats Age +0x30; definition holder +0xD0 -> def +0xD0 the age step.
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingPlayerCreature : public CThing { public:
    bool LearnAbility(long ability, bool notify, long level, bool updateMorph);   // 0x006AC690
};

class CTCHeroStats { public:
    unsigned char _pad_0x00[0x30];
    float         Age;                               // +0x30
    void SetAge(float age);                          // 0x0057B05F
};

struct CHeroAbilitiesDef {
    unsigned char _pad_0x00[0xD0];
    float         AgeIncreasePerUpgrade;             // +0xD0
};

struct CDefinitionHolder {
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

class CTCInventoryAbilities { public:
    void*                 __vftable;                 // +0x00
    CThingPlayerCreature* ParentThing;               // +0x04 CTCBase
    unsigned char         _pad_0x08[0x158];
    CAbilityEntryVector   Abilities;                 // +0x160

    CDefinitionHolder* GetDefinitions() const;       // 0x006869D0 (CTCBase)
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
        stats->SetAge(stats->Age + GetDefinitions()->PHeroAbilitiesDef->AgeIncreasePerUpgrade);
}