// Behaviour test for CTCWeapon::SetProjectileWeaponLoaded (0x005DAC50).
#include <cstdio>

struct Def { long RefCount; bool NeedsLoading; int Deleted; };
struct Thing { bool HasAugmentations; int CopiedFrom; };
static int g_copies = 0;

struct Weapon {
    Thing* ParentThing;
    Def* WeaponDef;
    bool ProjectileWeaponLoaded;
    long AimingTime;
    bool AimingTimeOverride;
    void SetProjectileWeaponLoaded(bool loaded, Thing* ammo)
    {
        if (ammo && ammo->HasAugmentations) { ++g_copies; ammo->CopiedFrom = ParentThing ? 1 : 0; }
        ProjectileWeaponLoaded = loaded;
        Def* def = WeaponDef;
        if (def) ++def->RefCount;              // by-value copy
        bool needsLoading = def->NeedsLoading;
        if (--def->RefCount == 0) ++def->Deleted;
        bool canAim = !needsLoading || ProjectileWeaponLoaded;
        if (canAim) {
            if (AimingTime == -1) AimingTime = 0;
        } else {
            AimingTime = -1;
            AimingTimeOverride = false;
        }
    }
};

#define FAIL() do { std::printf("005dac50_TEST FAIL\n"); return 1; } while (0)
int main()
{
    Thing parent = {false, 0}, ammo = {true, 0}, plain = {false, 0};
    Def def = {1, true, 0};
    Weapon w = {&parent, &def, false, 5, true};
    w.SetProjectileWeaponLoaded(false, &ammo);
    if (g_copies != 1 || ammo.CopiedFrom != 1 || w.AimingTime != -1 || w.AimingTimeOverride || w.ProjectileWeaponLoaded) FAIL();
    if (def.RefCount != 1 || def.Deleted != 0) FAIL();
    w.SetProjectileWeaponLoaded(true, &plain);
    if (g_copies != 1 || w.AimingTime != 0 || !w.ProjectileWeaponLoaded) FAIL();
    w.AimingTime = 7; w.AimingTimeOverride = true;
    w.SetProjectileWeaponLoaded(true, 0);
    if (w.AimingTime != 7 || !w.AimingTimeOverride) FAIL();
    def.NeedsLoading = false; w.AimingTime = -1;
    w.SetProjectileWeaponLoaded(false, 0);
    if (w.AimingTime != 0 || w.ProjectileWeaponLoaded) FAIL();
    std::printf("005dac50_TEST PASS\n");
    return 0;
}