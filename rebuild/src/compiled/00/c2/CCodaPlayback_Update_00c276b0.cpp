// CCodaPlayback::Update() -- under the audio module lock: note that the source has started once CODA reports a play position, and for a looping source count a pass each time the position runs past the loop point again (stopping the source after the last pass)
// Leaf: ?Update@CCodaPlayback@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C276B0, 164 bytes (`ret`). Descriptive names (no PDB or RTTI covers the libpk
//   audio library): the manifest's "DebugState_UpdateDisplay" is propagated. Only caller: 0x00C11FA5.
// Callees: CScopedLock @ 0x00C0FFC0 / ~CScopedLock @ 0x00C0FFE0 over the module lock 0x0143BAFC,
//   CODA_GetSourcePosition @ 0x00C35CC0, CODA_StopSource @ 0x00C35DD0 (both `__stdcall`).
// Shape: m_loopsLeft -1 loops forever, 0 is the last pass (stop), above 0 counts down; the `m_bArmed = 0` after the
//   three-way test is tail-duplicated into the count-down arm, and the constant 1 lives in ecx for both the byte
//   `m_bStarted = true` and the dword `m_bArmed = 1`. The EH frame with NO state-0 store (`mov eax,fs:[0]` first)
//   comes from the early `return` inside the lock's scope: both CODA calls are extern "C" (nothrow under /EHsc), and
//   without the return VC7.1 drops the frame and duplicates the lock-destructor exit three times. The count test is
//   `!= 0` first (`je` to the stop arm, then `jle` past the count-down); `== 0` first puts the stop arm on the
//   fall-through and pushes the known-zero eax instead of `push 0`.
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
    void Update();
};

void CCodaPlayback::Update()
{
    CScopedLock lock(g_soundLock);
    unsigned long position = CODA_GetSourcePosition(m_pSource);
    if (position)
        m_bStarted = true;
    if (!m_bLooping)
        return;
    if (m_bArmed) {
        if (position > m_loopPoint) {
            if (m_loopsLeft != 0) {
                if (m_loopsLeft > 0)
                    --m_loopsLeft;
            } else
                CODA_StopSource(m_pSource, false);
            m_bArmed = 0;
        }
    } else if (position < m_loopPoint)
        m_bArmed = 1;
}