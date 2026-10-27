// CReverbProcessor::UpdateLineLengths(SReverbState* state, const SReverbParams* params) -- libpk reverb: set the four delay lines' pre-delay (seconds * sample rate, rounded up to a multiple of 4) and lengths ((size * 20 + 1) * the line's tuning scale * sample rate / 10, plus the pre-delay, rounded up to a multiple of 4)
// Leaf: ?UpdateLineLengths@CReverbProcessor
// Retail-asm: 0x00C77B59 `fld dword ptr [esp+1Ch]` ; 0x00C77B5D `fistp dword ptr [esp+10h]` and 0x00C77BC1 `fld dword ptr [esp+1Ch]` ; 0x00C77BC5 `fistp dword ptr [esp+18h]` -- libpk's inline-assembly truncation helper (rasm1 family (a)) expanded twice: the value less 0.5 is stored to the dead `params` argument slot (the helper's by-value parameter; the pointer lives in ebp) and reloaded for a bare dword fistp into a frame word (the local, then the dead `state` slot), followed by the C++ `< 0 ? +1` fix-up (`test ; jge ; inc`); VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The unsigned sample-rate conversions, the loop and the round-up-to-4 arithmetic are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77B30, 193 bytes (+15 int3), __thiscall, `ret 8`. Constants: 2^32 @ 0x0122DCB4,
//   0.5f @ 0x0122F59C, 20.0f @ 0x0122F160, 1.0f @ 0x0122DED8, 0.1f @ 0x0122EE58; the line tuning table @ 0x012BF0EC (20-byte
//   entries, the scale at +4 is read as [eax+0x012BF0EC] with eax = 4, 0x18, 0x2C, 0x40). The catalog name
//   "CAudioChannel_ApplyDistortionEffect" is descriptive; the model names follow the parked reference.
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
    void UpdateLineLengths(SReverbState* state, const SReverbParams* params);
};

struct SLineTuning {                       // 0x14 bytes, four at 0x012BF0EC
    float m_baseLength;                    // +0x00
    float m_lengthScale;                   // +0x04
    float m_trim;                          // +0x08
    float m_pan;                           // +0x0c
    float m_extra;                         // +0x10
};
extern const SLineTuning g_lineTunings[4];

// libpk's round-half-down helper (landed twin parked at 0x00C3FF70): retail inlines a hand-written
// `fld value ; fistp result` here. This reference spelling converts with a truncating cast instead.
// libpk's inline-assembly truncation helper: round-to-nearest of (x - 0.5) by a bare fistp, corrected to truncation for negatives
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}

void CReverbProcessor::UpdateLineLengths(SReverbState* state, const SReverbParams* params)
{
    long preDelay = (FloatToLongTrunc((float)m_pFormat->m_sampleRate * params->m_preDelay) + 3) & ~3;
    for (unsigned long i = 0; i < 4; i++) {        // an unsigned index: the table offset loop ends on `cmp eax,54h ; jb`
        state->m_lines[i].m_preDelayLength = preDelay;
        long length = FloatToLongTrunc((params->m_size * 20.0f + 1.0f) * g_lineTunings[i].m_lengthScale
                                          * (float)m_pFormat->m_sampleRate * 0.1f);
        state->m_lines[i].m_length = (preDelay + length + 3) & ~3;
    }
}
