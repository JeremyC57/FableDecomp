// Behaviour test for 0x00C41FB0 CCoda3DSource::Commit. It links the row object (vland's source-object link): the real compiled Commit and
// its same-TU helpers CalcDirectionAndDistance / CalcDistanceGain / CalcDoppler / CalcConeGain / CalcEnvFxGain / sal_gainToLevel run on
// native-layout fixtures, and every field they write is compared with an independent reference model written from the SDK semantics.
// Fixtures: a 476-byte source, 76-byte listeners and a 128-byte environment laid out as the native classes (offsets asserted), the CODA
// context (Z-axis sign, HF reference, Doppler speed, rolloff factor), a recording distance callback and CCodaVector::Normalize
// (0x00C402E0's semantics: a zero vector is left alone, else divided by its length).
// Targeted cases cover each branch and boundary (head-relative bits, the nearest-listener tie, the 0x20000 / 0x88000 / caps masks, the
// callback arguments, the IsZero direction, distance mute / clamps, Doppler and cone clamps, sal_ftoi ties and signs, every -10000 clamp
// at -10000 / -10001, the environment-FX upper clamp at 0 / 1, fGain == 1 and fGain == 0), then 3000 pseudo-random cases on exact grids.
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"

CCodaObjectBase::~CCodaObjectBase() {}
extern "C" void* __cdecl CodaMalloc(unsigned int) { return 0; }
extern "C" void __cdecl CodaFree(void*) {}
CCodaContext CODA;

float CCodaVector::Normalize()
{
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return 0.0f;
    float length = (float)sqrt((double)(x * x + y * y + z * z));
    if (length != 1.0f) {
        float scale = 1.0f / length;
        x *= scale; y *= scale; z *= scale;
    }
    return length;
}

struct V3 { float x, y, z; };
struct Src {                                    // native CCoda3DSource layout (476 bytes)
    unsigned int vptr, prev, next, ffCaps, ffStatus, ffUpdate;
    unsigned char params[0xfc - 0x18];
    V3 p3, v3, c3, d3;
    float fDistance, fDistanceGain, fConeGain, fDoppler;
    int iDirect, iDirectHF; unsigned int uiDirectHFRef; int iEnvFx, iEnvFxHF; unsigned int uiEnvFxHFRef; float fMacroFx;
    V3 pos, vel, cone;
    float fMax, fMin, fRef, fRolloff, fDopp; unsigned int inner, outer; float outerGain; int iDirectSrc, iDirectHFSrc;
    int iEnv, iEnvHF, iObs; float fObsLF; int iOcc; float fOccLF; int iExc; float fExcLF; float fRollFactor, fAirAbs;
    int hrtf; float (__stdcall *cb)(void*, void*, float, unsigned int); unsigned int cbData; unsigned int ffCommit;
};
struct Lis { unsigned int vptr; Lis* prev; Lis* next; V3 pos, vel, front, top, right; unsigned int ffUpdate; };
struct Lst { unsigned int vptr; void* objects; void* pfree; int nFree; unsigned int nObjects; };
struct Env { unsigned int vptr, prev, next; V3 refl, rev; int iLevel, iLevelHF, iLevelLF; float f[14]; unsigned int uiHFRef, uiLFRef;
             float fRolloff; int iAirAbsHF; unsigned int ffCommit; void* pReverb; };
typedef char a1[(sizeof(Src) == 476 && sizeof(CCoda3DSource) == 476) ? 1 : -1];
typedef char a2[(offsetof(Src, p3) == 0xfc && offsetof(Src, pos) == 0x158 && offsetof(Src, iEnv) == 0x1a4 && offsetof(Src, ffCommit) == 0x1d8) ? 1 : -1];
typedef char a3[(sizeof(Lis) == 76 && sizeof(CCodaListener) == 76 && offsetof(Lis, right) == 0x3c) ? 1 : -1];
typedef char a4[(sizeof(Env) == 128 && sizeof(CCodaEnvironment) == 128 && offsetof(Env, uiHFRef) == 0x68 && offsetof(Env, fRolloff) == 0x70 && offsetof(Env, iAirAbsHF) == 0x74) ? 1 : -1];
typedef char a5[(sizeof(Lst) == 20) ? 1 : -1];
struct CtxTail { unsigned int uiHFRef; float fZAxis, fDistanceUnits, fDopplerFactor, fDopplerSpeed, fMacroFx, fRolloffFactor; unsigned int ffUpdate; };
typedef char a6[(sizeof(CCodaContext) == 0x88 + sizeof(CtxTail)) ? 1 : -1];   // native context: the tail starts at +0x88 (0x0143BBE0)
static CtxTail& Ctx() { return *reinterpret_cast<CtxTail*>(reinterpret_cast<char*>(&CODA) + 0x88); }

