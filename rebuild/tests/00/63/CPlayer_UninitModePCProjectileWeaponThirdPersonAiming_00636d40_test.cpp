// behaviour test for 0x00636D40 CPlayer::UninitModePCProjectileWeaponThirdPersonAiming: fifteen input processes leave
// the interface in retail order, the aiming flag clears and the controlled creature's mode 0x19 is removed.
#include <cstdio>
static int g_order[32]; static int g_count = 0;
struct Process { int id; void RemoveFromInterface() { g_order[g_count++] = id; } };
struct Modes { int removed; Modes() : removed(-1) { } void RemoveMode(int m) { removed = m; } };
struct Creature { Modes modes; };
struct Player {
    Process p[15]; bool aiming; Creature* creature;
    void Uninit()
    {
        for (int i = 0; i < 15; ++i) p[i].RemoveFromInterface();
        aiming = false;
        Modes* m = &creature->modes;
        m->RemoveMode(0x19);
    }
};
int main()
{
    Creature c; Player pl; pl.aiming = true; pl.creature = &c;
    for (int i = 0; i < 15; ++i) pl.p[i].id = 100 + i;
    pl.Uninit();
    bool ok = g_count == 15 && !pl.aiming && c.modes.removed == 0x19;
    for (int i = 0; i < 15; ++i) if (g_order[i] != 100 + i) ok = false;
    if (!ok) { std::printf("00636d40_TEST FAIL\n"); return 1; }
    std::printf("00636d40_TEST PASS\n");
    return 0;
}