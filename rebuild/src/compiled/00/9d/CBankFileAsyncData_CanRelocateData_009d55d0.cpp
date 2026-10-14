// CBankFileAsyncData::CanRelocateData(long) const -- relocatable and not already relocating, under the movable pool's lock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D55D0, oracle length 74 bytes.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov eax,[esi+24h] ; push 0 ; add eax,14Ch ; push eax ; lea ecx,[esp+0Ch] ; call CCriticalSectionLock ;
//                mov al,[esi+40h] ; test al,al ; je F ; mov al,[esi+41h] ; test al,al ; jne F ; lea ecx,[esp+4] ; call ~CCriticalSectionLock ; mov al,1 ; pop esi ; add esp,8 ; ret 4 ;
//                F: lea ecx,[esp+4] ; call ~CCriticalSectionLock ; xor al,al ; pop esi ; add esp,8 ; ret 4
// Virtual (RTTI name ?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z): the lock is taken on the
// movable resource pool at +0x24 (its critical section also at +0x14c), the two bytes at +0x40/+0x41
// decide, and the long argument is unused.
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
    bool CanRelocateData(long size) const;     // 0x009D55D0
};

bool CBankFileAsyncData::CanRelocateData(long size) const
{
    CCriticalSectionLock_Retail lock(&m_pMovablePool->m_section, false);
    if (m_relocatable && !m_relocating)
        return true;
    return false;
}