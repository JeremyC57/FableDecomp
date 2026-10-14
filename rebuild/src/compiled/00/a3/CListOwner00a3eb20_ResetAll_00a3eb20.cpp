// CListOwner_00a3eb20::ResetAll() -- prepare this object (0x00991670), reset the object at +0x44 (its virtual slot 14), then call slot 14 of every object (+8 of each node) on the list at +0x4c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EB20, length 64 bytes.
// Leaf: ResetAll
// Callees: 0x00991670 (thiscall, no arguments; the manifest label is garbage). The chunk label (CNavigatorManager::InterruptAllNavigators) is propagated; named by address. Shape: `this` in edi, the node in esi, the loop head aligned with a 6-byte `lea`, `ret`.
struct IResettable_00a3eb20 {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void Reset();                          // slot 14 (+0x38)
};
struct SNode_00a3eb20 {
    SNode_00a3eb20*        m_pNext;
    SNode_00a3eb20*        m_pPrev;
    IResettable_00a3eb20*  m_pObject;              // +0x08
};

struct CListOwner_00a3eb20 {
    unsigned char          _pad00[0x44];
    IResettable_00a3eb20*  m_pMain;                // +0x44
    long                   m_48;
    SNode_00a3eb20*        m_pList;                // +0x4c: the sentinel
    void Prepare();                                // 0x00991670
    void ResetAll();
};

void CListOwner_00a3eb20::ResetAll()
{
    Prepare();
    m_pMain->Reset();
    for (SNode_00a3eb20* node = m_pList->m_pNext; node != m_pList; node = node->m_pNext)
        node->m_pObject->Reset();
}