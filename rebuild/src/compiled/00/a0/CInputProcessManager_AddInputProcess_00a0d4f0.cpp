// CInputProcessManager::AddInputProcess(CInputProcessBase* process) -- link the process's list node (+8: owner, next, prev) at the tail of the manager's process list (+4 first, +8 last, +0xc count)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D4F0, length 64 bytes.
// Shape: the node address in eax, the old last in esi, `inc dword ptr [ecx+0Ch]`, `ret 4`.
struct CInputProcessBase;
struct SProcessNode_00a0d4f0 {                     // at +8 of every process
    CInputProcessBase*     m_pOwner;               // +0x00
    SProcessNode_00a0d4f0* m_pNext;                // +0x04
    SProcessNode_00a0d4f0* m_pPrev;                // +0x08
};
struct CInputProcessBase {
    unsigned char         _pad[8];
    SProcessNode_00a0d4f0 m_node;                  // +0x08
};
struct CInputProcessManager {
    virtual void v00();
    SProcessNode_00a0d4f0* m_pFirst;               // +0x04
    SProcessNode_00a0d4f0* m_pLast;                // +0x08
    long                   m_count;                // +0x0c
    void AddInputProcess(CInputProcessBase* process);
};

void CInputProcessManager::AddInputProcess(CInputProcessBase* process)
{
    SProcessNode_00a0d4f0* node = &process->m_node;
    node->m_pNext = 0;
    node->m_pPrev = m_pLast;
    node->m_pOwner = process;
    if (m_pLast)
        m_pLast->m_pNext = node;
    m_pLast = node;
    if (!m_pFirst)
        m_pFirst = node;
    ++m_count;
}