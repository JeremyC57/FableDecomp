// behaviour test for 00c64fb0 CEntity property setter (mesh components): empty or invalid lists fail with -3 and the owner's
// last error -2 without storing anything; a valid list stores each pair by GUID index and flags the current component
#include <cstdio>
#include <cstring>
struct G { unsigned long a, b, c, d; };
static const G g_ids[4] = { { 1, 0, 0, 0 }, { 2, 0, 0, 0 }, { 3, 0, 0, 0 }, { 4, 0, 0, 0 } };
struct E { G id; long a; long b; };
struct Rec { long a; long b; };
struct Ent { unsigned long flags; long lastError; long current; Rec rec[4]; };
static int IndexOf(const G& g) { for (int k = 0; k < 4; ++k) if (std::memcmp(&g, &g_ids[k], sizeof(G)) == 0) return k; return -1; }
static long Set(Ent& t, const E* list, unsigned long size)
{
    unsigned long count = size / sizeof(E);
    if (count == 0) { t.lastError = -2; return -3; }
    unsigned long i;
    for (i = 0; i < count; ++i) {
        const E& e = list[i];
        if (IndexOf(e.id) < 0 || e.a < -10000 || e.a > 0 || e.b < -10000 || e.b > 0) { t.lastError = -2; return -3; }
    }
    for (i = 0; i < count; ++i) {
        int k = IndexOf(list[i].id);
        t.rec[k].a = list[i].a; t.rec[k].b = list[i].b;
        if (k == t.current) t.flags |= 0x400000;
    }
    return 0;
}
int main()
{
    Ent t; std::memset(&t, 0, sizeof(t)); t.current = 2;
    E ok[2] = { { g_ids[2], -100, -5 }, { g_ids[0], 0, -5 } };
    if (Set(t, ok, 0) != -3 || t.lastError != -2) { std::printf("00c64fb0_TEST FAIL\n"); return 1; }
    t.lastError = 0;
    if (Set(t, ok, sizeof(ok)) != 0 || t.rec[2].a != -100 || t.rec[0].b != -5 || !(t.flags & 0x400000)) { std::printf("00c64fb0_TEST FAIL\n"); return 1; }
    E bad[1] = { { g_ids[1], -1, 3 } };
    if (Set(t, bad, sizeof(bad)) != -3 || t.rec[1].a != 0 || t.lastError != -2) { std::printf("00c64fb0_TEST FAIL\n"); return 1; }
    std::printf("00c64fb0_TEST PASS\n");
    return 0;
}