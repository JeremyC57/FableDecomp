// CBankFileAsync::~CBankFileAsync() -- under the bank lock, abort and drop every live async data instance (when one
//   cannot be aborted yet, release the lock, Sleep(10) and retake it); then leave the loading-memory failure handler,
//   delete the critical section, and destroy the members (disposal list, the two pointer sets, the three counted
//   pointers) before the CBankFile base
// Leaf: ??1CBankFileAsync@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D6490, 524 bytes (528 with padding).
// Label correction: catalogued as "CBankFile::CBankFile::Destructor"; it stores the CBankFileAsync vtable 0x0129B8D4
//   and destroys the async members (+0x110..+0x14c) of the landed CBankFileAsync ctor 0x009D5F80.
// Callees: CCriticalSectionLock ctor 0x00A62920 / Unlock 0x00A62960 / Lock 0x00A62980 / dtor 0x00A629A0,
//   CBankFileAsyncData::Abort 0x009D6350, _Rb_tree_rebalance_for_erase 0x0042A34E, free 0x00BFEA14,
//   Sleep 0x0143FE1C, CLoadingMemoryFailureHandler::RemoveBank 0x009D5A30, DeleteCriticalSection 0x0143FDE4,
//   list<CCountedPointer<CBankFileAsyncData>>::clear 0x009D7DB0, set<CBankFileAsyncData*>::_M_erase 0x009D7430,
//   operator delete 0x00BFE9BC, CBankFile::~CBankFile 0x009CE180 (manifest "NHeroInformationScreens::CBase::~CBase").
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CRITICAL_SECTION_Fable {                    // Win32 RTL_CRITICAL_SECTION, 24 bytes (dword aligned)
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);                         // 0x0143FE1C
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION_Fable* section);    // 0x0143FDE4
struct CCriticalSectionLock {                      // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    void Unlock();                                 // 0x00A62960
    void Lock();                                   // 0x00A62980
    ~CCriticalSectionLock();                       // 0x00A629A0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T*           Data;                             // +0
    SCountBlock* Info;                             // +4
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct SRbTreeNode {                               // 0x14 bytes
    unsigned char m_color;                         // +0x00
    SRbTreeNode*  m_pParent;                       // +0x04
    SRbTreeNode*  m_pLeft;                         // +0x08
    SRbTreeNode*  m_pRight;                        // +0x0c
    void*         m_value;                         // +0x10
};
SRbTreeNode* __cdecl RebalanceForErase(SRbTreeNode* z, SRbTreeNode*& root, SRbTreeNode*& leftmost,
                                       SRbTreeNode*& rightmost);   // 0x0042A34E
class CBankFileAsyncData { public:
    bool Abort();                                  // 0x009D6350
};
struct CPointerSet {                               // set<CBankFileAsyncData*> (12 bytes)
    SRbTreeNode*  m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_compare;
    void _M_erase(SRbTreeNode* node);              // 0x009D7430
    unsigned long size() const { return m_nodeCount; }
    SRbTreeNode* begin() const { return m_pHeader->m_pLeft; }
    __forceinline void erase(SRbTreeNode* position)
    {
        SRbTreeNode* y = RebalanceForErase(position, m_pHeader->m_pParent, m_pHeader->m_pLeft, m_pHeader->m_pRight);
        if (y)
            free(y);
        --m_nodeCount;
    }
    __forceinline ~CPointerSet()
    {
        if (m_nodeCount != 0) {
            _M_erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
        if (m_pHeader)
            free(m_pHeader);
    }
};
struct SListNode;
struct CDisposalList {                             // list<CCountedPointer<CBankFileAsyncData>> (STLport, 4 bytes)
    SListNode* m_pNode;
    void clear();                                  // 0x009D7DB0
    __forceinline ~CDisposalList()
    {
        clear();
        if (m_pNode)
            free(m_pNode);
    }
};

class CBankFile {
public:
    virtual ~CBankFile();                          // 0x009CE180
    unsigned char _body[0x10c];                    // retail CBankFile ends at +0x110
};
class CThreadedFile;
class CMovableResourceMemoryPool;
class CBankFileAsync;
class CLoadingMemoryFailureHandler {
public:
    void RemoveBank(CBankFileAsync* bank);         // 0x009D5A30
};

class CBankFileAsync : public CBankFile {          // vtable 0x0129B8D4
public:
    CCountedPointer<CThreadedFile> ThreadedFile;   // +0x110
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;   // +0x118
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;   // +0x120
    CPointerSet AsyncDataInstances;                // +0x128
    CPointerSet ReadsToRestart;                    // +0x134
    CBankFileAsyncData* LastRestartingData;        // +0x140
    CDisposalList DisposalList;                    // +0x144
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
    virtual ~CBankFileAsync();
};

CBankFileAsync::~CBankFileAsync()
{
    CCriticalSectionLock lock(&CriticalSection, false);
    while (AsyncDataInstances.size() != 0) {
        SRbTreeNode* first = AsyncDataInstances.begin();
        if (((CBankFileAsyncData*)first->m_value)->Abort()) {
            AsyncDataInstances.erase(first);
        } else {
            lock.Unlock();
            Sleep(10);
            lock.Lock();
        }
    }
    lock.Unlock();
    if (MemoryFailureHandler.Data)
        MemoryFailureHandler.Data->RemoveBank(this);
    DeleteCriticalSection(&CriticalSection);
}