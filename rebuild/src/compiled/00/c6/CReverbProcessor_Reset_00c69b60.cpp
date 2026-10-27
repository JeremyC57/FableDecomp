// CReverbProcessor::Reset() -- vtable slot 3 of the libpk software reverb: once its delay memory exists (+0x280), reset both gain stages to unity, zero the comb ring, the second ring, the 128-float scratch block, the late delay, the pre-delay and the four comb lines, then reset the tap bank and the diffuser and clear the stereo delay line
// Leaf: ?Reset@CReverbProcessor@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C69B60, 245 bytes, thiscall, plain `ret` (the stereo-delay clear is a tail jump). The zeroing
//   is the memset intrinsic (`shl ecx,2 ; rep stosd ; rep stosb` per counted float buffer, `rep stosd` 0x80 for the scratch block).
// Identity: slot 3 of the reverb vtable 0x012B9594 (xref 0x012B95A0) -- the slot the landed CReverbProcessor::Process 0x00C6A5D0
//   lists as "0x00C77590 / 0x00C69B60" (the settings-block base's empty slot 3 and this override). Its callees are the reverb parts'
//   resets: CDspGain::Reset 0x00C744E0 on +0x6C / +0x80, CReverbTapBank::Reset 0x00C75CA0 on +0x94, CReverbDiffuser::Reset
//   0x00C75670 on +0x144, CStereoDelayLine::Clear 0x00C75010 on +0x25C (catalog names). The method name is descriptive (libpk: no
//   PDB/RTTI); the catalog's "CTCNpcInteractable_Reset" is propagated.
// Layout: the landed CReverbProcessor model (ctor 0x00C6A650) with the parts' buffers named: ring +0x54 / +0x58, second ring +0x60 /
//   +0x64, pre-delay data +0x1E4 / size +0x1E8, combs data +4 / length +8 each (stride 0x14 from +0x1F8), late delay +0x24C / +0x250,
//   scratch block +0x288.
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)

class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CVtable_012bf0c8 : public CDspObject {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    virtual ~CVtable_012bf0c8();                        // 0x00C775A0
    virtual void SetSettings(const void* settings, bool immediate);   // slot 1: 0x00C774A0
    virtual bool CommitAndProcess_00c774f0(const float* dry, bool active, const float* const* inputs, unsigned long count);   // slot 2
    virtual void Reset();                               // slot 3: 0x00C77590 (empty)
    virtual bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count) = 0;   // slot 4
    void*         m_pSettings;                          // +0x04
    void*         m_pTargetSettings;                    // +0x08
    void*         m_pPendingSettings;                   // +0x0c
    unsigned long m_settingsSize;                       // +0x10
    bool          m_blending;                           // +0x14
    unsigned char _pad15[0x24 - 0x15];
    bool          m_dirty;                              // +0x24
};

class CSoundFormatBase {                                // vtable 0x012B89C4
public:
    virtual ~CSoundFormatBase() { }
    long m_channels;                                    // +0x04
    long m_sampleRate;                                  // +0x08
    long m_0c, m_10, m_14, m_18;                        // +0x0c..+0x18
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578, 0x14 bytes
public:
    float m_gain[4];                                    // +0x04
    void Reset();                                       // 0x00C744E0
};

class CReverbTapBank {                                  // vtable 0x012B9590, 0xb0 bytes
public:
    void Reset();                                       // 0x00C75CA0
    unsigned char _body[0xb0];
};

class CReverbDiffuser {                                 // vtable 0x012BF080, 0x9c bytes
public:
    void Reset();                                       // 0x00C75670
    unsigned char _body[0x9c];
};

class CReverbPreDelay : public CDspObject {             // vtable 0x012B9580, 0x18 bytes
public:
    float*        m_data;                               // +0x04
    unsigned long m_size;                               // +0x08
    unsigned char _pad0c[0x0c];
    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }
};

class CCombFilter : public CDspObject {                 // vtable 0x012B9588, 0x14 bytes
public:
    float*        m_data;                               // +0x04
    unsigned long m_length;                             // +0x08
    unsigned long m_position;                           // +0x0c
    long          m_mode;                               // +0x10
    void Clear() { memset(m_data, 0, m_length * sizeof(float)); }
};

class CReverbDelay : public CDspObject {                // vtable 0x012B957C, 0x14 bytes
public:
    float*        m_data;                               // +0x04
    unsigned long m_size;                               // +0x08
    unsigned char _pad0c[0x08];
    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }
};

class CDelayLine {                                      // vtable 0x012BF074, 0x10 bytes
public:
    virtual ~CDelayLine();                              // 0x00C75400
    float*       m_data;                                // +0x04
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
};

class CStereoDelayLine {                                // vtable 0x012BF078, 0x24 bytes
public:
    virtual ~CStereoDelayLine();                        // 0x00C75440
    void Clear();                                       // 0x00C75010
    CDelayLine m_lines[2];                              // +0x04
};

struct SReverbRing {                                    // a delay ring: data, length, position
    float*        m_data;
    unsigned long m_length;
    unsigned long m_position;
    void Clear() { memset(m_data, 0, m_length * sizeof(float)); }
};

class CReverbProcessor : public CVtable_012bf0c8 {      // vtable 0x012B9594, 0x298 bytes
public:
    virtual ~CReverbProcessor();
    void Reset();                                       // slot 3
    CSoundFormatBase m_format;                          // +0x028 (CVtable_012b9584)
    unsigned char    _pad44[0x54 - 0x44];
    SReverbRing      m_combLine;                        // +0x054
    SReverbRing      m_line60;                          // +0x060
    CDspGain         m_inputGain;                       // +0x06c
    CDspGain         m_outputGain;                      // +0x080
    CReverbTapBank   m_taps;                            // +0x094
    CReverbDiffuser  m_diffuser;                        // +0x144
    CReverbPreDelay  m_preDelay;                        // +0x1e0
    CCombFilter      m_combs[4];                        // +0x1f8
    CReverbDelay     m_delay;                           // +0x248
    CStereoDelayLine m_stereoDelay;                     // +0x25c
    void*            m_pMemory;                         // +0x280 pooled delay memory
    unsigned long    m_memoryBytes;                     // +0x284
    float*           m_scratch;                         // +0x288 128-float block
    bool             m_bWasActive;                      // +0x28c
    unsigned long    m_290;                             // +0x290
    bool             m_bReady;                          // +0x294
};

void CReverbProcessor::Reset()
{
    if (!m_pMemory)
        return;
    m_inputGain.Reset();
    m_outputGain.Reset();
    m_combLine.Clear();
    m_line60.Clear();
    memset(m_scratch, 0, 128 * sizeof(float));
    m_delay.Clear();
    m_preDelay.Clear();
    for (int i = 0; i < 4; ++i)
        m_combs[i].Clear();
    m_taps.Reset();
    m_diffuser.Reset();
    m_stereoDelay.Clear();
}