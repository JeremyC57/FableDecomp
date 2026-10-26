// CNavigatorAStar::GetNearestNavigablePosTo(const C3DVector&, float, C3DVector&) const -- the position itself when its quad-tree node is open, else the centre of the nearest navigable node within the radius (Z from the external interface); false when there is no node at all or none in reach
// Grade: RELOCATION_MATCH -- the nearest node's position is copied into a named C2DVector local through a user memberwise copy constructor and then handed to SetXY: the copy sends both floats through the FPU (Y loaded first), the plain member read moved X as a dword
// Leaf: ?GetNearestNavigablePosTo@CNavigatorAStar
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1AB40, 164 bytes (+ int3 padding; `ret 0Ch`). Public virtual const (UBE).
// Callees: the external interface's (+0x14) virtual slots 16 GetLayerAt (+0x40) and 4 FindPosZAt (+0x10, float on the x87
//   stack, called with `true`); CNavigatorManager::GetNodeAt(const C2DVector&, bool) 0x00A14F70 on NavigationManager (+0x1c);
//   CNavigatorBase::FindNearestNavigableNode 0x00A475C0 (from LastNavigableNodeVisited +0x88, the blocked node, the position, the
//   radius and the manager).
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, NavigationManager +0x1c}; CNavigatorAStar LastNavigableNodeVisited at PDB
//   +0x90 = retail +0x88 (two STLport containers 4 bytes smaller). The node's Blocked byte is +0x0f, its centre +0x04.
// Shape: the C2DVector conversion temporary is built before the layer query (as the landed IsNavigableAt 0x00A1A210); the
//   open-node copy is the implicit C3DVector assignment; the nearest node's centre goes through an inline two-float setter (Y loaded
//   first, as arguments are evaluated right to left) before the Z call.
class C3DVector { public:
    float X, Y, Z;
    void SetXY(float x, float y) { X = x; Y = y; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
class CANavQuadTreeNode {
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
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
    virtual bool GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const;
};

bool CNavigatorAStar::GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const
{
    CANavQuadTreeNode* node = NavigationManager->GetNodeAt(pos, PInterface->GetLayerAt(pos));
    if (node) {
        if (!node->Blocked) {
            out = pos;
            return true;
        }
        CANavQuadTreeNode* nearest = FindNearestNavigableNode(LastNavigableNodeVisited, node, pos, radius, NavigationManager);
        if (nearest) {
            C2DVector p = nearest->Position;
            out.SetXY(p.X, p.Y);
            out.Z = PInterface->FindPosZAt(out, true);
            return true;
        }
    }
    return false;
}