// CTCHeroExperience::GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction) -- how many levels the hero could buy for an ability with the experience at hand (the ability's stat pool plus the spendable points), and the fraction of the next level's cost covered; -1 without an ability inventory
// Grade: DIFFER(289v297) -- register allocation only: retail keeps this then experience in ebp and homes nextLevel in the dead ability slot; ours keeps this in ebx and experience in memory
// Leaf: GetAbilityPossibleLevelUpAmount
//
// Retail 0x006D7B80, 304 bytes incl. padding (`ret 8`). Ego_r.pdb
// `?GetAbilityPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroAbility@@AAM@Z`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCInventoryAbilities,
// type 0x6F, `test ch,ch ; jns`), CTCInventoryAbilities::IsAbilityPotentiallyAvailable @ 0x005EE02E, ::GetAbilityMaximumLevel
// @ 0x005EDEA3, ::GetAbilityLevel @ 0x005EDE93 (twice), ::GetAbilityStatType @ 0x005EDEEB, ::GetAbilityMaximumLevelWithRenown
// @ 0x005EE6BF (per pass), ::GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) @ 0x005EE77C.
// Layout: retail CTCHeroExperience ExperiencePointsAvailableToSpend +0x14, StatExperiencePoints +0x18 (12-byte vector; the
// engine header carries the same offsets up to here).
// Shape: a rotated while loop over the levels (count in ebx starting at 1, the bound re-queried each pass); the fraction is
// `(float)experience / cost` (`fild ; fidiv`).
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
enum EHeroAbility { HERO_ABILITY_NONE = 0 };
struct CTCInventoryAbilities {
    bool IsAbilityPotentiallyAvailable(EHeroAbility ability);       // 0x005EE02E
    long GetAbilityMaximumLevel(EHeroAbility ability);              // 0x005EDEA3
    long GetAbilityLevel(EHeroAbility ability);                     // 0x005EDE93
    long GetAbilityStatType(EHeroAbility ability);                  // 0x005EDEEB (EHeroExperienceStatCategory)
    long GetAbilityMaximumLevelWithRenown(EHeroAbility ability);    // 0x005EE6BF
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool flag);   // 0x005EE77C
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) const { return _M_start[n]; }
};
class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    CRetailVector<long> StatExperiencePoints;               // +0x18
    long GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction);
};

long CTCHeroExperience::GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction)
{
    fraction = 0.0f;
    long available = ExperiencePointsAvailableToSpend;
    CTCInventoryAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0x6f))
        return -1;
    if (abilities->IsAbilityPotentiallyAvailable(ability)) {
        long maxLevel = abilities->GetAbilityMaximumLevel(ability);
        if (abilities->GetAbilityLevel(ability) < maxLevel) {
            long experience = StatExperiencePoints[abilities->GetAbilityStatType(ability)] + available;
            long levels = 1;
            long nextLevel = abilities->GetAbilityLevel(ability) + 1;
            for (; abilities->GetAbilityMaximumLevelWithRenown(ability) >= nextLevel + levels - 1; ++levels) {
                long cost = abilities->GetExperienceCostToUpgradeAbility(ability, levels, true);
                if (cost == -1)
                    break;
                if (experience < cost) {
                    fraction = (float)experience / cost;
                    return levels - 1;
                }
                experience -= cost;
            }
            return levels - 1;
        }
    }
    return 0;
}
