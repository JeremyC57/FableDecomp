// CNavigatorAStar::CullWayPoints(long iterations) -- string-pull the route: from CullFront, try CullBack (walking back from the end) until the straight line between them is clear for the navigator's width, then drop the way points in between; at most `iterations` line tests per call, Culled once CullFront reaches the end
// Leaf: ?CullWayPoints@CNavigatorAStar@@AAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1C010, 1773 bytes, thiscall `ret 4`, 8-aligned ebp frame.
// Identity: Ego_r.pdb / FableWin.pdb ?CullWayPoints@CNavigatorAStar@@AAEXJ@Z, the only CNavigatorAStar method taking one long.
//   The single caller 0x00A1F299 passes `push 12Ch ; mov ecx,esi` (a 300-test budget on the navigator); the body walks the
//   Ego_r.pdb members CullFront / CullBack (retail +0x90 / +0x94, the STLport list iterators after the 4-byte Route list +0x8c)
//   and sets Culled (+0xC0). The catalog placeholder AStar_process_node_queue is replaced.
// Callees: CNavigatorAStar::IsRouteClear(const CNavWayPoint&, const CNavWayPoint&, bool) const is expanded (its out-of-line copy
//   0x00A1A260 is the same code with IsEqual 0x00A1A170 and GFGetSign 0x00437160 called); CNavigatorManager::GetNodeAt
//   0x00A14F70, CANavQuadTreeNode slot 8 GetPreferabilityCost (+0x20), ::operator== 0x00A763F0, ::GetSize 0x00A75B30 (out of
//   line in this TU), C2DVector::Normalise 0x00A14540, CNavQuadTree::GetSmallestNodeSize 0x00A76320, _free (list erase).
//   Constants 0.45f 0x0125D1E0, 0.125f 0x0123095C, 0.5f 0x0122F59C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Names: GFGetSign is Ego_r / FableWin ?GFGetSign@@YIJM@Z (fastcall, the float on the stack: `ret 4` at 0x00437160; the landed
//   ?Sign@@YGHM@Z spelling is wrong, NAME_FIXES.tsv); min is the two-evaluation macro (each cost getter is called twice).
// Shape: IsRouteClear with its real body (inline in this TU, every `return` a direct jump into CullWayPoints' cull / step-back
//   paths); the centre line (case 0) is compiled but the line loop starts at 1 (`mov ebx,1 ; test ebx,ebx`, unsigned `jbe`), as
//   retail; the end node of each line reuses to_node (retail keeps it in ebx, the walking node in esi from `xor esi,esi`); the
//   perpendicular is scaled in place (C2DVector::operator*=(float)); the edge-crossing point is stored member-wise; the
//   horizontal flag is declared before the vertical one; ++CullFront is its own statement before the range erase; the advance
//   test keeps `count <= iterations ||` (VC7.1 threads the counted exits past it, 0x00A1C698).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_ERASE_RANGE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORBASE
#include <math.h>
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavQuadTree.h"
#include "engine/C3DVector.h"
#include "engine/CNavigatorBase.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

inline long __fastcall GFGetSign(float value)
{
    return value < 0.0f ? -1 : 1;
}

#pragma pack(push, 1)
class CNavWayPoint {                                   // Ego_r.pdb: C2DVector Position +0, NavLayer +8, Removable +9 (10 bytes)
public:
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
};
#pragma pack(pop)

class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char           States[0xc];                     // +0x24 map<long, CNavigatorAStar::CAStarState>
    CNavBlockedQuadTreeNode BlockedStartNode;                // +0x30
    CNavBlockedQuadTreeNode BlockedEndNode;                  // +0x40
    CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;         // +0x50
    unsigned char           OpenList[0xc];                   // +0x60 CPriorityQueue<CNavigatorAStar::CAStarState*>
    CANavQuadTreeNode*      GoalNode;                        // +0x6c
    C2DVector               GoalPosition;                    // +0x70
    CANavQuadTreeNode*      NodeAskedToGetTo;                // +0x78
    C3DVector               PosAskedToGetTo;                 // +0x7c
    CANavQuadTreeNode*      LastNavigableNodeVisited;        // +0x88
    std::list<CNavWayPoint> Route;                           // +0x8c
    std::list<CNavWayPoint>::iterator CullFront;             // +0x90
    std::list<CNavWayPoint>::iterator CullBack;              // +0x94
    C2DVector               LastWayPointVisited;             // +0x98
    C2DVector               StringLeadPosition;              // +0xa0
    C3DVector               LastKnownNavPos;                 // +0xa8
    float                   LastKnownNavSpeed;               // +0xb4
    long                    LastKnownNavLayer;               // +0xb8
    bool                    HeadingForActionPoint;           // +0xbc
    bool                    Initialised;                     // +0xbd
    bool                    GotRoute;                        // +0xbe
    bool                    SlowedDownForLastPoint;          // +0xbf
    bool                    Culled;                          // +0xc0
    bool                    PartWayThroughAStar;             // +0xc1
    unsigned char           NoOfClockwiseTurns;              // +0xc2
    unsigned char           NoOfAntiClockwiseTurns;          // +0xc3

    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const
    {
        if (a == b)
            return true;
        if (!a || !b)
            return false;
        return *a == *b;
    }
