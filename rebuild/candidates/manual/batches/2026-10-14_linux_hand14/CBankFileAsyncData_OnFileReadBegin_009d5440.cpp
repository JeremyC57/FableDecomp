// CBankFileAsyncData::OnFileReadBegin(void**, EThreadedFilePriority) -- hand the read buffer to the file thread if the bank lock can be taken
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5440, oracle length 90 bytes.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0, CBankFileAsyncData::SetupBuffer @ 0x009D52D0.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov eax,[esi+28h] ; push 1 ; add eax,14Ch ; push eax ; lea ecx,[esp+0Ch] ; call CCriticalSectionLock ;
//                mov al,[esp+8] ; test al,al ; je F ; push 0 ; mov ecx,esi ; call SetupBuffer ; test al,al ; je F ; mov ecx,[esi+0Ch] ; mov edx,[esp+10h] ;
//                mov [edx],ecx ; lea ecx,[esp+4] ; call ~CCriticalSectionLock ; mov al,1 ; pop esi ; add esp,8 ; ret 8 ;
//                F: lea ecx,[esp+4] ; call ~CCriticalSectionLock ; xor al,al ; pop esi ; add esp,8 ; ret 8
// A try-only lock on the owning bank's critical section (+0x28 -> +0x14c, second argument 1); the
// lock object's own `m_locked` byte (frame +0x08) decides.  With the lock held and the buffer set
// up (SetupBuffer(false)), the read buffer at +0x0c is written to the out-pointer.  The priority
// argument is unused; each return path destroys the lock in place (`bool ? true : false` tails).
struct CRITICAL_SECTION_Fable {          // Win32 RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};

struct CCriticalSectionLock_Retail {            // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;  // +0x00
    bool                    m_locked;    // +0x04 set by a try-only acquisition
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};

struct CHandleBasedMemoryPool_Calls {
    void Lock(long handle);              // 0x00A026F0
    void Free(long handle);              // 0x00A62ED0
};

struct CBankFileAsync {                  // the owning bank (+0x28); its critical section sits at +0x14c
    unsigned char _pad[0x14c];
    CRITICAL_SECTION_Fable m_section;    // +0x14c
};

struct CMovableResourceMemoryPool_Calls {      // the relocating pool (+0x24); its critical section sits at +0x14c
    unsigned char _pad[0x14c];
    CRITICAL_SECTION_Fable m_section;    // +0x14c
};

struct CBankFileAsyncData {
    virtual void v00();
    void*                       m_pAllocation;    // +0x04 relocatable block base
    void*                       m_pData;          // +0x08 relocatable data pointer handed out by GetData
    void*                       m_pReadBuffer;    // +0x0c the buffer the file thread fills
    unsigned char               _pad10[0x08];
    unsigned long               m_bytesPending;   // +0x18
    CHandleBasedMemoryPool_Calls*     m_pPool;          // +0x1c
    long                        m_handle;         // +0x20 -1 when nothing is allocated
    CMovableResourceMemoryPool_Calls* m_pMovablePool;   // +0x24
    CBankFileAsync*             m_pBank;          // +0x28
    unsigned char               _pad2c[0x14];
    bool                        m_relocatable;    // +0x40
    bool                        m_relocating;     // +0x41
    unsigned char               _pad42[2];
    bool                        m_finished;       // +0x44
    bool                        m_dataLocked;     // +0x45
    bool                        m_completionPending; // +0x46
    bool SetupBuffer(bool allocate);                                    // 0x009D52D0
    bool OnFileReadBegin(void** ppBuffer, unsigned long priority);     // 0x009D5440
};

bool CBankFileAsyncData::OnFileReadBegin(void** ppBuffer, unsigned long priority)
{
    CCriticalSectionLock_Retail lock(&m_pBank->m_section, true);
    if (lock.m_locked) {
        if (SetupBuffer(false)) {
            *ppBuffer = m_pReadBuffer;
            return true;
        }
    }
    return false;
}
