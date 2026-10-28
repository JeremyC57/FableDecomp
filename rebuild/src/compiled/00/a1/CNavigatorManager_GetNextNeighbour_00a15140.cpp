// CNavigatorManager::GetNextNeighbour(const CANavQuadTreeNode& node, const CANavQuadTreeNode* previous, const C2DVector& position, bool* blocked) -- walk round a quad-tree node: from the previous neighbour (or, with none, from the smallest cell above the node's top-left corner) step to the next smallest-cell position clockwise round the node's edge, look the node up there (the static blocked node, flagged, when there is none), and return null once the walk is back at the start position
// Leaf: ?GetNextNeighbour@CNavigatorManager@@QAEPBVCANavQuadTreeNode@@ABV2@PBV2@ABVC2DVector@@PA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A15140, 731 bytes, thiscall `ret 0x10`. Callers 0x00A15B66, 0x00A15FA9,
//   0x00A1DA50, 0x00A1DD4A.
// Identity: Ego_r.pdb `?GetNextNeighbour@CNavigatorManager@@QAEPBVCANavQuadTreeNode@@ABV2@PBV2@ABVC2DVector@@PA_N@Z`
//   (donor 0x00B82770, directly after GetQuadTreeMemoryUsage 0x00B82730; retail has GetQuadTreeMemoryUsage at 0x00A15100
//   and this row next). The body fits the PDB signature: a node reference, a null-tested previous node, a position compared
//   with the result's Position (0.0001f) and a bool* written 1 / 0. Catalog label "CNavigatorManager_FindValidNavNode" is
//   descriptive only.
// Callees: CANavQuadTreeNode::GetSize() const 0x00A75B30, CNavQuadTree::GetSmallestNodeSize() 0x00A76320 (static
//   __fastcall), CNavigatorManager::GetNodeAt(const C2DVector&, unsigned char) const 0x00A14F70, CNavQuadTree::SetBlockedNode
//   (float, float, long, long) 0x00A76340 (static __fastcall: the two longs in ecx / edx, the floats on the stack);
//   CNavQuadTree::GetBlockedNode() is inline (&CNavQuadTree::BlockedNode 0x013D2D2C). 0.5f 0x0122F59C, 0.0001f 0x0129BA3C.
// Layout: CANavQuadTreeNode (Ego_r.pdb, 16 bytes): vptr, Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe,
//   Blocked +0xf. The generated engine headers carry no member functions, so the classes are modelled here.
// Shape: the four "beyond this edge" cases each either jump to the node's next corner cell (absolute stores) or step
//   along the edge through `testPos += C2DVector(dx, dy)` (the shared x/y add tail); the blocked node is fetched before
//   SetBlockedNode so it lives in edi across the call.
#include <math.h>
#pragma intrinsic(fabs)

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};

class CANavQuadTreeNode { public:
    virtual ~CANavQuadTreeNode();
    C2DVector     Position;        // +0x04
    unsigned char QuadTreeLevel;   // +0x0c
    unsigned char NavLayer;        // +0x0d
    bool          LeafNode;        // +0x0e
    bool          Blocked;         // +0x0f
    float GetSize() const;         // 0x00A75B30
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode { public:
};

class CNavQuadTree { public:
    static CNavBlockedQuadTreeNode BlockedNode;                                     // 0x013D2D2C
    static float __fastcall GetSmallestNodeSize();                                  // 0x00A76320
    static void __fastcall SetBlockedNode(float x, float y, long navLayer, long level);   // 0x00A76340
    static CNavBlockedQuadTreeNode* __fastcall GetBlockedNode() { return &BlockedNode; }
};

class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char navLayer) const;   // 0x00A14F70
    const CANavQuadTreeNode* GetNextNeighbour(const CANavQuadTreeNode& node, const CANavQuadTreeNode* previous,
                                              const C2DVector& position, bool* blocked);
};

const CANavQuadTreeNode* CNavigatorManager::GetNextNeighbour(const CANavQuadTreeNode& node, const CANavQuadTreeNode* previous,
                                                             const C2DVector& position, bool* blocked)
{
    float halfSize = node.GetSize() * 0.5f;
    float halfSmallest = CNavQuadTree::GetSmallestNodeSize() * 0.5f;
    C2DVector nodePos = node.Position;
    C2DVector testPos;
    if (previous == 0) {
        testPos = nodePos;
        testPos.X += halfSmallest - halfSize;
        testPos.Y = testPos.Y + halfSmallest + halfSize;
    } else {
        testPos = previous->Position;
        float prevHalf = previous->GetSize() * 0.5f;
        if (testPos.Y - prevHalf >= nodePos.Y + halfSize) {
            if (prevHalf + testPos.X > nodePos.X + halfSize) {
                testPos.X = nodePos.X + halfSmallest + halfSize;
                testPos.Y = nodePos.Y + halfSize - halfSmallest;
            } else
                testPos += C2DVector(prevHalf + halfSmallest, halfSmallest - prevHalf);
        } else if (testPos.X - prevHalf >= nodePos.X + halfSize) {
            if (testPos.Y - prevHalf < nodePos.Y - halfSize) {
                testPos.X = nodePos.X + halfSize - halfSmallest;
                testPos.Y = nodePos.Y - (halfSmallest + halfSize);
            } else
                testPos += C2DVector(halfSmallest - prevHalf, -(prevHalf + halfSmallest));
        } else if (prevHalf + testPos.Y <= nodePos.Y - halfSize) {
            if (testPos.X - prevHalf < nodePos.X - halfSize) {
                testPos.X = nodePos.X - (halfSmallest + halfSize);
                testPos.Y = nodePos.Y + halfSmallest - halfSize;
            } else
                testPos += C2DVector(-(prevHalf + halfSmallest), prevHalf - halfSmallest);
        } else if (prevHalf + testPos.X <= nodePos.X - halfSize) {
            if (prevHalf + testPos.Y > nodePos.Y + halfSize) {
                testPos.X = nodePos.X + halfSmallest - halfSize;
                testPos.Y = nodePos.Y + halfSmallest + halfSize;
            } else
                testPos += C2DVector(prevHalf - halfSmallest, prevHalf + halfSmallest);
        }
    }
    const CANavQuadTreeNode* next = GetNodeAt(testPos, node.NavLayer);
    if (next == 0) {
        next = CNavQuadTree::GetBlockedNode();
        CNavQuadTree::SetBlockedNode(testPos.X, testPos.Y, node.NavLayer, 6);
        if (blocked)
            *blocked = true;
    } else if (blocked)
        *blocked = false;
    if (previous && fabs(next->Position.X - position.X) <= 0.0001f && fabs(next->Position.Y - position.Y) <= 0.0001f)
        return 0;
    return next;
}