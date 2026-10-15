// Behaviour test for 0x004E3D4A (CWeaponDef destructor): members are destroyed in reverse declaration order, then the base.
#include <cstdio>
static char g_log[32]; static int g_n = 0;
struct Tracked { char tag; Tracked(char t) : tag(t) {} ~Tracked() { g_log[g_n++] = tag; } };
struct Root { virtual ~Root() { g_log[g_n++] = 'R'; } };
struct DefPointeeBase : Root { };
struct WeaponDef : DefPointeeBase {
    Tracked AnimationGroupName, FlourishParticles, WeaponTrails, HelperTip, HelperBase, ArrowTrails, UnsheatheCombat, UnsheatheNormal, SheatheCombat, SheatheNormal;
    WeaponDef() : AnimationGroupName('a'), FlourishParticles('b'), WeaponTrails('c'), HelperTip('d'), HelperBase('e'), ArrowTrails('f'),
                  UnsheatheCombat('g'), UnsheatheNormal('h'), SheatheCombat('i'), SheatheNormal('j') {}
};
int main()
{
    { WeaponDef w; }
    g_log[g_n] = 0;
    const char* want = "jihgfedcbaR";
    int ok = 1;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { ok = 0; break; }
    if (!ok) { std::printf("004e3d4a_TEST FAIL\n"); return 1; }
    std::printf("004e3d4a_TEST PASS\n");
    return 0;
}
