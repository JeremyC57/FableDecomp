// CCodaPlayback::SetVolume(CPanel* panel, float volume) -- under the audio module lock, scale a non-3D source's volume by the panel's master volume and hand it to CODA
// Leaf: ?SetVolume@CCodaPlayback@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27620, 75 bytes (`ret 8`, no EH frame). Descriptive names (no PDB or RTTI
//   covers the libpk audio library; the strings after the vtable read "CODA reports source is NOT playing"): the
//   manifest's "CEngineSubPrimitive_AllocateBuffer" is propagated. The function is slot 1 of vtable 0x012B4034.
// Callees: PKCProtectionInstance @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 over the module lock 0x0143BAFC,
//   CODA_SetSourceVolume @ 0x00C35AF0 (`__stdcall`, `ret 8`).
// Shape: no EH frame although the lock has a destructor: the only call inside the lock is an `extern "C"` CODA
//   function, which /EHsc assumes cannot throw. The scaled volume is stored back into the argument slot.
class PKCCriticalSection;
class PKCProtectionInstance { public:                            // 8-byte scoped lock (chunk 431 model)
    void*         m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);                 // 0x00C0FFC0
    ~PKCProtectionInstance();                             // 0x00C0FFE0
};

extern PKCCriticalSection g_soundLock;             // 0x0143BAFC the audio library's module lock

struct CODASource;                              // opaque CODA source object
extern "C" {                                    // the CODA C API (__stdcall; nothrow under /EHsc)
void __stdcall CODA_SetSourceVolume(CODASource* source, float volume);              // 0x00C35AF0
unsigned long __stdcall CODA_GetSourcePosition(CODASource* source);                 // 0x00C35CC0
long __stdcall CODA_StopSource(CODASource* source, bool immediate);                 // 0x00C35DD0
}

struct CPanel {
    unsigned char _pad000[0x7c0];
    float         m_masterVolume;               // +0x7C0
};

struct CCodaPlayback {                          // vtable 0x012B4034 (landed rows 0x00C275E0 / 0x00C27670 / 0x00C27760 name it CCodaPlayback_<addr>)
    virtual void SetSourceProperty20(float value);                        // slot 0 (0x00C275E0, landed)
    virtual void SetVolume(CPanel* panel, float volume);                   // slot 1 (0x00C27620)
    unsigned char _pad04[8];
    CODASource*   m_pSource;                    // +0x0C
    long          m_loopsLeft;                  // +0x10 -1 forever, 0 last pass
    unsigned long m_loopPoint;                  // +0x14 play position that ends one pass
    long          m_bArmed;                     // +0x18 position went back below the loop point
    unsigned char _pad1c[4];
    unsigned long m_length;                     // +0x20 source length (landed 0x00C27760)
    bool          m_bLooping;                   // +0x24
    bool          m_b3D;                        // +0x25
    bool          m_bStarted;                   // +0x26
};

void CCodaPlayback::SetVolume(CPanel* panel, float volume)
{
    PKCProtectionInstance lock(&g_soundLock);
    if (!m_b3D)
        volume *= panel->m_masterVolume;
    CODA_SetSourceVolume(m_pSource, volume);
}