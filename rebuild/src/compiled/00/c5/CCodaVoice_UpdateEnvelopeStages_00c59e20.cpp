// CCodaVoice::UpdateEnvelopeStages(const SEnvelopeParams* env, bool recompute) -- advance the amplitude envelope one block through its delay / attack / hold / decay / sustain stages (flag bits 0x5E of +0x28): count the delay and hold down, ramp the level up linearly to 1.0 during the attack, decay it exponentially towards the sustain level, re-track the sustain level on request; reports whether the level changed
// Leaf: ?UpdateEnvelopeStages@CCodaVoice@@QAE_NPBUSEnvelopeParams@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C59E20, 405 bytes (the last 25 are the case-index tables: a dword table of six
//   targets at 0x00C59F8C and a byte table at 0x00C59FA4), thiscall, `ret 8`.
// Identity: the landed caller CCodaVoice::UpdateEnvelope 0x00C5A480 (its only caller, call 0x00C5A4ED) binds this address as
//   `?UpdateEnvelopeStages@CCodaVoice@@QAE_NPBUSEnvelopeParams@@_N@Z` for every stage but release (0x20) and done (0x40).
//   GameCODA (no PDB/RTTI): descriptive names, the voice model of the landed CCodaVoice rows.
// Callees: LinearRampStep 0x00C59D40, ExponentialDecayStep 0x00C59D80, SecondsToMixBlocks 0x00C59B00 (landed, __cdecl).
// Constants: 1.0f 0x0122DED8, 1/32768 0x0129EF88.
// Jump table (checked by hand against retail): stage & 0x5E indexes the byte table {0:0, 2:1, 4:2, 8:3, 0x10:4, others:5}
//   whose dword targets are 0x00C59E66 (delay), 0x00C59E92 (attack), 0x00C59EEA (hold), 0x00C59F10 (decay), 0x00C59F57
//   (sustain), 0x00C59F76 (no change); delay falls into attack and hold into decay once their counters run out.
long __cdecl SecondsToMixBlocks(float seconds);                         // 0x00C59B00
float __cdecl LinearRampStep(float from, float to, float seconds);      // 0x00C59D40
float __cdecl ExponentialDecayStep(float from, float seconds);          // 0x00C59D80

struct SEnvelopeParams {                                // amplitude / pitch envelope description
    float delay;                                        // +0x00
    float attack;                                       // +0x04
    float hold;                                         // +0x08
    float decay;                                        // +0x0c
    float sustain;                                      // +0x10
    float release;                                      // +0x14
};

class CCodaVoice {                                      // vtable 0x012B5C10 (19 slots), 0xB4 bytes
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    unsigned char _pad04[0x24];
    unsigned long m_flags;                              // +0x28 (envelope stage bits 0x02..0x40)
    unsigned long m_x2c;                                // +0x2c
    float m_envLevel;                                   // +0x30
    float m_envDelta;                                   // +0x34 per-block step / factor
    long  m_envCounter;                                 // +0x38 mix blocks left in the stage
    bool UpdateEnvelopeStages(const SEnvelopeParams* env, bool recompute);
};

bool CCodaVoice::UpdateEnvelopeStages(const SEnvelopeParams* env, bool recompute)
{
    bool changed = false;
    if (env == 0) {
        if (recompute) {
            m_envLevel = 1.0f;
            return true;
        }
    } else {
        unsigned long stage = m_flags & 0x5e;
        switch (stage) {
        case 0:                                         // delay
            if (m_envCounter-- > 0)
                break;
            m_envDelta = LinearRampStep(0.0f, 1.0f, env->attack);
            stage = 2;
        case 2:                                         // attack
            if (recompute)
                m_envDelta = LinearRampStep(0.0f, 1.0f, env->attack);
            m_envLevel = m_envDelta + m_envLevel;
            if (m_envLevel >= 1.0f) {
                m_envLevel = 1.0f;
                m_envCounter = SecondsToMixBlocks(env->hold);
                stage = 4;
            }
            changed = true;
            break;
        case 4:                                         // hold
            if (m_envCounter-- > 0)
                break;
            m_envDelta = ExponentialDecayStep(1.0f, env->decay);
            stage = 8;
        case 8:                                         // decay
            if (recompute)
                m_envDelta = ExponentialDecayStep(1.0f, env->decay);
            if (env->sustain + 3.0517578125e-05f > m_envLevel) {
                m_envLevel = env->sustain;
                stage = 0x10;
            } else
                m_envLevel = m_envDelta * m_envLevel;
            changed = true;
            break;
        case 0x10:                                      // sustain
            if (recompute && m_envLevel != env->sustain) {
                m_envLevel = env->sustain;
                changed = true;
            }
            break;
        }
        m_flags = (m_flags & ~0x5e) | stage;
    }
    return changed;
}