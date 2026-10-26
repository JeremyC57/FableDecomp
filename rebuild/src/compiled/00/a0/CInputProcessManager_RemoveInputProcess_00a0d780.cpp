// CInputProcessManager::RemoveInputProcess(CInputProcessBase* process) -- unlink the process's list node (+8: owner, next, prev) from the manager's process list, first moving every registered list cursor that sits on it to its neighbour
// Leaf: RemoveInputProcess
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D780, length 132 bytes.
// Model: the list as the landed CInputProcessManager::AddInputProcess 0x00A0D4F0 (+4 first, +8 last, +0xc count, node at
// +8 of every process); +0x10 heads a chain of live cursors (forward position +4, backward position +8, next cursor
// +0x10) that iterate the list while processes run, so removing the current node steps them to next / prev.
// Callers: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (landed). Shape: the node address in eax, the cursor in edx;
// each cursor test is `pos == node && pos` (the redundant null test survives); the final clears store next, prev, owner.
struct CInputProcessBase;
struct SProcessNode {                              // at +8 of every process
    CInputProcessBase* m_pOwner;                   // +0x00
    SProcessNode*      m_pNext;                    // +0x04
    SProcessNode*      m_pPrev;                    // +0x08
};
struct CInputProcessBase {
    unsigned char _pad[8];
    SProcessNode  m_node;                          // +0x08
};
struct SProcessCursor {                            // a live iteration over the process list
    unsigned char   _pad00[4];
    SProcessNode*   m_pForward;                    // +0x04
    SProcessNode*   m_pBackward;                   // +0x08
    unsigned char   _pad0c[4];
    SProcessCursor* m_pNextCursor;                 // +0x10
};
class CInputProcessManager { public:
    virtual void v00();
    SProcessNode*   m_pFirst;                      // +0x04
    SProcessNode*   m_pLast;                       // +0x08
    long            m_count;                       // +0x0c
    SProcessCursor* m_pCursors;                    // +0x10
    void RemoveInputProcess(CInputProcessBase* process);
};

void CInputProcessManager::RemoveInputProcess(CInputProcessBase* process)
{
    SProcessNode* node = &process->m_node;
    for (SProcessCursor* cursor = m_pCursors; cursor; cursor = cursor->m_pNextCursor) {
        if (cursor->m_pForward == node && cursor->m_pForward)
            cursor->m_pForward = node->m_pNext;
        if (cursor->m_pBackward == node && cursor->m_pBackward)
            cursor->m_pBackward = node->m_pPrev;
    }
    if (m_pFirst == node)
        m_pFirst = node->m_pNext;
    if (m_pLast == node)
        m_pLast = node->m_pPrev;
    if (node->m_pPrev)
        node->m_pPrev->m_pNext = node->m_pNext;
    if (node->m_pNext)
        node->m_pNext->m_pPrev = node->m_pPrev;
    node->m_pNext = 0;
    node->m_pPrev = 0;
    node->m_pOwner = 0;
    --m_count;
}