#include <cstdio>

// Model: the analogue zoom parameter moves by half the delta and stays within [0, 1].
class CTCHeroRangedAttacks {
public:
    float ProjectileWeaponZoomParameter;
    void SetProjectileTargetingZoomDelta(float delta);
};

void CTCHeroRangedAttacks::SetProjectileTargetingZoomDelta(float delta)
{
    ProjectileWeaponZoomParameter += delta * 0.5f;
    if (ProjectileWeaponZoomParameter < 0.0f)
        ProjectileWeaponZoomParameter = 0.0f;
    else if (ProjectileWeaponZoomParameter > 1.0f)
        ProjectileWeaponZoomParameter = 1.0f;
}

int main()
{
    CTCHeroRangedAttacks r;
    r.ProjectileWeaponZoomParameter = 0.5f;
    r.SetProjectileTargetingZoomDelta(0.5f);
    bool ok = r.ProjectileWeaponZoomParameter == 0.75f;
    r.SetProjectileTargetingZoomDelta(1.0f);
    ok = ok && r.ProjectileWeaponZoomParameter == 1.0f;          // clamped high
    r.SetProjectileTargetingZoomDelta(-1.0f);
    ok = ok && r.ProjectileWeaponZoomParameter == 0.5f;
    r.SetProjectileTargetingZoomDelta(-4.0f);
    ok = ok && r.ProjectileWeaponZoomParameter == 0.0f;          // clamped low
    if (!ok) {
        std::printf("00789a10_TEST FAIL\n");
        return 1;
    }
    std::printf("00789a10_TEST PASS\n");
    return 0;
}