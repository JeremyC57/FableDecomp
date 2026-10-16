// Behaviour test for 0x005C4769 CTCInventoryWeapons::GetActiveMeleeWeaponDamage
#include <cstdio>
struct CTCWeapon { float damage; float GetDamage() const { return damage; } };
struct CThing { CTCWeapon* weapon; bool alive; };
static float GetActiveMeleeWeaponDamage(CThing* melee)
{
    float damage = 0.0f;
    if (melee && melee->alive)
        damage = melee->weapon->GetDamage();
    return damage;
}
int main()
{
    CTCWeapon w = { 12.5f }; CThing t = { &w, true };
    bool ok = GetActiveMeleeWeaponDamage(&t) == 12.5f && GetActiveMeleeWeaponDamage(0) == 0.0f;
    t.alive = false; ok = ok && GetActiveMeleeWeaponDamage(&t) == 0.0f;
    if (!ok) { std::printf("005c4769_TEST FAIL\n"); return 1; }
    std::printf("005c4769_TEST PASS\n");
    return 0;
}
