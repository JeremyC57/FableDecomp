// Behaviour test for CUserProfileManager::ApplyVideoValues (0x0040A7F0): the aspect-ratio switch (0 / other -> 0:0, 1 -> 4:3,
// 2 -> 16:9), the detail-to-quality steps, and the refresh choice among the adapter modes of the chosen size and depth (the one
// nearest the selected option, starting from the profile mode's own rate).
#include <cstdio>
#include <cstdlib>
static const long opts[4] = { 60, 75, 85, 100 };
static int Quality(float v) { int q = 0; if (v >= 1.0f) q = 1; if (v >= 2.0f) q = 2; if (v >= 3.0f) q = 3; return q; }
static void Aspect(unsigned long a, float& x, float& y)
{
    switch (a) { case 0: x = 0; y = 0; break; case 1: x = 4; y = 3; break; case 2: x = 16; y = 9; break; default: x = 0; y = 0; break; }
}
int main()
{
    float x, y;
    Aspect(1, x, y); if (x != 4 || y != 3) { std::printf("0040a7f0_TEST FAIL\n"); return 1; }
    Aspect(2, x, y); if (x != 16 || y != 9) { std::printf("0040a7f0_TEST FAIL\n"); return 1; }
    Aspect(7, x, y); if (x != 0 || y != 0) { std::printf("0040a7f0_TEST FAIL\n"); return 1; }
    if (Quality(0.5f) != 0 || Quality(1.0f) != 1 || Quality(2.5f) != 2 || Quality(3.0f) != 3) { std::printf("0040a7f0_TEST FAIL\n"); return 1; }
    long modes[4][4] = { { 1024, 768, 32, 60 }, { 1024, 768, 32, 72 }, { 1024, 768, 16, 85 }, { 800, 600, 32, 75 } };
    long refresh = 60;                      // the profile mode's own rate
    unsigned long selected = 1;             // 75 Hz wanted
    for (int i = 0; i < 4; ++i)
        if (modes[i][0] == 1024 && modes[i][1] == 768 && modes[i][2] == 32)
            if (std::labs(opts[selected] - modes[i][3]) < std::labs(opts[selected] - refresh)) refresh = modes[i][3];
    if (refresh != 72) { std::printf("0040a7f0_TEST FAIL\n"); return 1; }
    std::printf("0040a7f0_TEST PASS\n");
    return 0;
}
