// Behaviour test for 0x00808E40 CThingFilter_IsValidTargetForPlayerCreature::operator() (model copy of the filter logic).
#include <cstdio>
#include <cstring>
struct TC { int classIsWindow; int shop; bool stealable; long dangerTimer; unsigned long targetingFlags; };
struct Thing {
    unsigned long bits[9]; unsigned char type; unsigned long typeFlags; TC tcs[128];
    bool HasTC(int id) const { return (bits[id >> 5] & (1UL << (id & 31))) != 0; }
    void Add(int id) { bits[id >> 5] |= 1UL << (id & 31); }
};
static bool g_check;
static bool Vuln(const Thing& t, const Thing&) { return t.tcs[0].shop == 7; }
struct Filter {
    bool Accept, Child, Weapon; const Thing* Player; const Thing* Only;
    bool operator()(const Thing& thing) const
    {
        if (Only) { if (Only == &thing) return true; return false; }
        if (Child) {
            if (thing.HasTC(38)) return false;
            if (thing.HasTC(53)) return false;
            if (thing.HasTC(13)) { if (thing.tcs[13].classIsWindow) return false; }
        }
        if (thing.HasTC(84) && thing.tcs[84].shop) {
            if (!thing.tcs[84].stealable) return false;
            if (!Accept) return false;
        }
        if (thing.type == 13 && Player->HasTC(54) && Player->tcs[54].dangerTimer >= 0) return false;
        if (!g_check || !Weapon) return true;
        if (thing.typeFlags & 8) { if (!Vuln(thing, *Player)) return false; }
        else if (thing.HasTC(9) && !(thing.tcs[9].targetingFlags & 0x1c)) return false;
        return true;
    }
};
static int fails;
static void check(bool got, bool want, int n) { if (got != want) { std::printf("case %d wrong\n", n); ++fails; } }
int main()
{
    static Thing p, t, u;
    std::memset(&p, 0, sizeof p); std::memset(&t, 0, sizeof t); std::memset(&u, 0, sizeof u);
    Filter f = { false, false, false, &p, 0 };
    check(f(t), true, 1);
    f.Only = &t; check(f(t), true, 2); check(f(u), false, 3); f.Only = 0;
    f.Child = true; t.Add(38); check(f(t), false, 4);
    std::memset(t.bits, 0, sizeof t.bits); t.Add(13); t.tcs[13].classIsWindow = 1; check(f(t), false, 5);
    t.tcs[13].classIsWindow = 0; check(f(t), true, 6); f.Child = false;
    t.Add(84); t.tcs[84].shop = 1; check(f(t), false, 7);
    t.tcs[84].stealable = true; check(f(t), false, 8); f.Accept = true; check(f(t), true, 9);
    t.type = 13; p.Add(54); p.tcs[54].dangerTimer = -1; check(f(t), true, 10);
    p.tcs[54].dangerTimer = 5; check(f(t), false, 11); t.type = 0;
    g_check = true; f.Weapon = true; t.typeFlags = 8; check(f(t), false, 12);
    t.tcs[0].shop = 7; check(f(t), true, 13);
    t.typeFlags = 0; t.Add(9); check(f(t), false, 14); t.tcs[9].targetingFlags = 4; check(f(t), true, 15);
    if (fails) { std::printf("00808e40_TEST FAIL\n"); return 1; }
    std::printf("00808e40_TEST PASS\n");
    return 0;
}