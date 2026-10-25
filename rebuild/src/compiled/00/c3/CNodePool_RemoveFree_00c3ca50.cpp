// CNodePool::RemoveFree(CPooledNode* node) -- take a node off the pool's free chain: advance the free head past it when it is the head, unlink it from its neighbours and drop the free count
// Leaf: ?RemoveFree@CNodePool@@QAEXPAUCPooledNode@@@Z
// Naming (sweep1, 2026-10-25): the address suffix of the class is dropped so the libpk node pool has one name set
//   (CNodePool / CPooledNode) across its rows, as in the landed-error renames GetFree 0x00C3C9A0, PushActive 0x00C3C9B0,
//   PushFree 0x00C3CA00, PopFree 0x00C3CA30, CPooledNode::Link 0x00C4CED0 / Unlink 0x00C4CEF0 (FINDINGS chunk 438).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3CA50, 32 bytes, __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The free-chain twin of RemoveActive 0x00C3C9E0 on the 20-byte
//   two-chain node pool (landed PushActive 0x00C3C9B0 / PushFree 0x00C3CA00). The manifest's
//   "CShaderRenderManager_RemoveDisplacementRenderer" is propagated.
// Callee: the node's Unlink 0x00C4CEF0 (thiscall on the node).
// Shape: `m_pFreeHead == node` compares in source order (`cmp eax,ecx`, the mirror of RemoveActive's `cmp ecx,eax`).
struct CPooledNode {                   // intrusive doubly-linked node
    virtual ~CPooledNode();            // slot 0
    CPooledNode* m_pPrev;              // +0x04
    CPooledNode* m_pNext;              // +0x08
    void Unlink();                              // 0x00C4CEF0
};

struct CNodePool {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode* m_pActiveHead;        // +0x04
    CPooledNode* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void RemoveFree(CPooledNode* node);
};

void CNodePool::RemoveFree(CPooledNode* node)
{
    if (m_pFreeHead == node)
        m_pFreeHead = m_pFreeHead->m_pNext;
    node->Unlink();
    --m_freeCount;
}