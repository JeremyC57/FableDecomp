// Row dual-build test: CReverbFnsSSE::ApplyFilterOrder2_quad 0x00C88E60, a library row (toolchain_library: byte-identical to GameCODA 2.10
// CODAApi.lib member ReverbFns_SSE.obj). Built as-is, the row stub compiles to nothing and the call binds the CODAApi.lib member (LIB must
// hold GameCODA 2.10 CODA/lib/Win32, e.g. FABLE_VC71_LIB); built /DFABLE_PORTABLE, the stub compiles the twin of
// engine/ReverbFnsSSEFilterOrder2Quad.h. The two outputs must agree line for line. Exact data only (dyadic coefficients, small-integer
// samples and histories), because the VC7.1 portable build evaluates the twin's floats on x87; the inexact cases are in
// tests/portable/test_ReverbFnsSSEFilterOrder2Quad.cpp.
#include <stdio.h>
#include <string.h>
#include "../../../src/compiled/00/c8/CReverbFnsSSE_ApplyFilterOrder2quad_00c88e60.cpp"
#ifndef FABLE_PORTABLE
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
#pragma comment(lib, "CODAApi.lib")
#endif
static unsigned int U(float f) { unsigned int u; memcpy(&u, &f, 4); return u; }
static void* DummyStorage() { static double d[2]; return d; }
static float g_buf[10 * 128 + 16];
int main()
{
    static const float a1[4] = { 0.5f, 0.25f, -0.5f, -0.25f }, a2[4] = { -0.25f, 0.5f, 0.25f, -0.5f };
    static const float b0[4] = { 1.0f, 2.0f, 0.5f, -1.0f }, b1[4] = { -0.5f, 0.25f, 1.0f, 0.5f }, b2[4] = { 0.25f, -0.25f, 0.5f, 2.0f };
    static const float g[4] = { 0.5f, 1.0f, 2.0f, 0.25f }, h0[4] = { 1.0f, -3.0f, 2.5f, 4.0f }, h1[4] = { -2.0f, 1.5f, 3.0f, -0.5f };
    static const int dstOff[] = { 640, 640, 640, 0, 128, 0 }, srcOff[] = { 0, 0, 0, 0, 0, 128 };
    static const unsigned int count[] = { 1, 4, 9, 8, 8, 8 };
    for (int c = 0; c < 6; ++c) {
        CFloatFilterParams p[4]; CFloatFilterParams* pp[4]; float hist[4][2]; float* ph[4];
        for (int l = 0; l < 4; ++l) {
            const float v[7] = { g[l], 1.0f, a1[l], a2[l], b0[l], b1[l], b2[l] };
            memcpy(&p[l], v, sizeof v); pp[l] = &p[l]; hist[l][0] = h0[l]; hist[l][1] = h1[l]; ph[l] = hist[l];
        }
        for (int k = 0; k < (int)(sizeof g_buf / sizeof g_buf[0]); ++k) g_buf[k] = (float)((int)((k * 37) % 11) - 5);
        ((CReverbFnsSSE*)DummyStorage())->ApplyFilterOrder2_quad(g_buf + dstOff[c], g_buf + srcOff[c], ph, pp, count[c]);
        printf("case %d dst%+d src%+d n=%u\n", c, dstOff[c], srcOff[c], count[c]);
        for (int k = 0; k < (int)(sizeof g_buf / sizeof g_buf[0]); k += 8) {
            printf("%04d", k);
            for (int j = 0; j < 8 && k + j < (int)(sizeof g_buf / sizeof g_buf[0]); ++j) printf(" %08X", U(g_buf[k + j]));
            printf("\n");
        }
        for (int l = 0; l < 4; ++l) printf("hist %d %08X %08X\n", l, U(hist[l][0]), U(hist[l][1]));
    }
    printf("DONE\n");
    return 0;
}
