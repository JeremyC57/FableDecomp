// CNavigatorManager::InterruptAllNavigators() -- interrupt every navigator (+8 of each node) on the navigator list at +0x18 through its virtual slot 18 (+0x48)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14F40, length 48 bytes.
// Leaf: InterruptAllNavigators
// Shape: `this` in edi, the node in esi, the sentinel re-read for the loop test, the loop head aligned, `ret`.
struct IListed_00a14f40 {
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
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void Call();                           // slot 18 (+0x48)
};
struct SNode_00a14f40 {
    SNode_00a14f40*    m_pNext;                       // +0x00
    SNode_00a14f40*    m_pPrev;                       // +0x04
    IListed_00a14f40*  m_pObject;                     // +0x08
};

struct CNavigatorManager {
    unsigned char _pad00[0x18];
    SNode_00a14f40*  m_pList;                         // +0x18: the sentinel
    void InterruptAllNavigators();
};

void CNavigatorManager::InterruptAllNavigators()
{
    for (SNode_00a14f40* node = m_pList->m_pNext; node != m_pList; node = node->m_pNext)
        node->m_pObject->Call();
}
