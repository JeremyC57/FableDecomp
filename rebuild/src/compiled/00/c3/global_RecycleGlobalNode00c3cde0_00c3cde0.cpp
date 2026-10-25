// RecycleGlobalNode_00c3cde0(CPooledNode_00c3cde0* node) -- return a node to the library's global two-chain pool (0x0143BC00): take it off the active chain, then, when recycling is enabled (free count not negative), reset it (slot 1) and park it on the free chain, otherwise delete it
// Leaf: ?RecycleGlobalNode_00c3cde0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3CDE0, 59 bytes, __cdecl (plain `ret`; callers 0x00C36095 `push ; call ; pop ecx`
//   and 0x00C3D24F `push ; call ; add esp,4`).
// Library: libpk audio (no PDB/RTTI); descriptive names. The 20-byte pool object is the global at 0x0143BC00 (inside the
//   audio system singleton 0x0143BB58 + 0xA8); its free count is read straight from 0x0143BC0C. Same pool methods as
//   the landed PushActive 0x00C3C9B0 / PushFree 0x00C3CA00. The manifest's "ScreenEffect_EnqueueCombatAction" is
//   propagated.
// Callees: CNodePool::RemoveActive 0x00C3C9E0, CNodePool::PushFree 0x00C3CA00 (thiscall on the global), the node's
//   virtual slot 1 (+4, reset for reuse) and slot 0 (deleting destructor, `delete node`).
// Shape: `delete node` keeps its null test (the node was only passed by pointer before); the free-count test is `jl`
//   to the delete arm placed last.
struct CPooledNode_00c3cde0 {                   // intrusive doubly-linked node
    virtual ~CPooledNode_00c3cde0();            // slot 0
    virtual void Reset();                       // slot 1 (+4)
    CPooledNode_00c3cde0* m_pPrev;              // +0x04
    CPooledNode_00c3cde0* m_pNext;              // +0x08
};

struct CNodePool_00c3cde0 {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode_00c3cde0* m_pActiveHead;        // +0x04
    CPooledNode_00c3cde0* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void RemoveActive(CPooledNode_00c3cde0* node);  // 0x00C3C9E0
    void PushFree(CPooledNode_00c3cde0* node);      // 0x00C3CA00
};

extern CNodePool_00c3cde0 g_nodePool_0143bc00;  // 0x0143BC00

void __cdecl RecycleGlobalNode_00c3cde0(CPooledNode_00c3cde0* node)
{
    g_nodePool_0143bc00.RemoveActive(node);
    if (g_nodePool_0143bc00.m_freeCount >= 0) {
        node->Reset();
        g_nodePool_0143bc00.PushFree(node);
    } else
        delete node;
}