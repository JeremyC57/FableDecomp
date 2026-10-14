// CNavQuadTreeNode::CalculateChildrenNeighbours() -- every child (+0x10..+0x1c) calculates its own children's neighbours (slot 18), then the four children are linked as neighbours around the node: child 0 side 2 -> child 1, child 1 side 0 -> child 3, child 3 side 3 -> child 2, child 2 side 1 -> child 0 (slot 17 with (side, neighbour))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75DA0, length 96 bytes.
// Leaf: ?CalculateChildrenNeighbours@CNavQuadTreeNode@@MAEXXZ
// Shape: `this` in esi, each child and its vtable reloaded per call, `ret`.
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
    virtual void CalculateChildrenNeighbours();
};

void CNavQuadTreeNode::CalculateChildrenNeighbours()
{
    m_pChildren[0]->CalculateChildrenNeighbours();
    m_pChildren[1]->CalculateChildrenNeighbours();
    m_pChildren[2]->CalculateChildrenNeighbours();
    m_pChildren[3]->CalculateChildrenNeighbours();
    m_pChildren[0]->SetNeighbour(2, m_pChildren[1]);
    m_pChildren[1]->SetNeighbour(0, m_pChildren[3]);
    m_pChildren[3]->SetNeighbour(3, m_pChildren[2]);
    m_pChildren[2]->SetNeighbour(1, m_pChildren[0]);
}