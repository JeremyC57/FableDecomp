// test_CEngineLightingSSE.cpp -- dual-path test of engine/CEngineLightingSSE.h (GF_SSE_CALC_SW_LIGHTING_NO_CLIP and
// ..._SEPARATE_MAIN_LIGHT, the 0x00B44820 / 0x00B44A60 SSE kernels) against an in-test lane loop and (dual) the twin.
// rsqrtps / rcpps are approximate (Intel: relative error <= 1.5 * 2^-12; docs/pipeline/ASM_RULES.md "Approximate
// instructions"), the reference and the twin exact. Each case states k, the number of approximate instructions an output
// passed through: k = 0 must be bit-identical (dyadic inputs: every other op is exact), k > 0 is checked within
// (1 + 1.5 * 2^-12)^k - 1 relative, plus 8 single-precision ulps for the roundings after them. All contributions are positive
// (no cancellation), so the relative bound of each light term carries to the sum.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "engine/CEngineLightingSSE.h"
#include "fable_test_mode.h"

struct FABLE_TEST_ALIGN16 Block { float r[17][4]; };
struct Mgr { char pad[0x4690]; float* SIMDVarBlock; };
struct Lights { int Count; float L[3][13]; };   // a 32-bit count, then 0x34-byte lights
struct FABLE_TEST_ALIGN16 Stream { float v[8]; };
struct FABLE_TEST_ALIGN16 Vec4x8 { float v[32]; };

static void RunNoClip(Mgr* self, float* colours, const float* positions, const float* normalX, const float* normalY,
                      const float* normalZ, long count, const Lights& lights)
{
    GF_SSE_CALC_SW_LIGHTING_NO_CLIP(self, colours, positions, normalX, normalY, normalZ, count, lights);
}
static void RunSeparate(Mgr* self, float* colours, float* mainLightColours, const float* positions, const float* normalX,
                        const float* normalY, const float* normalZ, long count, const Lights& lights)
{
    GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(self, colours, mainLightColours, positions, normalX, normalY, normalZ,
                                                        count, lights);
}

// the reference lane loop (written from the instruction list, independently of the header's twin)
static float Mx(float a, float b) { return a > b ? a : b; }
static float Mn(float a, float b) { return a < b ? a : b; }
static void Reference(Block* bk, float* col, float* mainCol, const float* pos, const float* nX, const float* nY, const float* nZ,
                      int quads, const Lights& lt)
{
    float (*b)[4] = bk->r;
    for (int q = 0; q < quads; ++q)
        for (int l = 0; l < 4; ++l) {
            int v = 4 * q + l;
            float t1 = nY[v] * b[3][l];
            float d = nX[v] * b[2][l] + t1;
            d = d + nZ[v] * b[4][l];
            float k = (t1 - t1) - d;
            d = Mx(d, b[0][l]);
            k = Mx(k, b[0][l]);
            k = Mn(k * k, b[1][l]);
            float c[3];
            for (int ch = 0; ch < 3; ++ch) {
                if (mainCol) { mainCol[4 * v + ch] = b[5 + ch][l] * k; c[ch] = b[8 + ch][l] * d + b[11 + ch][l]; }
                else c[ch] = (b[8 + ch][l] * d + b[5 + ch][l] * k) + b[11 + ch][l];
            }
            if (mainCol) mainCol[4 * v + 3] = mainCol[4 * v + 2];
            for (int i = 0; i < lt.Count; ++i) {
                const float* L = lt.L[i];
                float vx = L[0] - pos[4 * v + 0], vy = L[1] - pos[4 * v + 1], vz = L[2] - pos[4 * v + 2];
                float qd = (vx * vx + vy * vy) + vz * vz;
                float n = Mx(((vx * nX[v] + vy * nY[v]) + vz * nZ[v]) * (float)(1.0 / sqrt((double)qd)), b[0][l]);
                float a = Mn(Mx(((float)(1.0 / (double)(qd * L[10])) + L[11]) * n, b[0][l]), b[1][l]);
                for (int ch = 0; ch < 3; ++ch) c[ch] = c[ch] + L[6 + ch] * a;
            }
            col[4 * v + 0] = c[0]; col[4 * v + 1] = c[1]; col[4 * v + 2] = c[2]; col[4 * v + 3] = c[2];
        }
}

static int Close(const float* got, const float* want, int n, int k, const char* what)
{
    int bad = 0;
    const double tol = pow(1.0 + 1.5 / 4096.0, k) - 1.0 + 8.0 / 16777216.0;
    for (int i = 0; i < n; ++i) {
        int ok = k == 0 ? memcmp(&got[i], &want[i], sizeof(float)) == 0
                        : fabs((double)got[i] - (double)want[i]) <= tol * fabs((double)want[i]);
        if (!ok) { if (bad < 4) printf("  %s[%d] = %.9g, want %.9g (k=%d)\n", what, i, got[i], want[i], k); ++bad; }
    }
    return bad;
}

struct Case { const char* name; int k; int lights; float scale; float bias; float rowD; float rowAdd; float lightZ; int up; };

