// CRetailDeque_009138b0::PopFrontAux() -- STLport deque `_M_pop_front_aux` for 48-byte elements (two per 128-byte buffer): free the front buffer, move to the next map node, reset the start cursor
// Leaf: PopFrontAux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009138B0, length 46 bytes.
// Callees: free @ 0x00BFEA14 (null-tested inline deallocate, `add esp,4`).
// Label: the manifest's "CTCCarryable::OnKill" is propagated and rejected (no caller or vtable
// reference found); named by its STL role.
// Shape: identical to the 0x0040F560 body except the buffer span: `add eax,60h` = 2 elements of
// 0x30 bytes (STLport buffer = 128 / sizeof(T) elements, rounded down), so T is a 48-byte record.
// `_M_set_node(node + 1)` stores node, first, last, then `_M_cur = _M_first` re-reads the member.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct SDequeElement_009138b0 { long m_words[12]; };   // 48 bytes

struct CRetailDeque_009138b0 {
    struct iterator {
        SDequeElement_009138b0*  m_pCur;           // +0x00
        SDequeElement_009138b0*  m_pFirst;         // +0x04
        SDequeElement_009138b0*  m_pLast;          // +0x08
        SDequeElement_009138b0** m_pNode;          // +0x0c
        void SetNode(SDequeElement_009138b0** node)
        {
            m_pNode = node;
            m_pFirst = *node;
            m_pLast = m_pFirst + 0x80 / sizeof(SDequeElement_009138b0);
        }
    };
    iterator                 m_start;              // +0x00
    iterator                 m_finish;             // +0x10
    SDequeElement_009138b0** m_pMap;               // +0x20
    unsigned long            m_mapSize;            // +0x24
    void DeallocateNode(SDequeElement_009138b0* p) { if (p != 0) free(p); }
    void PopFrontAux();
};

void CRetailDeque_009138b0::PopFrontAux()
{
    DeallocateNode(m_start.m_pFirst);
    m_start.SetNode(m_start.m_pNode + 1);
    m_start.m_pCur = m_start.m_pFirst;
}