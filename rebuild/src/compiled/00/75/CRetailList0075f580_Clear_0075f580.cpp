// CRetailList_0075f580::Clear() -- clear a list of CCharStrings (0x0099EAE0), every node freed, the sentinel relinked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F580, length 56 bytes.
// Leaf: Clear
// The chunk label (CNavigatorQueueManager::FrameUpdate) is propagated; named by effect. Callees: free @ 0x00BFEA14, CCharString::~CCharString @ 0x0099EAE0. Shape: the VC7.1 `list::clear`: the successor read before the node is destroyed, the sentinel relinked to itself at the end.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct SValue_0075f580 {
    CCharString m_0;
};
struct SNode_0075f580 {
    SNode_0075f580*   m_pNext;                        // +0x00
    SNode_0075f580*   m_pPrev;                        // +0x04
    SValue_0075f580   m_value;                        // +0x08
};

struct CRetailList_0075f580 {
    SNode_0075f580* m_pHead;                          // +0x00: the sentinel
    void Clear();
};

void CRetailList_0075f580::Clear()
{
    SNode_0075f580* next = m_pHead->m_pNext;
    while (next != m_pHead) {
        SNode_0075f580* node = next;
        next = next->m_pNext;
        node->m_value.~SValue_0075f580();
        free(node);
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}