// CBankFileAsync::RestartReads() -- under the bank lock, re-queue every read in ReadsToRestart with the threaded file and empty the set
// Leaf: RestartReads
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5A90, 200 bytes (+ int3 padding). __thiscall, no arguments, `ret`.
// Label correction: catalogued as "CMusicManager::UpdateRegistrations" (a forge auto-name). The lock is taken on the
//   critical section at +0x14c and the set walked is at +0x134: the retail CBankFileAsync layout of the landed ctor
//   0x009D5F80 (CriticalSection +0x14c, ReadsToRestart +0x134), and each element's +0x28 is CBankFileAsyncData::BankFile.
// Callees: CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0; CThreadedFile::AddObject(CThreadedFileObjectDesc*)
//   0x0098F350 (forwards to g_ThreadedFileManager.AddObject, returns the new object UID); the STLport
//   _Rb_tree::_M_erase(root) of the pointer set 0x009D7430 (frees the nodes with free 0x00BFEA14).
// Shape: a range-for over an STLport set<CBankFileAsyncData*>: the iterator's ++ (_M_increment) is inline -- right child then
//   leftmost, else climb while we are our parent's right child; the end test reloads the header. Each restarted read gets a
//   fresh UID from the bank's threaded file for its stored load descriptor. The set's clear() is inline too: _M_erase(root)
//   only when the count is non-zero, then the header is re-linked to itself (leftmost, root 0, rightmost) and the count zeroed.
struct RTL_CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CCriticalSectionLock {                      // 8 bytes on the stack
    RTL_CRITICAL_SECTION_* Section;
    bool Locked;
    CCriticalSectionLock(RTL_CRITICAL_SECTION_* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
};
struct CThreadedFileObjectDesc { unsigned char _body[0x18]; };
struct CThreadedFile {
    unsigned long AddObject(CThreadedFileObjectDesc* desc);              // 0x0098F350
};
template <class T> struct CCountedPointer {
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct SRbTreeNode {                               // STLport _Rb_tree_node<CBankFileAsyncData*>, 0x14 bytes
    unsigned char m_color;                         // +0x00
    SRbTreeNode*  m_pParent;                       // +0x04
    SRbTreeNode*  m_pLeft;                         // +0x08
    SRbTreeNode*  m_pRight;                        // +0x0c
    struct CBankFileAsyncData* m_value;            // +0x10
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
struct CBankFileAsync;
struct CMovableResourceMemoryPool;
struct CBankFileAsyncData {                        // Ego_r.pdb layout (72 bytes)
    virtual void v00();
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
struct CBankFileAsync {                            // retail layout (PDB - 0x64; CBankFile base is 0x110 bytes)
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
