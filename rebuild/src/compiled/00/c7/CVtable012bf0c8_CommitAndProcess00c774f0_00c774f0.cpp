// CVtable_012bf0c8::CommitAndProcess_00c774f0(const float* dry, bool active, const float* const* inputs, unsigned long count) -- vtable slot 2 of the DSP effects' settings-block base: when settings are pending and no blend is running, copy the pending block over the target block and start a blend (step 0, countdown 0x200 samples); run the effect's per-block Process (slot 4); while blending, count the block's samples off the countdown and at zero advance the blend step, ending the blend (step and countdown -1) after the last step; returns Process's result
// Leaf: ?CommitAndProcess_00c774f0@CVtable_012bf0c8@@UAE_NPBM_NPBQBMK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C774F0, 160 bytes, thiscall `ret 10h` (`this` in ebp; Process's bool stays in al).
// Identity: slot 2 of the settings-block base vtable 0x012BF0C8 (xref 0x012BF0D0) and, inherited, of the reverb's CReverbProcessor
//   vtable 0x012B9594 (xref 0x012B959C), the "commit-and-process" slot the landed CReverbProcessor::Process 0x00C6A5D0 documents;
//   slot 4 is that pure per-block Process(dry, active, inputs, count). libpk has no PDB/RTTI: the class keeps the landed vtable
//   name, the method name is descriptive. Catalogued as gap-scan sub_00c774f0.
// Layout: the landed CVtable_012bf0c8 model (0x00C774A0 SetSettings): target settings +0x08, pending +0x0C, size +0x10, blending
//   +0x14, dirty +0x24; plus the blend step +0x18, countdown +0x1C and step count +0x20 used here.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)

class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CVtable_012bf0c8 : public CDspObject {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    CVtable_012bf0c8();                                 // 0x00C77440
    virtual ~CVtable_012bf0c8();                        // 0x00C775A0
    virtual void SetSettings(const void* settings, bool immediate);   // slot 1: 0x00C774A0
    virtual bool CommitAndProcess_00c774f0(const float* dry, bool active, const float* const* inputs, unsigned long count);   // slot 2
    virtual void v03();                                 // slot 3 (0x00C77590; reverb 0x00C69B60)
    virtual bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count) = 0;   // slot 4
    void*         m_pSettings;                          // +0x04 current settings
    void*         m_pTargetSettings;                    // +0x08 settings being blended to
    void*         m_pPendingSettings;                   // +0x0c settings queued by SetSettings
    unsigned long m_settingsSize;                       // +0x10 bytes per settings block
    bool          m_blending;                           // +0x14
    long          m_blendStep;                          // +0x18
    long          m_blendCountdown;                     // +0x1c samples left in this step
    long          m_blendSteps;                         // +0x20
    bool          m_dirty;                              // +0x24 pending settings to commit
};

bool CVtable_012bf0c8::CommitAndProcess_00c774f0(const float* dry, bool active, const float* const* inputs, unsigned long count)
{
    if (m_dirty && !m_blending) {
        memcpy(m_pTargetSettings, m_pPendingSettings, m_settingsSize);
        m_dirty = false;
        m_blendStep = 0;
        m_blending = true;
        m_blendCountdown = 0x200;
    }
    bool result = Process(dry, active, inputs, count);
    if (m_blending) {
        m_blendCountdown -= count;
        if (m_blendCountdown <= 0) {
            if (++m_blendStep == m_blendSteps) {
                m_blendStep = -1;
                m_blending = false;
                m_blendCountdown = -1;
            } else {
                m_blendCountdown = 0x200;
            }
        }
    }
    return result;
}