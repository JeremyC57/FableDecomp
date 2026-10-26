// CCodaPlayback::SetPosition(const C3DVector& position) -- under the audio module lock, hand a 3D source's position to CODA (Y and Z swapped into CODA's axes)
// Leaf: ?SetPosition@CCodaPlayback@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27550, 139 bytes (`ret 4`, EH-framed). Descriptive names (no PDB or RTTI
//   covers the libpk audio library): the manifest's "CPhysicalPrimitive_InitializeFromMesh" is propagated. The
//   function is slot 7 of vtable 0x012B4034.
// Callees: CScopedLock @ 0x00C0FFC0 / ~CScopedLock @ 0x00C0FFE0 over the module lock 0x0143BAFC,
//   C3DVector::SwapYZ @ 0x00C1C070 (landed name; a C++ member, so the lock gets EH state 0),
//   CODA_SetSourcePosition @ 0x00C33FD0 (`__stdcall`, extern "C").
// Shape: the swapped vector is ASSIGNED to a declared local (return temporary at esp+0x18 copied dword by dword to
//   the local at esp+0xc); `C3DVector v = position.SwapYZ();` builds straight into the local (0x14-byte frame).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector SwapYZ() const;                   // 0x00C1C070
};

struct CScopedLock {                            // 8-byte scoped lock (chunk 431 model)
    void*         m_pSection;
    unsigned long m_tick;
    CScopedLock(void* section);                 // 0x00C0FFC0
    ~CScopedLock();                             // 0x00C0FFE0
};

extern unsigned char g_soundLock[];             // 0x0143BAFC the audio library's module lock

struct CODASource;                              // opaque CODA source object
extern "C" {                                    // the CODA C API (__stdcall; nothrow under /EHsc)
void __stdcall CODA_SetSourceVolume(CODASource* source, float volume);              // 0x00C35AF0
void __stdcall CODA_SetSourcePosition(CODASource* source, const C3DVector* position);   // 0x00C33FD0
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
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void SetSourcePropertyFC(float value);                        // slot 6 (0x00C27670, landed)
    virtual void SetPosition(const C3DVector& position);                   // slot 7 (0x00C27550)
};

void CCodaPlayback::SetPosition(const C3DVector& position)
{
    CScopedLock lock(g_soundLock);
    if (m_b3D) {
        C3DVector v;
        v = position.SwapYZ();
        CODA_SetSourcePosition(m_pSource, &v);
    }
}