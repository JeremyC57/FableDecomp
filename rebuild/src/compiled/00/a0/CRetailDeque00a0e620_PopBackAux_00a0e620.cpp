// CRetailDeque_00a0e620::PopBackAux() -- STLport deque `_M_pop_back_aux` for 8-byte elements: free the emptied back buffer, step to the previous map node, point the finish cursor at its last element
// Leaf: PopBackAux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0E620, length 49 bytes.
// Callees: free @ 0x00BFEA14 (null-tested inline deallocate, `add esp,4`: a speed TU, unlike 0x0059AD35).
// Label: the manifest's "CTCCarryable::OnKill" is propagated and rejected (no caller or vtable
// reference found); named by its STL role.
// Shape: as 0x0059AD35 with 8-byte elements (`sub eax,8` for `_M_last - 1`): release
// `_M_finish._M_first`, `_M_set_node(node - 1)` (node +0x1c, first +0x14, last = first + 0x80 at
// +0x18), then `_M_cur = _M_last - 1` from the register holding `last`.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct SDequeElement_00a0e620 { long m_words[2]; };   // 8 bytes

struct CRetailDeque_00a0e620 {
    struct iterator {
        SDequeElement_00a0e620*  m_pCur;           // +0x00
        SDequeElement_00a0e620*  m_pFirst;         // +0x04
        SDequeElement_00a0e620*  m_pLast;          // +0x08
        SDequeElement_00a0e620** m_pNode;          // +0x0c
        void SetNode(SDequeElement_00a0e620** node)
        {
            m_pNode = node;
            m_pFirst = *node;
            m_pLast = m_pFirst + 0x80 / sizeof(SDequeElement_00a0e620);
        }
    };
    iterator                 m_start;              // +0x00
    iterator                 m_finish;             // +0x10
    SDequeElement_00a0e620** m_pMap;               // +0x20
    unsigned long            m_mapSize;            // +0x24
    void DeallocateNode(SDequeElement_00a0e620* p) { if (p != 0) free(p); }
    void PopBackAux();
};

void CRetailDeque_00a0e620::PopBackAux()
{
    DeallocateNode(m_finish.m_pFirst);
    m_finish.SetNode(m_finish.m_pNode - 1);
    m_finish.m_pCur = m_finish.m_pLast - 1;
}