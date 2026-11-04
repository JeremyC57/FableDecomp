// CReverbProcessor::UpdateTapDelays(SReverbState* state, const SReverbParams* params) -- map the clamped room scale (0.9..2.5) to 0..1, set both outputs' base delay, then lay out each output's four tap delays from the early-reflection time table, one line segment per tap
// Leaf: ?UpdateTapDelays@CReverbProcessor@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C78150, 561 bytes, thiscall `ret 8`. Constants 0.9f (0x012BF1B4), 0.8f (0x01254DA8),
//   1/1.7 = 0.5882353f (0x012BF1B0), 2.5f (0x01241D68), 0.5f (0x0122F59C), 1.0f (0x0122DED8), 0.0f (0x0122DEDC), 2^32 (0x0122DCB4,
//   the unsigned fix-up); the early-reflection time table s_tapTimes[2][4] at 0x012BF138 (rows at 0x012BF138 / 0x012BF148).
// Retail-asm: 0x00C781CB `fld dword ptr [esp+28h]` + 0x00C781CF `fistp dword ptr [esp+8]`, 0x00C7827D `fld dword ptr [esp+20h]` +
//   0x00C78281 `fistp dword ptr [esp+24h]`, 0x00C782E1 `fld dword ptr [esp+20h]` + 0x00C782E5 `fistp dword ptr [esp+2Ch]` -- libpk's
//   round-half-down (landed RoundHalfDownToLong 0x00C3FF70) expanded in place: `value - 0.5f` homed and reloaded, a bare dword
//   fistp, then the C++ `+1 if negative`. VC7.1 C++ `(long)x` / `(int)x` / `(short)x` / temporaries / `(double)` / a same-TU C++
//   callee call __ftol2, /QIfist emits `fistp qword`, `_mm_cvtss_si32` / `_mm_cvttss_si32` emit cvtss2si / cvttss2si (NOTES.md
//   "C++ attempts before helper"), so the dword fistp is the original's inline asm, taken from the shared libpk/FtoL.h helper.
// Helper: libpk/FtoL.h (two-path: the byte-matching asm, and the FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Library: libpk software reverb (no PDB/RTTI); class, method and member names are descriptive (the CReverbProcessor family of the
//   landed 0x00C77FC0 / 0x00C78040 updates). Dispatcher: CEffectPart260::Configure 0x00C783F0 calls it first, for dirty bit 1
//   (binding the placeholder Stage_00c78150; NAME_FIXES.tsv), then marks 0x9106 dirty. The catalog's
//   "CAudioChannel_ApplyEqualizerEffect" is propagated.
// Shape: the clamp is `if (room < 0.9f) room = 0.9f;` then `room < 2.5f ? (room - 0.8f) * k : 1.0f` (retail keeps the
//   clamped value on the x87 stack); the column offset `i * segment` is a local of the inner loop body (VC7.1 hoists the
//   multiply but converts it per tap); the first-column factor is the inline `?:` (kept on the x87 stack and popped).
#include "libpk/FtoL.h"

struct SMixFormat {
    unsigned long m_flags;                 // +0x00
    unsigned long m_channels;              // +0x04
    unsigned long m_sampleRate;            // +0x08
    unsigned char _unk0c[0x04];
    unsigned long m_lineBase;              // +0x10 samples
    unsigned long m_lineBytes;             // +0x14 bytes per tap line segment
};

struct SReverbParams {
    unsigned long m_dirty;                 // +0x00
    unsigned char _unk04[0x60];
    float m_roomScale;                     // +0x64
};

struct SReverbOutput {                     // 0x3c bytes, left at state +0x1a8, right at +0x1e4
    unsigned long m_tapDelay[4];           // +0x00 samples into the tap buffer, multiple of 4
    unsigned char _unk10[0x24];
    long m_baseDelay;                      // +0x34 samples, multiple of 4
    float m_level;                         // +0x38
};

struct SReverbState {
    unsigned char _unk000[0x148];
    unsigned long m_tapExtent[3];          // +0x148
    unsigned char _unk154[0x54];
    SReverbOutput m_outputs[2];            // +0x1a8
};

class CReverbProcessor {
public:
    float m_lineScale[4];                  // +0x00
    float m_tapScale[6];                   // +0x10
    SMixFormat* m_pFormat;                 // +0x28
    static const float s_tapTimes[2][4];   // 0x012BF138
    void UpdateTapDelays(SReverbState* state, const SReverbParams* params);
};

__forceinline long RoundHalfDownToLong(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

void CReverbProcessor::UpdateTapDelays(SReverbState* state, const SReverbParams* params)
{
    float room = params->m_roomScale;
    float scale;
    if (room < 0.9f)
        room = 0.9f;
    if (room < 2.5f)
        scale = (room - 0.8f) * 0.5882353f;
    else
        scale = 1.0f;
    long base = (RoundHalfDownToLong((float)m_pFormat->m_sampleRate * scale) + 3) & ~3;
    unsigned long segment = m_pFormat->m_lineBytes >> 2;
    unsigned long lineBase = m_pFormat->m_lineBase;
    unsigned long extent = 0;
    state->m_outputs[0].m_baseDelay = base;
    state->m_outputs[1].m_baseDelay = base;
    float previous = 0.0f;
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 2; ++j) {
            unsigned long offset = i * segment;
            unsigned long delay = RoundHalfDownToLong((s_tapTimes[j][i] - previous) * (float)m_pFormat->m_sampleRate * scale);
            if (delay > segment - 128)
                delay = segment - 128;
            unsigned long tap = (RoundHalfDownToLong((float)lineBase * (i == 0 ? scale : 1.0f) + (float)delay + (float)offset) + 3) & ~3;
            state->m_outputs[j].m_tapDelay[i] = tap;
            if (i < 3 && extent < tap) {
                extent = tap;
                state->m_tapExtent[i] = tap;
            }
        }
        previous = 0.0f;
        if (previous < s_tapTimes[0][i])
            previous = s_tapTimes[0][i];
        if (previous < s_tapTimes[1][i])
            previous = s_tapTimes[1][i];
    }
}