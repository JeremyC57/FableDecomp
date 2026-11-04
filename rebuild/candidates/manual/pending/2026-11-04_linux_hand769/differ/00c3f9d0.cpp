// CUnknownSystem1D8::InitSubSystemB(void* owner, SSystemInitParams* params) -- bring up the CODA mixer back end: detect the CPU once, fail when audio is unavailable, create the 0x151C-byte SSE or x87 mixer (CodaMalloc), create the output device for params->m_type (1 / 4 / 3 by class, 2 DirectSound, 5 waveOut, 0 enumerate and fall back to 3) and initialise it, then clear the mix flags when only the mix-out path exists, publish the device block size and the stream window (start, length, end, rounded buffer size) to the global configuration, create the buffers and start the mixer (deleting both devices on failure)
// Grade: DIFFER(836v848) -- three residues: (1) the four flag clears on the two configuration words fold into one mask (`mov eax,0FFFFDF7Fh ; and`) where retail ands each bit separately (tried masks, bitfields, an array of 0x18-byte records, an inline Clear); (2) the new device pointer is copied to ecx before the null test (retail keeps it in eax and loads the Init arguments through ecx); (3) the `(m_1d4 != 0) & (m_pDevice2 == 0)` setcc pair uses other registers and no `xor ecx,ecx`; 10 graded attempts
// Leaf: ?InitSubSystemB@CUnknownSystem1D8@@QAE_NPAXPAUSSystemInitParams@@@Z
// Flags: /EHsc /GS
extern "C" void* __cdecl CodaMalloc(unsigned long size);   // 0x00C3C360
extern "C" void __cdecl CodaFree(void* p);                  // 0x00C3C370

void DetectCpu();                                           // 0x00C52E40
extern bool g_cpuDetected;                                  // 0x0143BC78
extern bool g_audioAvailable_0143bc7a;                      // 0x0143BC7A
extern bool g_cpuHasSSE2;                                   // 0x0143BC7D
struct CStaticRecord_0143bc18 { long m_0; };
struct SAudioFlags_0139c9c0 {                               // flag word of the two audio configuration records
    unsigned long m_bits0 : 7;
    unsigned long m_bit7 : 1;
    unsigned long m_bits8 : 5;
    unsigned long m_bit13 : 1;
    unsigned long m_bits14 : 6;
    unsigned long m_mode20 : 2;
    unsigned long m_bits22 : 10;
};
extern SAudioFlags_0139c9c0 g_flags_0139c9c0;               // 0x0139C9C0
extern SAudioFlags_0139c9c0 g_flags_0139c9d8;               // 0x0139C9D8
extern CStaticRecord_0143bc18 g_record_0143bc18;            // 0x0143BC18
extern unsigned long g_defaultWord_0139c9b8;                // 0x0139C9B8
extern unsigned long g_word_0139c9bc;                       // 0x0139C9BC
extern unsigned long g_defaultWord_0139c9d0;                // 0x0139C9D0
extern unsigned long g_word_0139c9d4;                       // 0x0139C9D4
extern unsigned long g_word_0139c9ec;                       // 0x0139C9EC
extern unsigned long g_defaultWord_0139c9f4;                // 0x0139C9F4
extern long g_SubSystemB14;                                 // 0x0139CA14
extern long g_SubSystemB18;                                 // 0x0139CA18

struct CCodaObject {
    static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }
    static void __cdecl operator delete(void* p) { CodaFree(p); }
};
struct CBase_00c3efa0 : CCodaObject {                       // the 0x151C-byte mixer
    CBase_00c3efa0();                                       // 0x00C3EFA0
    virtual ~CBase_00c3efa0();
    unsigned char _body[0x151c - 4];
};
struct CMixerSSE_00c3f9d0 : CBase_00c3efa0 { virtual ~CMixerSSE_00c3f9d0(); };    // vtable 0x012B5DE0
struct CMixerX87_00c3f9d0 : CBase_00c3efa0 { virtual ~CMixerX87_00c3f9d0(); };    // vtable 0x012B5D40

struct COutputDevice : CCodaObject {
    virtual ~COutputDevice();                               // slot 0
    virtual bool Init(long a, long b, bool c);              // slot 1
    virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned long GetBlockSize();                   // slot 9
};
struct COutputDevice_00c5eec0 : COutputDevice { COutputDevice_00c5eec0(); unsigned char _body[0x610 - 4]; };   // 0x00C5EEC0
struct COutputDevice_00c5ed90 : COutputDevice { COutputDevice_00c5ed90(); unsigned char _body[0x5c - 4]; };    // 0x00C5ED90
struct COutputDevice_00c5d750 : COutputDevice { COutputDevice_00c5d750(); unsigned char _body[0x38 - 4]; };    // 0x00C5D750

