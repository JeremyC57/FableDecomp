// behaviour test for 0x005EDF08
#include <stdio.h>
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
int main()
{
    CAbilityEntry e[3]; e[0].first = -1; e[1].first = 0; e[2].first = 3;
    CTCInventoryAbilities a; a.Abilities._M_start = e; a.Abilities._M_finish = e + 3; int ok = 1;
    ok &= !a.IsAbilityAvailable((EHeroAbility)0) && a.IsAbilityAvailable((EHeroAbility)1) && a.IsAbilityAvailable((EHeroAbility)2);
    printf(ok ? "005edf08_TEST PASS\n" : "005edf08_TEST FAIL\n");
    return ok ? 0 : 1;
}