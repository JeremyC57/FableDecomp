// CNavigatorAStar::IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) -- true when both end points sit on unblocked navigation quad-tree nodes (looked up on their navigation level) and the straight way-point route between them is clear
// Leaf: ?IsNavLineOfSightClear@CNavigatorAStar@@UBE_NABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1AF00, 227 bytes (+ int3 padding; `ret 8`). Public virtual const (UBE), navigator slot 10.
// Names (Ego_r PDB name set): ?IsNavLineOfSightClear@CNavigatorAStar@@UBE_NABVC3DVector@@0@Z; the private inline helper
//   ?GetNodeAt@CNavigatorAStar@@ABEPBVCANavQuadTreeNode@@ABVC3DVector@@@Z (inlined twice here);
//   ?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z @ 0x00A14F70 (const, uchar level -- the landed
//   (const C2DVector&, bool) spellings are wrong, see NAME_FIXES.tsv); ?IsRouteClear@CNavigatorAStar@@ABE_NABVCNavWayPoint@@0_N@Z
//   @ 0x00A1A260 (private const, called with false); the interface's slot 16 (+0x40) is
//   ?GetNavigationLevelAt@CGroundNavigatorExternalInterface@@UBEJABVC3DVector@@@Z (RTTI vtable 0x0125EC9C slot 16 = 0x006CDA00;
//   returns long, narrowed to the uchar level).
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, NavigationManager +0x1c}; CANavQuadTreeNode {Position +4, NavLayer +0xd,
//   Blocked +0xf}; CNavWayPoint (10 bytes) {Position +0, NavLayer +8, Removable +9}.
// Shape: what matched was the PDB signature set -- the const manager GetNodeAt(const C2DVector&, uchar) reached through the
//   private inline CNavigatorAStar::GetNodeAt(const C3DVector&) (the earlier non-const (C2DVector, bool) direct calls scheduled
//   the second `lea ecx` ahead of `push eax`). The start node stays in ebp across the second lookup; each way point is built
//   in place from the 2-float C2DVector constructor, the node's layer byte and `true`.
// Layout headers: the generated engine headers are pulled in under Layout names (the Ego_r layouts carry Dinkumware sizes and no
// virtuals); the classes below are the retail-shaped models.
#define CANavQuadTreeNode CANavQuadTreeNodeLayout
#include "engine/CANavQuadTreeNode.h"
#undef CANavQuadTreeNode
#define CNavigatorManager CNavigatorManagerLayout
#include "engine/CNavigatorManager.h"
#undef CNavigatorManager
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
class C3DVector { public: float X, Y, Z; };
class C2DVector {
public:
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
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetNavigationLevelAt(const C3DVector& position) const;          // slot 16 (+0x40)
};
class CNavWayPoint {                               // 10 bytes
public:
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
private:
    bool IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool b) const;   // 0x00A1A260
    const CANavQuadTreeNode* GetNodeAt(const C3DVector& pos) const
    {
        return NavigationManager->GetNodeAt(C2DVector(pos), (unsigned char)PInterface->GetNavigationLevelAt(pos));
    }
public:
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;
};

bool CNavigatorAStar::IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const
{
    const CANavQuadTreeNode* fromNode = GetNodeAt(from);
    if (!fromNode || fromNode->Blocked)
        return false;
    const CANavQuadTreeNode* toNode = GetNodeAt(to);
    if (!toNode || toNode->Blocked)
        return false;
    CNavWayPoint start(C2DVector(from.X, from.Y), fromNode->NavLayer, true);
    CNavWayPoint end(C2DVector(to.X, to.Y), toNode->NavLayer, true);
    return IsRouteClear(start, end, false);
}