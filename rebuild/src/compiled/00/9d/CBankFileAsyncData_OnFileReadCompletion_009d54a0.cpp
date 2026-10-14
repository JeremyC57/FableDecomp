// CBankFileAsyncData::OnFileReadCompletion(bool) -- the file thread is done: mark finished, nothing pending, completion to deliver
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D54A0, oracle length 63 bytes.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov eax,[esi+28h] ; push 0 ; add eax,14Ch ; push eax ; lea ecx,[esp+0Ch] ; call CCriticalSectionLock ;
//                cmp dword ptr [esi+20h],-1 ; mov al,1 ; mov [esi+44h],al ; mov dword ptr [esi+18h],0 ; je S ; mov [esi+46h],al ;
//                S: lea ecx,[esp+4] ; call ~CCriticalSectionLock ; pop esi ; add esp,8 ; ret 4
// Under the bank's critical section (blocking lock, second argument 0): finished = true, the pending
// byte count is zeroed, and when a pool handle is held (+0x20 != -1) the completion flag at +0x46 is
// raised too (the compiler shares the `1` in al between the two byte stores).  The success
// argument is unused.
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
    void OnFileReadCompletion(bool success);     // 0x009D54A0
};

void CBankFileAsyncData::OnFileReadCompletion(bool success)
{
    CCriticalSectionLock_Retail lock(&m_pBank->m_section, false);
    m_finished = true;
    m_bytesPending = 0;
    if (m_handle != -1)
        m_completionPending = true;
}