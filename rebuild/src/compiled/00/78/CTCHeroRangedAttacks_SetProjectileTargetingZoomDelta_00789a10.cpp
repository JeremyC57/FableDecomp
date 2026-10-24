// CTCHeroRangedAttacks::SetProjectileTargetingZoomDelta(float delta) -- nudge the analogue projectile-targeting zoom by half the stick delta and clamp it to [0, 1]
// Leaf: SetProjectileTargetingZoomDelta
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789A10, 64 bytes, __fastcall `ret 4`.
// Identity: Ego_r.pdb `?SetProjectileTargetingZoomDelta@CTCHeroRangedAttacks@@QAEXM@Z` (donor 0x007909F0, between
//   SetButtonPressure and GetProjectileTargetingZoomParameter -- the retail neighbours 0x007899E0 SetButtonPressure and the
//   landed ResetProjectileTargetingZoomParameter 0x00789B00 write the same +0x40 float). The catalog's
//   "ZoomControl_Update" is descriptive.
// Constants: 0x0122F59C = 0.5f, 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Layout: engine/CTCHeroRangedAttacks.h (Ego_r.pdb): ProjectileWeaponZoomParameter +0x40.
// Shape: `+=` then `if (< 0) = 0; else if (> 1) = 1;` -- the sum is compared against 0 and stored with `fst` in one go,
//   the less-than exit pops it and stores the 0.0f immediate, the other path compares the still-loaded sum with 1.0
//   (`fcomp ; test ah,41h`).
class CTCHeroRangedAttacks {
public:
    unsigned char _pad_0x00[0x40];
    float         ProjectileWeaponZoomParameter;         // +0x40

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