struct CDeviceEnumerator_00c5db30 {
    char m_names[0x100];
    long m_count;
    CDeviceEnumerator_00c5db30();                           // 0x00C5DB30
    long CountDevices(long kind);                           // 0x00C5DD70
};

struct SSystemInitParams {
    unsigned long m_0;
    unsigned long m_4;
    unsigned long m_8;
    long          m_c;
    long          m_10;
    long          m_14;
    long          m_type;                                   // +0x18
};

struct CUnknownSystem1D8 {
    unsigned char   _pad000[0x58];
    CBase_00c3efa0* m_pMixer;                               // +0x058
    unsigned char   _pad05c[0x180 - 0x5c];
    COutputDevice*  m_pDevice;                              // +0x180
    COutputDevice*  m_pDevice2;                             // +0x184
    unsigned char   _pad188[0x1c4 - 0x188];
    bool            m_1c4;                                  // +0x1c4
    long            m_1c8;                                  // +0x1c8
    long            m_1cc;                                  // +0x1cc
    unsigned char   _pad1d0[4];
    long            m_1d4;                                  // +0x1d4
    bool InitDirectSound(SSystemInitParams* params);        // 0x00C3EC80
    bool InitWaveOut(SSystemInitParams* params);            // 0x00C3EE10
    bool CreateBuffers();                                   // 0x00C3F800
    bool StartMixer();                                      // 0x00C3F710
    bool InitSubSystemB(void* owner, SSystemInitParams* params);
};

bool CUnknownSystem1D8::InitSubSystemB(void* owner, SSystemInitParams* params)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (!g_audioAvailable_0143bc7a)
        return false;
    if (g_cpuHasSSE2)
        m_pMixer = new CMixerSSE_00c3f9d0;
    else
        m_pMixer = new CMixerX87_00c3f9d0;
    if (!m_pMixer)
        return false;
    COutputDevice* device;
    switch (params->m_type) {
    case 1:
        device = new COutputDevice_00c5eec0;
        break;
    case 4:
        device = new COutputDevice_00c5ed90;
        break;
    case 2:
        if (InitDirectSound(params))
            goto ready;
        return false;
    case 5:
        if (InitWaveOut(params))
            goto ready;
        return false;
    case 0: {
        CDeviceEnumerator_00c5db30 devices;
        if (devices.CountDevices(params->m_10) > 0 && InitDirectSound(params))
            goto ready;
    }
    case 3:
        device = new COutputDevice_00c5d750;
        break;
    default:
        return false;
    }
    m_pDevice = device;
    if (!device)
        return false;
    if (!device->Init(params->m_10, params->m_14, false)) {
        delete m_pDevice;
        m_pDevice = 0;
        return false;
    }
ready:
    bool hasMixOut = m_1d4 != 0;
    bool noSecond = m_pDevice2 == 0;
    if (noSecond & hasMixOut) {
        g_flags_0139c9c0.m_bit7 = 0;
        g_flags_0139c9d8.m_bit7 = 0;
        g_flags_0139c9c0.m_bit13 = 0;
        g_flags_0139c9d8.m_bit13 = 0;
    }
    unsigned long blockSize = m_pDevice->GetBlockSize();
    g_word_0139c9ec = blockSize;
    g_defaultWord_0139c9f4 = blockSize;
    if (g_record_0143bc18.m_0 == 0)
        g_flags_0139c9d8.m_mode20 = 0;
    unsigned long start = params->m_0;
    g_defaultWord_0139c9b8 = start;
    g_word_0139c9bc = params->m_0;
    unsigned long length = params->m_4;
    g_defaultWord_0139c9d0 = length;
    g_word_0139c9d4 = params->m_4;
    long end = (start != (unsigned long)-1 && length != (unsigned long)-1) ? (long)(start + length) : -1;
    g_SubSystemB14 = end;
    g_SubSystemB18 = end;
    unsigned long size = params->m_8;
    if (size > blockSize) {
        if (size & 0x7f)
            size = (size / 128 + 1) * 128;
        g_defaultWord_0139c9f4 = size;
    }
    if (!CreateBuffers())
        return false;
    m_1c4 = false;
    m_1c8 = 0;
    m_1cc = 0;
    if (!StartMixer()) {
        delete m_pDevice;
        m_pDevice = 0;
        delete m_pDevice2;
        m_pDevice2 = 0;
        return false;
    }
    return true;
}
