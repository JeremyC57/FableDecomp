// Behaviour test for libvorbis mdct.c mdct_butterfly_32 (retail 0x00C9C000): the 32-point butterfly stage (with the 16- and
// 8-point stages it finishes with) against a double-precision reference written from the same mdct.c formulas, on two inputs;
// the first pair of the 32 stage is a plain copy (x[14] = r0, x[15] = r1), which is what distinguishes it from the 16 stage.
#include <cstdio>
#include <cmath>
typedef float DATA_TYPE; typedef float REG_TYPE;
#define cPI3_8 .38268343236508977175f
#define cPI2_8 .70710678118654752441f
#define cPI1_8 .92387953251128675613f
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
static void mdct_butterfly_32(DATA_TYPE *x){
  REG_TYPE r0     = x[30] - x[14];
  REG_TYPE r1     = x[31] - x[15];
           x[30] +=         x[14];
	   x[31] +=         x[15];
           x[14]  =         r0;
	   x[15]  =         r1;
           r0     = x[28] - x[12];
	   r1     = x[29] - x[13];
           x[28] +=         x[12];
	   x[29] +=         x[13];
           x[12]  = MULT_NORM( r0 * cPI1_8  -  r1 * cPI3_8 );
	   x[13]  = MULT_NORM( r0 * cPI3_8  +  r1 * cPI1_8 );
           r0     = x[26] - x[10];
	   r1     = x[27] - x[11];
	   x[26] +=         x[10];
	   x[27] +=         x[11];
	   x[10]  = MULT_NORM(( r0  - r1 ) * cPI2_8);
	   x[11]  = MULT_NORM(( r0  + r1 ) * cPI2_8);
	   r0     = x[24] - x[8];
	   r1     = x[25] - x[9];
	   x[24] += x[8];
	   x[25] += x[9];
	   x[8]   = MULT_NORM( r0 * cPI3_8  -  r1 * cPI1_8 );
	   x[9]   = MULT_NORM( r1 * cPI3_8  +  r0 * cPI1_8 );
	   r0     = x[22] - x[6];
	   r1     = x[7]  - x[23];
	   x[22] += x[6];
	   x[23] += x[7];
	   x[6]   = r1;
	   x[7]   = r0;
	   r0     = x[4]  - x[20];
	   r1     = x[5]  - x[21];
	   x[20] += x[4];
	   x[21] += x[5];
	   x[4]   = MULT_NORM( r1 * cPI1_8  +  r0 * cPI3_8 );
	   x[5]   = MULT_NORM( r1 * cPI3_8  -  r0 * cPI1_8 );
	   r0     = x[2]  - x[18];
	   r1     = x[3]  - x[19];
	   x[18] += x[2];
	   x[19] += x[3];
	   x[2]   = MULT_NORM(( r1  + r0 ) * cPI2_8);
	   x[3]   = MULT_NORM(( r1  - r0 ) * cPI2_8);
	   r0     = x[0]  - x[16];
	   r1     = x[1]  - x[17];
	   x[16] += x[0];
	   x[17] += x[1];
	   x[0]   = MULT_NORM( r1 * cPI3_8  +  r0 * cPI1_8 );
	   x[1]   = MULT_NORM( r1 * cPI1_8  -  r0 * cPI3_8 );
	   mdct_butterfly_16(x);
	   mdct_butterfly_16(x+16);
}

/* double-precision reference of the same stage, written as a rotation table: for pair k (0..7) of the lower half the
   difference (a, b) = (x[2k] - x[2k+16], x[2k+1] - x[2k+17]) (sign per mdct.c) is rotated by angle k*pi/8 */
static void ref_stage32(double *x){
    static const double c1 = 0.92387953251128675613, c2 = 0.70710678118654752441, c3 = 0.38268343236508977175;
    double y[32]; int k;
    for (k = 0; k < 32; k++) y[k] = x[k];
    /* sums into the upper half */
    for (k = 0; k < 16; k++) y[16 + k] = x[16 + k] + x[k];
    { double r0 = x[30]-x[14], r1 = x[31]-x[15]; y[14] = r0; y[15] = r1; }
    { double r0 = x[28]-x[12], r1 = x[29]-x[13]; y[12] = r0*c1 - r1*c3; y[13] = r0*c3 + r1*c1; }
    { double r0 = x[26]-x[10], r1 = x[27]-x[11]; y[10] = (r0-r1)*c2; y[11] = (r0+r1)*c2; }
    { double r0 = x[24]-x[8],  r1 = x[25]-x[9];  y[8]  = r0*c3 - r1*c1; y[9]  = r1*c3 + r0*c1; }
    { double r0 = x[22]-x[6],  r1 = x[7]-x[23];  y[6]  = r1;            y[7]  = r0; }
    { double r0 = x[4]-x[20],  r1 = x[5]-x[21];  y[4]  = r1*c1 + r0*c3; y[5]  = r1*c3 - r0*c1; }
    { double r0 = x[2]-x[18],  r1 = x[3]-x[19];  y[2]  = (r1+r0)*c2;    y[3]  = (r1-r0)*c2; }
    { double r0 = x[0]-x[16],  r1 = x[1]-x[17];  y[0]  = r1*c3 + r0*c1; y[1]  = r1*c1 - r0*c3; }
    for (k = 0; k < 32; k++) x[k] = y[k];
}
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
    double r0 = x[6] + x[2], r1 = x[6] - x[2], r2 = x[4] + x[0], r3 = x[4] - x[0], y[8];
    y[6] = r0 + r2; y[4] = r0 - r2;
    r0 = x[5] - x[1]; r2 = x[7] - x[3];
    y[0] = r1 + r0; y[2] = r1 - r0;
    r0 = x[5] + x[1]; r1 = x[7] + x[3];
    y[3] = r2 + r3; y[1] = r2 - r3; y[7] = r1 + r0; y[5] = r1 - r0;
    for (int k = 0; k < 8; k++) x[k] = y[k];
}
static void ref32(double *x){
    ref_stage32(x); ref_stage16(x); ref_stage16(x + 16);
    ref_stage8(x); ref_stage8(x + 8); ref_stage8(x + 16); ref_stage8(x + 24);
}

int main()
{
    int t, k;
    for (t = 0; t < 2; t++) {
        float x[32]; double r[32];
        for (k = 0; k < 32; k++) { x[k] = (float)(((k * 7 + t * 5) % 13) - 6) * 0.5f; r[k] = x[k]; }
        mdct_butterfly_32(x);
        ref32(r);
        for (k = 0; k < 32; k++)
            if (std::fabs(x[k] - r[k]) > 1e-4) { std::printf("00c9c000_TEST FAIL\n"); return 1; }
    }
    std::printf("00c9c000_TEST PASS\n");
    return 0;
}
