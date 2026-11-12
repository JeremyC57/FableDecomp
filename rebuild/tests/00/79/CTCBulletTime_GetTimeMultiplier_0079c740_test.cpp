// Behaviour test for CTCBulletTime::GetTimeMultiplier() const @ 0x0079c740 (standalone model): returns the float at +0x12C
// (HeroCombatSlowness) of the def that +0x20 (PDef) points at.
#include <cstdio>
#include <cstddef>
struct CDefModel { unsigned char _pad[0x12c]; float HeroCombatSlowness; };
struct CTCBulletTimeModel {
    unsigned char _pad[0x20];
    const CDefModel* PDef;                 // +0x20
    float GetTimeMultiplier() const { return PDef->HeroCombatSlowness; }
};
int main()
{
    CDefModel def; def.HeroCombatSlowness = 2.5f;
    CTCBulletTimeModel tc; tc.PDef = &def;
    bool ok = tc.GetTimeMultiplier() == 2.5f && offsetof(CDefModel, HeroCombatSlowness) == 0x12c;
    def.HeroCombatSlowness = -1.0f;
    ok = ok && tc.GetTimeMultiplier() == -1.0f;
    std::printf(ok ? "0079c740_TEST PASS\n" : "0079c740_TEST FAIL\n");
    return ok ? 0 : 1;
}
