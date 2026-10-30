// Behaviour test for 0x00C9C500 libvorbis mdct_bitreverse: one and two iterations checked against the formula written out
// element by element.
#include <cstdio>
#include <cmath>

typedef struct { int n; int log2n; float* trig; int* bitrev; float scale; } mdct_lookup;

static void mdct_bitreverse(mdct_lookup* init, float* x)
{
    int n = init->n;
    int* bit = init->bitrev;
    float* w0 = x;
    float* w1 = x = w0 + (n >> 1);
    float* T = init->trig + n;
    do {
        float* x0 = x + bit[0];
        float* x1 = x + bit[1];
        float r0 = x0[1] - x1[1];
        float r1 = x0[0] + x1[0];
        float r2 = r1 * T[0] + r0 * T[1];
        float r3 = r1 * T[1] - r0 * T[0];
        w1 -= 4;
        r0 = (x0[1] + x1[1]) * .5f;
        r1 = (x0[0] - x1[0]) * .5f;
        w0[0] = r0 + r2;
        w1[2] = r0 - r2;
        w0[1] = r1 + r3;
        w1[3] = r3 - r1;
        x0 = x + bit[2];
        x1 = x + bit[3];
        r0 = x0[1] - x1[1];
        r1 = x0[0] + x1[0];
        r2 = r1 * T[2] + r0 * T[3];
        r3 = r1 * T[3] - r0 * T[2];
        r0 = (x0[1] + x1[1]) * .5f;
        r1 = (x0[0] - x1[0]) * .5f;
        w0[2] = r0 + r2;
        w1[0] = r0 - r2;
        w0[3] = r1 + r3;
        w1[1] = r3 - r1;
        T += 4;
        bit += 4;
        w0 += 4;
    } while (w0 < w1);
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    // n = 16: one iteration; reads the upper half u = x+8 through bitrev, writes w0 = x[0..3] and w1 = x[4..7]
    float trig[24];
    for (int i = 0; i < 24; ++i) trig[i] = 0.1f * (float)(i + 1);
    int bitrev[4] = { 2, 0, 4, 6 };
    float x[16];
    for (int i = 0; i < 16; ++i) x[i] = (i < 8) ? -99.0f : (float)(i * 3 % 11) + 0.5f;
    float u[8];
    for (int i = 0; i < 8; ++i) u[i] = x[8 + i];
    mdct_lookup l; l.n = 16; l.log2n = 4; l.trig = trig; l.bitrev = bitrev; l.scale = 1.0f;
    const float* T = trig + 16;
    // first pair: x0 = u+2, x1 = u+0
    float a_r0 = u[3] - u[1], a_r1 = u[2] + u[0];
    float a_r2 = a_r1 * T[0] + a_r0 * T[1], a_r3 = a_r1 * T[1] - a_r0 * T[0];
    float a_h0 = (u[3] + u[1]) * .5f, a_h1 = (u[2] - u[0]) * .5f;
    // second pair: x0 = u+4, x1 = u+6
    float b_r0 = u[5] - u[7], b_r1 = u[4] + u[6];
    float b_r2 = b_r1 * T[2] + b_r0 * T[3], b_r3 = b_r1 * T[3] - b_r0 * T[2];
    float b_h0 = (u[5] + u[7]) * .5f, b_h1 = (u[4] - u[6]) * .5f;
    mdct_bitreverse(&l, x);
    int bad = !near(x[0], a_h0 + a_r2) || !near(x[1], a_h1 + a_r3) || !near(x[2], b_h0 + b_r2) || !near(x[3], b_h1 + b_r3)
              || !near(x[6], a_h0 - a_r2) || !near(x[7], a_r3 - a_h1) || !near(x[4], b_h0 - b_r2) || !near(x[5], b_r3 - b_h1);
    for (int i = 0; i < 8; ++i) if (x[8 + i] != u[i]) bad = 1;   // the upper half is only read
    // n = 32: two iterations, bitrev and T advance by 4 per iteration; every lower-half element is written
    float trig2[48];
    for (int i = 0; i < 48; ++i) trig2[i] = 0.05f * (float)(i % 7);
    int bitrev2[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
    float y[32];
    for (int i = 0; i < 32; ++i) y[i] = (i < 16) ? -99.0f : (float)(i - 16) * 0.25f;
    mdct_lookup l2; l2.n = 32; l2.log2n = 5; l2.trig = trig2; l2.bitrev = bitrev2; l2.scale = 1.0f;
    mdct_bitreverse(&l2, y);
    int written = 0;
    for (int i = 0; i < 16; ++i) if (y[i] != -99.0f) ++written;
    if (written != 16) bad = 1;
    // second iteration, first pair: x0 = y+16+1, x1 = y+16+5, T = trig2 + 32 + 4; w0 = y+4, w1 = y+8
    float* v = y + 16;
    float c_r0 = v[2] - v[6], c_r1 = v[1] + v[5];
    float c_r2 = c_r1 * trig2[36] + c_r0 * trig2[37];
    if (!near(y[4], (v[2] + v[6]) * .5f + c_r2) || !near(y[10], (v[2] + v[6]) * .5f - c_r2)) bad = 1;
    if (bad) { std::printf("00c9c500_TEST FAIL\n"); return 1; }
    std::printf("00c9c500_TEST PASS\n");
    return 0;
}