// CVtable_012bf0c8::StopBlend() -- end the settings blend: step -1, not blending, countdown -1
// Leaf: ?StopBlend@CVtable_012bf0c8@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77430, 14 bytes, thiscall `ret`: `or eax,-1 ; mov [ecx+18h],eax ; mov byte ptr [ecx+14h],0 ; mov [ecx+1Ch],eax`.
// Identity: no reference (an out-of-line copy of an inline member) in the settings-block base's TU, right before its constructor
//   0x00C77440 (StartBlend 0x00C77410, StopBlend 0x00C77430, then the ctor, ReleaseBlock 0x00C77470, SetSettings 0x00C774A0,
//   CommitAndProcess 0x00C774F0). The body is the blend end CommitAndProcess 0x00C774F0 expands after the last step (`m_blendStep = -1; m_blending = false; m_blendCountdown = -1;`, same stores in the same order); the fields are the landed CVtable_012bf0c8 model's (blending +0x14,
//   blend step +0x18, countdown +0x1C). libpk has no PDB/RTTI: the class keeps the landed vtable name, the method is descriptive.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420
    virtual ~CVtable_012bf050();                              // 0x00C74450
};

class CVtable_012bf0c8 : public CVtable_012bf050 {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    CVtable_012bf0c8();                                 // 0x00C77440
    virtual ~CVtable_012bf0c8();
    void StartBlend();                                  // 0x00C77410
    void StopBlend();                                   // 0x00C77430
    void*         m_pSettings;                          // +0x04 current settings
    void*         m_pTargetSettings;                    // +0x08 settings being blended to
    void*         m_pPendingSettings;                   // +0x0c settings queued by SetSettings
    unsigned long m_settingsSize;                       // +0x10 bytes per settings block
    bool          m_blending;                           // +0x14
    long          m_blendStep;                          // +0x18
    long          m_blendCountdown;                     // +0x1c
    long          m_blendSteps;                         // +0x20
    bool          m_dirty;                              // +0x24
};

void CVtable_012bf0c8::StopBlend()
{
    m_blendStep = -1;
    m_blending = false;
    m_blendCountdown = -1;
}