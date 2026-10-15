// Behaviour test for CHeroCombatDef::~CHeroCombatDef (0x00458B9D): heavy-weapons package, the two POD vectors (freed when
// allocated), the seven stat-weight maps in reverse order, then the parent def class.
#include <cstdio>
#include <cstdlib>
static int g_order[16]; static int g_n; static int g_frees;
struct Weight { int id; ~Weight() { g_order[g_n++] = id; } };
struct Package { ~Package() { g_order[g_n++] = 100; } };
struct Pod { void* first; ~Pod() { if (first) { std::free(first); ++g_frees; } } };
struct ParentDef { ~ParentDef() { g_order[g_n++] = 0; } };
struct CombatDef : ParentDef {
    Weight W1, W2, W3, W4, W5, W6, W7; Pod DefaultCombatAbilities, FOV; Package HeavyWeapons;
};
int main()
{
    CombatDef* d = new CombatDef;
    d->W1.id = 1; d->W2.id = 2; d->W3.id = 3; d->W4.id = 4; d->W5.id = 5; d->W6.id = 6; d->W7.id = 7;
    d->DefaultCombatAbilities.first = std::malloc(8); d->FOV.first = 0;
    delete d;
    const int want[9] = { 100, 7, 6, 5, 4, 3, 2, 1, 0 };
    for (int i = 0; i < 9; ++i) if (g_order[i] != want[i]) { std::printf("00458b9d_TEST FAIL\n"); return 1; }
    if (g_frees != 1) { std::printf("00458b9d_TEST FAIL\n"); return 1; }
    std::printf("00458b9d_TEST PASS\n");
    return 0;
}