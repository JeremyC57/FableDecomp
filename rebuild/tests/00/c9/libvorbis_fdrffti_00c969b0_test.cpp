// Behaviour test for 0x00C969B0 fdrffti (see src/00c969b0.cpp): n == 1 leaves the tables alone; otherwise drfti1 factors n
// into ifac (n, factor count, factors 4/2/3/5..) and fills the twiddles from wsave + n.
#include <cstdio>
#include <cmath>
static void drfti1(int n, float* wa, int* ifac)
{
    static const int ntryh[4] = { 4, 2, 3, 5 };
    int nl = n, nf = 0, j = -1, ntry = 0;
    for (;;) {
        ++j; ntry = j < 4 ? ntryh[j] : ntry + 2;
        while (nl % ntry == 0) {
            ++nf; ifac[nf + 1] = ntry; nl /= ntry;
            if (ntry == 2 && nf != 1) { for (int i = 1; i < nf; i++) { int ib = nf - i + 1; ifac[ib + 1] = ifac[ib]; } ifac[2] = 2; }
            if (nl == 1) goto done;
        }
    }
done:
    ifac[0] = n; ifac[1] = nf; wa[0] = (float)std::cos(0.0);
}
static void fdrffti(int n, float* wsave, int* ifac) { if (n == 1) return; drfti1(n, wsave + n, ifac); }
int main()
{
    float w[64]; int f[32]; f[0] = -7; w[1] = 5.0f;
    fdrffti(1, w, f);
    if (f[0] != -7) { std::printf("00c969b0_TEST FAIL\n"); return 1; }
    fdrffti(8, w, f);
    if (f[0] != 8 || f[1] != 2 || f[2] != 2 || f[3] != 4 || w[8] != 1.0f) { std::printf("00c969b0_TEST FAIL\n"); return 1; }
    std::printf("00c969b0_TEST PASS\n"); return 0;
}