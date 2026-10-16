#include <cstdio>
// Behaviour model of CTCHeroRangedAttacks::ResetProjectileWeaponMode (0x00789B30).
struct Player { bool modes[64];
    void Remove(int m) { modes[m] = false; } bool Has(int m) { return modes[m]; } void Add(int m) { modes[m] = true; } };
struct Ranged { Player* p; bool firing; long zoom;
    void Reset()
    {
        zoom = -1; p->Remove(0x19);
        if (firing) { if (!p->Has(0x22) && !p->Has(0x2e)) p->Add(0x22); }
    } };
int main()
{
    Player pl; for (int i = 0; i < 64; ++i) pl.modes[i] = false;
    pl.modes[0x19] = true;
    Ranged r; r.p = &pl; r.firing = false; r.zoom = 3;
    r.Reset();
    if (r.zoom != -1 || pl.modes[0x19] || pl.modes[0x22]) { std::printf("00789b30_TEST FAIL\n"); return 1; }
    r.firing = true; r.Reset();
    if (!pl.modes[0x22]) { std::printf("00789b30_TEST FAIL\n"); return 1; }
    pl.modes[0x22] = false; pl.modes[0x2e] = true; r.Reset();
    if (pl.modes[0x22]) { std::printf("00789b30_TEST FAIL\n"); return 1; }
    std::printf("00789b30_TEST PASS\n");
    return 0;
}