private:
    bool IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool same_layer) const;
    void CullWayPoints(long iterations);
};

inline bool CNavigatorAStar::IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool same_layer) const
{
    if (same_layer && from.NavLayer != to.NavLayer)
        return false;
    const CANavQuadTreeNode* from_node = NavigationManager->GetNodeAt(from.Position, from.NavLayer);
    const CANavQuadTreeNode* to_node = NavigationManager->GetNodeAt(to.Position, to.NavLayer);
    if (!from_node || !to_node || from_node->Blocked || to_node->Blocked)
        return false;
    long max_cost = min(from_node->GetPreferabilityCost(), to_node->GetPreferabilityCost());
    if (IsEqual(from_node, to_node))
        return true;
    C2DVector direction = to.Position - from.Position;
    if (!direction.IsNonZero())
        return true;
    direction.Normalise();
    bool horizontal = false;
    bool vertical = false;
    if (fabs(direction.X) < DELTA_NOTIONAL_ZERO || fabs(fabs(direction.Y) - 1.0f) < DELTA_NOTIONAL_ZERO)
        vertical = true;
    if (fabs(direction.Y) < DELTA_NOTIONAL_ZERO || fabs(fabs(direction.X) - 1.0f) < DELTA_NOTIONAL_ZERO)
        horizontal = true;
    C2DVector offset(direction.Y, -direction.X);
    offset *= CNavQuadTree::GetSmallestNodeSize() * 0.45f;
    C2DVector left_from = from.Position + offset;
    C2DVector left_to = to.Position + offset;
    C2DVector right_from = from.Position - offset;
    C2DVector right_to = to.Position - offset;
    const CANavQuadTreeNode* node = NULL;
    long step_x = GFGetSign(direction.X);
    long step_y = GFGetSign(direction.Y);
    for (unsigned long line = 1; line <= 2; ++line) {
        C2DVector start;
        if (line == 0) {
            node = NavigationManager->GetNodeAt(from.Position, from.NavLayer);
            to_node = NavigationManager->GetNodeAt(to.Position, from.NavLayer);
            if (!node || !to_node || node->Blocked || to_node->Blocked ||
                node->GetPreferabilityCost() > max_cost || to_node->GetPreferabilityCost() > max_cost)
                return false;
            start = from.Position;
        } else if (line == 1) {
            node = NavigationManager->GetNodeAt(left_from, from.NavLayer);
            to_node = NavigationManager->GetNodeAt(left_to, from.NavLayer);
            if (!node || !to_node || node->Blocked || to_node->Blocked ||
                node->GetPreferabilityCost() > max_cost || to_node->GetPreferabilityCost() > max_cost)
                return false;
            start = left_from;
        } else if (line == 2) {
            node = NavigationManager->GetNodeAt(right_from, from.NavLayer);
            to_node = NavigationManager->GetNodeAt(right_to, from.NavLayer);
            if (!node || !to_node || node->Blocked || to_node->Blocked ||
                node->GetPreferabilityCost() > max_cost || to_node->GetPreferabilityCost() > max_cost)
                return false;
            start = right_from;
        }
        while (!IsEqual(node, to_node)) {
            if (!node || node->Blocked || node->GetPreferabilityCost() > max_cost)
                return false;
            float edge_x = node->GetSize() * step_x * 0.5f + node->Position.X;
            float edge_y = node->GetSize() * step_y * 0.5f + node->Position.Y;
            C2DVector next;
            if (!vertical) {
                next.X = edge_x;
                next.Y = (edge_x - start.X) / direction.X * direction.Y + start.Y;
            }
            if (!horizontal) {
                C2DVector other((edge_y - start.Y) / direction.Y * direction.X + start.X, edge_y);
                if (vertical || (other - start).GetSquaredMagnitude() < (next - start).GetSquaredMagnitude())
                    next = other;
            }
            next += direction * CNavQuadTree::GetSmallestNodeSize() * 0.125f;
            const CANavQuadTreeNode* previous = node;
            node = NavigationManager->GetNodeAt(next, from.NavLayer);
            if (IsEqual(node, previous))
                return false;
        }
    }
    return true;
}

void CNavigatorAStar::CullWayPoints(long iterations)
{
    long count = 0;
    while (count <= iterations && CullFront != Route.end()) {
        bool culled = false;
        while (count <= iterations && CullFront != CullBack && !culled) {
            ++count;
            if (IsRouteClear(*CullFront, *CullBack, true)) {
                ++CullFront;
                CullFront = Route.erase(CullFront, CullBack);
                --CullFront;
                culled = true;
            } else {
                --CullBack;
            }
        }
        if (count <= iterations || CullFront == CullBack || culled) {
            ++CullFront;
            CullBack = Route.end();
            --CullBack;
        }
    }
    if (CullFront == Route.end())
        Culled = true;
}
