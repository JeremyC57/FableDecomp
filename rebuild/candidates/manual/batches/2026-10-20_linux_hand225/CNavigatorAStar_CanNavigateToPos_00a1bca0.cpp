// CNavigatorAStar::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool useLastNavigable) const -- find the quad-tree nodes under both positions (replacing a blocked start node by the last navigable node or the nearest navigable one within 4 m, and a blocked end node by the nearest navigable one that still reaches within the radius) and report whether both are navigable and their regions are connected
// Leaf: ?CanNavigateToPos@CNavigatorAStar@@UBE_NABVC3DVector@@0M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1BCA0, 297 bytes, `ret 10h`.
// Callees: the external interface's virtual slot 16 (+0x40, the nav layer of a position), CNavigatorManager::GetNodeAt(const
//   C2DVector&, unsigned char) 0x00A14F70, CNavigatorBase::FindNearestNavigableNode(from node, blocked node, const C3DVector&, float,
//   CNavigatorManager*) 0x00A475C0, CNavigatorBase::IsAnyPartOfNodeWithinDistOfPos(node, float, const C3DVector&, C2DVector*)
//   0x00A471D0, the node's virtual slot 7 (+0x1c, its region) and the region connectivity test 0x00A77780.
// Layout (retail, Ego_r.pdb less the 12-byte map / 4-byte list): CNavigatorBase PInterface +0x14, NavigationManager +0x1c;
//   CNavigatorAStar LastNavigableNodeVisited +0x88; CANavQuadTreeNode Blocked +0x0f.
// Shape: named C2DVector locals for the two lookups; the final test is one `&&` chain (int-valued exits), the early
//   failures are plain `return false`.
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector { float X, Y, Z; };
struct CNavRegion {
    bool IsConnectedTo(const CNavRegion* other) const;   // 0x00A77780
};
struct CANavQuadTreeNode {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual CNavRegion* GetRegion() const;         // slot 7 (+0x1c)
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};
struct CNavigatorManager {
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
};
struct CNavigatorExternalInterface {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual unsigned char GetNavLayer(const C3DVector& position) const;   // slot 16 (+0x40)
};
struct CSteeringAgent;
struct CNavigatorBase {
    virtual void v00();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;        // +0x14
    bool                         OnRoute;           // +0x18
    bool                         Interrupted;       // +0x19
    CNavigatorManager*           NavigationManager; // +0x1c
    CSteeringAgent*              SteeringAgent;     // +0x20
    CANavQuadTreeNode* FindNearestNavigableNode(CANavQuadTreeNode* from, CANavQuadTreeNode* node, const C3DVector& position, float distance, CNavigatorManager* manager) const;   // 0x00A475C0
    bool IsAnyPartOfNodeWithinDistOfPos(CANavQuadTreeNode* node, float distance, const C3DVector& position, C2DVector* nearest) const;   // 0x00A471D0
};
struct CNavigatorAStar : CNavigatorBase {
    unsigned char      _pad24[0x88 - 0x24];
    CANavQuadTreeNode* LastNavigableNodeVisited;   // +0x88
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool useLastNavigable) const;
};

bool CNavigatorAStar::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool useLastNavigable) const
{
    C2DVector from2d(from.X, from.Y);
    CANavQuadTreeNode* startNode = NavigationManager->GetNodeAt(from2d, PInterface->GetNavLayer(from));
    if (!startNode)
        return false;
    if (startNode->Blocked) {
        if (useLastNavigable && LastNavigableNodeVisited)
            startNode = LastNavigableNodeVisited;
        else
            startNode = FindNearestNavigableNode(startNode, startNode, from, 4.0f, NavigationManager);
        if (!startNode)
            return false;
    }
    C2DVector to2d(to.X, to.Y);
    CANavQuadTreeNode* endNode = NavigationManager->GetNodeAt(to2d, PInterface->GetNavLayer(to));
    if (!endNode)
        return false;
    if (endNode->Blocked) {
        C2DVector nearest;
        endNode = FindNearestNavigableNode(startNode, endNode, to, radius, NavigationManager);
        if (!endNode)
            return false;
        if (!IsAnyPartOfNodeWithinDistOfPos(endNode, radius, to, &nearest))
            return false;
    }
    return !endNode->Blocked && !startNode->Blocked && startNode->GetRegion()->IsConnectedTo(endNode->GetRegion());
}
