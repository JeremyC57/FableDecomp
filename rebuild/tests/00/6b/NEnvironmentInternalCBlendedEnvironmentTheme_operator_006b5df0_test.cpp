#include <cstdio>
static int g_assigns = 0;
template <class T> struct Arr { T v; Arr() : v(0) {} Arr& operator=(const Arr& o) { v = o.v; ++g_assigns; return *this; } };
struct Col { float R, G, B, A; };
struct Set { Arr<long> Textures; Arr<float> Blends; };
struct Theme {
#if 1
    float TimeOfDay;
#endif
    Col Colours[19]; float Scalers[30]; Set TextureSets[3];
};
int main()
{
    Theme a, theme;
#if 1
    theme.TimeOfDay = 0.5f; a.TimeOfDay = 0;
#endif
    for (int i = 0; i < 19; ++i) { theme.Colours[i].R = (float)i; theme.Colours[i].A = 1; a.Colours[i].R = -1; a.Colours[i].A = 0; }
    for (int i = 0; i < 30; ++i) { theme.Scalers[i] = (float)(i * 2); a.Scalers[i] = 0; }
    for (int i = 0; i < 3; ++i) { theme.TextureSets[i].Textures.v = i + 10; theme.TextureSets[i].Blends.v = i * 0.25f; }
    a = theme;
    bool ok = g_assigns == 6 && a.Colours[18].R == 18 && a.Colours[0].A == 1 && a.Scalers[29] == 58 && a.TextureSets[2].Textures.v == 12 && a.TextureSets[1].Blends.v == 0.25f;
#if 1
    ok = ok && a.TimeOfDay == 0.5f;
#endif
    if (!ok) { std::printf("006b5df0_TEST FAIL\n"); return 1; }
    std::printf("006b5df0_TEST PASS\n");
    return 0;
}