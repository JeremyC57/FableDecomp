// Behaviour test for 00c54c10: CSalSw3DVoice::CalcParams(const CCoda3DSource*, unsigned int) -- the row body verbatim over
// fixture models of the GameCODA interfaces it calls (pattern of the landed sibling 0x00C5ACB0 test). Every SDK callee is a
// recording double; no SDK implementation is copied. The float-to-int is the shared libpk/FtoL.h the row includes.
// Fixtures: the source's CODASOURCE / CODA3DSOURCE records and caps; the voice's status / params words, gains, cutoffs, play
// buffer (Release logged), two software filters (SetLP / SetEQ / ClearHistory logged), two IIR filters (SetLowPass logged),
// the render object (virtual SetPosition logged), the EAX context (mode), the CODA context (EAX pointer, distance units,
// MacroFX factor) and the platform's reverb pointer. sal_levelToGain returns a level-keyed exact value and logs its argument.
// 4096 pseudo-random cases over the update mask, the status / mode bits, the EAX / caps / reverb state and the filter pointers
// check the call sequence, every argument and the state written back.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libpk/FtoL.h"
struct CCodaVector { float x, y, z; };
struct _CLPFILTER { unsigned int uiFreq; float fQ; };
struct _CEQFILTER { unsigned int uiFreq; float fGain; float fQ; };
struct CODASOURCE { unsigned int ffUpdate; float fGain; int iLevel; float fPitch; _CLPFILTER* pLPFilter; _CEQFILTER* pEQFilter; void* pLevelEnv; void* pMultiEnv; void* pPitchLFO; void* pMultiLFO; };
struct CODA3DSOURCE { CCodaVector vPosition, vVelocity, vConeDirection, vDirection; float fDistance, fDistanceGain, fConeGain, fDoppler; int iDirect, iDirectHF; unsigned int uiDirectHFReference; int iEnvFx, iEnvFxHF; unsigned int uiEnvFxHFReference; float fMacroFx; };
#define C_CAPS_EAX (0x00002000)
struct Ev { int kind; const void* p; const void* q; int i; unsigned int u; float f[4]; int b[2]; };
static Ev g_ev[32]; static int g_n = 0; static bool g_overflow = false;
static Ev& Add(int kind) { static Ev junk; if (g_n == 32) { g_overflow = true; return junk; } Ev& e = g_ev[g_n++]; memset(&e, 0, sizeof(e)); e.kind = kind; return e; }
class CCoda3DSource { public:
    CODASOURCE m_Params; CODA3DSOURCE m_3D; unsigned int m_ffCaps;
    const CODASOURCE& GetParams() const { return m_Params; }
    const CODA3DSOURCE& Get3DParams() const { return m_3D; }
    unsigned int GetCaps() const { return m_ffCaps; }
};
class CSalSwBuffer { public: void Release() { Add(2).p = this; } };
class CSalSwFilter { public:
    void SetLP(const _CLPFILTER* f, float c) { Ev& e = Add(10); e.p = this; e.q = f; e.f[0] = c; }
    void SetEQ(const _CEQFILTER* f, float c) { Ev& e = Add(12); e.p = this; e.q = f; e.f[0] = c; }
    void ClearHistory() { Add(11).p = this; }
};
class CSalIIRFilter { public: void SetLowPass(int level, float ref) { Ev& e = Add(4); e.p = this; e.i = level; e.f[0] = ref; } };
class CSalRender { public:
    virtual ~CSalRender() {}
    virtual void SetPosition(float az, float el, float dist, float gain, const float* gains, bool b1, bool b2) {
        Ev& e = Add(9); e.p = this; e.q = gains; e.f[0] = az; e.f[1] = el; e.f[2] = dist; e.f[3] = gain; e.b[0] = b1; e.b[1] = b2; }
};
class CCodaEAX { public: enum EAX_MODE { EAX_SW, EAX_HW }; EAX_MODE m_iMode; bool IsHardwareEAX() { return m_iMode == EAX_HW; } };
class CCodaContext { public:
    CCodaEAX* m_EAX; float m_fDistanceUnits; float m_fMacroFxFactor;
    CCodaEAX* GetEAX() const { return m_EAX; }
    float GetDistanceUnits() const { return m_fDistanceUnits; }
    float GetMacroFxFactor() const { return m_fMacroFxFactor; }
};
static CCodaContext CODA;
class CSalPlatform { public: void* m_pReverb; void* GetReverb() const { return m_pReverb; } };
static CSalPlatform g_platform; static CSalPlatform* g_pSal = &g_platform;
static float sal_levelToGain(int iLevel) { Add(6).i = iLevel; return iLevel == 0 ? 0.5f : 0.25f + (float)((-iLevel) & 7) / 32.0f; }
static void SAL_CalcMacroFxGains(float az, float el, float dist, float macro, float* gains) {
    Ev& e = Add(8); e.q = gains; e.f[0] = az; e.f[1] = el; e.f[2] = dist; e.f[3] = macro; }
