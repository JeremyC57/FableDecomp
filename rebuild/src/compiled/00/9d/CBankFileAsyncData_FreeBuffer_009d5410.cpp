// CBankFileAsyncData::FreeBuffer() -- give the pool handle back and forget the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5410, oracle length 47 bytes.
// Callees: CHandleBasedMemoryPool::Free @ 0x00A62ED0.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+1Ch] ; test ecx,ecx ; je E ; mov eax,[esi+20h] ; cmp eax,-1 ; je E ; push eax ; call Free ;
//                mov dword ptr [esi+20h],-1 ; mov dword ptr [esi+0Ch],0 ; mov dword ptr [esi+8],0 ; E: pop esi ; ret
// No lock here: with a pool and a live handle the handle is freed (the pool pointer is the call's
// `this`, loaded straight into ecx), then the handle, the read buffer and the data pointer are reset.
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
    void FreeBuffer();     // 0x009D5410
};

void CBankFileAsyncData::FreeBuffer()
{
    if (m_pPool != 0 && m_handle != -1) {
        m_pPool->Free(m_handle);
        m_handle = -1;
        m_pReadBuffer = 0;
        m_pData = 0;
    }
}