// Behaviour test for 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad(float*,const float*,float**,CFloatFilterParams**,unsigned int):
// the row's defines, includes and body verbatim over the shared headers (+ the hand4d overlay: new engine/CReverbFnsSSE.h, new asm helper
// engine/ReverbFnsSSEFilterOrder2Quad.h, opt-in CREVERBFNSSSE_APPLYFILTERORDER2_QUAD in CFloatFilterParams.h), checked against an
// independent double model. The model rounds every step to single precision as SSE does (one IEEE single operation each), and
// outputs are compared bit for bit. Covers: the four filters gathered lane by lane (a1 = m_fA[1], a2 = m_fA[2], c2 = 1 - (a1 + a2), b0..b2 = m_fB,
// c6 = -(b0 + b1 + b2), g = m_fGain, h0 / h1 = history[0] / [1]); per sample w = (x + c2) + (a2*h1 + a1*h0), y = ((b0*w + (b2*h1 +
// b1*h0)) + c6) * g; lanes 128 floats apart; exactly uiSamples samples per lane (1 and 7); the histories written back (h0 = last w,
// h1 = the one before); nothing else in the destination, the source or the filter parameters moves. The CReverbFnsSSE object is never
// constructed (the function does not read this), so no vtable is needed.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define private public
#define protected public
// ---- row body ----
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
#include "engine/ReverbFnsSSEFilterOrder2Quad.h"

void CReverbFnsSSE::ApplyFilterOrder2_quad(float *pfDest, const float *pfSrc, float **apHistory, CFloatFilterParams **aFilterParams, unsigned int uiSamples)
{
    __declspec(align(16)) float fData[40];
    float *pfData;
    int i;

    pfData = fData;
    for (i = 0; i < 4; i++)
    {
        fData[i] = aFilterParams[i]->m_fA[1];
        fData[i + 4] = aFilterParams[i]->m_fA[2];
        fData[i + 8] = 1.0f - (aFilterParams[i]->m_fA[1] + aFilterParams[i]->m_fA[2]);
        fData[i + 12] = aFilterParams[i]->m_fB[0];
        fData[i + 16] = aFilterParams[i]->m_fB[1];
        fData[i + 20] = aFilterParams[i]->m_fB[2];
        fData[i + 24] = -(aFilterParams[i]->m_fB[0] + aFilterParams[i]->m_fB[1] + aFilterParams[i]->m_fB[2]);
        fData[i + 28] = apHistory[i][0];
        fData[i + 32] = apHistory[i][1];
        fData[i + 36] = aFilterParams[i]->m_fGain;
    }

    CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData);

    for (i = 0; i < 4; i++)
    {
        apHistory[i][0] = fData[i + 28];
        apHistory[i][1] = fData[i + 32];
    }
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static float F(double v) { return (float)v; }

