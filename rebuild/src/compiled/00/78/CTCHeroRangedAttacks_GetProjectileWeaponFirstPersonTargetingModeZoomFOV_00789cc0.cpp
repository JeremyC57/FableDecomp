// CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV() const -- the combat def's first-person zoom FOV for the
// current zoom mode, in turns (degrees * 1/360)
// Leaf: ?GetProjectileWeaponFirstPersonTargetingModeZoomFOV@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe @ 0x00789CC0, 22 bytes: mov eax,[ecx+10h] ; mov ecx,[ecx+44h] ; mov edx,[eax+128h] ; fld dword [edx+ecx*4] ;
//   fmul dword [0x01238E00] ; ret. 0x01238E00 is 1/360 (0.0027777778f).
// Naming (NF443, 2026-11-25; coordinator round 3): self respelt from ?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ, which belongs to
//   0x0078E7B0 (same batch). Evidence: both PDB name sets (Ego_r 0x00790B90, FableWin 0x024F9890); retail places it in the
//   CTCHeroRangedAttacks TU (0x00789BA0 IsUsingProjectileTargetingAnalogueZoom .. 0x00789C90 IncrementProjectileWeaponMode), as Ego_r
//   does (IsUsingProjectileTargetingAnalogueZoom -> this); its only caller 0x00698D10 (CGameCameraManager::PushProjectileWeaponMode)
//   names it GetProjectileWeaponFirstPersonTargetingModeZoomFOV; the body reads the combat def (+0x10) and
//   ProjectileWeaponFirstPersonZoomModeIndex (+0x44), the offsets engine/CTCHeroRangedAttacks.h asserts, and the def's
//   ProjectileWeaponFirstPersonModeMaxFOVValues vector<float> (+0x128, the vector 0x00789C90 IncrementProjectileWeaponMode wraps on).
// Layout: local models of the retail offsets (header debt kept from HEAD: engine/CTCHeroRangedAttacks.h, engine/CHeroCombatDef.h).
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
