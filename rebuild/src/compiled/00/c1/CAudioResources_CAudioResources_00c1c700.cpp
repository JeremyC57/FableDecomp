// CAudioResources::CAudioResources(void* owner, void* stream) -- the audio-resource holder's constructor: clears the three resource slots, flags device types 1/2/4 as hardware, takes the stream resource, runs the library's start-up calls and creates the voice resource
// Leaf: ??0CAudioResources
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C700, 157 bytes (`ret 8`). Descriptive name (no PDB covers this library): the
//   manifest's "CParticleMorphs_InitializeEntry" is propagated. Vtable 0x012B3290 (one slot, no RTTI); its destructor is
//   the landed 0x00C1C230 (CTripleResource_00c1c230: releases +0x10, +0xC and +8 through 0x00C33990 / 0x00C33820 /
//   0x00C334F0). Caller 0x00C11E7A.
// Callees (the library's `__stdcall` API in the 0x00C33xxx block, names descriptive): QueryDevice @ 0x00C33590
//   (&type, &detail), SetStreamFlags @ 0x00C33600 (0), GetDefaultSettings @ 0x00C33400 (twice into the same block),
//   SetMode @ 0x00C335C0 (1, 0), SetCallback @ 0x00C33670 (0), CreateVoice @ 0x00C33810 (), then the no-op member
//   @ 0x00C1C220.
// Shape: the init-list stores (vtable, owner, three zeroed slots, the two flag bytes) are scheduled among the first
//   call's pushes; ebx holds the 0 for every later store and push.
struct CDefaultSettings {
    unsigned long Words[0x28];              // 0xA0 bytes
};

void __stdcall QueryDevice(long* type, long* detail);        // 0x00C33590
void __stdcall SetStreamFlags(long flags);                   // 0x00C33600
void __stdcall GetDefaultSettings(CDefaultSettings* out);    // 0x00C33400
void __stdcall SetMode(long mode, long arg);                 // 0x00C335C0
void __stdcall SetCallback(void* callback);                  // 0x00C33670
void* __stdcall CreateVoice();                               // 0x00C33810

struct CAudioResources {                    // vtable 0x012B3290
    virtual void v00();
    void* m_pOwner;                         // +0x04
    void* m_pStream;                        // +0x08
    void* m_pVoice;                         // +0x0C
    void* m_pBuffer;                        // +0x10
    long m_14;                              // +0x14
    bool m_enabled;                         // +0x18
    bool m_hardware;                        // +0x19
    void Init();                            // 0x00C1C220
    CAudioResources(void* owner, void* stream);
};

CAudioResources::CAudioResources(void* owner, void* stream)
    : m_pOwner(owner), m_pStream(0), m_pVoice(0), m_pBuffer(0), m_enabled(true), m_hardware(false)
{
    long type, detail;
    QueryDevice(&type, &detail);
    if (type == 1 || type == 2 || type == 4)
        m_hardware = true;
    m_pStream = stream;
    SetStreamFlags(0);
    CDefaultSettings settings;
    GetDefaultSettings(&settings);
    GetDefaultSettings(&settings);
    SetMode(1, 0);
    SetCallback(0);
    m_pVoice = CreateVoice();
    Init();
}