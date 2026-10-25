// CReverbProcessor::~CReverbProcessor() -- EH-framed destructor of the libpk software reverb: not ready any more (+0x294), the pooled delay memory (+0x280) freed through the library allocator (0x00C3C370), the settings blocks released (0x00C77470), the stereo delay line detached (0x00C75000), then the members in reverse order (states 8..0) and the settings-block base (0x00C775A0)
// Leaf: ??1CReverbProcessor@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A780, 294 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC6A650/0xC6A780/0xC69930/0xC69990 (CTCKraken) and 0xC75440 ... are
//   the libpk reverb (vtable 0x12B9594)"):
//   - it stores the CReverbProcessor vtable 0x012B9594 (agent443: slot 4 Process 0x00C6A5D0) and is called by the scalar
//     deleting destructor 0x00C6A9F0 (slot 0);
//   - it tears down what the constructor 0x00C6A650 builds: CStereoDelayLine::~CStereoDelayLine 0x00C75440, the late
//     delay (vtable 0x012B957C), the comb filters (`__ehvec_dtor` with their destructor 0x00C698A0), the pre-delay
//     (0x012B9580), the diffuser (0x00C6A340), the tap bank (0x00C6A2E0), both gains (0x012B9578) and the format member
//     (0x012B89C4) before the base destructor 0x00C775A0;
//   - the leading sequence is the reverb's Shutdown 0x00C6A3D0 (same four steps), and +0x294 is the ready byte Process
//     0x00C6A5D0 tests.
//   The CTCKraken / CTCInventoryClothing names are propagated; descriptive names (libpk: no PDB/RTTI).
// Shape: the inline member destructors store their vtables before the out-of-line CDspObject destructor; the format
//   member's own (derived) store is dropped as dead before its root's inline store of 0x012B89C4.
class CPKObject {                                       // the libpk root: its allocator hooks
public:
    static void __cdecl operator delete(void* p);       // 0x00C3C370
};

class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CVtable_012bf0c8 : public CDspObject {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    CVtable_012bf0c8();                                 // 0x00C77440
    virtual ~CVtable_012bf0c8();                        // 0x00C775A0
    void ReleaseBlock();                                // 0x00C77470: free the settings blocks (+4), clear +4/+8/+0xc
    void*         m_pSettings;                          // +0x04 current settings
    void*         m_pTargetSettings;                    // +0x08 settings being blended to
    void*         m_pPendingSettings;                   // +0x0c settings queued by slot 1 (0x00C774A0)
    unsigned long m_settingsSize;                       // +0x10 bytes per settings block
    bool          m_blending;                           // +0x14
    unsigned char _pad15[0x24 - 0x15];
    bool          m_dirty;                              // +0x24 pending settings to commit
};

class CSoundFormatBase {                                // vtable 0x012B89C4 (scalar deleting destructor 0x00C53AE0)
public:
    CSoundFormatBase()
    {
        m_sampleRate = 48000;
        m_channels = 2;
        m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0;
    }
    virtual ~CSoundFormatBase() { }
    long m_channels;                                    // +0x04
    long m_sampleRate;                                  // +0x08
    long m_0c, m_10, m_14, m_18;                        // +0x0c..+0x18
};

class CVtable_012b9584 : public CSoundFormatBase {      // vtable 0x012B9584 (destructor out of line at 0x00C69870), 0x1c bytes
public:
    CVtable_012b9584() { }
    ~CVtable_012b9584() { }
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578, 0x14 bytes
public:
    CDspGain() { }                                      // out of line at 0x00C69660
    ~CDspGain() { }                                     // out of line at 0x00C69680
    float m_gain[4];                                    // +0x04
};

class CReverbTapBank {                                  // vtable 0x012B9590, 0xb0 bytes
public:
    CReverbTapBank();                                   // 0x00C6A280
    ~CReverbTapBank();                                  // 0x00C6A2E0
    unsigned char _body[0xb0];
};

class CReverbDiffuser {                                 // vtable 0x012BF080, 0x9c bytes
public:
    CReverbDiffuser();                                  // 0x00C75980
    ~CReverbDiffuser();                                 // 0x00C6A340
    unsigned char _body[0x9c];
};

class CReverbPreDelay : public CDspObject {             // vtable 0x012B9580, 0x18 bytes
public:
    CReverbPreDelay() { }
    ~CReverbPreDelay() { }                              // out of line at 0x00C697C0
    unsigned char _body[0x14];
};

class CCombFilter : public CDspObject {                 // vtable 0x012B9588, 0x14 bytes (destructor compiler-generated: 0x00C698A0)
public:
    CCombFilter();                                      // 0x00C69880
    unsigned char _pad04[0x0c];
    long          m_mode;                               // +0x10
};

class CReverbDelay : public CDspObject {                // vtable 0x012B957C, 0x14 bytes
public:
    CReverbDelay() { }
    ~CReverbDelay() { }                                 // out of line at 0x00C69740
    unsigned char _body[0x10];
};

class CDelayLine {                                      // vtable 0x012BF074, 0x10 bytes
public:
    CDelayLine();                                       // 0x00C74F60
    virtual ~CDelayLine();                              // 0x00C75400
    float*       m_data;                                // +0x04
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
};

class CStereoDelayLine {                                // vtable 0x012BF078, 0x24 bytes
public:
    CStereoDelayLine();                                 // 0x00C75410
    virtual ~CStereoDelayLine();                        // 0x00C75440
    void Detach();                                      // 0x00C75000: both lines' data and size zeroed
    CDelayLine m_lines[2];                              // +0x04
};

struct SReverbRing {                                    // the comb filters' shared delay ring
    float*        m_data;
    unsigned long m_length;
    unsigned long m_position;
};

class CReverbProcessor : public CVtable_012bf0c8 {      // vtable 0x012B9594, 0x298 bytes
public:
    CReverbProcessor();
    virtual ~CReverbProcessor();
    CVtable_012b9584 m_format;                          // +0x028 2 channels, 48000 Hz
    unsigned char    _pad44[0x54 - 0x44];
    SReverbRing      m_combLine;                        // +0x054
    unsigned char    _pad60[0x6c - 0x60];
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
    unsigned long    m_288;                             // +0x288
    bool             m_bWasActive;                      // +0x28c
    unsigned long    m_290;                             // +0x290
    bool             m_bReady;                          // +0x294
};

CReverbProcessor::~CReverbProcessor()
{
    m_bReady = false;
    if (m_pMemory) {
        CPKObject::operator delete(m_pMemory);
        m_pMemory = 0;
    }
    ReleaseBlock();
    m_stereoDelay.Detach();
}
