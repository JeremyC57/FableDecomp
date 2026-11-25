// CTCDivineWrath::GetDamageMultiplier() const -- the divine-wrath def's damage multiplier for the current spell level
// Leaf: ?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ
// Retail: Fable.exe @ 0x0078E7B0, 16 bytes: mov eax,[ecx+0Ch] ; mov ecx,[ecx+50h] ; mov edx,[eax+0E8h] ; fld dword [edx+ecx*4] ; ret
// Naming (NF443, 2026-11-25; coordinator round 3): self respelt from the stand-in ?GetDamageMultiplier@@YIMPAUS@@@Z. Evidence: both PDB
//   name sets (?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ); Ego_r donor order GetDamageMultiplier -> Attackable -> SetPTarget =
//   retail 0x0078E7B0 -> 0x0078E7C0 Attackable -> 0x0078E820 SetPTarget; the body reads PDef (+0x0C) and SpellLevel and the def's
//   DamageTakenMultiplier vector<float> (+0xE8 in engine/CSpecialAbilitiesDivineWrathDef.h), the CTCDivineWrath layout. The address
//   0x00789CC0 that carried this name is CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV (same batch).
// Layout: local models of the retail offsets (engine/CTCDivineWrath.h pulls STLport map/containers and its SpellLevel offset is
//   not asserted; header debt as at HEAD, which modelled the same fields with a local struct).
class CSpecialAbilitiesDivineWrathDef {
public:
    unsigned char _pad00[0xE8];
    float*        DamageTakenMultiplier;                 // +0xE8 vector<float> begin
};
class CTCDivineWrath {
public:
    unsigned char                          _pad00[0x0C];
    const CSpecialAbilitiesDivineWrathDef* PDef;         // +0x0C
    unsigned char                          _pad10[0x50 - 0x10];
    long                                   SpellLevel;   // +0x50
    float GetDamageMultiplier() const;
};
float CTCDivineWrath::GetDamageMultiplier() const
{
    return PDef->DamageTakenMultiplier[SpellLevel];
}
