// behaviour test for 0x0040B350 CUserProfileManager::SetMusicVolume: the live value is always stored; the new-profile copy only while UseNewProfileValues; ApplyValues runs every time, Save only when asked
#include <cstdio>
struct Profile { bool UseNew; float Value; float NewValue; int applies; int saves; };
static void Set(Profile& p, float v, bool save)
{
    if (p.UseNew) p.NewValue = v;
    p.Value = v;
    ++p.applies;
    if (save) ++p.saves;
}
int main()
{
    int fails = 0;
    Profile p = { false, 0.5f, 0.25f, 0, 0 };
    Set(p, 0.75f, false);
    if (p.Value != 0.75f || p.NewValue != 0.25f || p.applies != 1 || p.saves != 0) fails |= 1;
    p.UseNew = true;
    Set(p, 1.0f, true);
    if (p.Value != 1.0f || p.NewValue != 1.0f || p.applies != 2 || p.saves != 1) fails |= 2;
    if (fails) { std::printf("0040b350_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b350_TEST PASS\n");
    return 0;
}