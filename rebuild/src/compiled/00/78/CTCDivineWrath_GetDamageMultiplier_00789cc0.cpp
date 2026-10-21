// CTCDivineWrath::GetDamageMultiplier() const -- the def's per-spell-level damage multiplier, scaled by 1/360
// Leaf: GetDamageMultiplier@CTCDivineWrath
// Retail: Fable.exe @ 0x00789CC0, 22 bytes; Ego_r.pdb ?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ (const, float). 0x01238E00
// is 1/360 (0.0027777778f). Only caller 0x00698DCE (manifest: CGameCameraManager::PushProjectileWeaponMode, unverified).
// The catalog name is kept; the field names are inferred from Ego_r.pdb (PDef, SpellLevel, def DamageTakenMultiplier
// vector<float>), and the retail offsets do not line up 1:1 with the PDB (PDB PDef +0x0c, SpellLevel +0x54, def vector
// +0x120), so treat the names as best guesses.
// Layout: retail offsets from the bytes: def pointer +0x10, level +0x44, def float array begin +0x128.
class CSpecialAbilitiesDivineWrathDef {
public:
    unsigned char _pad00[0x128];
    float*        DamageTakenMultiplier;                 // +0x128 vector<float> begin
};
class CTCDivineWrath {
public:
    unsigned char                          _pad00[0x10];
    const CSpecialAbilitiesDivineWrathDef* PDef;         // +0x10
    unsigned char                          _pad14[0x44 - 0x14];
    long                                   SpellLevel;   // +0x44
    float GetDamageMultiplier() const;
};
float CTCDivineWrath::GetDamageMultiplier() const
{
    return PDef->DamageTakenMultiplier[SpellLevel] * (1.0f / 360.0f);
}