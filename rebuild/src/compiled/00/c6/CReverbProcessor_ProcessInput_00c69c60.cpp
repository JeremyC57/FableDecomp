// CReverbProcessor::ProcessInput(const float* input, bool active, unsigned long count) -- feed one block into the reverb's shared delay line: when the input has just switched off, fade out the last (up to one pre-delay of) samples already written and reset both gains; write the block through the input gain (+0x6c) and the in-place line gain (+0x80), cross-fading current -> target settings while this stage's blend is active, in the tail run and the wrapped head run; when the input has just switched on, fade the new block in; then advance the write position
// Leaf: ?ProcessInput@CReverbProcessor@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C69C60, 837 bytes (+ int3 padding), `ret 0Ch`.
// Label: the manifest's "NScript::CQ_GuildTrainingSkillScript::UpdateAnimation" is propagated; the body belongs to libpk's
//   software reverb (vtable 0x012B9594, no PDB/RTTI): it is the first stage the per-block Process 0x00C6A5D0 runs
//   (h1(p1, p2, count), then ProcessCombs 0x00C69FB0, 0x00C6A080, ProcessStage94 0x00C6A180, ProcessStage144
//   0x00C6A200), and Process then stores `active` into +0x28c. Class, method and member names are descriptive.
// Callees: CDspGain::Reset 0x00C744E0, CDspGain::Apply 0x00C74CE0 (ret 14h), the gain's blend variant 0x00C74D30
//   (ret 1Ch: current params, target params, source, block, mode, count, blend position; the manifest's
//   "Animation_InterpolateHelper" is propagated too), and slot 2 ScaleRamp of the shared ops table 0x0143CCDC.
// Constants: 4294967296.0f @ 0x0122DCB4 (unsigned -> float), -1.0f @ 0x0122DEE0, 1.0f @ 0x0122DED8.
// Layout: local model, offsets as in the landed ProcessCombs / ProcessStage94 models: current / target settings +0x04 /
//   +0x08 (two 0x1c-byte gain parameter sets at +0, the first reverb line's pre-delay length at +0x38), blending flag
//   +0x14, blend element +0x18, blend position +0x1c, shared delay line +0x54, input gain +0x6c, line gain +0x80, the
//   previous block's `active` +0x28c, this stage's blend element +0x290.
// Shape: both settings pointers read first; each ramp writes `(float)n` at every use (VC7.1 shares the one conversion in a
//   frame slot and keeps two `fdiv`s; a float local would become one reciprocal multiply); the split-run length is
//   recomputed in each arm from the members.
struct CDelayLine {
    float*        m_pBuffer;
    unsigned long m_length;
    unsigned long m_position;
};

struct CDspOps {
    virtual long GetKind();
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned long count);
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned long count);   // slot 2
};

struct CDspObject {
    virtual ~CDspObject();
    static CDspOps* s_pOps;                        // 0x0143CCDC
};

struct SGainParams {
    float m_master;
    float m_current[3];
    float m_target[3];
};

struct SReverbLine {
    long  m_preDelayLength;
    long  m_length;
    float _pad08[2];
    float m_gain;
};

struct SReverbSettings {
    SGainParams Gains[2];                          // +0x00 input gain, line gain
    SReverbLine Lines[4];                          // +0x38
};

struct CDspGain : CDspObject {
    float m_gain[4];
    void Reset();                                                                                     // 0x00C744E0
    void Apply(const SGainParams* params, const float* source, float* block, bool mode, unsigned long count);   // 0x00C74CE0
    void Blend(const SGainParams* current, const SGainParams* target, const float* source, float* block, bool mode,
               unsigned long count, unsigned long blendPosition);                                    // 0x00C74D30
};

