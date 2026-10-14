// CListOwner_00a36fe0::CallAll() -- call virtual slot 1 of every object (+8 of each node) on the list at +0x18; the chunk label (CNavigatorManager::InterruptAllNavigators) is propagated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36FE0, length 34 bytes.
// Leaf: CallAll
// Shape: `this` in edi, the node in esi, the sentinel re-read for the loop test, the loop head aligned, `ret`.
struct IListed_00a36fe0 {
    virtual void v000();
    virtual void Call();                           // slot 1 (+0x4)
};
struct SNode_00a36fe0 {
    SNode_00a36fe0*    m_pNext;                       // +0x00
    SNode_00a36fe0*    m_pPrev;                       // +0x04
    IListed_00a36fe0*  m_pObject;                     // +0x08
};

struct CListOwner_00a36fe0 {
    unsigned char _pad00[0x18];
    SNode_00a36fe0*  m_pList;                         // +0x18: the sentinel
    void CallAll();
};

void CListOwner_00a36fe0::CallAll()
{
    for (SNode_00a36fe0* node = m_pList->m_pNext; node != m_pList; node = node->m_pNext)
        node->m_pObject->Call();
}