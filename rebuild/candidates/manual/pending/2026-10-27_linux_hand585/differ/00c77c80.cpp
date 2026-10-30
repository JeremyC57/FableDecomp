// CReverbProcessor::UpdateLineAndTapLengths(SReverbState* state, const SReverbParams* params) -- size the four lines' all-pass delays (table delay in ms * (size * 20 + 1) at the mix rate) and the four taps' two delays ((size + table delay in s) at the mix rate), each truncated and rounded up to a multiple of 4 samples, and copy each line's all-pass coefficient from the table
// Grade: DIFFER(259v259) -- one register swap in the tap loop: retail keeps the store pointer in edx and the rate/length temporary in esi (as in the line loop), VC7.1 picks esi/edx; everything else (both loops, the unsigned 0x14-stride line loop, the retail-asm round-half-down helper, the 4x2 down-counted tap nest, frame and constants) matches. Tried (compile-only sweep, RTM and QFE 4035): unsigned/int counter mixes, separate tap/k counters, `!=` bounds, a named length temporary, a tap pointer, a table pointer, record references, a forceinline AlignedLength helper.
// Leaf: ?UpdateLineAndTapLengths@CReverbProcessor@@
// Retail-asm: 0x00C77CEF / 0x00C77D55 `fistp dword ptr [esp+10h]` / `fistp dword ptr [esp+1Ch]` after `fld dword ptr [esp+1Ch]` / `fld dword ptr [esp+20h]` (0x00C77CEB, 0x00C77D51) -- libpk's inline-assembly float-to-long helper inside its round-half-down (as the landed 0x00C77FC0 UpdateChorusLength and 0x00C3FF70: `value - 0.5f` stored to a dead slot and reloaded, a bare dword fistp, `if (n < 0) ++n`). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77C80, length 259 bytes, `ret 8`. Constants 20.0f (0x0122F160), 1.0f
// (0x0122DED8), 2^32 (0x0122DCB4, the unsigned rate fix-up), 0.001f (0x01266300), 0.5f (0x0122F59C). Tables: the four
// 0x14-byte line records at 0x012BF0E0 ({1.05, -0.4, 3.651 ms, 915.6, 0.1052}, ...; this row reads +4 and +8) and the
// four taps' two delays in seconds at 0x012BF178 ({0, 0.002449}, {0.000839, 0.002299}, ...).
// Library: libpk software reverb (no PDB/RTTI); class, method, member and table names are descriptive. The dispatcher
// 0x00C783F0 calls this update for dirty bit 0x10000 (0x00C784FB) and again after 0x00C77B30 for bits 0x12000
// (0x00C78515), always `push params ; push state ; mov ecx,processor`, like the landed UpdateTapLevels 0x00C77C00 just
// before it and UpdateChorusLength / UpdateDelayLength 0x00C77FC0 / 0x00C78040.
// Shape: the all-pass scale `size * 20 + 1` lives in the dead params slot and a float copy of the size in a frame word;
// the sample rate is re-read through m_pFormat every pass (the stores may alias it). Each length is
// `(RoundHalfDownToLong(x) + 3) & ~3` with the helpers of the landed UpdateChorusLength; the line coefficient is stored
// before the length (retail's `mov esi,[table+4] ; mov [edx+4],esi` first). The line loop is a `for` over 4 records
// (VC7.1 strength-reduces it to the 0x14 byte stride with `cmp eax,50h ; jb`), the tap loops a 4 x 2 nest that VC7.1
// turns into down-counters with one table pointer.
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
    long m_allpassLength;                  // +0x08 samples, multiple of 4 (set here)
    float m_allpassCoef;                   // +0x0c copied from the line table (set here)
    float m_gain;                          // +0x10
};

struct SReverbTap {                        // 0x30 bytes, four at state +0x88 (this row's stores start the record 8 bytes
                                           // before the landed copies' +0x90 base; panA/panB/level keep their state offsets)
    long m_delayLength[2];                 // +0x00 samples, multiples of 4 (set here)
    float m_panA;                          // +0x08
    float m_panB;                          // +0x0c
    unsigned char _unk10[0x1c];
    float m_level;                         // +0x2c
};

struct SReverbState {
    float m_roomGain;                      // +0x000
    unsigned char _unk004[0x34];
    SReverbLine m_lines[4];                // +0x038
    SReverbTap m_taps[4];                  // +0x088
    unsigned char _unk148[0x70];
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
    void UpdateLineAndTapLengths(SReverbState* state, const SReverbParams* params);
};

struct SReverbLineDesc {                   // 0x14 bytes, the four-line table at 0x012BF0E0
    float m_scale;                         // +0x00 (1.05, 1.14, 0.84, 0.93)
    float m_allpassCoef;                   // +0x04 (-0.4, 0.4, -0.4, -0.4)
    float m_allpassDelayMs;                // +0x08 (3.651, 3.515, 3.795, 3.946)
    float m_unk0c;                         // +0x0c (915.6, 872.5, 902.1, 922.4)
    float m_unk10;                         // +0x10 (0.1052, 0.0673, 0.1737, 0.2404)
};
extern const SReverbLineDesc g_reverbLineDescs[4];     // 0x012BF0E0
extern const float g_reverbTapDelays[4][2];            // 0x012BF178 seconds

// libpk's inline-assembly float-to-long helper (round to nearest, the FPU default)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
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

void CReverbProcessor::UpdateLineAndTapLengths(SReverbState* state, const SReverbParams* params)
{
    float allpassScale = params->m_size * 20.0f + 1.0f;
    float size = params->m_size;
    unsigned int i, j;
    for (i = 0; i < 4; i++) {
        state->m_lines[i].m_allpassCoef = g_reverbLineDescs[i].m_allpassCoef;
        state->m_lines[i].m_allpassLength =
            (RoundHalfDownToLong((float)m_pFormat->m_sampleRate * g_reverbLineDescs[i].m_allpassDelayMs * allpassScale * 0.001f) + 3) & ~3;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            state->m_taps[i].m_delayLength[j] =
                (RoundHalfDownToLong((size + g_reverbTapDelays[i][j]) * (float)m_pFormat->m_sampleRate) + 3) & ~3;
        }
    }
}
