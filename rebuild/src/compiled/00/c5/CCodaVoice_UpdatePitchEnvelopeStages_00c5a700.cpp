// CCodaVoice::UpdatePitchEnvelopeStages(const SPitchEnvelopeParams* env, bool recompute) -- advance the pitch envelope one block through its delay / attack / hold / decay / sustain / release stages (flag bits 0x7E20 of +0x28): octave ramps of the two pitch scales up to 2^depth, a hold, a decay to the sustain fraction and the release back to 1.0; reports whether a scale changed
// Leaf: ?UpdatePitchEnvelopeStages@CCodaVoice@@QAE_NPBUSPitchEnvelopeParams@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5A700, 715 bytes, thiscall `ret 8` (a compare-tree switch over 0, 0x20, 0x200, 0x400,
//   0x800, 0x1000).
// Identity (descriptive; GameCODA has no PDB or RTTI): the pitch-envelope twin of the landed CCodaVoice::UpdateEnvelopeStages
//   0x00C59E20 (same null / recompute head, same stage-switch-and-write-back shape) over the pitch envelope +0x74..+0x84 the
//   landed voice model names; the landed OctaveRampStep 0x00C59DE0 names this body's call sites as "the pitch-envelope update
//   of the software voice". Only caller: the software voice block update 0x00C5ACB0 (call at 0x00C5AE81). The descriptor is
//   the envelope description plus two octave depths (+0x18, +0x1C). The catalog's "CAudioSource_UpdateScaleTransform" is
//   propagated.
// Callees: OctaveRampStep 0x00C59DE0, SecondsToMixBlocks 0x00C59B00 (landed, __cdecl), _CIpow 0x00BFEB40 (pow(2.0, x)).
// Shape: delay falls into attack and hold into decay once their counters run out; an expiring attack or decay sets
//   `recompute = true` and falls into the next case, which VC7.1 threads straight into that case's body (retail
//   `mov ebx,400h ; jmp`); the release stage ends in the done bit 0x4000.
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
long __cdecl SecondsToMixBlocks(float seconds);                         // 0x00C59B00
float __cdecl OctaveRampStep(float from, float to, float seconds);      // 0x00C59DE0

struct SEnvelopeParams {
    float delay;                                        // +0x00
    float attack;                                       // +0x04
    float hold;                                         // +0x08
    float decay;                                        // +0x0c
    float sustain;                                      // +0x10
    float release;                                      // +0x14
};
struct SPitchEnvelopeParams : SEnvelopeParams {
    float depth;                                        // +0x18 octaves (scale +0x74)
    float secondDepth;                                  // +0x1c octaves (scale +0x7c)
};

class CCodaVoice {
public:
    virtual void v00();
    unsigned char _pad04[0x24];
    unsigned long m_flags;                              // +0x28
    unsigned char _pad2c[0x74 - 0x2c];
    float m_pitchScale;                                 // +0x74
    float m_pitchStep;                                  // +0x78
    float m_secondScale;                                // +0x7c
    float m_secondStep;                                 // +0x80
    long  m_pitchCounter;                               // +0x84
    bool UpdatePitchEnvelopeStages(const SPitchEnvelopeParams* env, bool recompute);
};

bool CCodaVoice::UpdatePitchEnvelopeStages(const SPitchEnvelopeParams* env, bool recompute)
{
    bool changed = false;
    if (env == 0) {
        if (recompute) {
            m_pitchScale = 1.0f;
            m_secondScale = 1.0f;
            return true;
        }
    } else {
        unsigned long stage = m_flags & 0x7e20;
        switch (stage) {
        case 0x20:                                      // release
            changed = true;
            if (m_pitchCounter-- > 0) {
                if (env->depth != 0.0f)
                    m_pitchScale = m_pitchStep * m_pitchScale;
                if (env->secondDepth != 0.0f)
                    m_secondScale = m_secondStep * m_secondScale;
            } else {
                m_pitchScale = 1.0f;
                m_secondScale = 1.0f;
                stage = 0x4000;
            }
            break;
        case 0:                                         // delay
            if (m_pitchCounter-- > 0)
                break;
            m_pitchStep = OctaveRampStep(0.0f, env->depth, env->attack);
            m_secondStep = OctaveRampStep(0.0f, env->secondDepth, env->attack);
            m_pitchCounter = SecondsToMixBlocks(env->attack);
            stage = 0x200;
        case 0x200:                                     // attack
            changed = true;
            if (m_pitchCounter-- > 0) {
                if (env->depth != 0.0f)
                    m_pitchScale = m_pitchStep * m_pitchScale;
                if (env->secondDepth != 0.0f)
                    m_secondScale = m_secondStep * m_secondScale;
                break;
            }
            stage = 0x400;
            recompute = true;
        case 0x400:                                     // hold
            if (recompute) {
                m_pitchScale = (float)pow(2.0, env->depth);
                m_secondScale = (float)pow(2.0, env->secondDepth);
                m_pitchCounter = SecondsToMixBlocks(env->hold);
                changed = true;
            }
            if (m_pitchCounter-- > 0)
                break;
            m_pitchStep = OctaveRampStep(env->depth, 0.0f, env->decay);
            m_secondStep = OctaveRampStep(env->secondDepth, 0.0f, env->decay);
            m_pitchCounter = SecondsToMixBlocks((1.0f - env->sustain) * env->decay);
            stage = 0x800;
        case 0x800:                                     // decay
            changed = true;
            if (m_pitchCounter-- > 0) {
                if (env->depth != 0.0f)
                    m_pitchScale = m_pitchStep * m_pitchScale;
                if (env->secondDepth != 0.0f)
                    m_secondScale = m_secondStep * m_secondScale;
                break;
            }
            stage = 0x1000;
            recompute = true;
        case 0x1000:                                    // sustain
            if (recompute) {
                m_pitchScale = (float)pow(2.0, env->depth * env->sustain);
                m_secondScale = (float)pow(2.0, env->secondDepth * env->sustain);
                changed = true;
            }
            break;
        }
        m_flags = (m_flags & ~0x7e20) | stage;
    }
    return changed;
}