// CNavigatorAStar::IsNewDestinationGoingToCauseRestart(const C3DVector& destination, float radius) const -- whether moving the goal to a new destination forces the A* search to restart: always when idle or interrupted or off the map; while routing, unless the new node is (a neighbour of) the node asked for, or reaches the same goal node; otherwise when the new spot leaves the goal's reach circle
// Grade: DIFFER(324v324) -- scheduling only: retail issues `mov eax,[esi+78h] ; cmp edi,eax` before the `fadd [esp+1Ch]` of the reach radius, ours after it (RTM and QFE alike); everything else matches
// Chunk 638 (2026-10-31) tried: PDB respell kept here (agent633: const manager GetNodeAt(const C2DVector&, uchar) through the private inline CNavigatorAStar::GetNodeAt(const C3DVector&); interface slot 16 GetNavigationLevelAt, slot 6 PeekNavigatorRadius(bool); IsEqual private, FindNearestNavigableNode protected, const nodes) (same); askedTo named local (329); named bool sameNode (same); float range + radius = range*range (same).
// Leaf: ?IsNewDestinationGoingToCauseRestart@CNavigatorAStar
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1ABF0, length 322 bytes (padded to 336), `ret 8`. Public virtual const (UBE).
// Callees: as the landed GetNearestNavigablePosTo 0x00A1AB40: the external interface's (+0x14) virtual slots 16 GetLayerAt (+0x40)
//   and 6 (+0x18, the navigator's reach radius, float, called with false), CNavigatorManager::GetNodeAt(const C2DVector&, bool)
//   @ 0x00A14F70, CNavigatorBase::FindNearestNavigableNode @ 0x00A475C0; CNavigatorAStar::IsEqual @ 0x00A1A170 (landed),
//   CANavQuadTreeNode::GetSize @ 0x00A75B30 (landed), CANavQuadTreeNode::operator== @ 0x00A763F0, the node's virtual slot 4
//   IsNeighbourOf (+0x10).
// Layout: Ego_r.pdb CNavigatorAStar with the retail containers: GoalNode +0x6c, GoalPosition +0x70, NodeAskedToGetTo +0x78,
//   Route (STLport list, the sentinel pointer) +0x8c, HeadingForActionPoint +0xbc; CNavigatorBase OnRoute +0x18, Interrupted +0x19.
// Shape: one trailing `return true` shared by the early outs (keeps the three pushes at entry); the nearest node is a named local
//   (evaluated before GoalNode is re-read for IsEqual); returns are `cond ? false : true` (8-bit neg/sbb/inc); the reach radius is
//   accumulated into the radius parameter and squared in place; the planar distance test is
//   `!(dx*dx + dy*dy > radius)` on a C2DVector copy of the destination (VC emits the Y term first).
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
extern float g_quadTreeLevelCellSize[];            // 0x0129DCAC
class CANavQuadTreeNode {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* other) const;   // slot 4 (+0x10)
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    float GetSize() const;                         // 0x00A75B30
    bool operator==(const CANavQuadTreeNode& other) const;   // 0x00A763F0
};
class CNavigatorManager {
public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual float PeekNavigatorRadius(bool running) const;                       // slot 6 (+0x18)
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetNavigationLevelAt(const C3DVector& position) const;          // slot 16 (+0x40)
};
struct CNavWayPointNode {
    CNavWayPointNode* _M_next;
    CNavWayPointNode* _M_prev;
};
struct CNavRoute {                                 // STLport list<CNavWayPoint>: the sentinel pointer
    CNavWayPointNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CNavigatorBase {
public:
    virtual void v00();                            // +0x00 vptr
    unsigned char                _pad04[0x10];     // CClass registration
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;   // +0x1c
    void*                        SteeringAgent;    // +0x20
protected:
    const CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* blocked, const C3DVector& pos, float radius, const CNavigatorManager* manager) const;   // 0x00A475C0
};
class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char      _pad24[0x48];
    CANavQuadTreeNode* GoalNode;                   // +0x6c
    C2DVector          GoalPosition;               // +0x70
    CANavQuadTreeNode* NodeAskedToGetTo;           // +0x78
    unsigned char      _pad7c[0x10];
    CNavRoute          Route;                      // +0x8c
    unsigned char      _pad90[0x2c];
    bool               HeadingForActionPoint;      // +0xbc
private:
    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const;   // 0x00A1A170
    const CANavQuadTreeNode* GetNodeAt(const C3DVector& pos) const
    {
        return NavigationManager->GetNodeAt(C2DVector(pos), (unsigned char)PInterface->GetNavigationLevelAt(pos));
    }
public:
    virtual bool IsNewDestinationGoingToCauseRestart(const C3DVector& destination, float radius) const;
};

bool CNavigatorAStar::IsNewDestinationGoingToCauseRestart(const C3DVector& destination, float radius) const
{
    if (!Interrupted && OnRoute) {
        const CANavQuadTreeNode* node = GetNodeAt(destination);
        if (node) {
            if (!Route.empty() && !HeadingForActionPoint && !IsEqual(node, NodeAskedToGetTo)) {
                if (!node->Blocked)
                    return node->IsNeighbourOf(NodeAskedToGetTo) ? false : true;
                const CANavQuadTreeNode* nearest = FindNearestNavigableNode(GoalNode, node, destination, GoalNode->GetSize(), NavigationManager);
                return IsEqual(nearest, GoalNode) ? false : true;
            }
            radius = PInterface->PeekNavigatorRadius(false) + radius;
            radius *= radius;
            if (node == NodeAskedToGetTo || (NodeAskedToGetTo && *node == *NodeAskedToGetTo)) {
                C2DVector target(destination);
                float dx = GoalPosition.X - target.X;
                float dy = GoalPosition.Y - target.Y;
                if (!(dx * dx + dy * dy > radius))
                    return false;
            }
        }
    }
    return true;
}
