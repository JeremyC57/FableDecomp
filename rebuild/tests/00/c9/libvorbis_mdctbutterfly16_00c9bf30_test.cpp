// Behaviour test for libvorbis mdct.c mdct_butterfly_16 (retail 0x00C9BF30): the 16-point butterfly stage (with the two 8-point
// stages it finishes with) against a double-precision reference written from the same mdct.c formulas, on two inputs; the stage's
// own pairs are the x[8..15] += x[0..7] sums, the cPI2_8 rotations of pairs 0 and 2 and the plain copies of pairs 1 and 3.
#include <cstdio>
#include <cmath>
typedef float DATA_TYPE; typedef float REG_TYPE;
#define cPI2_8 .70710678118654752441f
#define MULT_NORM(x) (x)

static void mdct_butterfly_8(DATA_TYPE *x){
  REG_TYPE r0   = x[6] + x[2];
  REG_TYPE r1   = x[6] - x[2];
  REG_TYPE r2   = x[4] + x[0];
  REG_TYPE r3   = x[4] - x[0];
	   x[6] = r0   + r2;
	   x[4] = r0   - r2;
	   r0   = x[5] - x[1];
	   r2   = x[7] - x[3];
	   x[0] = r1   + r0;
	   x[2] = r1   - r0;
	   r0   = x[5] + x[1];
	   r1   = x[7] + x[3];
	   x[3] = r2   + r3;
	   x[1] = r2   - r3;
	   x[7] = r1   + r0;
	   x[5] = r1   - r0;
}
static void mdct_butterfly_16(DATA_TYPE *x){
  REG_TYPE r0     = x[1]  - x[9];
  REG_TYPE r1     = x[0]  - x[8];
           x[8]  += x[0];
           x[9]  += x[1];
           x[0]   = MULT_NORM((r0   + r1) * cPI2_8);
           x[1]   = MULT_NORM((r0   - r1) * cPI2_8);
           r0     = x[3]  - x[11];
           r1     = x[10] - x[2];
           x[10] += x[2];
           x[11] += x[3];
           x[2]   = r0;
           x[3]   = r1;
           r0     = x[12] - x[4];
           r1     = x[13] - x[5];
           x[12] += x[4];
           x[13] += x[5];
           x[4]   = MULT_NORM((r0   - r1) * cPI2_8);
           x[5]   = MULT_NORM((r0   + r1) * cPI2_8);
           r0     = x[14] - x[6];
           r1     = x[15] - x[7];
           x[14] += x[6];
           x[15] += x[7];
           x[6]  = r0;
           x[7]  = r1;
	   mdct_butterfly_8(x);
	   mdct_butterfly_8(x+8);
}

/* double-precision references of the two stages, written from the same formulas */
static void ref_stage16(double *x){
    static const double c2 = 0.70710678118654752441;
    double y[16]; int k;
    for (k = 0; k < 16; k++) y[k] = x[k];
    for (k = 0; k < 8; k++) y[8 + k] = x[8 + k] + x[k];
    { double r0 = x[1]-x[9],  r1 = x[0]-x[8];  y[0] = (r0+r1)*c2; y[1] = (r0-r1)*c2; }
    { double r0 = x[3]-x[11], r1 = x[10]-x[2]; y[2] = r0; y[3] = r1; }
    { double r0 = x[12]-x[4], r1 = x[13]-x[5]; y[4] = (r0-r1)*c2; y[5] = (r0+r1)*c2; }
    { double r0 = x[14]-x[6], r1 = x[15]-x[7]; y[6] = r0; y[7] = r1; }
    for (k = 0; k < 16; k++) x[k] = y[k];
}
static void ref_stage8(double *x){
    double r0 = x[6]+x[2], r1 = x[6]-x[2], r2 = x[4]+x[0], r3 = x[4]-x[0], y[8];
    y[6] = r0 + r2; y[4] = r0 - r2;
    r0 = x[5]-x[1]; r2 = x[7]-x[3];
    y[0] = r1 + r0; y[2] = r1 - r0;
    r0 = x[5]+x[1]; r1 = x[7]+x[3];
    y[3] = r2 + r3; y[1] = r2 - r3; y[7] = r1 + r0; y[5] = r1 - r0;
    for (int k = 0; k < 8; k++) x[k] = y[k];
}

static int fail(const char* what) { std::printf("%s\n00c9bf30_TEST FAIL\n", what); return 1; }

int main()
{
    float x[16]; double r[16]; int i, pass;
    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < 16; i++) {
            double v = pass ? std::sin(0.7 * i + 0.3) * 4.0 : (double)((i * 5) % 7) - 3.0;
            x[i] = (float)v; r[i] = (double)x[i];
        }
        mdct_butterfly_16(x);
        ref_stage16(r); ref_stage8(r); ref_stage8(r + 8);
        for (i = 0; i < 16; i++)
            if (std::fabs((double)x[i] - r[i]) > 1e-4) return fail("stage mismatch");
    }
    /* the stage is linear: doubling the input doubles the output */
    {
        float a[16], b[16];
        for (i = 0; i < 16; i++) { a[i] = (float)(i % 5) - 2.f; b[i] = 2.f * a[i]; }
        mdct_butterfly_16(a); mdct_butterfly_16(b);
        for (i = 0; i < 16; i++) if (std::fabs(b[i] - 2.f * a[i]) > 1e-5f) return fail("linearity");
    }
    std::printf("00c9bf30_TEST PASS\n");
    return 0;
}