// behaviour test for 0x00789CC0
#include <stdio.h>
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
int main()
{
    float mults[3] = { 360.0f, 720.0f, 180.0f };
    static CSpecialAbilitiesDivineWrathDef def; def.DamageTakenMultiplier = mults;
    static CTCDivineWrath w; w.PDef = &def; int ok = 1;
    w.SpellLevel = 0; ok &= w.GetDamageMultiplier() > 0.9999f && w.GetDamageMultiplier() < 1.0001f;
    w.SpellLevel = 1; ok &= w.GetDamageMultiplier() > 1.9999f && w.GetDamageMultiplier() < 2.0001f;
    w.SpellLevel = 2; ok &= w.GetDamageMultiplier() > 0.4999f && w.GetDamageMultiplier() < 0.5001f;
    printf(ok ? "00789cc0_TEST PASS\n" : "00789cc0_TEST FAIL\n");
    return ok ? 0 : 1;
}