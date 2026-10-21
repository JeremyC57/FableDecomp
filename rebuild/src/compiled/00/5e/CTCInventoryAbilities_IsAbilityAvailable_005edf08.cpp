// CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility) const -- an ability is available once its level entry is non-negative
// Leaf: IsAbilityAvailable@CTCInventoryAbilities
// Retail: Fable.exe @ 0x005EDF08, 23 bytes, `ret 4`; Ego_r.pdb ?IsAbilityAvailable@CTCInventoryAbilities@@QBE_NW4EHeroAbility@@@Z.
// Abilities is vector<pair<long, CDefPointer<CHeroAbilityDef const>>> (8-byte elements, `[eax+edx*8]`); first is the level.
// Callers include CTCInventoryExperience::UpdateExperienceSpendAbility 0x0057240E, CThingPlayerCreature::LearnAbility 0x006AC6E3.
// Layout: retail Abilities begin pointer +0x160 (Ego_r.pdb +0x180: the containers before it are smaller in STLport).
// Shape: size TU (`xor ecx,ecx ; cmp [mem],ecx ; setge cl ; mov al,cl`).
#pragma optimize("s", on)
enum EHeroAbility { HERO_ABILITY_NONE = 0 };
class CHeroAbilityDef;
template <class T> class CDefPointer { public: T* Object; };
struct CAbilityEntry {                                   // std::pair<long, CDefPointer<CHeroAbilityDef const> >
    long                                first;
    CDefPointer<const CHeroAbilityDef> second;
};
template <class T> class CVector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};
class CTCInventoryAbilities {
public:
    unsigned char          _pad00[0x160];
    CVector<CAbilityEntry> Abilities;                    // +0x160
    bool IsAbilityAvailable(EHeroAbility ability) const;
};
bool CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility ability) const
{
    return Abilities[ability].first >= 0;
}