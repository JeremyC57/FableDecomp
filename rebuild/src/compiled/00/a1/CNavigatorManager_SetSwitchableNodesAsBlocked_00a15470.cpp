// CNavigatorManager::SetSwitchableNodesAsBlocked(long id, long a, long b, long c) -- the navigation map with the id (+0x10 of the nodes on the list at +0x24) blocks the node range (0x00A79240 with (a, b, c)), then every navigator on the list at +0x18 is interrupted (virtual slot 18)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A15470, length 96 bytes.
// Leaf: SetSwitchableNodesAsBlocked
// Shape: `this` in edi, the id search with its `jmp` past the call, the navigator loop head aligned with `mov edi,edi`, `ret 10h`.
struct CNavQuadTree {
    void BlockRange(long a, long b, long c);       // 0x00A79240
};
struct SMapNode_00a15470 {
    SMapNode_00a15470* m_pNext;
    SMapNode_00a15470* m_pPrev;
    CNavQuadTree*      m_pTree;                    // +0x08
    long               m_c;
    long               m_id;                       // +0x10
};
struct INavigator_00a15470 {
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
    virtual void Interrupt();                      // slot 18 (+0x48)
};
struct SNavNode_00a15470 {
    SNavNode_00a15470*   m_pNext;
    SNavNode_00a15470*   m_pPrev;
    INavigator_00a15470* m_pNavigator;             // +0x08
};

struct CNavigatorManager {
    unsigned char       _pad00[0x18];
    SNavNode_00a15470*  m_pNavigators;             // +0x18: the sentinel
    unsigned char       _pad1c[8];
    SMapNode_00a15470*  m_pMaps;                   // +0x24: the sentinel
    void SetSwitchableNodesAsBlocked(long id, long a, long b, long c);
};

void CNavigatorManager::SetSwitchableNodesAsBlocked(long id, long a, long b, long c)
{
    for (SMapNode_00a15470* node = m_pMaps->m_pNext; node != m_pMaps; node = node->m_pNext) {
        if (node->m_id == id) {
            node->m_pTree->BlockRange(a, b, c);
            break;
        }
    }
    for (SNavNode_00a15470* node = m_pNavigators->m_pNext; node != m_pNavigators; node = node->m_pNext)
        node->m_pNavigator->Interrupt();
}