#include <cstdio>
// Behaviour model of CCombatActionBase::~CCombatActionBase (0x0062E900): members are destroyed in reverse declaration
// order, then the base destructor runs.
static int g_order[8]; static int g_n = 0;
struct M { int id; ~M() { g_order[g_n++] = id; } };
struct Base { ~Base() { g_order[g_n++] = 0; } };
struct Combat : Base { M target, camera, def, strikes, anims, weapon; Combat() { target.id = 1; camera.id = 2; def.id = 3; strikes.id = 4; anims.id = 5; weapon.id = 6; } };
int main()
{
    { Combat c; }
    static const int want[7] = { 6, 5, 4, 3, 2, 1, 0 };
    if (g_n != 7) { std::printf("0062e900_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (g_order[i] != want[i]) { std::printf("0062e900_TEST FAIL\n"); return 1; }
    std::printf("0062e900_TEST PASS\n");
    return 0;
}
