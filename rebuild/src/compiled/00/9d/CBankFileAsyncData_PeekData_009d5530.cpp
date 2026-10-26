// CBankFileAsyncData::PeekData() -- lock the pool handle on first access and return the data pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5530, oracle length 77 bytes (0x009D5530 and 0x009D5580 are byte-identical bodies).
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0, CHandleBasedMemoryPool::Lock @ 0x00A026F0.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov eax,[esi+28h] ; push 0 ; add eax,14Ch ; push eax ; lea ecx,[esp+0Ch] ; call CCriticalSectionLock ;
//                mov ecx,[esi+1Ch] ; test ecx,ecx ; je R ; mov eax,[esi+20h] ; cmp eax,-1 ; je R ; mov dl,[esi+45h] ; test dl,dl ; jne R ; push eax ; call Lock ;
//                mov byte ptr [esi+45h],1 ; R: mov esi,[esi+8] ; lea ecx,[esp+4] ; call ~CCriticalSectionLock ; mov eax,esi ; pop esi ; add esp,8 ; ret
// Under the bank's critical section: when the data lives in a handle-based pool (+0x1c with a handle
// at +0x20) and has not been pinned yet (+0x45), the pool is asked to lock the handle (the pool
// pointer stays in ecx as `this`) and the pinned flag is set; the data pointer at +0x08 is read
// into the return register before the lock is released.
struct CRITICAL_SECTION_Fable {          // Win32 RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};

struct CCriticalSectionLock_Retail {            // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;  // +0x00
    bool                    m_locked;    // +0x04 set by a try-only acquisition
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};

class CHandleBasedMemoryPool { public:
    void Free(long handle);              // 0x00A62ED0
public:
    void Lock(long);   // 0x00A026F0
};

class CBankFileAsync { public:                  // the owning bank (+0x28); its critical section sits at +0x14c
    unsigned char _pad[0x14c];
    CRITICAL_SECTION_Fable m_section;    // +0x14c
};

struct CMovableResourceMemoryPool_Calls {      // the relocating pool (+0x24); its critical section sits at +0x14c
    unsigned char _pad[0x14c];
    CRITICAL_SECTION_Fable m_section;    // +0x14c
};

class CBankFileAsyncData { public:
    virtual void v00();
    void*                       m_pAllocation;    // +0x04 relocatable block base
    void*                       m_pData;          // +0x08 relocatable data pointer handed out by GetData
    void*                       m_pReadBuffer;    // +0x0c the buffer the file thread fills
    unsigned char               _pad10[0x08];
    unsigned long               m_bytesPending;   // +0x18
    CHandleBasedMemoryPool*     m_pPool;          // +0x1c
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
    void* PeekData();     // 0x009D5530 / 0x009D5580
};

void* CBankFileAsyncData::PeekData()
{
    CCriticalSectionLock_Retail lock(&m_pBank->m_section, false);
    if (m_pPool != 0 && m_handle != -1 && !m_dataLocked) {
        m_pPool->Lock(m_handle);
        m_dataLocked = true;
    }
    return m_pData;
}