// CFileInstaller::ProcessFileReadCompletion() -- under the installer's critical section (+4): when a completion record is queued at +0x74, process it (0x009C4120) and clear the slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C42F0, length 64 bytes.
// Callees: CCriticalSectionLock::CCriticalSectionLock @ 0x00A62920 (the section, 0) and its destructor @ 0x00A629A0 (an 8-byte local), the processing member
// @ 0x009C4120 (on `this`, the record; the manifest label "FileReadCompletion_ProcessData" is propagated). Shape: the record pointer is re-read from memory for the
// push after the null test: `volatile` (the completion thread writes it).
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock_Retail {              // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};

class CFileInstaller { public:
    unsigned char          _pad[4];
    CRITICAL_SECTION_Fable m_section;              // +0x04
    unsigned char          _pad1c[0x58];
    void* volatile         m_pReadCompletion;      // +0x74
    void ProcessReadCompletionData(void* record);  // 0x009C4120
    void ProcessFileReadCompletion();
};

void CFileInstaller::ProcessFileReadCompletion()
{
    CCriticalSectionLock_Retail lock(&m_section, false);
    if (m_pReadCompletion) {
        ProcessReadCompletionData(m_pReadCompletion);
        m_pReadCompletion = 0;
    }
}