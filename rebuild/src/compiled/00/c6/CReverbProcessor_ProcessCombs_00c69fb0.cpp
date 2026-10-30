// CReverbProcessor::ProcessCombs(const float* const* inputs, unsigned long count, float* output) -- clear the output block, then run the four comb filters over it, each fed from the next input channel (wrapping at the channel count) and cross-faded between the current and target settings while a blend is active on that comb's mode
// Leaf: ?ProcessCombs@CReverbProcessor@@QAEXPBQBMKPAM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69FB0, 197 bytes (row padded to 208), `ret 0Ch`.
// Naming (sweep1, 2026-10-25; FINDINGS chunk 443): the address suffix is dropped so the software reverb (vtable
//   0x012B9594) has one class name, agent443's CReverbProcessor (ProcessStage94 0x00C6A180, ProcessStage144 0x00C6A200,
//   and the per-block CReverbProcessor::Process 0x00C6A5D0, this function's only caller).
// Label: the manifest's "NScript::CQ_GuildTrainingSkillScript::ProcessMask" is propagated: the body belongs to the audio DSP
// module at 0x00C6xxxx (its only caller is 0x00C6A615; the four 0x14-byte combs at +0x1F8 are the ones the set-up routine
// 0x00C6A460 initialises). Class and member names are descriptive.
// Callees: comb Blend(current, target, line, output, input, count, blendPosition) @ 0x00C76120 and comb Process(current, line,
// output, input, count) @ 0x00C75FF0 (both thiscall on the comb).
// Layout: current / target settings pointers +0x04 / +0x08 (comb settings, 0x14 bytes each, at settings +0x38), blending flag
// +0x14, blend mode +0x18, blend position +0x1c, input channel count +0x2c, shared delay line +0x54, combs +0x1f8 (mode at +0x10).
// Shape: both settings pointers are read into locals before the memset (`rep stosd` of `count` dwords); the per-comb target
// settings address is strength-reduced to `current + (target - current)`; the channel index wraps with an unsigned `>=`.
#include <string.h>

struct SCombSettings { unsigned char _data[0x14]; };
struct SReverbSettings {
    unsigned char _pad00[0x38];
    SCombSettings Combs[4];                        // +0x38
};
struct CRingBuffer {
    float*        m_pBuffer;
    unsigned long m_length;
    unsigned long m_position;
};
struct CCombFilter {
    unsigned char _pad00[0x10];
    long          m_mode;                          // +0x10
    void Blend(const SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output,
               const float* input, unsigned long count, unsigned long blendPosition);          // 0x00C76120
    void Process(const SCombSettings* current, CRingBuffer* line, float* output, const float* input,
                 unsigned long count);                                                     // 0x00C75FF0
};

struct CReverbProcessor {
    void*            _vtable;                      // +0x00
    SReverbSettings* m_pSettings;                  // +0x04
    SReverbSettings* m_pTargetSettings;            // +0x08
    unsigned char    _pad0c[0x14 - 0x0c];
    bool             m_blending;                   // +0x14
    long             m_blendMode;                  // +0x18
    unsigned long    m_blendPosition;              // +0x1c
    unsigned char    _pad20[0x2c - 0x20];
    unsigned long    m_inputCount;                 // +0x2c
    unsigned char    _pad30[0x54 - 0x30];
    CRingBuffer       m_line;                       // +0x54
    unsigned char    _pad60[0x1f8 - 0x60];
    CCombFilter      m_combs[4];                   // +0x1f8
    void ProcessCombs(const float* const* inputs, unsigned long count, float* output);
};

void CReverbProcessor::ProcessCombs(const float* const* inputs, unsigned long count, float* output)
{
    SReverbSettings* settings = m_pSettings;
    SReverbSettings* target = m_pTargetSettings;
    memset(output, 0, count * sizeof(float));
    unsigned long input = 0;
    for (int i = 0; i < 4; ++i) {
        if (m_blending && m_blendMode == m_combs[i].m_mode)
            m_combs[i].Blend(&settings->Combs[i], &target->Combs[i], &m_line, output, inputs[input], count, m_blendPosition);
        else
            m_combs[i].Process(&settings->Combs[i], &m_line, output, inputs[input], count);
        if (++input >= m_inputCount)
            input = 0;
    }
}