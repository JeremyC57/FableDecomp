// CBankFileAsync::ProcessDisposalList() -- under the bank lock, abort the async reads queued for disposal from the front, dropping each one whose abort succeeded; stop at the first that cannot be aborted yet
// Leaf: ProcessDisposalList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6730, length 164 bytes (+12 int3).
// Callees: CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0 (blocking), CBankFileAsyncData::Abort @ 0x009D6350,
// operator delete @ 0x00BFE9BC (count block), free @ 0x00BFEA14 (list node).
// Shape: STLport list inlines -- size() walks the ring (re-evaluated every pass), front() reads the first node's pointer,
// pop_front() unlinks, runs the counted-pointer dtor (count release, Data/Info zeroed) and frees the node.
// Layout: retail CBankFileAsync (see the ctor 0x009D5F80): DisposalList +0x144, CriticalSection +0x14c.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct _RTL_CRITICAL_SECTION {                    // Win32 RTL_CRITICAL_SECTION, 24 bytes (dword aligned)
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
struct CCriticalSectionLock {                      // 8 bytes on the stack
    _RTL_CRITICAL_SECTION* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920 ??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z
    ~CCriticalSectionLock();                       // 0x00A629A0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
class CBankFileAsync;
class CBankFileAsyncData { public:
    virtual void v00();
    unsigned char _pad04[0x24];                    // +0x04
    CBankFileAsync* m_pBank;                       // +0x28
    unsigned char _pad2c[0x18];                    // +0x2c
    bool m_finished;                               // +0x44
    bool m_dataLocked;                             // +0x45
    bool m_completionPending;                      // +0x46
    bool Abort();                                  // 0x009D6350
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
    _RTL_CRITICAL_SECTION CriticalSection;        // +0x14c
    void ProcessDisposalList();                    // 0x009D6730
    void DiscardAsyncData(CAsyncDataPointer& data);
};

void CBankFileAsync::ProcessDisposalList()
{
    CCriticalSectionLock lock(CriticalSection, false);
    while (DisposalList.size() != 0 && DisposalList.front().Data->Abort())
        DisposalList.pop_front();
}