int main()
{
    int bad = 0;
    static const Case cases[] = {
        // k = 0: no light reaches the output, every value dyadic
        { "no lights", 0, 0, 1.0f, 0.0f, 0.5f, 0.25f, 0.0f, 0 },
        // k = 0: the attenuation clamps to the ceiling (bias 4, light high above +Z normals) whatever the approximations give
        { "clamped to ceiling", 0, 2, 1.0f, 4.0f, 0.5f, 0.25f, 4.0f, 1 },
        // k = 0: the light is behind every vertex (N.L < 0, clamped to the floor)
        { "behind", 0, 1, 1.0f, 0.5f, 0.5f, 0.25f, -4.0f, 1 },
        // k = 1: rsqrtps only (scale 2^40: rcp(q * scale) < 2^-26 vanishes against the bias 0.5); D and constant rows zero
        { "rsqrt only", 1, 1, 1099511627776.0f, 0.5f, 0.0f, 0.0f, 3.0f, 1 },
        // k = 2: rsqrtps and rcpps, three lights over positive D / constant terms
        { "rsqrt and rcp", 2, 3, 0.25f, 0.125f, 0.5f, 0.25f, 3.0f, 0 },
    };
    Mgr* mgr = new Mgr;
    for (unsigned ci = 0; ci < sizeof cases / sizeof cases[0]; ++ci) {
        const Case& c = cases[ci];
        for (int separate = 0; separate < 2; ++separate) {
            Block blk; memset(&blk, 0, sizeof blk);
            for (int l = 0; l < 4; ++l) {
                blk.r[0][l] = 0.0f; blk.r[1][l] = 1.0f;                                     // floor, ceiling
                blk.r[2][l] = 0.0f; blk.r[3][l] = 0.5f; blk.r[4][l] = 0.5f;                 // direction
                for (int ch = 0; ch < 3; ++ch) {
                    blk.r[5 + ch][l] = 0.125f * (ch + 1) * (c.rowD != 0.0f);                  // scaled by k
                    blk.r[8 + ch][l] = c.rowD * (ch + 1);                                     // scaled by d
                    blk.r[11 + ch][l] = c.rowAdd;                                             // added
                }
                blk.r[14][l] = blk.r[15][l] = blk.r[16][l] = -99.0f;                        // scratch sentinel
            }
            mgr->SIMDVarBlock = &blk.r[0][0];
            Stream nx, ny, nz; Vec4x8 pos;
            for (int v = 0; v < 8; ++v) {
                nx.v[v] = (v & 1) ? 0.5f : -0.25f; ny.v[v] = (v & 2) ? 0.75f : -0.5f; nz.v[v] = (v & 4) ? 0.5f : 0.25f;
                if (c.up) { nx.v[v] = 0.0f; ny.v[v] = 0.0f; nz.v[v] = 0.5f + 0.125f * v; }   // facing +Z
                pos.v[4 * v + 0] = 0.25f * v; pos.v[4 * v + 1] = -0.5f * (v & 3); pos.v[4 * v + 2] = 0.125f * v - 0.5f;
                pos.v[4 * v + 3] = 1.0f;
            }
            Lights lt; memset(&lt, 0, sizeof lt); lt.Count = c.lights;
            for (int i = 0; i < 3; ++i) {
                float* L = lt.L[i];
                L[0] = 1.0f + 0.5f * i; L[1] = 0.5f - 0.25f * i; L[2] = c.lightZ + 0.75f * i;
                L[6] = 1.0f; L[7] = 0.5f + 0.25f * i; L[8] = 0.25f; L[10] = c.scale; L[11] = c.bias;
            }
            FABLE_TEST_ALIGN16 float col[32], mainCol[32], rcol[32], rmain[32];
            memset(col, 0x7f, sizeof col); memset(mainCol, 0x7f, sizeof mainCol);
            Block before = blk;
            if (separate) RunSeparate(mgr, col, mainCol, pos.v, nx.v, ny.v, nz.v, 2, lt);
            else RunNoClip(mgr, col, pos.v, nx.v, ny.v, nz.v, 2, lt);
            Block after = blk;
            Reference(&before, rcol, separate ? rmain : 0, pos.v, nx.v, ny.v, nz.v, 2, lt);
            char what[64];
            sprintf(what, "%s/%s colours", c.name, separate ? "separate" : "noclip");
            bad += Close(col, rcol, 32, c.k, what);
            if (separate) { sprintf(what, "%s main", c.name); bad += Close(mainCol, rmain, 32, 0, what); }
            // the scratch rows: untouched without lights, else the second quad's positions transposed
            for (int l = 0; l < 4; ++l)
                for (int r = 0; r < 3; ++r) {
                    float want = c.lights ? pos.v[4 * (4 + l) + r] : -99.0f;
                    if (memcmp(&after.r[14 + r][l], &want, sizeof want)) { printf("  %s scratch %d.%d\n", c.name, r, l); ++bad; }
                }
#ifdef FABLE_TEST_DUAL
            Block tb = before;
            FABLE_TEST_ALIGN16 float tcol[32], tmain[32];
            fable_portable::CalcSWLightingNoClipSSE(&tb.r[0][0], tcol, separate ? tmain : 0, pos.v, nx.v, ny.v, nz.v, 2u, &lt);
            sprintf(what, "%s/%s twin", c.name, separate ? "separate" : "noclip");
            bad += Close(col, tcol, 32, c.k, what);
            if (separate) bad += Close(mainCol, tmain, 32, 0, "twin main");
            if (memcmp(&tb, &after, sizeof tb)) { printf("  %s twin scratch differs\n", c.name); ++bad; }
#endif
        }
    }
    delete mgr;
    printf("test_CEngineLightingSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
