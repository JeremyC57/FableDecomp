// Behaviour test for 0x00C77FC0 (source body copied from src/00c77fc0.cpp; the retail inline-asm idioms are
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
// CReverbProcessor::UpdateChorusLength(SReverbState* state, const SReverbParams* params) -- set the chorus delay length in samples: round(sample rate * chorus delay - 0.5) biased up when negative, rounded up to a multiple of 4
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?UpdateChorusLength@CReverbProcessor@@
// Retail-asm: 0x00C77FE5 `fld dword ptr [esp+0Ch]` + 0x00C77FE9 `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long helper inside its round-half-down (as 0x00C3FF70: the `value - 0.5f` stored to the dead params slot and reloaded, a bare dword fistp into the `push ecx` frame word). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77FC0, length 72 bytes, `ret 8`. Constants 2^32 (0x0122DCB4, unsigned rate fix-up), 0.5f (0x0122F59C).
// Dispatcher bit 0x20000.
// Library: libpk software reverb (no PDB/RTTI); class, method and member names are descriptive. The dispatcher
// 0x00C783F0 calls this update when its dirty bit is set (push params ; push state ; mov ecx,processor).
// ---- libpk software reverb model (no PDB/RTTI: every name below is descriptive) ----
// The processor (`this`) holds per-line / per-tap scale factors and the mixer format; the parameter block is the
// dirty-flagged reverb description handed to the dispatcher 0x00C783F0, the state block is what the per-sample
// loop reads. Layouts are taken from the offsets the dispatcher and its update helpers touch.
struct SMixFormat {
    unsigned long m_flags;                 // +0x00
    unsigned long m_channels;              // +0x04
    unsigned long m_sampleRate;            // +0x08
    unsigned char _unk0c[0x0c];
    unsigned long m_maxDelaySamples;       // +0x18
};

struct SReverbParams {
    unsigned long m_dirty;                 // +0x00 bit mask of changed parameters (cleared by the dispatcher)
    long m_room;                           // +0x04 millibels
    long m_roomHF;                         // +0x08
    long m_reflections;                    // +0x0c
    float m_decayTime;                     // +0x10
    float m_decayHFRatio;                  // +0x14
    float m_decayLFRatio;                  // +0x18
    float m_reflectionsDelay;              // +0x1c
    long m_reverb;                         // +0x20
    float m_feedback;                      // +0x24 doubled into the four line gains
    float m_preDelay;                      // +0x28 seconds before the lines start
    float m_tapLevel;                      // +0x2c
    float m_size;                          // +0x30 line-length scale
    float m_spread;                        // +0x34 tap pan / dry split
    float m_width;                         // +0x38
    float m_chorusDelay;                   // +0x3c seconds
    float m_mixScale;                      // +0x40
    float m_delayTime;                     // +0x44
    float m_delayDepth;                    // +0x48
};

struct SReverbLine {                       // 0x14 bytes, four at state +0x38
    long m_preDelayLength;                 // +0x00 samples, multiple of 4
    long m_length;                         // +0x04 samples, multiple of 4
    unsigned char _unk08[8];
    float m_gain;                          // +0x10
};

struct SReverbTap {                        // 0x30 bytes, four at state +0x90
    float m_panA;                          // +0x00
    float m_panB;                          // +0x04
    unsigned char _unk08[0x1c];
    float m_level;                         // +0x24
    unsigned char _unk28[8];
};

struct SReverbState {
    float m_roomGain;                      // +0x000
    unsigned char _unk004[0x34];
    SReverbLine m_lines[4];                // +0x038
    unsigned char _unk088[0x08];
    SReverbTap m_taps[4];                  // +0x090
    unsigned char _unk150[0x68];
    float m_leftDirect;                    // +0x1b8
    float m_leftCross;                     // +0x1bc
    unsigned char _unk1c0[0x20];
    float m_leftLevel;                     // +0x1e0
    unsigned char _unk1e4[0x10];
    float m_rightDirect;                   // +0x1f4
    float m_rightCross;                    // +0x1f8
    unsigned char _unk1fc[0x20];
    float m_rightLevel;                    // +0x21c
    float m_modulationRate;                // +0x220
    float m_delayLength;                   // +0x224 samples
    long m_chorusLength;                   // +0x228 samples, multiple of 4
    unsigned char _unk22c[4];
    float m_feedbackMix;                   // +0x230 clamped to 1
};

class CReverbProcessor {
public:
    float m_lineScale[4];                  // +0x00
    float m_tapScale[6];                   // +0x10 four taps, then the left / right outputs
    SMixFormat* m_pFormat;                 // +0x28
    void UpdateChorusLength(SReverbState* state, const SReverbParams* params);
};

// libpk's inline-assembly float-to-long helper (round to nearest, the FPU default)
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}
// libpk's round-half-down helper (out-of-line copy at 0x00C3FF70): round(value - 0.5), negatives step back up by one
__forceinline long RoundHalfDownToLong(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

void CReverbProcessor::UpdateChorusLength(SReverbState* state, const SReverbParams* params)
{
    state->m_chorusLength = (RoundHalfDownToLong((float)m_pFormat->m_sampleRate * params->m_chorusDelay) + 3) & ~3;
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

static long g_buf[0x100], g_state[0x100];
int main()
{
    CReverbProcessor* p = (CReverbProcessor*)g_buf;
    SMixFormat fmt; memset(&fmt, 0, sizeof(fmt));
    SReverbParams prm; memset(&prm, 0, sizeof(prm));
    SReverbState* st = (SReverbState*)g_state;
    p->m_pFormat = &fmt;
    fmt.m_sampleRate = 44100; prm.m_chorusDelay = 0.5f;   p->UpdateChorusLength(st, &prm);
    check(st->m_chorusLength == 22052, "22050 (22049.5 -> 22050) up to a multiple of 4");
    fmt.m_sampleRate = 48000; prm.m_chorusDelay = 0.25f;  p->UpdateChorusLength(st, &prm);
    check(st->m_chorusLength == 12000, "11999.5 -> 12000");
    fmt.m_sampleRate = 1000; prm.m_chorusDelay = 0.0035f; p->UpdateChorusLength(st, &prm);
    check(st->m_chorusLength == ((ref_trunc(1000 * 0.0035f) + 3) & ~3), "small");
    if (!g_fail) printf("00c77fc0_TEST PASS\n");
    return g_fail;
}