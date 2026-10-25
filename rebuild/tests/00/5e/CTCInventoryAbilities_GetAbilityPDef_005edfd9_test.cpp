// Behaviour test for CTCInventoryAbilities::GetAbilityPDef (0x005EDFD9): returns a counted copy of the indexed ability's def.
#include <stdio.h>
enum EHeroAbility { HERO_ABILITY_NULL };
class CHeroAbilityDef { public: void* vtbl; long RefCount; };
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++((CHeroAbilityDef*)Object)->RefCount; }
};
template <class T1, class T2> struct SPair { T1 first; T2 second; };
class CTCInventoryAbilities {
public:
    unsigned char _pad000[0x160];
    SPair<long, CDefPointer<const CHeroAbilityDef> >* AbilitiesStart;
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const { return AbilitiesStart[ability].second; }
};
int main()
{
    CHeroAbilityDef a; a.vtbl = 0; a.RefCount = 5;
    CHeroAbilityDef b; b.vtbl = 0; b.RefCount = 100;
    static unsigned char raw[3 * 8];
    SPair<long, CDefPointer<const CHeroAbilityDef> >* tbl = (SPair<long, CDefPointer<const CHeroAbilityDef> >*)raw;
    tbl[0].second.Object = &a; tbl[1].second.Object = 0; tbl[2].second.Object = &b;
    static CTCInventoryAbilities inv; inv.AbilitiesStart = tbl;
    CDefPointer<const CHeroAbilityDef> r0 = inv.GetAbilityPDef((EHeroAbility)0);
    CDefPointer<const CHeroAbilityDef> r1 = inv.GetAbilityPDef((EHeroAbility)1);
    CDefPointer<const CHeroAbilityDef> r2 = inv.GetAbilityPDef((EHeroAbility)2);
    int ok = r0.Object == &a && a.RefCount == 6 && r1.Object == 0 && r2.Object == &b && b.RefCount == 101;
    printf(ok ? "OK_0x005edfd9\n" : "FAIL_0x005edfd9\n");
    return ok ? 0 : 1;
}