static void run_case(const char* name, unsigned int n)
{
    char what[160];
    static float src[4 * 128 + 4], dst[4 * 128 + 4], srcbefore[4 * 128 + 4];
    static CFloatFilterParams fp[4], fpbefore[4];
    float hist[4][3];
    float* aphist[4];
    CFloatFilterParams* apfp[4];
    // coefficients: powers of two (different per lane and per field), so every intermediate stays exact in single precision
    static const float A1[4] = { 0.5f, 0.25f, -0.5f, -0.25f }, A2[4] = { -0.25f, 0.5f, 0.25f, -0.5f };
    static const float B0[4] = { 1.0f, 2.0f, 0.5f, -1.0f }, B1[4] = { -0.5f, 0.25f, 1.0f, 0.5f }, B2[4] = { 0.25f, -0.25f, 0.5f, 2.0f };
    static const float G[4] = { 0.5f, 1.0f, 2.0f, 0.25f }, H0[4] = { 1.0f, -3.0f, 2.5f, 4.0f }, H1[4] = { -2.0f, 1.5f, 3.0f, -0.5f };
    for (int l = 0; l < 4; ++l) {
        fp[l].m_fGain = G[l];
        fp[l].m_fA[0] = 3.0f;                      // decoy: never read
        fp[l].m_fA[1] = A1[l];
        fp[l].m_fA[2] = A2[l];
        fp[l].m_fB[0] = B0[l];
        fp[l].m_fB[1] = B1[l];
        fp[l].m_fB[2] = B2[l];
        hist[l][0] = H0[l];
        hist[l][1] = H1[l];
        hist[l][2] = 99.0f;                        // guard
        aphist[l] = hist[l];
        apfp[l] = &fp[l];
    }
    for (int k = 0; k < 4 * 128 + 4; ++k) { src[k] = (float)((k * 37) % 11) - 5.0f; dst[k] = -1000.0f - k; }
    memcpy(srcbefore, src, sizeof(src)); memcpy(fpbefore, fp, sizeof(fp));
    // model in double; the inputs keep every intermediate exact in single precision (checked), so SSE rounding is the identity
    double model_out[4][8], model_h0[4], model_h1[4];
    for (int l = 0; l < 4; ++l) {
        double a1 = fp[l].m_fA[1], a2 = fp[l].m_fA[2], c2 = 1.0 - (a1 + a2);
        double b0 = fp[l].m_fB[0], b1 = fp[l].m_fB[1], b2 = fp[l].m_fB[2], c6 = -(b0 + b1 + b2), g = fp[l].m_fGain;
        double h0 = hist[l][0], h1 = hist[l][1];
        for (unsigned int s = 0; s < n; ++s) {
            double x = src[l * 128 + s];
            double fb = a2 * h1 + a1 * h0, ff = b2 * h1 + b1 * h0;
            double w = (x + c2) + fb;
            double y = ((b0 * w + ff) + c6) * g;
            sprintf(what, "%s: model exact in single precision (lane %d sample %u)", name, l, s);
            check(F(fb) == fb && F(ff) == ff && F(x + c2) == x + c2 && F(w) == w && F(b0 * w) == b0 * w && F(b0 * w + ff) == b0 * w + ff
                  && F(b0 * w + ff + c6) == b0 * w + ff + c6 && F(y) == y, what);
            h1 = h0; h0 = w;
            model_out[l][s] = y;
        }
        model_h0[l] = h0; model_h1[l] = h1;
    }

    ((CReverbFnsSSE*)0x10)->ApplyFilterOrder2_quad(dst, src, aphist, apfp, n);

    for (int l = 0; l < 4; ++l) {
        for (unsigned int s = 0; s < n; ++s) {
            float mf = (float)model_out[l][s]; unsigned int bd, bm; memcpy(&bd, &dst[l * 128 + s], 4); memcpy(&bm, &mf, 4);
            sprintf(what, "%s: lane %d sample %u output (%08x v %08x)", name, l, s, bd, bm);
            check((double)dst[l * 128 + s] == model_out[l][s], what);
        }
        for (unsigned int s = n; s < 128; ++s)
            if (dst[l * 128 + s] != -1000.0f - (l * 128 + s)) { sprintf(what, "%s: lane %d sample %u beyond uiSamples written", name, l, s); check(0, what); break; }
        sprintf(what, "%s: lane %d history h0 (%g v %g)", name, l, hist[l][0], model_h0[l]); check((double)hist[l][0] == model_h0[l], what);
        sprintf(what, "%s: lane %d history h1 (%g v %g)", name, l, hist[l][1], model_h1[l]); check((double)hist[l][1] == model_h1[l], what);
        sprintf(what, "%s: lane %d history guard", name, l); check(hist[l][2] == 99.0f, what);
    }
    for (int k = 4 * 128; k < 4 * 128 + 4; ++k) { sprintf(what, "%s: past-the-end dst %d", name, k); check(dst[k] == -1000.0f - k, what); }
    sprintf(what, "%s: source untouched", name); check(memcmp(src, srcbefore, sizeof(src)) == 0, what);
    sprintf(what, "%s: filter parameters untouched", name); check(memcmp(fp, fpbefore, sizeof(fp)) == 0, what);
}

int main()
{
    run_case("one sample", 1);
    run_case("seven samples", 7);
    if (!g_fail) printf("00c88e60_TEST PASS\n");
    return g_fail;
}
