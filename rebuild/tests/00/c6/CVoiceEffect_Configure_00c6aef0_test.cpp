// Behaviour test for 0x00C6AEF0 (source body copied from src/00c6aef0.cpp; the retail inline-asm idioms are
// spelled through RA_* macros: the same __asm under VC7.1, a round-half-to-even C fallback under g++).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
// the retail inline-asm idioms, exactly as in the source (one instruction per __asm, no trailing semicolon)
#define RA_FISTP(v, r) __asm fld v __asm fistp r
#define RA_FISTP_PTR(v, p) __asm fld v __asm mov edx, p __asm fistp dword ptr [edx]
#define RA_RDTSC0(c) __asm xor eax, eax __asm xor edx, edx __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#define RA_RDTSC(c) __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#else
// g++ C fallback (test only): fistp in the default FPU mode rounds to nearest, ties to even
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
static long ra_round(double v) { return (long)__builtin_nearbyint(v); }
static unsigned long long ra_tsc = 1000000;
#define RA_FISTP(v, r) (r) = ra_round(v);
#define RA_FISTP_PTR(v, p) *(p) = ra_round(v);
#define RA_RDTSC0(c) (c) = (ra_tsc += 123457);
#define RA_RDTSC(c) (c) = (ra_tsc += 123457);
#endif
// ---- source body ----
// CVoiceEffect::Configure(const SVoiceEffectParameters* parameters, long mode) -- set up a voice's +0x2f8 effect from the voice parameter block: configure the +0x260 part (0x00C783F0) and, through its vtable slot 1, the +0x28c stage, then derive the delay length m_delaySamples = m_blockLength * trunc(parameters->DelayScale)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?Configure@CVoiceEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6AEF0, 92 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer (no PDB/RTTI; class and member names descriptive). Callers 0x00C544FE / 0x00C546BE
//   configure the effect at voice +0x2f8 from the voice's +0x290 parameter block with mode 0, right after the two float
//   setters 0x00C53740 / 0x00C537A0 on that block.
// Callees: 0x00C783F0 (thiscall on +0x260; the manifest's "CAudioChannel_ProcessEffects" is propagated), the +0x28c stage's
//   vtable slot 1 (thiscall, (owner, mode)).
// Retail-asm: 0x00C6AF1A `fld [edi+10h] ; fsub [0.5f] ; fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp+10h] ; test ; jge ; inc` -- libpk's inline-assembly truncation helper (value homed in the dead `parameters` slot, result in the dead `mode` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Layout: local model. Retail reaches the +0x28c stage through its vtable (`mov eax,[esi+28Ch] ; call [eax+4]`), so the
//   model calls it through a pointer (a member called by name is devirtualised).
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
struct CVoiceEffect;
struct SVoiceEffectParameters {
    unsigned char _pad00[0x10];
    float         DelayScale;                      // +0x10 delay length in blocks
};
struct CEffectPart260 {
    void Configure(CVoiceEffect* owner, const SVoiceEffectParameters* parameters);   // 0x00C783F0
};
struct CEffectStage28c {
    virtual void v00();
    virtual void Configure(CVoiceEffect* owner, long mode);                       // slot 1
};
struct CVoiceEffect {
    unsigned char   _pad000[0x23c];
    long            m_blockLength;                 // +0x23c samples per block
    unsigned char   _pad240[0x260 - 0x240];
    CEffectPart260  m_part260;                     // +0x260
    unsigned char   _pad261[0x28c - 0x261];
    CEffectStage28c m_stage28c;                    // +0x28c (polymorphic)
    unsigned char   _pad290[0x524 - 0x290];
    long            m_delaySamples;                // +0x524
    void Configure(const SVoiceEffectParameters* parameters, long mode);
};
void CVoiceEffect::Configure(const SVoiceEffectParameters* parameters, long mode)
{
    m_part260.Configure(this, parameters);
    CEffectStage28c* stage = &m_stage28c;
    stage->Configure(this, mode);
    m_delaySamples = m_blockLength * FloatToLongTruncate(parameters->DelayScale);
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

static long g_part = 0, g_mode = 0;
void CEffectPart260::Configure(CVoiceEffect*, const SVoiceEffectParameters*) { ++g_part; }
void CEffectStage28c::v00() { }
void CEffectStage28c::Configure(CVoiceEffect*, long mode) { g_mode = mode; }
int main()
{
    CVoiceEffect* e = new CVoiceEffect;
    SVoiceEffectParameters p; memset(&p, 0, sizeof(p));
    e->m_blockLength = 128;
    p.DelayScale = 3.7f; e->Configure(&p, 5);
    check(e->m_delaySamples == 128 * 3 && g_part == 1 && g_mode == 5, "3.7 -> 3 blocks");
    p.DelayScale = 2.0f; e->Configure(&p, 6);
    check(e->m_delaySamples == 128 * ref_trunc(2.0) && g_mode == 6, "2.0 (1.5 -> 2)");
    p.DelayScale = 3.0f; e->Configure(&p, 7);
    check(e->m_delaySamples == 128 * ref_trunc(3.0), "3.0 (2.5 -> 2: libpk's helper)");
    if (!g_fail) printf("00c6aef0_TEST PASS\n");
    return g_fail;
}