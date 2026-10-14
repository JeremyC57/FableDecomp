// CRetailList_0070b8a0::Clear() -- clear a list whose values own two malloc'd buffers (node +0x18 freed first, then +0xc, each when set), every node freed, the sentinel relinked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B8A0, length 80 bytes.
// Leaf: Clear
// The chunk label (CNavigatorQueueManager::FrameUpdate) is propagated; named by effect. Callees: free @ 0x00BFEA14. Shape: the VC7.1 `list::clear`: the successor read before the node is destroyed, the sentinel relinked to itself at the end.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct SValue_0070b8a0 {                           // the two malloc'd buffers at +0xc and +0x18 of the node are freed when set
    long  m_0;
    char* m_pBufferA;                              // node +0x0c
    unsigned char _pad08[8];
    char* m_pBufferB;                              // node +0x18
};
struct SNode_0070b8a0 {
    SNode_0070b8a0*   m_pNext;                        // +0x00
    SNode_0070b8a0*   m_pPrev;                        // +0x04
    SValue_0070b8a0   m_value;                        // +0x08
};

struct CRetailList_0070b8a0 {
    SNode_0070b8a0* m_pHead;                          // +0x00: the sentinel
    void Clear();
};

void CRetailList_0070b8a0::Clear()
{
    SNode_0070b8a0* next = m_pHead->m_pNext;
    while (next != m_pHead) {
        SNode_0070b8a0* node = next;
        next = next->m_pNext;
        if (node->m_value.m_pBufferB)
            free(node->m_value.m_pBufferB);
        if (node->m_value.m_pBufferA)
            free(node->m_value.m_pBufferA);
        free(node);
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}
