// CRetailList_009f9b30::Clear() -- clear a list whose values hold a wide string (node +0x38) and a malloc'd buffer (node +0x54, freed first when set), every node freed, the sentinel relinked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9B30, length 80 bytes.
// Leaf: Clear
// The chunk label (CNavigatorQueueManager::FrameUpdate) is propagated; named by effect. Callees: free @ 0x00BFEA14, CWideString::~CWideString @ 0x0099B510. Shape: the VC7.1 `list::clear`: the successor read before the node is destroyed, the sentinel relinked to itself at the end.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
struct SValue_009f9b30 {                           // the wide string at node +0x38, a malloc'd buffer at node +0x54
    unsigned char _pad00[0x30];
    CWideString   m_name;                          // node +0x38
    unsigned char _pad34[0x18];
    char*         m_pBuffer;                       // node +0x54
};
struct SNode_009f9b30 {
    SNode_009f9b30*   m_pNext;                        // +0x00
    SNode_009f9b30*   m_pPrev;                        // +0x04
    SValue_009f9b30   m_value;                        // +0x08
};

struct CRetailList_009f9b30 {
    SNode_009f9b30* m_pHead;                          // +0x00: the sentinel
    void Clear();
};

void CRetailList_009f9b30::Clear()
{
    SNode_009f9b30* next = m_pHead->m_pNext;
    while (next != m_pHead) {
        SNode_009f9b30* node = next;
        next = next->m_pNext;
        if (node->m_value.m_pBuffer)
            free(node->m_value.m_pBuffer);
        node->m_value.m_name.~CWideString();
        free(node);
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}
