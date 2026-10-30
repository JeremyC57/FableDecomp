// CCodaPlayback::GetPlayProgress(CPanel* panel, SPlayProgress* progress) -- under the audio module lock, report how far the CODA source has played (position / length) and its loops left; a stopped source, or a one-shot that sat at position 0 after it started for longer than the panel's end timeout, reports 1.0 and 0 loops
// Leaf: ?GetPlayProgress@CCodaPlayback@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27890, 257 bytes, thiscall, `ret 8`. Slot 2 of the CCodaPlayback vtable 0x012B4034
//   (at 0x012B403C; slots 0/1/6/7 are the landed SetSourceProperty20 0x00C275E0, SetVolume 0x00C27620,
//   SetSourcePropertyFC 0x00C27670, SetPosition 0x00C27550). The row is referenced only from that slot.
// Name: descriptive (libpk / GameCODA has no PDB or RTTI); the manifest's "sub_00c27890" carries none.
// Callees: PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*) 0x00C0FFC0 / ~PKCProtectionInstance 0x00C0FFE0
//   over the module lock 0x0143BAFC; the CODA C API (`__stdcall`, nothrow under /EHsc) CODA_IsSourcePlaying 0x00C35CA0
//   (the gate of the "CODA reports source IS playing" line in LogStatus 0x00C27760) and CODA_GetSourcePosition 0x00C35CC0;
//   GetTickCount (IAT 0x0143FDFC).
// Shape: the EH frame with no state-0 store (`mov eax,fs:[0]` first) comes from the early returns inside the lock's
//   scope (the landed Update 0x00C276B0 lesson); the finished result stores the loop count before the 1.0f; both
//   unsigned-to-float conversions get the `jge ; fadd [2^32]` fix-up.
class PKCCriticalSection {                               // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
};
class PKCProtectionInstance {                            // scoped guard: section + entry tick
public:
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();                            // 0x00C0FFE0
    PKCCriticalSection* m_pSection;                      // +0
    unsigned long m_tick;                                // +4
};
extern PKCCriticalSection g_soundLock;                   // 0x0143BAFC the audio library's module lock

struct CODASource;                                       // opaque CODA source object
extern "C" {                                             // the CODA C API (__stdcall; nothrow under /EHsc)
long __stdcall CODA_IsSourcePlaying(CODASource* source);             // 0x00C35CA0
unsigned long __stdcall CODA_GetSourcePosition(CODASource* source);  // 0x00C35CC0
__declspec(dllimport) unsigned long __stdcall GetTickCount(void);    // IAT 0x0143FDFC
}

struct CPanel {
    unsigned char _pad000[0x7c0];
    float         m_masterVolume;               // +0x7C0
    unsigned char _pad7c4[0xc4];
    unsigned long m_endTimeout;                 // +0x888 ms a finished one-shot may sit at position 0
};

struct SPlayProgress {
    float         m_fraction;                   // +0x00 position / length, 1.0 when done
    long          m_loopsLeft;                  // +0x04
};

struct CCodaPlayback {                          // vtable 0x012B4034
    virtual void SetSourceProperty20(float value);                        // slot 0 (0x00C275E0)
    virtual void SetVolume(CPanel* panel, float volume);                   // slot 1 (0x00C27620)
    virtual void GetPlayProgress(CPanel* panel, SPlayProgress* progress);  // slot 2 (0x00C27890)
    unsigned char _pad04[8];
    CODASource*   m_pSource;                    // +0x0C
    long          m_loopsLeft;                  // +0x10 -1 forever, 0 last pass
    unsigned long m_loopPoint;                  // +0x14 play position that ends one pass
    long          m_bArmed;                     // +0x18 position went back below the loop point
    unsigned char _pad1c[4];
    unsigned long m_length;                     // +0x20 source length
    bool          m_bLooping;                   // +0x24
    bool          m_b3D;                        // +0x25
    bool          m_bStarted;                   // +0x26 CODA reported a play position once
    bool          m_bEndTimerRunning;           // +0x27
    unsigned long m_endTimerStart;              // +0x28 GetTickCount when the position fell back to 0
};

void CCodaPlayback::GetPlayProgress(CPanel* panel, SPlayProgress* progress)
{
    PKCProtectionInstance lock(&g_soundLock);
    if (CODA_IsSourcePlaying(m_pSource)) {
        progress->m_loopsLeft = m_loopsLeft;
        unsigned long position = CODA_GetSourcePosition(m_pSource);
        if (position == 0 && m_bStarted && m_loopsLeft == 0) {
            unsigned long now = GetTickCount();
            if (!m_bEndTimerRunning) {
                m_bEndTimerRunning = true;
                m_endTimerStart = now;
            } else if (now - m_endTimerStart > panel->m_endTimeout) {
                progress->m_loopsLeft = 0;
                progress->m_fraction = 1.0f;
                return;
            }
            position = m_length - 1;
        } else if (position == m_length)
            --position;
        progress->m_fraction = (float)position / (float)m_length;
    } else {
        progress->m_loopsLeft = 0;
        progress->m_fraction = 1.0f;
    }
}