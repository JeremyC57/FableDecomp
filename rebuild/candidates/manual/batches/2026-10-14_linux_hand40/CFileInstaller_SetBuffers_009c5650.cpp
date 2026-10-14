// CFileInstaller::SetBuffers(void* buffers, long count) -- under the installer's critical section (+4), hand the buffer block to InitBuffers (0x009C5310)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C5650, length 64 bytes.
// Callees: CCriticalSectionLock::CCriticalSectionLock @ 0x00A62920 (the section, 0) and its destructor @ 0x00A629A0 (an 8-byte local), InitBuffers @ 0x009C5310
// (on `this`, both arguments forwarded). Shape: the two arguments re-read from the stack after the lock constructor, `ret 8`.
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
    void InitBuffers(void* buffers, long count);   // 0x009C5310
    void SetBuffers(void* buffers, long count);
};

void CFileInstaller::SetBuffers(void* buffers, long count)
{
    CCriticalSectionLock_Retail lock(&m_section, false);
    InitBuffers(buffers, count);
}
