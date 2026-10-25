// CReverbProcessor::Process(const float* dry, bool active, const float* const* inputs, unsigned long count) -- the software reverb's per-block entry: false until the reverb is set up (+0x294); else feed the dry block into the input delay line, run the comb bank into an aligned 0x200-byte scratch block, post-process it, run the +0x94 and +0x144 stages over the channel blocks, and remember whether this block was active (+0x28c)
// Leaf: ?Process@CReverbProcessor@@UAE_NPBM_NPBQBMK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A5D0, 123 bytes, thiscall `ret 10h`, `and esp,-16` frame.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 443 "0xC6A5D0 "CEditControlCentre::GetCurrentMask" = reverb per-block
//   Process"): `this` is agent443's CReverbProcessor (vtable 0x012B9594; ctor 0x00C6A650, dtor 0x00C6A780, which clears the
//   +0x294 ready byte): it is the only caller of the landed ProcessCombs 0x00C69FB0 (inputs, count, output), ProcessStage94
//   0x00C6A180 and ProcessStage144 0x00C6A200 (inputs, count). 0x00C69C60 mixes the first argument into the +0x54 delay
//   line (length +0x58, position +0x5c) through the DSP ops table 0x0143CCDC and tests the second against the stored
//   +0x28c byte; 0x00C6A080 takes (count, scratch block). Descriptive names (libpk: no PDB/RTTI); the CEditControlCentre
//   label and its h1..h5 callees are propagated.
// Virtual: the function's only reference is slot 4 of the reverb vtable (0x012B95A4 in 0x012B9594); the base table
//   0x012BF0C8 (the DSP effects' settings-block base, ctor 0x00C77440) has `_purecall` there, so Process is the base's
//   pure per-block entry (slots 1..3: settings copy 0x00C774A0, commit-and-process 0x00C774F0, 0x00C77590 / 0x00C69B60).
// Shape: the scratch block is a 16-aligned 128-float local (the landed char[0x1f4] gives the same frame) (`and esp,0FFFFFFF0h ; sub esp,204h`); the early `return false`
//   precedes the arguments' loads; `active` is re-read from its slot for the final byte store.
class CVtable_012bf0c8 {                          // vtable 0x012BF0C8: the DSP effects' settings-block base
public:
    virtual ~CVtable_012bf0c8();                   // slot 0
    virtual void v01();                            // slot 1 (0x00C774A0)
    virtual void v02();                            // slot 2 (0x00C774F0)
    virtual void v03();                            // slot 3
    virtual bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count) = 0;   // slot 4
};

class CReverbProcessor : public CVtable_012bf0c8 { // vtable 0x012B9594
public:
    unsigned char _pad004[0x28c - 4];
    bool          m_bWasActive;                    // +0x28c
    unsigned char _pad28d[0x294 - 0x28d];
    bool          m_bReady;                        // +0x294 (cleared by the destructor 0x00C6A780)
    void WriteInput(const float* dry, bool active, unsigned long count);           // 0x00C69C60
    void ProcessCombs(const float* const* inputs, unsigned long count, float* output);   // 0x00C69FB0
    void ProcessMix(unsigned long count, float* mix);                              // 0x00C6A080
    void ProcessStage94(const float* const* inputs, unsigned long count);          // 0x00C6A180
    void ProcessStage144(const float* const* inputs, unsigned long count);         // 0x00C6A200
    bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count);   // slot 4
};

bool CReverbProcessor::Process(const float* dry, bool active, const float* const* inputs, unsigned long count)
{
    __declspec(align(16)) float mix[128];                // one 128-sample mix block
    if (!m_bReady)
        return false;
    WriteInput(dry, active, count);
    ProcessCombs(inputs, count, mix);
    ProcessMix(count, mix);
    ProcessStage94(inputs, count);
    ProcessStage144(inputs, count);
    m_bWasActive = active;
    return true;
}
