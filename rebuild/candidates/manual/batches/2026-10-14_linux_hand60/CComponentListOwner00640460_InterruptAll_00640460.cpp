// CComponentListOwner_00640460::InterruptAll() -- call virtual slot 54 (+0xd8) of every object (+8 of each node) on the list at +4; the chunk label (CNavigatorManager::InterruptAllNavigators) is propagated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640460, length 48 bytes.
// Leaf: InterruptAll
// Shape: `this` in edi, the node in esi, the sentinel re-read for the loop test, the loop head aligned, `ret`.
struct IListed_00640460 {
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
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void Call();                           // slot 54 (+0xd8)
};
struct SNode_00640460 {
    SNode_00640460*    m_pNext;                       // +0x00
    SNode_00640460*    m_pPrev;                       // +0x04
    IListed_00640460*  m_pObject;                     // +0x08
};

struct CComponentListOwner_00640460 {
    unsigned char _pad00[0x4];
    SNode_00640460*  m_pList;                         // +0x4: the sentinel
    void InterruptAll();
};

void CComponentListOwner_00640460::InterruptAll()
{
    for (SNode_00640460* node = m_pList->m_pNext; node != m_pList; node = node->m_pNext)
        node->m_pObject->Call();
}
