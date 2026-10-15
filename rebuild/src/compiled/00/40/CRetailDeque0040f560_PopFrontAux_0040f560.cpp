// CRetailDeque_0040f560::PopFrontAux() -- STLport deque `_M_pop_front_aux`: free the exhausted front buffer, step the start iterator to the next map node and point it at that buffer's first element
// Leaf: PopFrontAux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040F560, length 48 bytes.
// Callees: free @ 0x00BFEA14 (the node allocator's deallocate, null-tested inline, `add esp,4`).
// Label: the manifest's "CTCCarryable::OnKill" is propagated over byte-identical bodies and rejected
// (the real CTCCarryable::OnKill is vtable slot 14 @ 0x00705150). No direct caller or vtable
// reference was found, so the owning class is not established; named by its STL role.
// Shape: STLport deque {_M_start, _M_finish} iterators of {cur, first, last, node}: the front buffer
// (`_M_start._M_first`, +4) is released, `_M_set_node(node + 1)` stores node (+0xc), first (+4) and
// last = first + 0x80 bytes (STLport's 128-byte buffers: _MAX_BYTES / sizeof(T) elements), then
// `_M_cur = _M_first` re-reads the member just stored (the retail `mov eax,[esi+4]`). The element
// size only fixes the 0x80 constant; 4-byte elements are modelled.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct SDequeElement_0040f560 { long m_value; };   // any size dividing 128 gives the same bytes

struct CRetailDeque_0040f560 {
    struct iterator {
        SDequeElement_0040f560*  m_pCur;           // +0x00
        SDequeElement_0040f560*  m_pFirst;         // +0x04
        SDequeElement_0040f560*  m_pLast;          // +0x08
        SDequeElement_0040f560** m_pNode;          // +0x0c
        void SetNode(SDequeElement_0040f560** node)
        {
            m_pNode = node;
            m_pFirst = *node;
            m_pLast = m_pFirst + 0x80 / sizeof(SDequeElement_0040f560);
        }
    };
    iterator                 m_start;              // +0x00
    iterator                 m_finish;             // +0x10
    SDequeElement_0040f560** m_pMap;               // +0x20
    unsigned long            m_mapSize;            // +0x24
    void DeallocateNode(SDequeElement_0040f560* p) { if (p != 0) free(p); }
    void PopFrontAux();
};

void CRetailDeque_0040f560::PopFrontAux()
{
    DeallocateNode(m_start.m_pFirst);
    m_start.SetNode(m_start.m_pNode + 1);
    m_start.m_pCur = m_start.m_pFirst;
}