struct CReverbProcessor {
    void*            _vtable;                      // +0x000
    SReverbSettings* m_pSettings;                  // +0x004
    SReverbSettings* m_pTargetSettings;            // +0x008
    unsigned char    _pad0c[0x14 - 0x0c];
    bool             m_blending;                   // +0x014
    long             m_blendElement;               // +0x018
    unsigned long    m_blendPosition;              // +0x01c
    unsigned char    _pad20[0x54 - 0x20];
    CDelayLine       m_line;                       // +0x054
    unsigned char    _pad60[0x6c - 0x60];
    CDspGain         m_inputGain;                  // +0x06c
    CDspGain         m_lineGain;                   // +0x080
    unsigned char    _pad94[0x28c - 0x94];
    bool             m_wasActive;                  // +0x28c
    long             m_inputElement;               // +0x290
    void ProcessInput(const float* input, bool active, unsigned long count);
};

void CReverbProcessor::ProcessInput(const float* input, bool active, unsigned long count)
{
    SReverbSettings* settings = m_pSettings;
    SReverbSettings* target = m_pTargetSettings;
    if (!active && m_wasActive) {
        unsigned long fade = count;
        if (fade > (unsigned long)settings->Lines[0].m_preDelayLength)
            fade = settings->Lines[0].m_preDelayLength;
        if (fade != 0) {
            unsigned long pos = m_line.m_position;
            unsigned long start;
            if (pos >= fade)
                start = pos - fade;
            else
                start = m_line.m_length - fade + pos;
            unsigned long first = fade;
            if (start + fade > m_line.m_length)
                first = m_line.m_length - start;
            float step = -1.0f / (float)fade;
            CDspObject::s_pOps->ScaleRamp(&m_line.m_pBuffer[start], 1.0f, step, first);
            if (first < fade)
                CDspObject::s_pOps->ScaleRamp(m_line.m_pBuffer, 1.0f - (float)first / (float)fade, step, fade - first);
        }
        m_inputGain.Reset();
        m_lineGain.Reset();
    }
    if (m_blending && m_blendElement == m_inputElement) {
        unsigned long first = count;
        if (m_line.m_position + count > m_line.m_length)
            first = m_line.m_length - m_line.m_position;
        m_inputGain.Blend(&settings->Gains[0], &target->Gains[0], input, &m_line.m_pBuffer[m_line.m_position], active, first,
                          m_blendPosition);
        m_lineGain.Blend(&settings->Gains[1], &target->Gains[1], &m_line.m_pBuffer[m_line.m_position],
                         &m_line.m_pBuffer[m_line.m_position], true, first, m_blendPosition);
        if (first < count) {
            m_inputGain.Blend(&settings->Gains[0], &target->Gains[0], input + first, m_line.m_pBuffer, active, count - first,
                              m_blendPosition - first);
            m_lineGain.Blend(&settings->Gains[1], &target->Gains[1], m_line.m_pBuffer, m_line.m_pBuffer, true, count - first,
                             m_blendPosition - first);
        }
    } else {
        unsigned long first = count;
        if (m_line.m_position + count > m_line.m_length)
            first = m_line.m_length - m_line.m_position;
        m_inputGain.Apply(&settings->Gains[0], input, &m_line.m_pBuffer[m_line.m_position], active, first);
        m_lineGain.Apply(&settings->Gains[1], &m_line.m_pBuffer[m_line.m_position], &m_line.m_pBuffer[m_line.m_position], true,
                         first);
        if (first < count) {
            m_inputGain.Apply(&settings->Gains[0], input + first, m_line.m_pBuffer, active, count - first);
            m_lineGain.Apply(&settings->Gains[1], m_line.m_pBuffer, m_line.m_pBuffer, true, count - first);
        }
    }
    if (active && !m_wasActive) {
        unsigned long first = count;
        if (m_line.m_position + count > m_line.m_length)
            first = m_line.m_length - m_line.m_position;
        float step = 1.0f / (float)count;
        CDspObject::s_pOps->ScaleRamp(&m_line.m_pBuffer[m_line.m_position], 0.0f, step, first);
        if (first < count)
            CDspObject::s_pOps->ScaleRamp(m_line.m_pBuffer, (float)first / (float)count, step, count - first);
    }
    m_line.m_position += count;
    if (m_line.m_position >= m_line.m_length)
        m_line.m_position -= m_line.m_length;
}