class CSalSw3DVoice { public:
    const void* m_pcBuffer; unsigned int m_ffStatus, m_ffParams; float m_fLevelEnvGain, m_fMultiEnvCutoff, m_fMultiLFOGain, m_fMultiLFOCutoff;
    CSalSwBuffer m_PlayBuffer; CSalSwFilter m_Filter[2]; float m_fAzimuth, m_fElevation, m_fMacroFXGain[2]; CSalRender* m_pRender;
    CSalIIRFilter m_filterDirect, m_filterEnvironment; unsigned short m_gainEnvironment, m_gainEnvironmentNew; bool m_bUpdateEnvironmentGain;
    bool IsStarting() const { return (m_ffStatus & 1) != 0; }
    bool IsLevelEnvReleased() const { return (m_ffStatus & 0x40) != 0; }
    unsigned int GetHrtfMode() const { return m_ffParams & 0x3c00; }
    static void CalcAzimuthElevation(float& az, float& el, const CCodaVector& v) { Ev& e = Add(3); e.p = &az; e.q = &el; e.u = (unsigned int)(size_t)&v; }
    void SetPitch(float pitch, const CODASOURCE& params) { Ev& e = Add(1); e.f[0] = pitch; e.q = &params; }
    void ReleasePlayBuffer() { m_PlayBuffer.Release(); m_pcBuffer = 0; }
    void CalcParams(const CCoda3DSource* pcSource, unsigned int ffUpdate);
};
// ---- row body (src/00c54c10.cpp) ----
void CSalSw3DVoice::CalcParams(const CCoda3DSource* pcSource, unsigned int ffUpdate)
{
    const CODASOURCE& params = pcSource->GetParams();
    const CODA3DSOURCE& params3D = pcSource->Get3DParams();
    if (ffUpdate & 0x10074)
        SetPitch(params.fPitch * params3D.fDoppler, params);
    if (IsLevelEnvReleased())
        ReleasePlayBuffer();
    if (ffUpdate & 0x8000)
        CalcAzimuthElevation(m_fAzimuth, m_fElevation, params3D.vDirection);
    if (ffUpdate & 0x400000)
        m_filterDirect.SetLowPass(params3D.iDirectHF, (float)params3D.uiDirectHFReference);
    if (g_pSal->GetReverb()) {
        CCodaEAX* pEAX = CODA.GetEAX();
        float fGain;
        if (pEAX && !pEAX->IsHardwareEAX() && (pcSource->GetCaps() & C_CAPS_EAX))
            fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * sal_levelToGain(0);
        else
            fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * sal_levelToGain(params3D.iEnvFx);
        unsigned short gain = (unsigned short)FtoL(fGain * 32767.0f);
        if (gain != m_gainEnvironmentNew) {
            m_gainEnvironmentNew = gain;
            m_bUpdateEnvironmentGain = true;
        }
        if (ffUpdate & 0x1000000)
            m_filterEnvironment.SetLowPass(params3D.iEnvFxHF, (float)params3D.uiEnvFxHFReference);
    }
    if ((ffUpdate & 0x800000) && GetHrtfMode() != 0x2000)
        SAL_CalcMacroFxGains(m_fAzimuth, m_fElevation, CODA.GetDistanceUnits() * params3D.fDistance, CODA.GetMacroFxFactor() * params3D.fMacroFx, m_fMacroFXGain);
    float fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * params3D.fConeGain * params3D.fDistanceGain * sal_levelToGain(params3D.iDirect);
    m_pRender->SetPosition(m_fAzimuth, m_fElevation, CODA.GetDistanceUnits() * params3D.fDistance, fGain, m_fMacroFXGain, !IsStarting(), false);
    if (ffUpdate & 0x1d0) {
        float fCutOff = m_fMultiEnvCutoff * m_fMultiLFOCutoff;
        m_ffStatus &= ~0x1e0000;
        if (params.pLPFilter) {
            m_Filter[0].SetLP(params.pLPFilter, fCutOff);
            m_ffStatus |= 0x20000;
        } else
            m_Filter[0].ClearHistory();
        if (params.pEQFilter) {
            m_Filter[1].SetEQ(params.pEQFilter, fCutOff);
            m_ffStatus |= 0x100000;
        } else
            m_Filter[1].ClearHistory();
    }
}
// ---- driver ----
static unsigned int g_rand = 0x2468ace1u;
static unsigned int Next() { g_rand = g_rand * 1664525u + 1013904223u; return g_rand ^ (g_rand >> 15); }
static int fails = 0;
#define CHECK(c) do { if (!(c)) { if (fails < 20) printf("FAIL case %d line %d: %s\n", id, __LINE__, #c); ++fails; } } while (0)
static float Gain(int level) { return level == 0 ? 0.5f : 0.25f + (float)((-level) & 7) / 32.0f; }
int main()
{
    static const unsigned int bits[] = { 0x4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x10000, 0x8000, 0x400000, 0x1000000, 0x800000, 0x200, 0x1 };
    static const unsigned int refs[] = { 5000u, 0x80000000u, 0xFFFFFF00u, 0u };
    static const float fgains[] = { 0.5f, 0.75f, 1.0f, 0.375f };
    _CLPFILTER lp; _CEQFILTER eq; memset(&lp, 0, sizeof(lp)); memset(&eq, 0, sizeof(eq));
    CCodaEAX eax; CSalRender render; int reverb = 0;
    for (int id = 0; id < 4096; ++id) {
        unsigned int a = Next(), b = Next();
        unsigned int ffUpdate = 0;
        for (int k = 0; k < 13; ++k) if (a & (1u << k)) ffUpdate |= bits[k];
        CCoda3DSource src; memset(&src, 0, sizeof(src));
        src.m_Params.fGain = fgains[(b >> 0) & 3]; src.m_Params.fPitch = 1.5f; src.m_Params.pLPFilter = (b & 4) ? &lp : 0; src.m_Params.pEQFilter = (b & 8) ? &eq : 0;
        src.m_3D.vDirection.x = 1.0f; src.m_3D.fDistance = 3.0f; src.m_3D.fDistanceGain = 0.5f; src.m_3D.fConeGain = 0.75f; src.m_3D.fDoppler = 1.25f;
        src.m_3D.iDirect = -(int)((b >> 4) & 7) * 100; src.m_3D.iDirectHF = -1200; src.m_3D.uiDirectHFReference = refs[(b >> 7) & 3];
        src.m_3D.iEnvFx = -(int)(((b >> 9) & 7) + 1) * 100; src.m_3D.iEnvFxHF = -700; src.m_3D.uiEnvFxHFReference = refs[(b >> 12) & 3]; src.m_3D.fMacroFx = 2.0f;
        src.m_ffCaps = (b & 0x4000) ? C_CAPS_EAX : 0x80;
        CSalSw3DVoice v; memset(&v, 0, sizeof(v));
        v.m_pcBuffer = &lp; v.m_ffStatus = ((a >> 13) & 1) | (((a >> 14) & 1) << 6) | 0x1e0000 | 0x80000000u;
        v.m_ffParams = ((b >> 15) & 1) ? 0x2000 : 0x800;
        v.m_fLevelEnvGain = 0.75f; v.m_fMultiLFOGain = 1.0f; v.m_fMultiEnvCutoff = 0.5f; v.m_fMultiLFOCutoff = 0.25f;
        v.m_fAzimuth = 30.0f; v.m_fElevation = -10.0f; v.m_pRender = &render;
        v.m_gainEnvironmentNew = (unsigned short)((b & 0x10000) ? 0 : 6144); v.m_bUpdateEnvironmentGain = false;
        g_platform.m_pReverb = (b & 0x20000) ? &reverb : 0;
        CODA.m_EAX = (b & 0x40000) ? &eax : 0; eax.m_iMode = (b & 0x80000) ? CCodaEAX::EAX_HW : CCodaEAX::EAX_SW;
        CODA.m_fDistanceUnits = 2.0f; CODA.m_fMacroFxFactor = 0.5f;
        unsigned int status0 = v.m_ffStatus;
        g_n = 0; g_overflow = false;
        v.CalcParams(&src, ffUpdate);
        // expected sequence
        int i = 0;
        CHECK(!g_overflow);
        if (ffUpdate & 0x10074) { CHECK(i < g_n && g_ev[i].kind == 1 && g_ev[i].f[0] == 1.5f * 1.25f && g_ev[i].q == &src.m_Params); ++i; }
        if (status0 & 0x40) { CHECK(i < g_n && g_ev[i].kind == 2 && g_ev[i].p == &v.m_PlayBuffer); ++i; CHECK(v.m_pcBuffer == 0); }
        else CHECK(v.m_pcBuffer == &lp);
        if (ffUpdate & 0x8000) { CHECK(i < g_n && g_ev[i].kind == 3 && g_ev[i].p == &v.m_fAzimuth && g_ev[i].q == &v.m_fElevation && g_ev[i].u == (unsigned int)(size_t)&src.m_3D.vDirection); ++i; }
        if (ffUpdate & 0x400000) { CHECK(i < g_n && g_ev[i].kind == 4 && g_ev[i].p == &v.m_filterDirect && g_ev[i].i == -1200 && g_ev[i].f[0] == (float)(double)src.m_3D.uiDirectHFReference); ++i; }
        unsigned short wantGain = (unsigned short)((b & 0x10000) ? 0 : 6144); bool wantFlag = false;
        if (g_platform.m_pReverb) {
            bool sw = CODA.m_EAX && eax.m_iMode != CCodaEAX::EAX_HW && (src.m_ffCaps & C_CAPS_EAX);
            int level = sw ? 0 : src.m_3D.iEnvFx;
            CHECK(i < g_n && g_ev[i].kind == 6 && g_ev[i].i == level); ++i;
            double g = (double)src.m_Params.fGain * 0.75 * 1.0 * Gain(level) * 32767.0;
            unsigned short gs = (unsigned short)(long)floor(g + 0.5);
            if (gs != wantGain) { wantGain = gs; wantFlag = true; }
            if (ffUpdate & 0x1000000) { CHECK(i < g_n && g_ev[i].kind == 4 && g_ev[i].p == &v.m_filterEnvironment && g_ev[i].i == -700 && g_ev[i].f[0] == (float)(double)src.m_3D.uiEnvFxHFReference); ++i; }
        }
        CHECK(v.m_gainEnvironmentNew == wantGain && v.m_bUpdateEnvironmentGain == wantFlag);
        if ((ffUpdate & 0x800000) && v.m_ffParams != 0x2000) { CHECK(i < g_n && g_ev[i].kind == 8 && g_ev[i].f[0] == 30.0f && g_ev[i].f[1] == -10.0f && g_ev[i].f[2] == 6.0f && g_ev[i].f[3] == 1.0f && g_ev[i].q == v.m_fMacroFXGain); ++i; }
        CHECK(i < g_n && g_ev[i].kind == 6 && g_ev[i].i == src.m_3D.iDirect); ++i;
        float wantDirect = (float)((double)src.m_Params.fGain * 0.75 * 1.0 * 0.75 * 0.5 * Gain(src.m_3D.iDirect));
        CHECK(i < g_n && g_ev[i].kind == 9 && g_ev[i].p == &render && g_ev[i].f[0] == 30.0f && g_ev[i].f[1] == -10.0f && g_ev[i].f[2] == 6.0f && g_ev[i].f[3] == wantDirect
              && g_ev[i].q == v.m_fMacroFXGain && g_ev[i].b[0] == !(status0 & 1) && g_ev[i].b[1] == 0); ++i;
        unsigned int wantStatus = status0;
        if (ffUpdate & 0x1d0) {
            wantStatus &= ~0x1e0000u;
            if (src.m_Params.pLPFilter) { CHECK(i < g_n && g_ev[i].kind == 10 && g_ev[i].p == &v.m_Filter[0] && g_ev[i].q == &lp && g_ev[i].f[0] == 0.125f); ++i; wantStatus |= 0x20000; }
            else { CHECK(i < g_n && g_ev[i].kind == 11 && g_ev[i].p == &v.m_Filter[0]); ++i; }
            if (src.m_Params.pEQFilter) { CHECK(i < g_n && g_ev[i].kind == 12 && g_ev[i].p == &v.m_Filter[1] && g_ev[i].q == &eq && g_ev[i].f[0] == 0.125f); ++i; wantStatus |= 0x100000; }
            else { CHECK(i < g_n && g_ev[i].kind == 11 && g_ev[i].p == &v.m_Filter[1]); ++i; }
        }
        CHECK(i == g_n);
        CHECK(v.m_ffStatus == wantStatus);
        CHECK(v.m_fAzimuth == 30.0f && v.m_fElevation == -10.0f && v.m_ffParams == (((b >> 15) & 1) ? 0x2000u : 0x800u));
    }
    if (fails == 0) printf("00c54c10_TEST PASS\n");
    else printf("00c54c10_TEST FAIL %d\n", fails);
    return fails != 0;
}