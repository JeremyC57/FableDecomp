// Behaviour test for NThingShot::CTCShotDefinitionBase::GetPWeapon 0x007BEFF0: returns the thing the PWeapon intelligent pointer
// currently resolves to (0 once the weapon is gone), not the shooter.
#include <cstdio>
struct CThing { int id; };
struct CIntelligentPointer {
    CThing* PData;
    CThing* Get() const { return PData; }
};
struct CTCShotDefinitionBase {
    CIntelligentPointer PShooter, PWeapon;
    CThing* GetPWeapon() const { return PWeapon.Get(); }
};
int main()
{
    CThing hero = { 1 }, bow = { 2 };
    CTCShotDefinitionBase shot;
    shot.PShooter.PData = &hero; shot.PWeapon.PData = &bow;
    if (shot.GetPWeapon() != &bow) { std::printf("007beff0_TEST FAIL\n"); return 1; }
    shot.PWeapon.PData = 0;
    if (shot.GetPWeapon() != 0) { std::printf("007beff0_TEST FAIL\n"); return 1; }
    std::printf("007beff0_TEST PASS\n");
    return 0;
}