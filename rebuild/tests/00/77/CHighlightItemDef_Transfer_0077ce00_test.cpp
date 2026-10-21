// Behaviour test for CHighlightItemDef::Transfer @ 0x0077CE00: CRC check per member, load in mode 2,
// save in mode 3, nothing otherwise; retail member order.
#include <cstdio>
struct Def { bool run; int pe, ppe; float rot, dist, bob, bobt, delay; };
static int crc = 0; static char trace[64]; static int n = 0;
struct In { int next; };
struct Out { char kinds[16]; float vals[16]; int k; };
struct Ctx {
    int mode; In* in; Out* out;
    void Check() { ++crc; }
    void T(bool& v) { Check(); if (mode == 2) { v = in->next++ != 0; trace[n++] = 'b'; } else if (mode == 3) { out->kinds[out->k] = 'b'; out->vals[out->k++] = v ? 1.0f : 0.0f; } }
    void T(int& v) { Check(); if (mode == 2) { v = in->next++; trace[n++] = 'l'; } else if (mode == 3) { out->kinds[out->k] = 'l'; out->vals[out->k++] = (float)v; } }
    void T(float& v) { Check(); if (mode == 2) { v = (float)in->next++; trace[n++] = 'f'; } else if (mode == 3) { out->kinds[out->k] = 'f'; out->vals[out->k++] = v; } }
};
static void Transfer(Def& d, Ctx& c)
{
    c.T(d.run); c.T(d.pe); c.T(d.ppe); c.T(d.rot); c.T(d.dist); c.T(d.bobt); c.T(d.bob); c.T(d.delay);
}
int main()
{
    Def d; In in; in.next = 1; Out out; out.k = 0; Ctx c; c.in = &in; c.out = &out;
    c.mode = 2; Transfer(d, c);
    if (!d.run || d.pe != 2 || d.ppe != 3 || d.rot != 4.0f || d.dist != 5.0f || d.bobt != 6.0f || d.bob != 7.0f || d.delay != 8.0f) { std::printf("0077ce00_TEST FAIL\n"); return 1; }
    c.mode = 3; Transfer(d, c);
    if (out.k != 8 || out.kinds[0] != 'b' || out.kinds[1] != 'l' || out.kinds[7] != 'f' || out.vals[5] != 6.0f || out.vals[6] != 7.0f) { std::printf("0077ce00_TEST FAIL\n"); return 1; }
    c.mode = 1; Transfer(d, c);
    if (crc != 24 || n != 8 || out.k != 8) { std::printf("0077ce00_TEST FAIL\n"); return 1; }
    std::printf("0077ce00_TEST PASS\n");
    return 0;
}