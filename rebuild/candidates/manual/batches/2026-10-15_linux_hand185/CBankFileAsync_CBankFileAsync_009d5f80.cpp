// CBankFileAsync::CBankFileAsync() -- the asynchronous bank file: CBankFile base, null threaded file / memory pool / failure handler, empty async-data sets and disposal list, flags clear, critical section initialised
// Leaf: ??0CBankFileAsync@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5F80, length 224 bytes, no arguments, returns `this`.
// Label correction: catalogued as "CBankFile::CBankFile::ConstructorInitialize"; the vtable stored is 0x0129B8D4 and the members
// are Ego_r.pdb CBankFileAsync's (Ego_r ??0CBankFileAsync@@QAE@XZ).
// Callees: CBankFile::CBankFile() @ 0x009CD480, malloc @ 0x00BFEA0E (two 0x14-byte set headers, one 0x10-byte list node),
// InitializeCriticalSection (kernel32 import 0x0143FEE4).
// Shape: the counted pointers and the STLport sets/list are inline constructors in declaration order (set header as the
// landed CLookDrawRequest 0x007554C0; the list allocates into a local and self-links it before storing); NonCached,
// DisableAsync and LastRestartingData are assigned in the body.
// Layout (retail = PDB - 0x64: the retail CBankFile base is 0x110 bytes): ThreadedFile +0x110, LoadingMemoryPool +0x118,
// MemoryFailureHandler +0x120, AsyncDataInstances +0x128, ReadsToRestart +0x134, LastRestartingData +0x140, DisposalList
// +0x144, NonCached +0x148, DisableAsync +0x149 (the landed OpenFileReadWrite 0x009D5200 sets it), CriticalSection +0x14c.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
struct RTL_CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(RTL_CRITICAL_SECTION_* section);   // import 0x0143FEE4

template <class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct SRbTreeNode {                               // 0x14 bytes
    unsigned char m_color;                         // +0x00 (0 = red)
    SRbTreeNode*  m_pParent;                       // +0x04
    SRbTreeNode*  m_pLeft;                         // +0x08
    SRbTreeNode*  m_pRight;                        // +0x0c
    void*         m_value;                         // +0x10
};
struct CRbTreeBase {
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};
struct CPointerSet : CRbTreeBase {                 // set<CBankFileAsyncData*> (12 bytes)
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CPointerSet() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};
struct SListNode {                                 // 0x10 bytes
    SListNode* m_pNext;
    SListNode* m_pPrev;
    void* m_data;                                  // CCountedPointer<CBankFileAsyncData>
    void* m_info;
};
struct CDisposalList {                             // list<CCountedPointer<CBankFileAsyncData>> (STLport, 4 bytes)
    SListNode* m_pNode;
    CDisposalList() : m_pNode(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pNode = node;
    }
};

class CBankFile {
public:
    virtual void v00();
    unsigned char _body[0x10c];                    // retail CBankFile ends at +0x110
    CBankFile();                                   // 0x009CD480
};
struct CThreadedFile;
struct CMovableResourceMemoryPool;
struct CLoadingMemoryFailureHandler;
struct CBankFileAsyncData;

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
    RTL_CRITICAL_SECTION_ CriticalSection;         // +0x14c
    virtual void v00();
    CBankFileAsync();
};

CBankFileAsync::CBankFileAsync()
{
    NonCached = false;
    DisableAsync = false;
    LastRestartingData = 0;
    InitializeCriticalSection(&CriticalSection);
}
