// Behaviour test for mdct_butterfly_generic (0x00C9C2F0): the unrolled Xiph body against a loop-form reference,
// with a trig stride (trigint) larger than 1.
#include <cstdio>
#include <cmath>
static void mdct_butterfly_generic(float* T, float* x, int points, int trigint)
{
    float* x1 = x + points - 8;
    float* x2 = x + (points >> 1) - 8;
    float r0, r1;
    do {
        r0 = x1[6] - x2[6]; r1 = x1[7] - x2[7]; x1[6] += x2[6]; x1[7] += x2[7];
        x2[6] = r1 * T[1] + r0 * T[0]; x2[7] = r1 * T[0] - r0 * T[1];
        T += trigint;
        r0 = x1[4] - x2[4]; r1 = x1[5] - x2[5]; x1[4] += x2[4]; x1[5] += x2[5];
        x2[4] = r1 * T[1] + r0 * T[0]; x2[5] = r1 * T[0] - r0 * T[1];
        T += trigint;
        r0 = x1[2] - x2[2]; r1 = x1[3] - x2[3]; x1[2] += x2[2]; x1[3] += x2[3];
        x2[2] = r1 * T[1] + r0 * T[0]; x2[3] = r1 * T[0] - r0 * T[1];
        T += trigint;
        r0 = x1[0] - x2[0]; r1 = x1[1] - x2[1]; x1[0] += x2[0]; x1[1] += x2[1];
        x2[0] = r1 * T[1] + r0 * T[0]; x2[1] = r1 * T[0] - r0 * T[1];
        T += trigint;
        x1 -= 8; x2 -= 8;
    } while (x2 >= x);
}
int main()
{
    const int points = 64;
    const int trigint = 8;
    float x[points], ref[points], T[trigint * (points / 2) + 2];
    int i;
    for (i = 0; i < points; i++) x[i] = ref[i] = (float)((i * 5) % 11) - 4.0f + 0.125f * i;
    for (i = 0; i < trigint * (points / 2) + 2; i++) T[i] = (float)std::sin(0.07 * i + 0.3);
    // reference: pairs walk down from the top of each half; pair number n (0 = topmost) uses T[n * trigint]
    int half = points / 2;
    for (int n = 0; n < half / 2; n++) {
        int lo = half - 2 - 2 * n, hi = points - 2 - 2 * n;
        const float* t = T + n * trigint;
        float r0 = ref[hi] - ref[lo], r1 = ref[hi + 1] - ref[lo + 1];
        ref[hi] += ref[lo]; ref[hi + 1] += ref[lo + 1];
        ref[lo] = r1 * t[1] + r0 * t[0]; ref[lo + 1] = r1 * t[0] - r0 * t[1];
    }
    mdct_butterfly_generic(T, x, points, trigint);
    for (i = 0; i < points; i++)
        if (std::fabs(x[i] - ref[i]) > 1e-4f) { std::printf("00c9c2f0_TEST FAIL\n"); return 1; }
    std::printf("00c9c2f0_TEST PASS\n");
    return 0;
}