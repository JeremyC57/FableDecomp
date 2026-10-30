// Behaviour test for 0x007B7C30 CTCOverheadDisplay::AddDamageChange (see src/007b7c30.cpp): when the definition's
// DisplayDamage flag is clear nothing happens; otherwise (expiry frames, value) is appended at the back of DamageValues.
#include <cstdio>
#include <list>
#include <utility>
static float g_OverheadValueExpiryFrames = 45.9f;
struct Def { bool DisplayDamage; };
struct Display {
    std::list<std::pair<long, float> > DamageValues;
    const Def* PDef;
    void AddDamageChange(float value)
    {
        if (!PDef->DisplayDamage) return;
        DamageValues.push_back(std::pair<long, float>((long)g_OverheadValueExpiryFrames, value));
    }
};
int main()
{
    Def on = { true }, off = { false };
    Display d; d.PDef = &on;
    d.AddDamageChange(3.5f); d.AddDamageChange(-1.0f);
    Display e; e.PDef = &off; e.AddDamageChange(2.0f);
    bool ok = d.DamageValues.size() == 2 && d.DamageValues.front().first == 45 && d.DamageValues.front().second == 3.5f
        && d.DamageValues.back().second == -1.0f && e.DamageValues.empty();
    std::printf(ok ? "007b7c30_TEST PASS\n" : "007b7c30_TEST FAIL\n");
    return ok ? 0 : 1;
}