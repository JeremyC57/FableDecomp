// CFileInstaller::Unpause() -- under the installer's critical section (+4): when the pause count at +0x7c is positive, drop it, and when the installer is waiting (+0x7b) and the count reached zero, signal the two events at +0x90 and +0x84
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3250, length 89 bytes.
// Callees: CCriticalSectionLock::CCriticalSectionLock @ 0x00A62920 (the section, 0) and its destructor @ 0x00A629A0 (an 8-byte local), KERNEL32 SetEvent (IAT 0x0143FDE0,
// twice through edi: `__declspec(dllimport)`). Shape: the unsigned count tests (`jbe` / `ja`), the flag byte through al, edi saved only around the two signals.
// The count at +0x7c is re-read from memory after every access and the flag byte is read after the count store: both are `volatile` (another thread pauses).
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);   // IAT 0x0143FDE0

struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };

struct CCriticalSectionLock_Retail {              // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};

struct CFileInstaller {
    unsigned char          _pad[4];
    CRITICAL_SECTION_Fable m_section;              // +0x04
    unsigned char          _pad1c[0x5f];
    volatile bool          m_waiting;              // +0x7b (read after the count store: volatile too)
    volatile unsigned long m_pauseCount;           // +0x7c (re-read after every access: volatile, another thread pauses)
    unsigned char          _pad80[4];
    void*                  m_hResumeEvent;         // +0x84
    unsigned char          _pad88[8];
    void*                  m_hWakeEvent;           // +0x90
    void Unpause();
};

void CFileInstaller::Unpause()
{
    CCriticalSectionLock_Retail lock(&m_section, false);
    if (m_pauseCount > 0) {
        --m_pauseCount;
        if (m_waiting) {
            if (!(m_pauseCount > 0)) {
                SetEvent(m_hWakeEvent);
                SetEvent(m_hResumeEvent);
            }
        }
    }
}
