// CNavQuadTreeNode::MakeRegion(const CNavRegion* region, CNavNodeList& list) -- true when any child (+0x10..+0x1c, slot 12 with (region, list)) makes the region; the children are tried in order and the fourth child's answer is returned as is
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75FC0, length 80 bytes.
// Leaf: ?MakeRegion@CNavQuadTreeNode@@MAE_NPBVCNavRegion@@AAV?$list@PAVCANavQuadTreeNode@@V?$allocator@PAVCANavQuadTreeNode@@@std@@@std@@@Z
// Shape: the region in ebx, the list in edi, `this` in esi, the first three results merged into one `mov al,1` exit, the fourth call's al returned directly, `ret 8`.
struct CNavRegion;
struct CNavNodeList;
struct CANavQuadTreeNode {
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
    virtual bool MakeRegion(const CNavRegion* region, CNavNodeList& list);   // slot 12 (+0x30)
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion* region, CNavNodeList& list);   // slot 13 (+0x34)
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void SetNeighbour(long side, CANavQuadTreeNode* neighbour);   // slot 17 (+0x44)
    virtual void CalculateChildrenNeighbours();   // slot 18 (+0x48)
};
struct CNavQuadTreeNode : CANavQuadTreeNode {
    unsigned char      _pad04[0xc];
    CANavQuadTreeNode* m_pChildren[4];             // +0x10
    virtual bool MakeRegion(const CNavRegion* region, CNavNodeList& list);
};

bool CNavQuadTreeNode::MakeRegion(const CNavRegion* region, CNavNodeList& list)
{
    if (m_pChildren[0]->MakeRegion(region, list))
        return true;
    if (m_pChildren[1]->MakeRegion(region, list))
        return true;
    if (m_pChildren[2]->MakeRegion(region, list))
        return true;
    return m_pChildren[3]->MakeRegion(region, list);
}