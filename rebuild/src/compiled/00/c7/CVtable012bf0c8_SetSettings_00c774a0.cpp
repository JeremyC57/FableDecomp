// CVtable_012bf0c8::SetSettings(const void* settings, bool immediate) -- copy a DSP effect's settings block in: straight into the current block when immediate, else into the pending block and flag it for the next commit
// Leaf: ?SetSettings@CVtable_012bf0c8@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C774A0, 67 bytes, thiscall, ret 8. No callees (memcpy is the
//   rep movsd / rep movsb intrinsic).
// Identity: slot 1 of the settings-block base vtable 0x012BF0C8 (xref 0x012BF0CC) and, inherited, of the reverb's
//   CReverbProcessor vtable 0x012B9594 (xref 0x012B9598); the landed CReverbProcessor ctor 0x00C6A650 models the base:
//   current settings +4, target +8, pending +0xc ("queued by slot 1 (0x00C774A0)"), block size +0x10, dirty flag +0x24.
//   libpk has no PDB/RTTI: the class keeps the landed vtable name, the method name is descriptive.
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
    virtual void SetSettings(const void* settings, bool immediate);   // slot 1
    void*         m_pSettings;                          // +0x04 current settings
    void*         m_pTargetSettings;                    // +0x08 settings being blended to
    void*         m_pPendingSettings;                   // +0x0c settings queued here
    unsigned long m_settingsSize;                       // +0x10 bytes per settings block
    bool          m_blending;                           // +0x14
    unsigned char _pad15[0x24 - 0x15];
    bool          m_dirty;                              // +0x24 pending settings to commit
};

void CVtable_012bf0c8::SetSettings(const void* settings, bool immediate)
{
    if (immediate) {
        memcpy(m_pSettings, settings, m_settingsSize);
    } else {
        memcpy(m_pPendingSettings, settings, m_settingsSize);
        m_dirty = true;
    }
}