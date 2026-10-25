// CReverbProcessor::UpdateTapPans(SReverbState* state, const SReverbParams* params) -- set the four taps' pan pair from the fixed sign table times the spread (and width * spread), then the left / right direct (0.52 / 0.55 of 1 - spread) and cross (2 * spread * (1 - spread)) gains
// Grade: DIFFER(116v133) -- loop induction variables: retail walks the signs with a pointer (edi) plus a byte offset (eax) and the two tap fields with two pointers (edx/esi); ours shares one IV per pair (eax-4/eax-8, ecx-30h/ecx-2Ch)
// Leaf: ?UpdateTapPans@CReverbProcessor@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C780C0, length 133 bytes (+ int3 padding), `ret 8`. Never reads `this`. Sign table 0x012BF158 = {1,-1}, {-1,1}, {1,1},
// {-1,-1}; constants 1.0f (0x0122DED8), 0.52f (0x012BF1AC), 0.55f (0x0126663C). Dispatcher bits 0x6 (called
// twice there).
// Library: libpk software reverb (no PDB/RTTI); class, method and member names are descriptive. The dispatcher
// 0x00C783F0 calls this update when its dirty bit is set (push params ; push state ; mov ecx,processor).
// Shape: retail keeps the loop rolled with FOUR induction variables -- the sign pointer edi (x), the byte offset
// eax = 4*i (y, also the `cmp eax,24h ; jb` counter), and separate pointers for the taps' +0 and +4 fields (edx,
// esi). A plain `for (i = 0; i < 4; i++)` over the pairs is fully unrolled by VC7.1; only a step-2 unsigned loop
// (`i = 1; i < 9; i += 2`) stays rolled, but it shares one IV between both signs and one between both tap fields.
// Also probed (cc.py, no grade): `do { ... } while (++i < 4)` over m_taps[i] / a [4][2] table stays rolled with retail's
// `mov eax,4 ... cmp eax,24h` counter but still two shared IVs; pointer walks (tap++, sign += 2) give a `dec esi` counter.
// The four separate IVs look like LTCG (/GL) loop codegen; libpk's vorbisfile statics in this chunk are LTCG-private.
// The dry part and the cross gain stay on the x87 stack for the four output stores, in retail's store order.
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
    void UpdateTapPans(SReverbState* state, const SReverbParams* params);
};

extern const float g_tapPanSigns[8];          // 0x012BF158: {1,-1}, {-1,1}, {1,1}, {-1,-1} as (A, B) pairs

void CReverbProcessor::UpdateTapPans(SReverbState* state, const SReverbParams* params)
{
    SReverbTap* tap = state->m_taps;
    for (unsigned long i = 1; i < 9; i += 2, tap++) {
        tap->m_panA = g_tapPanSigns[i - 1] * params->m_spread;
        tap->m_panB = g_tapPanSigns[i] * params->m_spread * params->m_width;
    }
    float dry = 1.0f - params->m_spread;
    float cross = dry * params->m_spread * 2.0f;
    state->m_leftCross = cross;
    state->m_leftDirect = dry * 0.52f;
    state->m_rightCross = cross;
    state->m_rightDirect = dry * 0.55f;
}
