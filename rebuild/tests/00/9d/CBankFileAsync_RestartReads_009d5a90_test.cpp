#include <cstdio>
// Behaviour test for 009d5a90 CBankFileAsync::RestartReads: the lock is held around the walk, every set element (in order)
// gets a fresh UID from its bank's threaded file for its own load descriptor, and the set is cleared afterwards.
struct RTL_CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CCriticalSectionLock {                      // 8 bytes on the stack
    RTL_CRITICAL_SECTION_* Section;
    bool Locked;
    CCriticalSectionLock(RTL_CRITICAL_SECTION_* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
};
class CThreadedFileObjectDesc { public: unsigned char _body[0x18]; };
class CThreadedFile { public:
    unsigned long AddObject(CThreadedFileObjectDesc* desc);              // 0x0098F350
};
template <class T> class CCountedPointer { public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct SRbTreeNode {                               // STLport _Rb_tree_node<CBankFileAsyncData*>, 0x14 bytes
    unsigned char m_color;                         // +0x00
    SRbTreeNode*  m_pParent;                       // +0x04
    SRbTreeNode*  m_pLeft;                         // +0x08
    SRbTreeNode*  m_pRight;                        // +0x0c
    class CBankFileAsyncData* m_value;            // +0x10
};
struct CPointerSetIterator {
    SRbTreeNode* m_pNode;
    CPointerSetIterator(SRbTreeNode* node) : m_pNode(node) {}
    CBankFileAsyncData* operator*() const { return m_pNode->m_value; }
    bool operator!=(const CPointerSetIterator& other) const { return m_pNode != other.m_pNode; }
    static SRbTreeNode* Increment(SRbTreeNode* node)   // STLport _Rb_global::_M_increment
    {
        if (node->m_pRight != 0) {
            node = node->m_pRight;
            while (node->m_pLeft != 0)
                node = node->m_pLeft;
        } else {
            SRbTreeNode* y = node->m_pParent;
            while (node == y->m_pRight) {
                node = y;
                y = y->m_pParent;
            }
            if (node->m_pRight != y)
                node = y;
        }
        return node;
    }
    void operator++() { m_pNode = Increment(m_pNode); }
};
struct CPointerSet {                               // set<CBankFileAsyncData*> (STLport, 12 bytes)
    SRbTreeNode*  m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CPointerSetIterator begin() { return CPointerSetIterator(m_pHeader->m_pLeft); }
    CPointerSetIterator end() { return CPointerSetIterator(m_pHeader); }
    void _M_erase(SRbTreeNode* node);              // 0x009D7430
    void clear()
    {
        if (m_nodeCount != 0) {
            _M_erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
    }
};
class CBankFileAsync;
class CMovableResourceMemoryPool;
class CBankFileAsyncData { public:                        // Ego_r.pdb layout (72 bytes)
    void*                   Vtbl;
    void*                   Allocation;            // +0x04
    void*                   Data;                  // +0x08
    void*                   Buffer;                // +0x0c
    unsigned long           Length;                // +0x10
    unsigned long           SkipBytes;             // +0x14
    unsigned long           ThreadedFileUID;       // +0x18
    CMovableResourceMemoryPool* MemoryPool;        // +0x1c
    long                    MemoryHandle;          // +0x20
    unsigned long           InternalAllocatedBufferSize;   // +0x24
    CBankFileAsync*         BankFile;              // +0x28
    CThreadedFileObjectDesc LoadDesc;              // +0x2c
    bool                    Finished;              // +0x44
    bool                    BufferLocked;          // +0x45
    bool                    AllowRestart;          // +0x46
    bool                    Aborted;               // +0x47
};
class CBankFileAsync { public:                            // retail layout (PDB - 0x64; CBankFile base is 0x110 bytes)
    unsigned char _base[0x110];                    // CBankFile
    CCountedPointer<CThreadedFile> ThreadedFile;   // +0x110
    unsigned char _pad118[0x10];                   // LoadingMemoryPool, MemoryFailureHandler
    CPointerSet AsyncDataInstances;                // +0x128
    CPointerSet ReadsToRestart;                    // +0x134
    CBankFileAsyncData* LastRestartingData;        // +0x140
    void* DisposalList;                            // +0x144
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    RTL_CRITICAL_SECTION_ CriticalSection;         // +0x14c
    void RestartReads();
};

void CBankFileAsync::RestartReads()
{
    CCriticalSectionLock lock(&CriticalSection, false);
    for (CPointerSetIterator it = ReadsToRestart.begin(); it != ReadsToRestart.end(); ++it) {
        CBankFileAsyncData* data = *it;
        data->ThreadedFileUID = data->BankFile->ThreadedFile->AddObject(&data->LoadDesc);
    }
    ReadsToRestart.clear();
}

static int g_locked = 0, g_lockCalls = 0, g_erased = 0, g_order[8], g_nOrder = 0;
static SRbTreeNode* g_erasedRoot = 0;
CCriticalSectionLock::CCriticalSectionLock(RTL_CRITICAL_SECTION_* section, bool tryOnly) : Section(section), Locked(true) { ++g_locked; ++g_lockCalls; }
CCriticalSectionLock::~CCriticalSectionLock() { --g_locked; }
static unsigned long g_nextUid = 100;
static CThreadedFileObjectDesc* g_descs[8];
unsigned long CThreadedFile::AddObject(CThreadedFileObjectDesc* desc)
{
    if (g_locked != 1) return 0;
    g_descs[g_nextUid - 100] = desc;
    return g_nextUid++;
}
void CPointerSet::_M_erase(SRbTreeNode* node) { ++g_erased; g_erasedRoot = node; }

int main()
{
    static CBankFileAsync bank;
    CThreadedFile file;
    bank.ThreadedFile.Data = &file;
    CBankFileAsyncData d[3];
    for (int i = 0; i < 3; ++i) { d[i].BankFile = &bank; d[i].ThreadedFileUID = 0; }
    // header + balanced tree: n1 (root) with children n0 (left) and n2 (right)
    SRbTreeNode header, n[3];
    for (int i = 0; i < 3; ++i) { n[i].m_pLeft = 0; n[i].m_pRight = 0; n[i].m_value = &d[i]; }
    n[1].m_pParent = &header; n[1].m_pLeft = &n[0]; n[1].m_pRight = &n[2];
    n[0].m_pParent = &n[1]; n[2].m_pParent = &n[1];
    header.m_pParent = &n[1]; header.m_pLeft = &n[0]; header.m_pRight = &n[2];
    bank.ReadsToRestart.m_pHeader = &header; bank.ReadsToRestart.m_nodeCount = 3;
    bank.RestartReads();
    bool ok = g_lockCalls == 1 && g_locked == 0;
    ok = ok && d[0].ThreadedFileUID == 100 && d[1].ThreadedFileUID == 101 && d[2].ThreadedFileUID == 102;
    ok = ok && g_descs[0] == &d[0].LoadDesc && g_descs[1] == &d[1].LoadDesc && g_descs[2] == &d[2].LoadDesc;
    ok = ok && g_erased == 1 && g_erasedRoot == &n[1];
    ok = ok && header.m_pLeft == &header && header.m_pRight == &header && header.m_pParent == 0 && bank.ReadsToRestart.m_nodeCount == 0;
    // empty set: no AddObject, no erase
    bank.RestartReads();
    ok = ok && g_lockCalls == 2 && g_erased == 1 && g_nextUid == 103;
    if (!ok) { std::printf("009d5a90_TEST FAIL\n"); return 1; }
    std::printf("009d5a90_TEST PASS\n");
    return 0;
}