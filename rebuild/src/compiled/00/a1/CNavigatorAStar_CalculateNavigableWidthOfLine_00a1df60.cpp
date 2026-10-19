// CNavigatorAStar::CalculateNavigableWidthOfLine(const C3DVector&, const C3DVector&, float&, float&) -- find the layer of the node under each end (0 when off the map) and measure the line between the two removable way points with the way-point overload
// Grade: RELOCATION_MATCH -- the PDB constructors CNavWayPoint(const C2DVector&, unsigned char, bool) and C2DVector(const C2DVector&) with each way point built from a C2DVector(x, y) temporary (both floats through the FPU, Y loaded first) as NAMED locals inside their own {} block (the block lets the way points share frame words with the dead lookup temporaries: 0x1c frame)
// Leaf: ?CalculateNavigableWidthOfLine@CNavigatorAStar@@UAEXABVC3DVector@@0AAM1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1DF60, 206 bytes (+ int3 padding; `ret 10h`). Public virtual (UAE).
// Callees: the external interface's (+0x14) slot 16 GetLayerAt (+0x40), CNavigatorManager::GetNodeAt 0x00A14F70 (twice),
//   CNavigatorAStar::CalculateNavigableWidthOfLine(const CNavWayPoint&, const CNavWayPoint&, float&, float&, bool) 0x00A1D6E0
//   (called with true).
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, NavigationManager +0x1c}; CANavQuadTreeNode NavLayer +0xd; CNavWayPoint
//   (10 bytes) {Position +0, NavLayer +8, Removable +9}.
// Shape: each layer is a long local zeroed up front and set from the node's byte when there is a node; the way points are
//   built in place from the 2-float constructor (Y loaded first) and the `1` in eax serves both Removable bytes and the
//   final bool argument.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
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
    CNavWayPoint(const C2DVector& p, unsigned char layer, bool removable) : Position(p), NavLayer(layer), Removable(removable) { }
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
    void CalculateNavigableWidthOfLine(const CNavWayPoint& from, const CNavWayPoint& to, float& width1, float& width2, bool b);   // 0x00A1D6E0
    virtual void CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to, float& width1, float& width2);
};

void CNavigatorAStar::CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to, float& width1, float& width2)
{
    long fromLayer = 0;
    long toLayer = 0;
    CANavQuadTreeNode* fromNode = NavigationManager->GetNodeAt(from, PInterface->GetLayerAt(from));
    if (fromNode)
        fromLayer = fromNode->NavLayer;
    CANavQuadTreeNode* toNode = NavigationManager->GetNodeAt(to, PInterface->GetLayerAt(to));
    if (toNode)
        toLayer = toNode->NavLayer;
    {
    CNavWayPoint start(C2DVector(from.X, from.Y), (unsigned char)fromLayer, true);
    CNavWayPoint end(C2DVector(to.X, to.Y), (unsigned char)toLayer, true);
    CalculateNavigableWidthOfLine(start, end, width1, width2, true);
    }
}