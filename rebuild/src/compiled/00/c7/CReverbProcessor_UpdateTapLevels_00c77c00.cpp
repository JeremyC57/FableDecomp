// CReverbProcessor::UpdateTapLevels(SReverbState* state, const SReverbParams* params) -- set the four tap levels and the two output levels from 0.3 * the tap-level parameter and their scales, then the feedback mix (2 - spread) * mixScale clamped to 1
// Leaf: ?UpdateTapLevels@CReverbProcessor@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77C00, length 125 bytes (+ int3 padding), `ret 8`. Constants 0.3f (0x01238170), 2.0f (0x0122DCDC), 1.0f (0x0122DED8).
// Dispatcher bits 0xC000.
// Library: libpk software reverb (no PDB/RTTI); class, method and member names are descriptive. The dispatcher
// 0x00C783F0 calls this update when its dirty bit is set (push params ; push state ; mov ecx,processor).
// Shape: the scaled level stays on the x87 stack for the six products; the mix is a named local stored with
// `fst`, then compared (`fcomp 1.0 ; test ah,41h ; jne`) and overwritten with 1.0f when larger.
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
    void UpdateTapLevels(SReverbState* state, const SReverbParams* params);
};

void CReverbProcessor::UpdateTapLevels(SReverbState* state, const SReverbParams* params)
{
    float level = params->m_tapLevel * 0.3f;
    state->m_taps[0].m_level = level * m_tapScale[0];
    state->m_taps[1].m_level = level * m_tapScale[1];
    state->m_taps[2].m_level = level * m_tapScale[2];
    state->m_taps[3].m_level = level * m_tapScale[3];
    state->m_leftLevel = level * m_tapScale[4];
    state->m_rightLevel = level * m_tapScale[5];
    float mix = (2.0f - params->m_spread) * params->m_mixScale;
    state->m_feedbackMix = mix;
    if (mix > 1.0f)
        state->m_feedbackMix = 1.0f;
}