// behaviour test for 0x00789CC0 (CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV; NF443 renamed the model
// with the row's self respell from CTCDivineWrath::GetDamageMultiplier)
#include <stdio.h>
class CHeroCombatDef {
public:
    unsigned char _pad00[0x128];
    float*        ProjectileWeaponFirstPersonModeMaxFOVValues;   // +0x128 vector<float> begin
};
class CTCHeroRangedAttacks {
public:
    unsigned char         _pad00[0x10];
    const CHeroCombatDef* PHeroCombatDef;                              // +0x10
    unsigned char         _pad14[0x44 - 0x14];
    long                  ProjectileWeaponFirstPersonZoomModeIndex;    // +0x44
    float GetProjectileWeaponFirstPersonTargetingModeZoomFOV() const;
};
float CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV() const
{
    return PHeroCombatDef->ProjectileWeaponFirstPersonModeMaxFOVValues[ProjectileWeaponFirstPersonZoomModeIndex] * (1.0f / 360.0f);
}
int main()
{
    float fovs[3] = { 360.0f, 720.0f, 180.0f };
    static CHeroCombatDef def; def.ProjectileWeaponFirstPersonModeMaxFOVValues = fovs;
    static CTCHeroRangedAttacks w; w.PHeroCombatDef = &def; int ok = 1;
    w.ProjectileWeaponFirstPersonZoomModeIndex = 0; ok &= w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() > 0.9999f && w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() < 1.0001f;
    w.ProjectileWeaponFirstPersonZoomModeIndex = 1; ok &= w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() > 1.9999f && w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() < 2.0001f;
    w.ProjectileWeaponFirstPersonZoomModeIndex = 2; ok &= w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() > 0.4999f && w.GetProjectileWeaponFirstPersonTargetingModeZoomFOV() < 0.5001f;
    printf(ok ? "00789cc0_TEST PASS\n" : "00789cc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
