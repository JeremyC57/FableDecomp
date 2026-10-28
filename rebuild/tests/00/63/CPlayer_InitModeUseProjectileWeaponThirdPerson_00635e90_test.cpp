// behaviour test for 0x00635E90 CPlayer::InitModeUseProjectileWeaponThirdPerson: the twelve input processes join the
// player interface in retail order, then a live controlled creature gets mode 0x19 unless it is already current.
#include <cstdio>
static int g_order[32]; static int g_count = 0;
struct Manager { };
struct Process { int id; void AddToInterface(Manager&) { g_order[g_count++] = id; } };
struct Modes { int current; int added; Modes() : current(0), added(-1) { } int GetType() const { return current; } void AddMode(int m) { added = m; } };
struct Creature { bool dead; Modes modes; };
struct Player {
    Process p[12]; Manager* iface; Creature* creature;
    void Init()
    {
        for (int i = 0; i < 12; ++i) p[i].AddToInterface(*iface);
        Creature* c = creature;
        if (c && !c->dead) {
            Modes* m = &c->modes;
            if (m->GetType() != 0x19) m->AddMode(0x19);
        }
    }
};
int main()
{
    Manager mgr; Creature c; c.dead = false; Player pl; pl.iface = &mgr; pl.creature = &c;
    for (int i = 0; i < 12; ++i) pl.p[i].id = i;
    pl.Init();
    bool ok = g_count == 12 && c.modes.added == 0x19;
    for (int i = 0; i < 12; ++i) if (g_order[i] != i) ok = false;
    c.modes.added = -1; c.modes.current = 0x19; g_count = 0; pl.Init();
    if (c.modes.added != -1 || g_count != 12) ok = false;
    c.modes.current = 0; c.dead = true; pl.Init();
    if (c.modes.added != -1) ok = false;
    pl.creature = 0; pl.Init();
    if (!ok) { std::printf("00635e90_TEST FAIL\n"); return 1; }
    std::printf("00635e90_TEST PASS\n");
    return 0;
}