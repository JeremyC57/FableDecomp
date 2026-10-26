// CBankFileAsyncData::WaitUntilFinished() -- block until this asynchronous read has completed: unless async reads are off or the read already finished, set up the buffer, kick the bank's pending requests, drop the bank lock and wait on the threaded file for this read
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5B60, 130 bytes (+14 int3), __fastcall, no arguments, returns bool.
// Callees: CCriticalSectionLock::CCriticalSectionLock(CRITICAL_SECTION*, bool) 0x00A62920, ~CCriticalSectionLock
// 0x00A629A0, CCriticalSectionLock::Unlock 0x00A62960, CBankFileAsyncData::SetupBuffer(bool) 0x009D52D0, the
// bank's request kick 0x009D5A90 (label propagated), the threaded file's wait for a read UID 0x0098F370 (label
// propagated).
// Layout: as the landed CBankFileAsyncData::GetData 0x009D5580 (the bank at +0x28 with its critical section
// at +0x14C); Ego_r.pdb names ThreadedFileUID +0x18, Finished +0x44; the bank's ThreadedFile object at
// +0x110 and DisableAsync at +0x149 (the retail CBankFileAsync offsets).
// Shape: the stack lock (8 bytes, `sub esp,8`) is taken first; the early-success tests share the final
// `~lock ; return true` tail, the SetupBuffer failure has its own `~lock ; return false` exit; the lock is
// released explicitly before the blocking wait and destroyed again (a no-op) on the way out.
struct CRITICAL_SECTION_Fable {          // Win32 RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};

struct CCriticalSectionLock_Retail {     // 8 bytes on the stack (engine/CCriticalSectionLock.h layout)
    CRITICAL_SECTION_Fable* CriticalSection;   // +0x00
    bool                    Locked;            // +0x04
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
    void Unlock();                                                                // 0x00A62960
};

class CThreadedFile {
public:
    void WaitForRead(unsigned long readUID);     // 0x0098F370
};

class CBankFileAsync { public:
    unsigned char _pad[0x110];
    CThreadedFile* ThreadedFile;                 // +0x110 (CCountedPointer object)
    unsigned char _pad2[0x35];
    bool DisableAsync;                           // +0x149
    unsigned char _pad3[2];
    CRITICAL_SECTION_Fable m_section;            // +0x14c
    void KickPendingReads();                     // 0x009D5A90
};

class CBankFileAsyncData { public:
    virtual void v00();
    void*                       m_pAllocation;    // +0x04
    void*                       m_pData;          // +0x08
    void*                       m_pReadBuffer;    // +0x0c
    unsigned char               _pad10[0x08];
    unsigned long               ThreadedFileUID;  // +0x18
    void*                       m_pPool;          // +0x1c
    long                        m_handle;         // +0x20
    void*                       m_pMovablePool;   // +0x24
    CBankFileAsync*             m_pBank;          // +0x28
    unsigned char               _pad2c[0x18];
    bool                        Finished;         // +0x44
    bool                        m_dataLocked;     // +0x45
    bool                        m_completionPending; // +0x46
    bool SetupBuffer(bool block);                 // 0x009D52D0
    bool WaitUntilFinished();
};

bool CBankFileAsyncData::WaitUntilFinished()
{
    CCriticalSectionLock_Retail lock(&m_pBank->m_section, false);
    if (!m_pBank->DisableAsync && !Finished) {
        if (!SetupBuffer(true))
            return false;
        m_completionPending = false;
        m_pBank->KickPendingReads();
        lock.Unlock();
        m_pBank->ThreadedFile->WaitForRead(ThreadedFileUID);
    }
    return true;
}