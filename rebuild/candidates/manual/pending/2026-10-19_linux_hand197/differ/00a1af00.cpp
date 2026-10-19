// CNavigatorAStar::IsNavLineOfSightClear(const C3DVector&, const C3DVector&) const -- both ends must sit on open quad-tree nodes; then the straight route between them (each end a removable way point on its node's layer) is tested with IsRouteClear
// Leaf: ?IsNavLineOfSightClear@CNavigatorAStar
// Grade: DIFFER(223v227) -- way-point build: retail copies each end's X/Y through the FPU (fld X ; fld Y ; fxch ; fstp) into two 12-byte way points beside two 8-byte conversion temporaries (0x28 frame, this in edi); the lane copies with mov and shares a temporary slot (0x20 frame, this in esi) (tried C2DVector temporaries, a float-pair CNavWayPoint ctor, flat early returns)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1AF00, 227 bytes (+ int3 padding; `ret 8`). Public virtual const (UBE), navigator slot 10.
// Callees: the external interface's (+0x14) virtual slot 16 GetLayerAt (+0x40); CNavigatorManager::GetNodeAt(const C2DVector&,
//   bool) 0x00A14F70 on NavigationManager (+0x1c); CNavigatorAStar::IsRouteClear(const CNavWayPoint&, const CNavWayPoint&, bool)
//   0x00A1A260 (called with false, its bool returned).
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, NavigationManager +0x1c}; CANavQuadTreeNode {Position +4, NavLayer +0xd,
//   Blocked +0xf}; CNavWayPoint (10 bytes) {Position +0, NavLayer +8, Removable +9}.
// Shape: the start node is a named pointer kept in ebp across the second lookup; each way point is built in place from the
//   2-float C2DVector constructor (fld X ; fld Y ; fxch), the node's layer byte and `true`; the two refusals keep their own
//   epilogues (the second also pops esi).
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
    C2DVector(float x, float y) : X(x), Y(y) { }
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
struct CNavWayPoint {                              // 10 bytes
    C2DVector     Position;                        // +0x00
    unsigned char NavLayer;                        // +0x08
    bool          Removable;                       // +0x09
    CNavWayPoint(float x, float y, unsigned char layer, bool removable) : Position(x, y), NavLayer(layer), Removable(removable) { }
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
};
class CNavigatorAStar : public CNavigatorBase {
public:
    bool IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool b) const;   // 0x00A1A260
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;
};

bool CNavigatorAStar::IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const
{
    CANavQuadTreeNode* fromNode = NavigationManager->GetNodeAt(from, PInterface->GetLayerAt(from));
    if (!fromNode || fromNode->Blocked)
        return false;
    CANavQuadTreeNode* toNode = NavigationManager->GetNodeAt(to, PInterface->GetLayerAt(to));
    if (!toNode || toNode->Blocked)
        return false;
    return IsRouteClear(CNavWayPoint(from.X, from.Y, fromNode->NavLayer, true),
                        CNavWayPoint(to.X, to.Y, toNode->NavLayer, true), false);
}
