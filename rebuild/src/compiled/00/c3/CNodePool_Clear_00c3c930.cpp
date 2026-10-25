// CNodePool::Clear() -- empty a two-chain node pool: a pool that recycles into a preallocated block (free count not negative) just forgets both chains and switches recycling off (free count -1); a heap pool deletes every node on its active chain
// Leaf: ?Clear@CNodePool@@QAEXXZ
// Naming (sweep1, 2026-10-25): the address suffix of the class is dropped so the libpk node pool has one name set
//   (CNodePool / CPooledNode) across its rows, as in the landed-error renames GetFree 0x00C3C9A0, PushActive 0x00C3C9B0,
//   PushFree 0x00C3CA00, PopFree 0x00C3CA30, CPooledNode::Link 0x00C4CED0 / Unlink 0x00C4CEF0 (FINDINGS chunk 438).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C930, 79 bytes, __fastcall, plain `ret`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 20-byte pool of the landed PushActive 0x00C3C9B0 /
//   PushFree 0x00C3CA00 (and Init 0x00C3C8C0, which threads a preallocated block onto the free chain). Callers: the
//   audio system's shutdown 0x00C3D160 (its four pools at +0x1c/+0x30/+0x44/+0x58), 0x00C3CADB, 0x00C4D1F7.., and
//   0x00C33390 on the global pool 0x0143BC00. The manifest's "LinkedList_ClearAndDestructor" is propagated.
// Callee: each node's deleting destructor (slot 0, `delete node`).
// Shape: the reset arm stores immediates in source order (+4, +8, +0x10, then -1 at +0xc); the delete loop is a
//   rotated `while (m_pActiveHead)`; advancing through the member (`m_pActiveHead = m_pActiveHead->m_pNext`) keeps the
//   delete's null test (advancing through the `node` local lets VC7.1 prove it non-null and drop the test).
struct CPooledNode {                   // intrusive doubly-linked node
    virtual ~CPooledNode();            // slot 0
    CPooledNode* m_pPrev;              // +0x04
    CPooledNode* m_pNext;              // +0x08
};

struct CNodePool {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode* m_pActiveHead;        // +0x04
    CPooledNode* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void Clear();
};

void CNodePool::Clear()
{
    if (m_freeCount >= 0) {
        m_pActiveHead = 0;
        m_pFreeHead = 0;
        m_activeCount = 0;
        m_freeCount = -1;
        return;
    }
    while (m_pActiveHead) {
        CPooledNode* node = m_pActiveHead;
        m_pActiveHead = m_pActiveHead->m_pNext;
        delete node;
        --m_activeCount;
    }
}