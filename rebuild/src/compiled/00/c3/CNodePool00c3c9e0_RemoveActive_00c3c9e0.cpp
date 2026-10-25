// CNodePool_00c3c9e0::RemoveActive(CPooledNode_00c3c9e0* node) -- take a node off the pool's active chain: advance the active head past it when it is the head, unlink it from its neighbours and drop the active count
// Leaf: ?RemoveActive@CNodePool_00c3c9e0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C9E0, 32 bytes, __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 20-byte two-chain node pool of the landed siblings
//   PushActive 0x00C3C9B0 / PushFree 0x00C3CA00 / RemoveFree 0x00C3CA50 (landed under propagated "CCombatActionPool" /
//   "CCombatActionManager" names; the pools sit in the libpk global 0x0143BC00 and at +0x1c/+0x30/+0x44/+0x58 of libpk
//   owners). Callers include the recycle helpers 0x00C3CDE0, 0x00C3D610, 0x00C3D660, 0x00C3D6A0. The manifest's
//   "ScreenEffect_RemoveFromList" is propagated.
// Callee: the node's Unlink 0x00C4CEF0 (thiscall on the node).
// Shape: `node == m_pActiveHead` compares in source order (`cmp ecx,eax`); the head's successor is read through the head.
struct CPooledNode_00c3c9e0 {                   // intrusive doubly-linked node
    virtual ~CPooledNode_00c3c9e0();            // slot 0
    CPooledNode_00c3c9e0* m_pPrev;              // +0x04
    CPooledNode_00c3c9e0* m_pNext;              // +0x08
    void Unlink();                              // 0x00C4CEF0
};

struct CNodePool_00c3c9e0 {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode_00c3c9e0* m_pActiveHead;        // +0x04
    CPooledNode_00c3c9e0* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void RemoveActive(CPooledNode_00c3c9e0* node);
};

void CNodePool_00c3c9e0::RemoveActive(CPooledNode_00c3c9e0* node)
{
    if (node == m_pActiveHead)
        m_pActiveHead = m_pActiveHead->m_pNext;
    node->Unlink();
    --m_activeCount;
}