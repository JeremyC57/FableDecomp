// CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode*, const CANavQuadTreeNode*, const C2DVector&) -- walk clockwise round a node's border: from the previous neighbour (or the top-left corner) step to a point just outside the next stretch of edge and return the nav node there (a shared blocked node when none), or 0 once the walk is back at the start
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A472F0, 720 bytes (`ret 0Ch`).
// Callees: CANavQuadTreeNode::GetSize 0x00A75B30, CNavQuadTree::GetSmallestNodeSize 0x00A76320 (static __fastcall, returns 0.5f),
//   CNavigatorManager::GetNodeAt 0x00A14F70, CNavQuadTree::SetBlockedNode 0x00A76340 (static __fastcall: x, y, layer, level;
//   Ego_r, as 0x00A15140 spells it; was a free (long, long, C2DVector) stand-in, landing 2026-11-03g);
//   the shared blocked node 0x013D2D2C (the static CNavQuadTree::BlockedNode, Ego_r.pdb dynamic initializer; was g_BlockedNode,
//   landing 2026-11-03g); 0.5f 0x0122F59C, epsilon 0x0129BA3C (1e-4f).
#include <math.h>
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
#define CANavQuadTreeNode CANavQuadTreeNodeLayout
#include "engine/CANavQuadTreeNode.h"
#undef CANavQuadTreeNode
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
class CANavQuadTreeNode : public CANavQuadTreeNodeLayout { public:
    const C2DVector& GetPosition() const { return *(const C2DVector*)&Position_X; }
    float GetSize() const;                                                   // 0x00A75B30
};
class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70 (Ego_r.pdb, const)
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode { };              // RTTI .?AVCNavBlockedQuadTreeNode@@ : CANavQuadTreeNode
class CNavQuadTree { public:
    static float __fastcall GetSmallestNodeSize();                           // 0x00A76320 (Ego_r ?GetSmallestNodeSize@CNavQuadTree@@SIMXZ)
    static void __fastcall SetBlockedNode(float x, float y, long navLayer, long level);   // 0x00A76340 (Ego_r ?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z)
    static CNavBlockedQuadTreeNode BlockedNode;                              // 0x013D2D2C ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A
};
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C
class CNavigatorBase : public CNavigatorBaseLayout { public:
    CANavQuadTreeNode* GetNextNeighbour(const CANavQuadTreeNode* node, const CANavQuadTreeNode* prev, const C2DVector& start);
};

CANavQuadTreeNode* CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode* node, const CANavQuadTreeNode* prev, const C2DVector& start)
{
    float halfSize = node->GetSize() * 0.5f;
    float step = CNavQuadTree::GetSmallestNodeSize() * 0.5f;
    C2DVector centre = node->GetPosition();
    C2DVector pos;
    if (prev == 0) {
        pos = centre;
        pos.X += step - halfSize;
        pos.Y = pos.Y + step + halfSize;
    } else {
        pos = prev->GetPosition();
        float prevHalf = prev->GetSize() * 0.5f;
        if (pos.Y - prevHalf >= centre.Y + halfSize) {
            if (prevHalf + pos.X > centre.X + halfSize) {
                pos.X = centre.X + step + halfSize;
                pos.Y = centre.Y + halfSize - step;
            } else {
                pos += C2DVector(prevHalf + step, step - prevHalf);
            }
        } else if (pos.X - prevHalf >= centre.X + halfSize) {
            if (pos.Y - prevHalf < centre.Y - halfSize) {
                pos.X = centre.X + halfSize - step;
                pos.Y = centre.Y - (step + halfSize);
            } else {
                pos += C2DVector(step - prevHalf, -(prevHalf + step));
            }
        } else if (prevHalf + pos.Y <= centre.Y - halfSize) {
            if (pos.X - prevHalf < centre.X - halfSize) {
                pos.X = centre.X - (step + halfSize);
                pos.Y = centre.Y + step - halfSize;
            } else {
                pos += C2DVector(-(prevHalf + step), prevHalf - step);
            }
        } else if (prevHalf + pos.X <= centre.X - halfSize) {
            if (prevHalf + pos.Y > centre.Y + halfSize) {
                pos.X = centre.X + step - halfSize;
                pos.Y = centre.Y + step + halfSize;
            } else {
                pos += C2DVector(prevHalf - step, prevHalf + step);
            }
        }
    }
    CANavQuadTreeNode* found = (CANavQuadTreeNode*)NavigationManager->GetNodeAt(pos, node->NavLayer);
    if (found == 0) {
        found = &CNavQuadTree::BlockedNode;
        CNavQuadTree::SetBlockedNode(pos.X, pos.Y, node->NavLayer, 6);
    }
    if (prev != 0 && fabs(found->Position_X - start.X) <= DELTA_NOTIONAL_ZERO &&
        fabs(found->Position_Y - start.Y) <= DELTA_NOTIONAL_ZERO)
        return 0;
    return found;
}