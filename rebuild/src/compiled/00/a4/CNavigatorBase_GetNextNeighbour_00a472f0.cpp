// CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode*, const CANavQuadTreeNode*, const C2DVector&) -- walk clockwise round a node's border: from the previous neighbour (or the top-left corner) step to a point just outside the next stretch of edge and return the nav node there (a shared blocked node when none), or 0 once the walk is back at the start
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A472F0, 720 bytes (`ret 0Ch`).
// Callees: CANavQuadTreeNode::GetSize 0x00A75B30, the smallest node size 0x00A76320 (cdecl, returns 0.5f),
//   CNavigatorManager::GetNodeAt 0x00A14F70, SetBlockedNode 0x00A76340 (__fastcall: layer, level, position);
//   the shared blocked node 0x013D2D2C; 0.5f 0x0122F59C, epsilon 0x0129BA3C (1e-4f).
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
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
};
float GetSmallestNodeSize();                                                 // 0x00A76320
void __fastcall SetBlockedNode(long layer, long level, C2DVector position);   // 0x00A76340
extern CANavQuadTreeNode g_BlockedNode;                                      // 0x013D2D2C
extern const float g_epsilon_0129ba3c;                                       // 0x0129BA3C
class CNavigatorBase : public CNavigatorBaseLayout { public:
    CANavQuadTreeNode* GetNextNeighbour(const CANavQuadTreeNode* node, const CANavQuadTreeNode* prev, const C2DVector& start);
};

CANavQuadTreeNode* CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode* node, const CANavQuadTreeNode* prev, const C2DVector& start)
{
    float halfSize = node->GetSize() * 0.5f;
    float step = GetSmallestNodeSize() * 0.5f;
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
    CANavQuadTreeNode* found = NavigationManager->GetNodeAt(pos, node->NavLayer);
    if (found == 0) {
        found = &g_BlockedNode;
        SetBlockedNode(node->NavLayer, 6, pos);
    }
    if (prev != 0 && fabs(found->Position_X - start.X) <= g_epsilon_0129ba3c &&
        fabs(found->Position_Y - start.Y) <= g_epsilon_0129ba3c)
        return 0;
    return found;
}