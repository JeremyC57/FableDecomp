// Behaviour test for CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed (0x006C6040): an unknown handle informs nobody; witnesses are
// informed once (KnowsOfDeed guards a second report), the recipient only with post bit 1, the others only with post bit 2, guards are
// added with post bit 0x10 without duplicates, and the result says whether anyone was informed.
#include <cstdio>
#include <cstring>
struct Creature { bool alive, knows, eligible, guard; int informed; };
static bool Inform(int handle, Creature* c, int n, int recipient, int post)
{
    if (handle != 7) return false;
    int w[16]; int nw = 0; bool informed = false;
    if (post & 2) for (int i = 0; i < n; ++i) if (c[i].alive && c[i].eligible) w[nw++] = i;
    if (recipient >= 0 && c[recipient].alive) w[nw++] = recipient;
    if (post & 0x10)
        for (int i = 0; i < n; ++i)
            if (c[i].alive && c[i].guard) { bool dup = false; for (int k = 0; k < nw; ++k) dup = dup || w[k] == i; if (!dup) w[nw++] = i; }
    for (int k = 0; k < nw; ++k) {
        int x = w[k];
        if (((post & 1) && x == recipient) || ((post & 2) && x != recipient))
            if (!c[x].knows) { c[x].knows = true; ++c[x].informed; informed = true; }
    }
    return informed;
}
int main()
{
    Creature c[4] = { { true, false, true, false, 0 }, { true, false, false, true, 0 }, { false, false, true, false, 0 }, { true, false, false, false, 0 } };
    bool ok = !Inform(3, c, 4, 3, 3);
    ok = ok && Inform(7, c, 4, 3, 3) && c[0].informed == 1 && c[3].informed == 1 && c[2].informed == 0 && c[1].informed == 0;
    ok = ok && Inform(7, c, 4, -1, 0x12) && c[1].informed == 1 && c[0].informed == 1;
    ok = ok && !Inform(7, c, 4, 3, 3);
    if (!ok) { std::printf("006c6040_TEST FAIL\n"); return 1; }
    std::printf("006c6040_TEST PASS\n");
    return 0;
}
