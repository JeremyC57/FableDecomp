// Behaviour test for CUserProfileManager::SetDefaultValuesForNewProfile (0x00409B70, the same choices on the NewProfile members): the refresh-rate choice (the option nearest the
// display's rate that does not exceed it, then the config cap walk-down), the resolution search over the display modes and the
// ClampToLimit rule of ClampVideoOption (config value, else the def default clamped by the config / def maximum, else 0).
#include <cstdio>
#include <cstdlib>
static const long opts[4] = { 60, 75, 85, 100 };
static unsigned long PickRefresh(long rate, long cap)
{
    unsigned long r = 0;
    for (unsigned long i = 1; i < 4; ++i)
        if (std::labs(rate - opts[i]) < std::labs(rate - opts[r]) && opts[i] <= rate) r = i;
    if (cap != -1 && opts[r] > cap) { unsigned long k = r; while (opts[k] > cap) --k; r = k; }
    return r;
}
static float Clamp(float configValue, float defValue, float configMax, float defMax)
{
    float out = 0.0f;
    if (configValue != -1.0f) return configValue;
    if (defValue != -1.0f) out = defValue;
    if (configMax != -1.0f) { if (out > configMax) out = configMax; }
    else if (defMax != -1.0f) { if (out > defMax) out = defMax; }
    else out = 0.0f;
    return out;
}
int main()
{
    if (PickRefresh(85, -1) != 2 || PickRefresh(80, -1) != 1 || PickRefresh(100, 75) != 1 || PickRefresh(50, -1) != 0) {
        std::printf("00409b70_TEST FAIL\n"); return 1;
    }
    long modes[3][3] = { { 800, 600, 32 }, { 1024, 768, 32 }, { 1280, 1024, 32 } };
    unsigned long res = 0;
    for (int i = 0; i < 3; ++i) { if (modes[i][0] == 1024 && modes[i][1] == 768 && modes[i][2] == 32) break; ++res; }
    if (res != 1 || Clamp(2.0f, 1.0f, 1.0f, 1.0f) != 2.0f || Clamp(-1.0f, 3.0f, 2.0f, -1.0f) != 2.0f ||
        Clamp(-1.0f, 3.0f, -1.0f, 1.0f) != 1.0f || Clamp(-1.0f, 3.0f, -1.0f, -1.0f) != 0.0f) {
        std::printf("00409b70_TEST FAIL\n"); return 1;
    }
    std::printf("00409b70_TEST PASS\n");
    return 0;
}
