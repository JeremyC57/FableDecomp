// CBankFileAsync::DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data) -- under the bank lock: a finished read just loses its restart flag; an unfinished one that cannot be aborted now is queued for disposal; the caller's pointer is released, then the disposal list is processed
// Leaf: DiscardAsyncData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D67E0, length 180 bytes (+12 int3), ret 4.
// Callees: CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0 (blocking; the inner lock is the data's own bank's),
// CBankFileAsyncData::Abort @ 0x009D6350, list<CCountedPointer<CBankFileAsyncData>>::push_back @ 0x009D7B00 (catalog
// "_Buynode<CCountedPointer<CSpawnedFuncBase>>", propagated), operator delete @ 0x00BFE9BC, ProcessDisposalList @ 0x009D6730.
// Shape: the inner lock lives in an inline CancelIfFinished() that returns from inside the lock's scope, so the lock's
// destructor is emitted on both return paths and the caller's test threads into them (a bool local spills to bl); the pointer reset releases the count and zeroes Info before Data.
// Layout: retail CBankFileAsyncData (landed OnFileReadCompletion 0x009D54A0): m_pBank +0x28, m_finished +0x44,
// m_completionPending +0x46.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CRITICAL_SECTION_Fable {                    // Win32 RTL_CRITICAL_SECTION, 24 bytes (dword aligned)
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
struct CCriticalSectionLock {                      // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
class CBankFileAsync;
struct CBankFileAsyncData {
    virtual void v00();
    unsigned char _pad04[0x24];                    // +0x04
    CBankFileAsync* m_pBank;                       // +0x28
    unsigned char _pad2c[0x18];                    // +0x2c
    bool m_finished;                               // +0x44
    bool m_dataLocked;                             // +0x45
    bool m_completionPending;                      // +0x46
    bool Abort();                                  // 0x009D6350
    inline bool CancelIfFinished();                // under the bank lock (defined below)
};
struct CAsyncDataPointer {                         // CCountedPointer<CBankFileAsyncData>
    CBankFileAsyncData* Data;                      // +0
    SCountBlock*        Info;                      // +4
    __forceinline void ReleaseInfo()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
    __forceinline ~CAsyncDataPointer()
    {
        ReleaseInfo();
        Data = 0;
        Info = 0;
    }
    __forceinline void Reset()
    {
        ReleaseInfo();
        Info = 0;
        Data = 0;
    }
};
struct SDisposalNode {                             // STLport list node, 0x10 bytes
    SDisposalNode* _M_next;
    SDisposalNode* _M_prev;
    CAsyncDataPointer _M_data;
};
struct CDisposalList {                             // list<CCountedPointer<CBankFileAsyncData>> (STLport, 4 bytes)
    SDisposalNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SDisposalNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    CAsyncDataPointer& front() { return _M_node->_M_next->_M_data; }
    void pop_front()
    {
        SDisposalNode* n = _M_node->_M_next;
        SDisposalNode* next = n->_M_next;
        SDisposalNode* prev = n->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CAsyncDataPointer();
        free(n);
    }
    void push_back(const CAsyncDataPointer& value);   // 0x009D7B00
};

class CBankFileAsync {
public:
    unsigned char _pad[0x144];                     // CBankFile base + async members up to +0x143
    CDisposalList DisposalList;                    // +0x144
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
    void ProcessDisposalList();                    // 0x009D6730
    void DiscardAsyncData(CAsyncDataPointer& data);
};

inline bool CBankFileAsyncData::CancelIfFinished()
{
    CCriticalSectionLock lock(&m_pBank->CriticalSection, false);
    if (m_finished) {
        m_completionPending = false;
        return true;
    }
    return false;
}

void CBankFileAsync::DiscardAsyncData(CAsyncDataPointer& data)
{
    CCriticalSectionLock lock(&CriticalSection, false);
    CBankFileAsyncData* asyncData = data.Data;
    if (asyncData) {
        if (!asyncData->CancelIfFinished() && !data.Data->Abort())
            DisposalList.push_back(data);
        data.Reset();
    }
    ProcessDisposalList();
}
