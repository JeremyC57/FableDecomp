// CTCBerserk::IncreaseBerserkDamageToEnemyMultiplier()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BEC0, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+68h] ; fld dword ptr [eax+94h] ; fadd dword ptr [ecx+28h] ; fstp dword ptr [ecx+28h] ; ret
// engine/CTCBerserk.h (PDB): +0x28 float BerserkDamageToEnemy, +0x6C PDef (retail +0x68: the
// DependentThings vector at +0x4C is 4 bytes smaller in retail). The def's per-frame increment at
// +0x94 is added onto the multiplier. The manifest leaf says "...ToMe"; the member the bytes touch is
// BerserkDamageToEnemy (+0x28), so the leaf follows the member.
struct CSpecialAbilitiesBerserkDefCalls {
    unsigned char _pad_0x00[0x94];
    float         DamageToEnemyIncrement;   // +0x94 (name from use)
};

struct CTCBerserk_Retail {
    unsigned char                     _pad_0x00[0x28];
    float                             BerserkDamageToEnemy;   // +0x28 (PDB)
    unsigned char                     _pad_0x2C[0x3C];
    CSpecialAbilitiesBerserkDefCalls* PDef;                   // +0x68 (PDB +0x6C)
    void IncreaseBerserkDamageToEnemyMultiplier();
};

void CTCBerserk_Retail::IncreaseBerserkDamageToEnemyMultiplier()
{
    this->BerserkDamageToEnemy = this->PDef->DamageToEnemyIncrement + this->BerserkDamageToEnemy;
}
