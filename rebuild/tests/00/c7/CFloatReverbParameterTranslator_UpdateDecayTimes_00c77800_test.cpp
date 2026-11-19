// Behaviour test for 0x00C77800 CFloatReverbParameterTranslator::UpdateDecayTimes(CFloatReverbParameters*,CHLRvbParams*): the row's
// defines, includes and body verbatim over the shared headers (+ the hand4d overlay opt-in CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES),
// checked against an independent double-precision model. Covers: the 60 dB decay-sample count (unsigned sample rate x decay time / ln 1000),
// both LF / HF decay factors, the 4 short-reverb, 2 long-reverb and 3 long-input filters (gain stored, Set_ShelfLfHf called once per filter in
// that order with LF ratio, LF reference, HF ratio, HF reference and the sample rate), the input-filter delay = mean of the two long modules'
// input delay i+1 (truncating) - (i+1) * (MaxLong >> 2) - MaxShort, unsigned wrap of that delay, the echo feedback gain, and that no other
// field moves. Doubles: CFloatFilterParams::Set_ShelfLfHf (records each call), the CReverbConstants / CFloatReverbConstants destructors, the
// CHLRvbParams constructor.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define private public
#define protected public
// ---- row body ----
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
#include <math.h>

void CFloatReverbParameterTranslator::UpdateDecayTimes(CFloatReverbParameters *pReverbParams, CHLRvbParams *pSrcParams)
{
    float fDecaySamples;
    float fDecayFactorHF;
    unsigned int i;
    float fDecayFactorLF;
    float fGainRatioHF;
    unsigned int uiFilterSpacing;
    float fGainRatioLF;
    float fGain;

    // Samples for a 60 dB decay: exp(-n / fDecaySamples) is the gain of an n-sample delay.
    fDecaySamples = (float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fDecayTime / 6.907755f;
    fDecayFactorLF = (pSrcParams->m_fDecayTimeLFRatio - 1.0f) / (fDecaySamples * pSrcParams->m_fDecayTimeLFRatio);
    fDecayFactorHF = (pSrcParams->m_fDecayTimeHFRatio - 1.0f) / (fDecaySamples * pSrcParams->m_fDecayTimeHFRatio);

    // Short reverb feedback filters
    for (i = 0; i < 4; i++)
    {
        unsigned int uiBufferSamples = m_pReverbConstants->m_ShortRvbConstants[i].uiBufferLength;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_ShortReverb.m_aShortRvb[i].m_fFilter.SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_ShortReverb.m_aShortRvb[i].m_fFilter.Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Long reverb feedback filters
    for (i = 0; i < 2; i++)
    {
        unsigned int uiBufferSamples = pReverbParams->m_LongReverb.m_aLongRvb[i].m_uiFeedbackDelay;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_LongReverb.m_aLongRvb[i].m_fFeedbackFilter.SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_LongReverb.m_aLongRvb[i].m_fFeedbackFilter.Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Long reverb input filters
    uiFilterSpacing = m_pReverbConstants->m_uiMaxLongReverbInputSamples >> 2;
    for (i = 0; i < 3; i++)
    {
        unsigned int uiBufferSamples = 0;
        for (unsigned int j = 0; j < 2; j++)
            uiBufferSamples += pReverbParams->m_LongReverb.m_aLongRvb[j].m_auiInputDelay[i + 1];
        uiBufferSamples = uiBufferSamples / 2 - (i + 1) * uiFilterSpacing - m_pReverbConstants->m_uiMaxShortReverbInputSamples;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_LongReverb.m_aInputFilter[i].SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_LongReverb.m_aInputFilter[i].Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Echo feedback gain
    fGain = expf(-((float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fEchoTime / fDecaySamples));
    pReverbParams->m_Echo.m_fFeedbackGain = fGain;
}

// ---- doubles ----
struct ShelfCall { const CFloatFilterParams* self; float lf, flf, hf, fhf; unsigned int sps; };
static ShelfCall g_calls[16];
static int g_ncalls = 0;
void CFloatFilterParams::Set_ShelfLfHf(float fLFRatio, float fFreqLF, float fHFRatio, float fFreqHF, unsigned int uiSamplesPerSecond)
{
    if (g_ncalls < 16) { ShelfCall c = { this, fLFRatio, fFreqLF, fHFRatio, fFreqHF, uiSamplesPerSecond }; g_calls[g_ncalls] = c; }
    ++g_ncalls;
}
CReverbConstants::~CReverbConstants() { }
CFloatReverbConstants::~CFloatReverbConstants() { }
CHLRvbParams::CHLRvbParams() { }

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int feq(double a, double b) { if (a == b) return 1; return fabs(a - b) <= 2e-5 * (1.0 + fabs(b)); }

static void run_case(const char* name, unsigned int sps, float decay, float lfr, float hfr, unsigned int maxlong, unsigned int maxshort,
                     const unsigned int shortlen[4], const unsigned int fbdelay[2], const unsigned int indelay[2][4], float echo)
{
    char what[200];
    CFloatReverbConstants k;
    k.m_uiNumOutputs = 2; k.m_uiSamplesPerSecond = sps; k.m_uiMaxReflectionsInputSamples = 77;
    k.m_uiMaxShortReverbInputSamples = maxshort; k.m_uiMaxLongReverbInputSamples = maxlong; k.m_uiMaxModulationSamples = 99;
    for (int i = 0; i < 4; ++i) k.m_ShortRvbConstants[i].uiBufferLength = shortlen[i];
    CFloatReverbParameterTranslator t;
    memset(&t, 0xAB, sizeof(t));
    t.m_pReverbConstants = &k;
    CHLRvbParams h;
    memset(&h, 0, sizeof(h));
    h.m_fDecayTime = decay; h.m_fDecayTimeLFRatio = lfr; h.m_fDecayTimeHFRatio = hfr;
    h.m_fRefFrequencyLF = 250.0f; h.m_fRefFrequencyHF = 5000.0f; h.m_fEchoTime = echo;
    h.m_fReflectionsDelay = 0.25f; h.m_fReverbDelay = 0.5f;   // decoys: never read
    static CFloatReverbParameters p, before;
    unsigned char* raw = (unsigned char*)&p;
    for (unsigned int b = 0; b < sizeof(p); ++b) raw[b] = (unsigned char)(b * 7 + 3);
    for (int m = 0; m < 2; ++m) {
        p.m_LongReverb.m_aLongRvb[m].m_uiFeedbackDelay = fbdelay[m];
        for (int d = 0; d < 4; ++d) p.m_LongReverb.m_aLongRvb[m].m_auiInputDelay[d] = indelay[m][d];
    }
    memcpy(&before, &p, sizeof(p));
    CHLRvbParams hbefore; memcpy(&hbefore, &h, sizeof(h));
    g_ncalls = 0;

    t.UpdateDecayTimes(&p, &h);

    // independent model
    double ds = (double)sps * decay / log(1000.0);
    double flf = (lfr - 1.0) / (ds * lfr), fhf = (hfr - 1.0) / (ds * hfr);
    const CFloatFilterParams* want_this[9];
    double want_gain[9], want_lf[9], want_hf[9];
    int n = 0;
    for (int i = 0; i < 4; ++i, ++n) {
        double s = shortlen[i];
        want_this[n] = &p.m_ShortReverb.m_aShortRvb[i].m_fFilter; want_gain[n] = exp(-s / ds); want_lf[n] = exp(s * flf); want_hf[n] = exp(s * fhf);
    }
    for (int i = 0; i < 2; ++i, ++n) {
        double s = fbdelay[i];
        want_this[n] = &p.m_LongReverb.m_aLongRvb[i].m_fFeedbackFilter; want_gain[n] = exp(-s / ds); want_lf[n] = exp(s * flf); want_hf[n] = exp(s * fhf);
    }
    unsigned int spacing = maxlong / 4;
    for (int i = 0; i < 3; ++i, ++n) {
        unsigned int sum = indelay[0][i + 1] + indelay[1][i + 1];
        unsigned int ui = sum / 2 - (unsigned int)(i + 1) * spacing - maxshort;
        double s = (double)ui;
        want_this[n] = &p.m_LongReverb.m_aInputFilter[i]; want_gain[n] = exp(-s / ds); want_lf[n] = exp(s * flf); want_hf[n] = exp(s * fhf);
    }
    sprintf(what, "%s: Set_ShelfLfHf called 9 times (got %d)", name, g_ncalls); check(g_ncalls == 9, what);
    for (int c = 0; c < 9 && c < g_ncalls; ++c) {
        sprintf(what, "%s: call %d receiver", name, c); check(g_calls[c].self == want_this[c], what);
        sprintf(what, "%s: call %d gain stored (%g v %g)", name, c, want_this[c]->m_fGain, want_gain[c]); check(feq(want_this[c]->m_fGain, want_gain[c]), what);
        sprintf(what, "%s: call %d LF ratio (%g v %g)", name, c, g_calls[c].lf, want_lf[c]); check(feq(g_calls[c].lf, want_lf[c]), what);
        sprintf(what, "%s: call %d HF ratio (%g v %g)", name, c, g_calls[c].hf, want_hf[c]); check(feq(g_calls[c].hf, want_hf[c]), what);
        sprintf(what, "%s: call %d LF / HF reference", name, c); check(g_calls[c].flf == 250.0f && g_calls[c].fhf == 5000.0f, what);
        sprintf(what, "%s: call %d sample rate", name, c); check(g_calls[c].sps == sps, what);
    }
    double we = exp(-((double)sps * echo / ds));
    sprintf(what, "%s: echo feedback gain (%g v %g)", name, p.m_Echo.m_fFeedbackGain, we); check(feq(p.m_Echo.m_fFeedbackGain, we), what);
    // nothing else moved: restore the nine gains and the echo gain, then compare
    for (int c = 0; c < 9; ++c) {
        CFloatFilterParams* f = (CFloatFilterParams*)((unsigned char*)want_this[c] - (unsigned char*)&p + (unsigned char*)&before);
        ((CFloatFilterParams*)want_this[c])->m_fGain = f->m_fGain;
    }
    p.m_Echo.m_fFeedbackGain = before.m_Echo.m_fFeedbackGain;
    sprintf(what, "%s: no other parameter field written", name); check(memcmp(&p, &before, sizeof(p)) == 0, what);
    sprintf(what, "%s: source parameters untouched", name); check(memcmp(&h, &hbefore, sizeof(h)) == 0, what);
    sprintf(what, "%s: constants untouched", name);
    check(k.m_uiSamplesPerSecond == sps && k.m_uiMaxLongReverbInputSamples == maxlong && k.m_uiMaxShortReverbInputSamples == maxshort, what);
}

int main()
{
    // case A: ordinary values; odd input-delay sums (truncating mean); LF ratio > 1, HF ratio < 1
    const unsigned int shortA[4] = { 37, 53, 71, 97 };
    const unsigned int fbA[2] = { 211, 307 };
    const unsigned int inA[2][4] = { { 1, 401, 503, 611 }, { 2, 410, 520, 630 } };
    run_case("A", 1000, 2.0f, 1.5f, 0.5f, 200, 30, shortA, fbA, inA, 0.125f);
    // case B: sample rate >= 2^31 (unsigned conversion) and an input delay that wraps below zero (unsigned, gain 0)
    const unsigned int shortB[4] = { 1000000, 2000000, 3000000, 4000000 };
    const unsigned int fbB[2] = { 5000000, 7000000 };
    const unsigned int inB[2][4] = { { 9, 4000000, 6000000, 10 }, { 9, 4000002, 6000004, 12 } };
    run_case("B", 0x80000010u, 0.001f, 0.75f, 1.25f, 400000, 1000, shortB, fbB, inB, 0.0000001f);
    if (!g_fail) printf("00c77800_TEST PASS\n");
    return g_fail;
}