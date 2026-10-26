// CBankFileAsyncData::RelocateData(long, void*, void*, long) -- rebase the two block pointers after the movable pool moved the block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5670, oracle length 78 bytes.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov eax,[esi+24h] ; push 0 ; add eax,14Ch ; push eax ; lea ecx,[esp+0Ch] ; call CCriticalSectionLock ;
//                mov eax,[esi+8] ; test eax,eax ; mov ecx,[esp+18h] ; mov edx,[esp+14h] ; je A ; sub eax,edx ; add eax,ecx ; mov [esi+8],eax ;
//                A: mov eax,[esi+4] ; test eax,eax ; je B ; sub eax,edx ; add eax,ecx ; mov [esi+4],eax ; B: lea ecx,[esp+4] ; call ~CCriticalSectionLock ; pop esi ; add esp,8 ; ret 10h
// Virtual (RTTI name ?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z): under the movable pool's lock
// each non-null pointer (+0x08 first, then +0x04) is moved by the same delta: `(p - oldBase) +
// newBase`.  The first and last arguments are unused.
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
    void RelocateData(long handle, void* oldBase, void* newBase, long size);     // 0x009D5670
};

void CBankFileAsyncData::RelocateData(long handle, void* oldBase, void* newBase, long size)
{
    CCriticalSectionLock_Retail lock(&m_pMovablePool->m_section, false);
    if (m_pData != 0)
        m_pData = (char*)newBase + ((char*)m_pData - (char*)oldBase);
    if (m_pAllocation != 0)
        m_pAllocation = (char*)newBase + ((char*)m_pAllocation - (char*)oldBase);
}