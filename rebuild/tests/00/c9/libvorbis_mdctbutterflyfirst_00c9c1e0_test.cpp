// Behaviour test for mdct_butterfly_first (0x00C9C1E0): the unrolled Xiph body against a loop-form reference.
#include <cstdio>
#include <cmath>
static void mdct_butterfly_first(float* T, float* x, int points)
{
    float* x1 = x + points - 8;
    float* x2 = x + (points >> 1) - 8;
    float r0, r1;
    do {
        r0 = x1[6] - x2[6]; r1 = x1[7] - x2[7]; x1[6] += x2[6]; x1[7] += x2[7];
        x2[6] = r1 * T[1] + r0 * T[0]; x2[7] = r1 * T[0] - r0 * T[1];
        r0 = x1[4] - x2[4]; r1 = x1[5] - x2[5]; x1[4] += x2[4]; x1[5] += x2[5];
        x2[4] = r1 * T[5] + r0 * T[4]; x2[5] = r1 * T[4] - r0 * T[5];
        r0 = x1[2] - x2[2]; r1 = x1[3] - x2[3]; x1[2] += x2[2]; x1[3] += x2[3];
        x2[2] = r1 * T[9] + r0 * T[8]; x2[3] = r1 * T[8] - r0 * T[9];
        r0 = x1[0] - x2[0]; r1 = x1[1] - x2[1]; x1[0] += x2[0]; x1[1] += x2[1];
        x2[0] = r1 * T[13] + r0 * T[12]; x2[1] = r1 * T[12] - r0 * T[13];
        x1 -= 8; x2 -= 8; T += 16;
    } while (x2 >= x);
}
int main()
{
    const int points = 64;
    float x[points], ref[points], T[16 * (points / 16)];
    int i;
    for (i = 0; i < points; i++) x[i] = ref[i] = (float)((i * 7) % 13) - 6.0f + 0.25f * i;
    for (i = 0; i < 16 * (points / 16); i++) T[i] = (float)std::cos(0.1 * i);
    // reference: pair k (from the top) of each half; block b uses T[16b + 4*(3-p)] for pair p of the block
    int half = points / 2;
    for (int b = 0; b < half / 8; b++) {
        for (int p = 3; p >= 0; p--) {
            int lo = half - 8 - 8 * b + 2 * p, hi = points - 8 - 8 * b + 2 * p;
            const float* t = T + 16 * b + 4 * (3 - p);
            float r0 = ref[hi] - ref[lo], r1 = ref[hi + 1] - ref[lo + 1];
            ref[hi] += ref[lo]; ref[hi + 1] += ref[lo + 1];
            ref[lo] = r1 * t[1] + r0 * t[0]; ref[lo + 1] = r1 * t[0] - r0 * t[1];
        }
    }
    mdct_butterfly_first(T, x, points);
    for (i = 0; i < points; i++)
        if (std::fabs(x[i] - ref[i]) > 1e-4f) { std::printf("00c9c1e0_TEST FAIL\n"); return 1; }
    std::printf("00c9c1e0_TEST PASS\n");
    return 0;
}