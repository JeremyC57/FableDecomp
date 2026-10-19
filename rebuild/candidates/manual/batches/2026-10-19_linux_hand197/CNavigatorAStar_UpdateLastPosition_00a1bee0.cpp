// CNavigatorAStar::UpdateLastPosition(const C3DVector&) -- keep LastNavigableNodeVisited (and its layer) current: nothing to do while the last node still covers the position; otherwise take the open node under it, or -- when there is no last node yet -- the nearest navigable node within four cells of the blocked one
// Leaf: UpdateLastPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1BEE0, 220 bytes (+ int3 padding; `ret 4`).
// Callees: CANavQuadTreeNode::GetAreaCovering() 0x00A763B0 (C2DBoxF by hidden pointer) and C2DBoxF::ContainsPoint(const C2DVector&)
//   0x00535CD0 (a bool in al), the external interface's (+0x14) slot 16 GetLayerAt (+0x40), CNavigatorManager::GetNodeAt
//   0x00A14F70, CANavQuadTreeNode::GetSize() 0x00A75B30 (float), CNavigatorBase::FindNearestNavigableNode 0x00A475C0; 4.0f
//   0x0123088C.
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, Interrupted +0x19, NavigationManager +0x1c}; CNavigatorAStar with STLport
//   containers: LastNavigableNodeVisited +0x88 (PDB +0x90), LastKnownNavLayer +0xb8 (PDB +0xc8).
// Shape: the outer call's argument (the C2DVector conversion temporary, the manager) is pushed before the inner call that
//   feeds it (GetAreaCovering, GetSize), as VC does for a call used as `this` / an argument; the open-node update returns
//   through its own epilogue.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
struct C2DBoxF {
    float MinX, MinY, MaxX, MaxY;
    C2DBoxF(float minX, float minY, float maxX, float maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
    bool ContainsPoint(const C2DVector& p) const;  // 0x00535CD0
};
class CANavQuadTreeNode {
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    C2DBoxF GetAreaCovering() const;               // 0x00A763B0
    float GetSize() const;                         // 0x00A75B30 (cell size of the quad-tree level)
};
class CNavigatorManager {
public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual bool GetLayerAt(const C3DVector& position) const;                    // slot 16 (+0x40)
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
    CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* blocked, const C3DVector& pos, float radius, CNavigatorManager* manager) const;   // 0x00A475C0
};
class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char      _pad24[0x64];
    CANavQuadTreeNode* LastNavigableNodeVisited;   // +0x88
    unsigned char      _pad8c[0x2c];
    long               LastKnownNavLayer;          // +0xb8
    void UpdateLastPosition(const C3DVector& pos);
};

void CNavigatorAStar::UpdateLastPosition(const C3DVector& pos)
{
    if (!Interrupted && LastNavigableNodeVisited && LastNavigableNodeVisited->GetAreaCovering().ContainsPoint(pos))
        return;
    CANavQuadTreeNode* node = NavigationManager->GetNodeAt(pos, PInterface->GetLayerAt(pos));
    if (node && node != LastNavigableNodeVisited) {
        if (!node->Blocked) {
            LastNavigableNodeVisited = node;
            LastKnownNavLayer = node->NavLayer;
            return;
        }
        if (!LastNavigableNodeVisited) {
            CANavQuadTreeNode* nearest = FindNearestNavigableNode(node, node, pos, node->GetSize() * 4.0f, NavigationManager);
            if (nearest) {
                LastNavigableNodeVisited = nearest;
                LastKnownNavLayer = nearest->NavLayer;
            }
        }
    }
}
