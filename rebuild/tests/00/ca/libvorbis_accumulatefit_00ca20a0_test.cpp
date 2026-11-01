// Test for libvorbis 1.0.1 floor1.c accumulate_fit (0x00CA20A0): quantised floor points split by mdct+atten >= floor
// into the "a" (used) and "b" sets, b absorbs a, and the a sums are weighted by nb*twofitweight/(na+1).
#include <cstdio>
#include <cstring>
struct Info { float twofitweight; float twofitatten; };
struct Acc { long x0, x1, xa, ya, x2a, y2a, xya, an; };
static int dBquant(const float* x) { int i = (int)(*x * 7.3142857f + 1023.5f); if (i > 1023) return 1023; if (i < 0) return 0; return i; }
static int accumulate_fit(const float* flr, const float* mdct, int x0, int x1, Acc* a, int n, const Info* info)
{
    long xa = 0, ya = 0, x2a = 0, y2a = 0, xya = 0, na = 0, xb = 0, yb = 0, x2b = 0, y2b = 0, xyb = 0, nb = 0;
    std::memset(a, 0, sizeof(*a)); a->x0 = x0; a->x1 = x1; if (x1 >= n) x1 = n - 1;
    for (long i = x0; i <= x1; i++) {
        int q = dBquant(flr + i);
        if (!q) continue;
        if (mdct[i] + info->twofitatten >= flr[i]) { xa += i; ya += q; x2a += i * i; y2a += q * q; xya += i * q; na++; }
        else { xb += i; yb += q; x2b += i * i; y2b += q * q; xyb += i * q; nb++; }
    }
    xb += xa; yb += ya; x2b += x2a; y2b += y2a; xyb += xya; nb += na;
    int weight = (int)(nb * info->twofitweight / (na + 1));
    a->xa = xa * weight + xb; a->ya = ya * weight + yb; a->x2a = x2a * weight + x2b;
    a->y2a = y2a * weight + y2b; a->xya = xya * weight + xyb; a->an = na * weight + nb;
    return (int)na;
}
int main()
{
    float flr[4] = { 0.0f, 0.0f, 0.0f, -200.0f };     // index 3 quantises to 0 and is skipped
    float mdct[4] = { 1.0f, -5.0f, 1.0f, 0.0f };      // index 1 falls in the "b" set
    Info info; info.twofitweight = 2.0f; info.twofitatten = 0.0f;
    Acc a;
    int na = accumulate_fit(flr, mdct, 0, 9, &a, 4, &info);
    // q = 1024 -> clamps to 1023 for every non-skipped point; a = {0,2}, b = {1}; nb = 3, weight = 3*2/3 = 2
    bool ok = na == 2 && a.x0 == 0 && a.x1 == 9 && a.an == 2 * 2 + 3 && a.xa == 2 * 2 + 3
           && a.ya == 2046 * 2 + 3069 && a.x2a == 4 * 2 + 5;
    std::printf(ok ? "00ca20a0_TEST PASS\n" : "00ca20a0_TEST FAIL\n");
    return ok ? 0 : 1;
}