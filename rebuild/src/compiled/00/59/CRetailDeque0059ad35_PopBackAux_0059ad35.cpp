// CRetailDeque_0059ad35::PopBackAux() -- STLport deque `_M_pop_back_aux` for 4-byte elements: free the emptied back buffer, step the finish iterator to the previous map node and point it at that buffer's last element
// Leaf: PopBackAux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AD35, length 47 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: free @ 0x00BFEA14 (null-tested inline deallocate; the size TU pops the argument with `pop ecx`).
// Label: the manifest's "CTCCarryable::OnKill" is propagated and rejected; called from 0x005959E0
// and 0x0059AEF4 (deque pop_back with the inline fast path); named by its STL role.
// Shape: the finish iterator is the second {cur, first, last, node} block (+0x10): the buffer at
// `_M_finish._M_first` (+0x14) is released, `_M_set_node(node - 1)` stores node (+0x1c), first (+0x14)
// and last = first + 0x80 (+0x18), and `_M_cur = _M_last - 1` is computed from the register that
// still holds `last` (`sub eax,4`). `pop ecx` after the one-argument free marks the size TU.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct SDequeElement_0059ad35 { long m_value; };   // 4 bytes (`sub eax,4`)

struct CRetailDeque_0059ad35 {
    struct iterator {
        SDequeElement_0059ad35*  m_pCur;           // +0x00
        SDequeElement_0059ad35*  m_pFirst;         // +0x04
        SDequeElement_0059ad35*  m_pLast;          // +0x08
        SDequeElement_0059ad35** m_pNode;          // +0x0c
        void SetNode(SDequeElement_0059ad35** node)
        {
            m_pNode = node;
            m_pFirst = *node;
            m_pLast = m_pFirst + 0x80 / sizeof(SDequeElement_0059ad35);
        }
    };
    iterator                 m_start;              // +0x00
    iterator                 m_finish;             // +0x10
    SDequeElement_0059ad35** m_pMap;               // +0x20
    unsigned long            m_mapSize;            // +0x24
    void DeallocateNode(SDequeElement_0059ad35* p) { if (p != 0) free(p); }
    void PopBackAux();
};

void CRetailDeque_0059ad35::PopBackAux()
{
    DeallocateNode(m_finish.m_pFirst);
    m_finish.SetNode(m_finish.m_pNode - 1);
    m_finish.m_pCur = m_finish.m_pLast - 1;
}