// recording distance callback
static void* g_cbSrc; static void* g_cbLis; static float g_cbDist; static unsigned int g_cbData; static int g_cbCalls;
static float __stdcall DistanceCallback(void* src, void* lis, float d, unsigned int data)
{ g_cbSrc = src; g_cbLis = lis; g_cbDist = d; g_cbData = data; ++g_cbCalls; return 0.375f + (float)(data & 3); }

// ---- reference model ----
static int m_cbCalls; static Lis* m_cbLis; static float m_cbDist;
static float Dot(const V3& a, const V3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static V3 Sub(const V3& a, const V3& b) { V3 r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static float Mag(const V3& a) { return (float)sqrt((double)Dot(a, a)); }
static void Norm(V3& v) { if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) return; float l = Mag(v); if (l != 1.0f) { float s = 1.0f / l; v.x *= s; v.y *= s; v.z *= s; } }
static double RintEven(double w) { double r = floor(w + 0.5); if (r - w == 0.5 && fmod(r, 2.0) != 0.0) r -= 1.0; return r; }
static int Ftoi(double level) { float w = (float)level - 0.5f; int i = (int)RintEven((double)w); if (i < 0) ++i; return i; }
static int GainToLevel(float g) { if (g == 0.0f) return -10000; int l = (int)RintEven((double)(float)(log10((double)g) * 2000.0)); return l < -10000 ? -10000 : l; }
static float Clamped(const Src& s) { return s.fDistance < s.fMin ? s.fMin : (s.fDistance > s.fMax ? s.fMax : s.fDistance); }
static float DistGain(const Src& s, float expo)
{ if ((s.ffStatus & 0x80000) && s.fDistance >= s.fMax) return 0.0f; return (float)pow((double)(s.fRef / Clamped(s)), (double)expo); }
static float Cone(const Src& s)
{
    V3 m = Sub(s.c3, s.d3); V3 p = { s.c3.x + s.d3.x, s.c3.y + s.d3.y, s.c3.z + s.d3.z };
    float mm = Mag(m), mp = Mag(p), a;
    if (mp < mm) a = (float)atan2((double)mp, (double)mm) * 229.18311f; else a = 360.0f - (float)atan2((double)mm, (double)mp) * 229.18311f;
    float in = (float)s.inner, out = (float)s.outer;
    if (a <= in) return 1.0f;
    if (a >= out) return s.outerGain;
    return (s.outerGain - 1.0f) * (a - in) / (out - in) + 1.0f;
}
static void Model(Src& s, Lis* head, Env* env, unsigned int g)
{
    s.ffCommit |= g;
    if (!s.ffCommit) return;
    Lis* L = 0; V3 right = { 0, 0, 0 };
    if (s.ffStatus & 0x40000) {
        if (s.ffCommit & 0x8000) s.p3 = s.pos;
        if (s.ffCommit & 0x10000) s.v3 = s.vel;
        if (s.ffCommit & 0x80000) { Norm(s.cone); s.c3 = s.cone; }
    } else {
        if (s.ffCommit & 0x20000) s.ffCommit |= 0x8000;
        L = head; V3 dir = Sub(s.pos, L->pos); float best = Dot(dir, dir);
        for (Lis* o = head->next; o; o = o->next) { V3 v = Sub(s.pos, o->pos); float f = Dot(v, v); if (f < best) { best = f; dir = v; L = o; } }
        if (s.ffCommit & 0x88000) { right = L->right; right.x *= Ctx().fZAxis; }
        if (s.ffCommit & 0x8000) { s.p3.x = Dot(dir, right); s.p3.y = Dot(dir, L->top); s.p3.z = Dot(dir, L->front); }
        if (s.ffCommit & 0x10000) s.v3 = Sub(s.vel, L->vel);
        if (s.ffCommit & 0x80000) { Norm(s.cone); s.c3.x = Dot(s.cone, right); s.c3.y = Dot(s.cone, L->top); s.c3.z = Dot(s.cone, L->front); }
    }
    if (s.ffCommit & 0x8000) {
        if (s.p3.x == 0.0f && s.p3.y == 0.0f && s.p3.z == 0.0f) { s.fDistance = 0.0f; s.d3.x = 0.0f; s.d3.y = 0.0f; s.d3.z = 1.0f; }
        else { s.d3 = s.p3; s.fDistance = Mag(s.p3); Norm(s.d3); }
        s.ffCommit |= 0xd0000;
    }
    if (s.ffCommit & 0x40000) {
        if (!s.cb) s.fDistanceGain = DistGain(s, Ctx().fRolloffFactor * s.fRolloff);
        else { s.fDistanceGain = 0.375f + (float)(s.cbData & 3); ++m_cbCalls; m_cbLis = L; m_cbDist = s.fDistance; }
        s.ffCommit |= 0x1800000;
    }
    if (s.ffCommit & 0x10000) { float d = 1.0f - Dot(s.v3, s.d3) * s.fDopp * Ctx().fDopplerSpeed; s.fDoppler = d < 0.0f ? 0.0f : d; }
    if (s.ffCommit & 0x380000) { s.fConeGain = Cone(s); s.ffCommit |= 0x200000; }
    if (s.ffCommit & 0x1400000) { s.iDirect = s.iDirectSrc; s.iDirectHF = s.iDirectHFSrc; s.uiDirectHFRef = Ctx().uiHFRef; s.ffCommit |= 0x1400000; }
    if (!(s.ffCaps & 0x1000)) s.ffCommit &= ~0x400000u;
    if (!(s.ffCaps & 0x80)) s.ffCommit &= ~0x1000000u;
    if ((s.ffCommit & 0x1000000) && env) {
        float gain = DistGain(s, env->fRolloff * s.fRollFactor);
        s.iDirect += Ftoi((double)s.iObs * s.fObsLF + (double)s.iOcc * s.fOccLF);
        if (s.iDirect < -10000) s.iDirect = -10000;
        s.iDirectHF += Ftoi((double)s.iObs * (1.0f - s.fObsLF) + (double)s.iOcc * (1.0f - s.fOccLF) + (double)env->iAirAbsHF * s.fDistance * s.fAirAbs);
        if (s.iDirectHF < -10000) s.iDirectHF = -10000;
        s.uiDirectHFRef = env->uiHFRef;
        s.iEnvFx = s.iEnv + Ftoi((double)s.iOcc * s.fOccLF + (double)s.iExc * s.fExcLF);
        if (gain != 1.0f) s.iEnvFx += GainToLevel(gain);
        if (s.iEnvFx < -10000) s.iEnvFx = -10000; else if (s.iEnvFx > 0) s.iEnvFx = 0;
        s.iEnvFxHF = s.iEnvHF + Ftoi((double)s.iOcc * (1.0f - s.fOccLF) + (double)s.iExc * (1.0f - s.fExcLF));
        if (s.iEnvFxHF < -10000) s.iEnvFxHF = -10000;
        s.uiEnvFxHFRef = env->uiHFRef;
    }
    if (!(s.ffCaps & 0x800000)) s.ffCommit &= ~0x800000u;
    s.ffUpdate |= s.ffCommit;
    s.ffCommit = 0;
}

// ---- comparison ----
static int g_bad = 0, g_case = 0;
static bool Near(float a, float b) { if (a == b) return true; float d = (float)fabs(a - b); return d <= 1e-4f || d <= 1e-4f * (float)fabs(b); }
static bool NearV(const V3& a, const V3& b) { return Near(a.x, b.x) && Near(a.y, b.y) && Near(a.z, b.z); }
static void Fail(const char* what) { if (g_bad < 20) printf("case %d: %s\n", g_case, what); ++g_bad; }
static void Compare(const Src& r, const Src& m)
{
    if (r.ffUpdate != m.ffUpdate) Fail("ffUpdate");
    if (r.ffCommit != m.ffCommit) Fail("ffCommit");
    if (!NearV(r.p3, m.p3)) Fail("m_3D.vPosition");
    if (!NearV(r.v3, m.v3)) Fail("m_3D.vVelocity");
    if (!NearV(r.c3, m.c3)) Fail("m_3D.vConeDirection");
    if (!NearV(r.d3, m.d3)) Fail("m_3D.vDirection");
    if (!NearV(r.cone, m.cone)) Fail("m_vConeDirection");
    if (!Near(r.fDistance, m.fDistance)) Fail("fDistance");
    if (!Near(r.fDistanceGain, m.fDistanceGain)) Fail("fDistanceGain");
    if (!Near(r.fConeGain, m.fConeGain)) Fail("fConeGain");
    if (!Near(r.fDoppler, m.fDoppler)) Fail("fDoppler");
    if (r.iDirect != m.iDirect) Fail("iDirect");
    if (r.iDirectHF != m.iDirectHF) Fail("iDirectHF");
    if (r.uiDirectHFRef != m.uiDirectHFRef) Fail("uiDirectHFReference");
    if (r.iEnvFx != m.iEnvFx) Fail("iEnvFx");
    if (r.iEnvFxHF != m.iEnvFxHF) Fail("iEnvFxHF");
    if (r.uiEnvFxHFRef != m.uiEnvFxHFRef) Fail("uiEnvFxHFReference");
    if (memcmp(&r.pos, &m.pos, 2 * sizeof(V3)) != 0 || r.fMacroFx != m.fMacroFx) Fail("inputs changed");
}

static Lis g_lis[3]; static Env g_env;
// Run the real Commit on a copy of s and the model on another; compare; check the callback.
static void Run(const Src& s, int nLis, bool withEnv, unsigned int g)
{
    ++g_case;
    for (int i = 0; i < 3; ++i) { g_lis[i].next = (i + 1 < nLis) ? &g_lis[i + 1] : 0; g_lis[i].prev = i ? &g_lis[i - 1] : 0; }
    Lst listeners = { 0, nLis ? &g_lis[0] : 0, 0, 0, (unsigned int)nLis };
    Lst envs = { 0, withEnv ? &g_env : 0, 0, 0, withEnv ? 1u : 0u };
    Src real = s, model = s;
    g_cbCalls = 0; g_cbSrc = g_cbLis = 0; m_cbCalls = 0; m_cbLis = 0; m_cbDist = 0;
    reinterpret_cast<CCoda3DSource*>(&real)->Commit(*reinterpret_cast<CCodaObjectList*>(&listeners), *reinterpret_cast<CCodaObjectList*>(&envs), g);
    Model(model, nLis ? &g_lis[0] : 0, withEnv ? &g_env : 0, g);
    Compare(real, model);
    if (g_cbCalls != m_cbCalls) Fail("callback count");
    else if (g_cbCalls && (g_cbSrc != &real || g_cbLis != m_cbLis || !Near(g_cbDist, m_cbDist) || g_cbData != s.cbData)) Fail("callback args");
}

static unsigned int g_seed = 12345;
static unsigned int Rnd() { g_seed = g_seed * 1103515245u + 12345u; return (g_seed >> 8) & 0xffffff; }
static int RndI(int lo, int hi) { return lo + (int)(Rnd() % (unsigned int)(hi - lo + 1)); }
static float Eighths(int lo, int hi) { return (float)RndI(lo * 8, hi * 8) / 8.0f; }
static void Axes(Lis& l)
{
    V3 r = { 1, 0, 0 }, t = { 0, 1, 0 }, f = { 0, 0, 1 };
    l.right = r; l.top = t; l.front = f;
}
static Src Base()
{
    Src s; memset(&s, 0, sizeof(s));
    s.ffCaps = 0x1000 | 0x80 | 0x800000; s.ffUpdate = 0x5;
    s.p3.x = 11; s.v3.y = 12; s.c3.z = 13; s.d3.x = 14; s.fDistance = 2; s.fDistanceGain = 15; s.fConeGain = 16; s.fDoppler = 17;
    s.iDirect = -5; s.iDirectHF = -6; s.uiDirectHFRef = 7; s.iEnvFx = -8; s.iEnvFxHF = -9; s.uiEnvFxHFRef = 10; s.fMacroFx = 0.5f;
    s.pos.x = 3; s.pos.y = 4; s.vel.x = 1; s.vel.z = -2; s.cone.y = 2;
    s.fMax = 100; s.fMin = 1; s.fRef = 2; s.fRolloff = 1; s.fDopp = 1; s.inner = 100; s.outer = 300; s.outerGain = 0.25f;
    s.iDirectSrc = -100; s.iDirectHFSrc = -200; s.iEnv = -300; s.iEnvHF = -400;
    s.iObs = -1000; s.fObsLF = 0.25f; s.iOcc = -2000; s.fOccLF = 0.5f; s.iExc = -400; s.fExcLF = 0.125f; s.fRollFactor = 0; s.fAirAbs = 0;
    return s;
}

int main()
{
    Ctx().fZAxis = -1.0f; Ctx().uiHFRef = 5000; Ctx().fDopplerSpeed = 0.5f; Ctx().fRolloffFactor = 1.0f;
    for (int i = 0; i < 3; ++i) { memset(&g_lis[i], 0, sizeof(Lis)); Axes(g_lis[i]); }
    g_lis[0].pos.x = 0; g_lis[1].pos.x = 10; g_lis[2].pos.x = -10;
    g_lis[0].vel.x = 0.5f; g_lis[1].vel.y = 1; g_lis[2].vel.z = 2;
    memset(&g_env, 0, sizeof(g_env)); g_env.uiHFRef = 7777; g_env.fRolloff = 0; g_env.iAirAbsHF = -3;

    Src s;
    // 1. nothing to commit: returns before touching the (empty) listener list
    s = Base(); Run(s, 0, true, 0);
    // 2. head-relative: each bit alone, all bits, and the callback receiving a null listener
    s = Base(); s.ffStatus = 0x40000; Run(s, 0, false, 0x8000); Run(s, 0, false, 0x10000); Run(s, 0, false, 0x80000);
    s.cone.x = 3; s.cone.y = 4; Run(s, 0, false, 0x98000);
    s.cb = DistanceCallback; s.cbData = 6; Run(s, 0, false, 0x40000);
    // 3. listeners: nearest of three (later strictly nearer wins; a tie keeps the earlier one)
    s = Base(); s.pos.x = 8; s.pos.y = 0; Run(s, 3, false, 0x98000);
    s.pos.x = 5; Run(s, 2, false, 0x98000);                          // tie between listener 0 (0) and 1 (10): keeps 0
    s.pos.x = -6; Run(s, 3, false, 0x98000);                         // nearest is the third
    s.cb = DistanceCallback; s.cbData = 3; s.pos.x = 9; Run(s, 3, false, 0x8000);   // callback gets the nearest listener
    // 4. 0x20000 implies 0x8000; 0x88000 gate on the right axis (Z-axis sign); position equal to the listener (IsZero direction)
    s = Base(); Run(s, 1, false, 0x20000); Run(s, 1, false, 0x80000); Run(s, 1, false, 0x10000);
    s.pos.x = 0; s.pos.y = 0; s.pos.z = 0; Run(s, 1, false, 0x8000);
    Ctx().fZAxis = 1.0f; s = Base(); Run(s, 1, false, 0x88000); Ctx().fZAxis = -1.0f;
    // 5. distance gain: mute bit with distance at / below the maximum, clamps at both ends
    s = Base(); s.ffStatus = 0x80000; s.fMax = 5; Run(s, 1, false, 0x8000);                       // d == max: muted to 0
    s.fMax = 5.5f; Run(s, 1, false, 0x8000);                                                       // below max: pow
    s = Base(); s.fMin = 6; Run(s, 1, false, 0x8000); s = Base(); s.fMax = 4; Run(s, 1, false, 0x8000);
    s = Base(); s.fMin = 5; s.fMax = 5; s.fRolloff = 2; Run(s, 1, false, 0x8000);
    s = Base(); s.fRolloff = 0.5f; Ctx().fRolloffFactor = 2.0f; Run(s, 1, false, 0x8000); Ctx().fRolloffFactor = 1.0f;   // context rolloff factor
    // 6. Doppler (positive, clamped below 0) and cone gain (inside, outside, between; both atan2 arms; inner == outer == 0)
    s = Base(); s.vel.x = 0.5f; Run(s, 1, false, 0x18000); s.vel.x = 20; Run(s, 1, false, 0x18000);
    s = Base(); s.cone.x = 3; s.cone.y = 4; s.cone.z = 0; s.inner = 10; s.outer = 20; Run(s, 1, false, 0x88000);    // cone == direction: 360
    s.cone.x = -3; s.cone.y = -4; Run(s, 1, false, 0x88000);                                       // opposite: angle 0
    s.inner = 0; s.outer = 0; Run(s, 1, false, 0x88000);
    s.cone.x = 4; s.cone.y = 3; s.inner = 100; s.outer = 300; Run(s, 1, false, 0x88000);           // perpendicular: about 180
    s.cone.x = 4; s.cone.y = 1; Run(s, 1, false, 0x88000); s.cone.x = 1; s.cone.y = -4; Run(s, 1, false, 0x88000);
    // 7. direct levels, caps masks
    s = Base(); Run(s, 1, false, 0x400000); s.ffCaps = 0; Run(s, 1, false, 0x400000); Run(s, 1, false, 0x1000000);
    s.ffCaps = 0x80; Run(s, 1, false, 0x800000); s.ffCaps = 0x800000; Run(s, 1, false, 0x1800000);
    // 8. environment levels: sal_ftoi ties / signs, every clamp at the boundary, upper clamp of the FX level, fGain == 1 / 0 / other
    s = Base(); Run(s, 1, true, 0x1000000); Run(s, 1, false, 0x1000000); Run(s, 1, true, 0x1400000);
    s.ffCaps = 0x1000 | 0x800000; Run(s, 1, true, 0x1000000);                                      // no reverb cap: env skipped
    s = Base(); s.iObs = 2; s.fObsLF = 0.5f; s.iOcc = 0; Run(s, 1, true, 0x1000000);                // level 1.0: tie rounds to 0
    s.iObs = 3; Run(s, 1, true, 0x1000000); s.iObs = -3; Run(s, 1, true, 0x1000000); s.iObs = 5; Run(s, 1, true, 0x1000000);
    // (0x1000000 lies inside the 0x1400000 mask, so the direct levels start from m_iDirect / m_iDirectHF)
    s = Base(); s.iObs = 0; s.iOcc = 0; s.iDirectSrc = -10000; Run(s, 1, true, 0x1000000);          // -10000 stays
    s.iDirectSrc = -10001; Run(s, 1, true, 0x1000000); s.iDirectSrc = -9999; s.iObs = -8; s.fObsLF = 0.25f; Run(s, 1, true, 0x1000000);
    s = Base(); s.iObs = 0; s.iOcc = 0; s.iDirectHFSrc = -10000; Run(s, 1, true, 0x1000000); s.iDirectHFSrc = -10001; Run(s, 1, true, 0x1000000);
    s = Base(); s.iOcc = 0; s.iExc = 0; s.iEnv = -10000; Run(s, 1, true, 0x1000000); s.iEnv = -10001; Run(s, 1, true, 0x1000000);
    s.iEnv = 0; Run(s, 1, true, 0x1000000); s.iEnv = 1; Run(s, 1, true, 0x1000000); s.iEnv = 7; s.iExc = 3; s.fExcLF = 1.0f; Run(s, 1, true, 0x1000000);
    s = Base(); s.iOcc = 0; s.iExc = 0; s.iEnvHF = -10000; Run(s, 1, true, 0x1000000); s.iEnvHF = -10001; Run(s, 1, true, 0x1000000);
    s.iEnvHF = 50; s.iExc = 16; s.fExcLF = 0.25f; Run(s, 1, true, 0x1000000);                      // HF uses (1 - ratio)
    s = Base(); s.fAirAbs = 2; g_env.iAirAbsHF = -3; Run(s, 1, true, 0x1008000);                   // air absorption: -3 * 5 * 2
    s = Base(); g_env.fRolloff = 1; s.fRollFactor = 1; s.fRef = 0.5f; Run(s, 1, true, 0x1008000);   // fGain = 0.1 -> -2000 mB
    s.ffStatus = 0x80000; s.fMax = 5; Run(s, 1, true, 0x1008000);                                   // fGain == 0 -> -10000
    s.ffStatus = 0; s.fRef = 5; Run(s, 1, true, 0x1008000);                                         // fGain == 1: no level change
    s.fRef = 10; s.iEnv = -100; Run(s, 1, true, 0x1008000);                                         // fGain == 2: +602 mB
    s.iEnv = 600; Run(s, 1, true, 0x1008000);                                                      // ... raising the FX level past 0: clamped
    s = Base(); s.pos.x = 30; s.pos.y = 40; s.fRef = 0.5f; s.iEnv = 12000; g_env.fRolloff = 3; s.fRollFactor = 1;
    Run(s, 1, true, 0x1008000);                                                                     // fGain 1e-6: -12000 mB clamped to -10000
    s = Base(); s.fRef = 0.5f; s.iEnv = -100; g_env.fRolloff = 2; s.fRollFactor = 1; Run(s, 1, true, 0x1008000);   // 0.1^2: -4000 mB
    g_env.fRolloff = 1; s.fRollFactor = 2; Run(s, 1, true, 0x1008000);                              // exponent from the source's factor
    g_env.fRolloff = 0;
    // 9. random sweep on exact grids
    for (int k = 0; k < 3000; ++k) {
        s = Base();
        static const unsigned int bits[] = { 0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x1 };
        unsigned int g = 0, c = 0;
        for (int b = 0; b < 11; ++b) { if (Rnd() & 1) g |= bits[b]; if ((Rnd() & 3) == 0) c |= bits[b]; }
        if ((Rnd() & 7) == 0) { g = 0; c = 0; }
        s.ffCommit = c; s.ffStatus = (Rnd() & 1) ? 0x40000 : 0; if (Rnd() & 1) s.ffStatus |= 0x80000;
        s.ffCaps = ((Rnd() & 1) ? 0x1000 : 0) | ((Rnd() & 1) ? 0x80 : 0) | ((Rnd() & 1) ? 0x800000 : 0);
        s.pos.x = (float)RndI(-20, 20); s.pos.y = (float)RndI(-20, 20); s.pos.z = (float)RndI(-20, 20);
        s.vel.x = Eighths(-4, 4); s.vel.y = Eighths(-4, 4); s.vel.z = Eighths(-4, 4);
        s.cone.x = (float)RndI(-3, 3); s.cone.y = (float)RndI(-3, 3); s.cone.z = (float)RndI(-3, 3);
        s.fMin = (float)RndI(1, 5); s.fMax = s.fMin + (float)RndI(0, 30); s.fRef = (float)RndI(1, 8); s.fRolloff = Eighths(0, 2);
        s.fDopp = Eighths(0, 2); s.inner = (unsigned int)RndI(0, 200); s.outer = s.inner + (unsigned int)RndI(0, 160); s.outerGain = Eighths(0, 1);
        s.iDirectSrc = RndI(-11000, 0); s.iDirectHFSrc = RndI(-11000, 0); s.iDirect = RndI(-11000, 0); s.iDirectHF = RndI(-11000, 0);
        s.iEnv = RndI(-11000, 100); s.iEnvHF = RndI(-11000, 100);
        s.iObs = RndI(-3000, 50); s.fObsLF = Eighths(0, 1); s.iOcc = RndI(-3000, 50); s.fOccLF = Eighths(0, 1); s.iExc = RndI(-3000, 50); s.fExcLF = Eighths(0, 1);
        if (Rnd() & 1) { s.cb = DistanceCallback; s.cbData = Rnd() & 7; }
        Ctx().fZAxis = (Rnd() & 1) ? 1.0f : -1.0f; Ctx().fRolloffFactor = (Rnd() & 1) ? 1.0f : 0.5f; s.ffUpdate = Rnd() & 0x3000f;
        for (int i = 0; i < 3; ++i) { g_lis[i].pos.x = (float)RndI(-20, 20); g_lis[i].pos.y = (float)RndI(-20, 20); g_lis[i].pos.z = (float)RndI(-20, 20);
            g_lis[i].vel.x = Eighths(-2, 2); g_lis[i].vel.y = Eighths(-2, 2); g_lis[i].vel.z = Eighths(-2, 2); }
        int n = (s.ffStatus & 0x40000) ? RndI(0, 3) : RndI(1, 3);
        Run(s, n, (Rnd() & 3) != 0, g);
    }
    printf(g_bad ? "00c41fb0_TEST FAIL (%d)\n" : "00c41fb0_TEST PASS\n", g_bad);
    return g_bad ? 1 : 0;
}