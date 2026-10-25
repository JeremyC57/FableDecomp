// CReverbProcessor::ProcessStage94(const float* const* inputs, unsigned long count) -- run the reverb's +0x94 stage on this block: cross-fade it (current -> target settings, at the blend position) when the element being blended lies in the stage's element range, else process it with the current settings
// Leaf: ?ProcessStage94@CReverbProcessor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A180, 128 bytes, `ret 8`.
// Module: libpk's software reverb (vtable 0x012B9594, no PDB/RTTI). The only caller is the per-block Process 0x00C6A5D0
//   (landed under the propagated label CEditControlCentre::GetCurrentMask): 0x00C69C60, ProcessCombs 0x00C69FB0 (landed
//   as CReverbProcessor_00c6a460), 0x00C6A080, then ProcessStage94 0x00C6A180 and ProcessStage144 0x00C6A200 with
//   (inputs, count); the stages are the +0x94 / +0x144 parts the landed constructor
//   0x00C6A650 builds. The stage callees take the channel-pointer array as their fifth argument (0x00C75E90 indexes
//   it) and compare the blend element against their taps like the combs do, so the names stay descriptive.
// Callees: stage Blend 0x00C75E90, stage Process 0x00C75CE0 (thiscall on the stage, callee-popped).
// Layout: local model (no PDB class), offsets as in the landed ProcessCombs model: settings +0x04, target +0x08,
//   blend element +0x18, blend position +0x1c, delay line +0x60, the stage, the output buffer +0x288.
// Shape: the stage pointer is a named local formed first (`lea ecx,[eax+94h]`, `this` moves to eax), then both
//   settings pointers are read into locals (esi/edi) before the test; the range test is an inline `Covers(element)`
//   (`>=` then `<`: `jl` / `jge` to the Process arm).
struct SStageSettings94 { unsigned char _data[0xc0]; };
struct SStageSettings144 { unsigned char _data[0x40]; };
struct SReverbSettings {                           // the processor's current / target settings block
    unsigned char     _combs[0x88];                // +0x00 (the four 0x14-byte comb settings sit at +0x38)
    SStageSettings94  Stage94;                     // +0x88
    SStageSettings144 Stage144;                    // +0x148
};
struct CDelayLine {
    float*        m_pBuffer;
    unsigned long m_length;
    unsigned long m_position;
};
struct CReverbStage94 {                            // 0xb0-byte part at +0x94 (ctor 0x00C6A280)
    unsigned char _pad00[0xa8];
    long          m_firstElement;                  // +0xa8 first blend element this stage owns
    long          m_endElement;                    // +0xac one past its last
    bool Covers(long element) const { return element >= m_firstElement && element < m_endElement; }
    void Blend(const SStageSettings94* current, const SStageSettings94* target, CDelayLine* line, float* output,
               const float* const* inputs, long blendElement, unsigned long count, unsigned long blendPosition);   // 0x00C75E90
    void Process(const SStageSettings94* current, CDelayLine* line, float* output, const float* const* inputs,
                 unsigned long count);                                                                         // 0x00C75CE0
};
struct CReverbStage144 {                           // 0x9c-byte part at +0x144 (ctor 0x00C75980)
    unsigned char _pad00[0x94];
    long          m_firstElement;                  // +0x94
    long          m_endElement;                    // +0x98
    bool Covers(long element) const { return element >= m_firstElement && element < m_endElement; }
    void Blend(const SStageSettings144* current, const SStageSettings144* target, CDelayLine* line, float* output,
               const float* const* inputs, long blendElement, unsigned long count, unsigned long blendPosition);   // 0x00C75AB0
    void Process(const SStageSettings144* current, CDelayLine* line, float* output, const float* const* inputs,
                 unsigned long count);                                                                         // 0x00C75A20
};
struct CReverbProcessor {                          // vtable 0x012B9594 (ctor 0x00C6A650, dtor 0x00C6A780)
    void*            _vtable;                      // +0x00
    SReverbSettings* m_pSettings;                  // +0x04
    SReverbSettings* m_pTargetSettings;            // +0x08
    unsigned char    _pad0c[0x18 - 0x0c];
    long             m_blendElement;               // +0x18 the element being cross-faded
    unsigned long    m_blendPosition;              // +0x1c
    unsigned char    _pad20[0x60 - 0x20];
    CDelayLine       m_line60;                     // +0x60
    unsigned char    _pad6c[0x94 - 0x6c];
    CReverbStage94   m_stage94;                    // +0x94
    CReverbStage144  m_stage144;                   // +0x144
    unsigned char    _pad1e0[0x288 - 0x1e0];
    float*           m_pOutput;                    // +0x288
    void ProcessStage94(const float* const* inputs, unsigned long count);
};

void CReverbProcessor::ProcessStage94(const float* const* inputs, unsigned long count)
{
    CReverbStage94* stage = &m_stage94;
    SReverbSettings* settings = m_pSettings;
    SReverbSettings* target = m_pTargetSettings;
    if (stage->Covers(m_blendElement))
        stage->Blend(&settings->Stage94, &target->Stage94, &m_line60, m_pOutput, inputs, m_blendElement, count, m_blendPosition);
    else
        stage->Process(&settings->Stage94, &m_line60, m_pOutput, inputs, count);
}