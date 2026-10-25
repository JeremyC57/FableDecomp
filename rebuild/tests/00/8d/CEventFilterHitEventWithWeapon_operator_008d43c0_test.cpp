#include <cstdio>
// Behaviour test for 0x008D43C0 `anonymous namespace'::CEventFilter_HitEventWithWeapon::operator(): the operator has
// internal linkage in its TU, so the test carries the model (same body) with GetPWeaponUsed stubbed.
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;
    unsigned char  _pad8e[3];
    unsigned char  Dead : 1;
};
namespace NEntityEvents {
class CEventHitBase {
public:
    CThing* Weapon;
    CThing* GetPWeaponUsed() const { return Weapon; }
};
}
class CEventFilter_HitEventWithWeapon {
public:
    long WeaponDefIndex;
    bool operator()(NEntityEvents::CEventHitBase* event) const
    {
        CThing* weapon = event->GetPWeaponUsed();
        if (weapon && !weapon->Dead && weapon->DefGlobalIndex == WeaponDefIndex)
            return true;
        return false;
    }
};
int main()
{
    if ((char*)&((CThing*)0)->DefGlobalIndex - (char*)0 != 0x8c || sizeof(CEventFilter_HitEventWithWeapon) != 4) { std::printf("008d43c0_TEST FAIL layout\n"); return 1; }
    CThing sword; sword.DefGlobalIndex = 0x1234; sword.Dead = 0;
    NEntityEvents::CEventHitBase hit; hit.Weapon = &sword;
    CEventFilter_HitEventWithWeapon filter; filter.WeaponDefIndex = 0x1234;
    if (!filter(&hit)) { std::printf("008d43c0_TEST FAIL match\n"); return 1; }
    filter.WeaponDefIndex = 0x1235;
    if (filter(&hit)) { std::printf("008d43c0_TEST FAIL other\n"); return 1; }
    filter.WeaponDefIndex = 0x1234; sword.Dead = 1;
    if (filter(&hit)) { std::printf("008d43c0_TEST FAIL dead\n"); return 1; }
    hit.Weapon = 0;
    if (filter(&hit)) { std::printf("008d43c0_TEST FAIL unarmed\n"); return 1; }
    std::printf("008d43c0_TEST PASS\n");
    return 0;
}
