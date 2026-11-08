// Grade: DIFFER(5423v5437) -- 15 hunks / 115 diff lines; call profile identical to retail (every callee and inline expansion: IsRouteClear expanded x2 + called x1, IsEqual inline once, IsNonZero expanded everywhere); the residue is one temp shape (line-of-sight block), x87 stack choices in the two IsRouteClear expansions, and the eax/ecx/edx rotation that follows them
// Residue: (1) line-of-sight block 0x00A1EFF0: retail copies C2DVector(position) as dwords through a frame temp ([esp+40h] then the way point at [esp+94h]) and loads destination X-first + fxch; all 6 spellings tried give Y-first fld/fstp with no temp; (2) x87 stack in the IsRouteClear expansions: 0x00A1ED18 retail `fadd [mem]` vs ours `fld ; fadd st(1) ; fstp st(0)`, 0x00A1F65A/0x00A1F9D1 retail keeps the GetSmallestNodeSize scale live (`fld ; fmul st(2)`, two `fstp st(0)`) where ours consumes it (`fxch ; fmul [mem]`); (3) integer colouring: recalc block 0x00A1F099..0x00A1F180 (eax/ecx/edx rotated, FindNearestNavigableNode pushes), front GetNodeAt 0x00A1F073 (`mov ecx,eax ; mov eax,[ecx]`), MoveTowardNextWayPoint 0x00A1FAAD (eax/ecx swap); (4) alignment pads 0x00A1F2F9/0x00A1F6EA follow the 14-byte size gap. Probe quantity: c2prio.py on this source -- startNode (id 30, prio 60) is split eax+memory, toNode/fromNode (prio 37/18) memory, nearest (44) split eax; the recalc-block rotation follows those splits, which every declaration order / statement order tried leaves unchanged (F_RECALC 0/1/2, F_START 0/1/2 identical there).
// Next: (a) LOS block: a CNavWayPoint ctor or C2DVector conversion that keeps a dword-copied temp (retail [esp+40h] is the slot shared with the IsActionPoint argument temp and goalPosition) -- try a by-value C2DVector parameter on a helper, or the position way point built through a named C2DVector that is also used later; (b) x87: run c2prio-like tracing on the x87 stackifier for the two IsRouteClear expansions (scale kept live in retail); (c) then re-bisect the integer colouring with c2prio (--blocks) on the recalc block.
// CNavigatorAStar::CalculateAndMoveToPosProximity(const C3DVector& position, const C3DVector& destination, float proximity, float speed, C3DVector& movement, bool lineOfSight, bool flag2, bool flag3, bool flag4, bool flag5, bool flag6, bool flag7, float timeLimit) -- route the navigator toward a destination: extend or keep the current route when the goal only moved a little, otherwise re-plan with A* (nearest navigable node for a blocked goal, region connectivity), cull and walk the way points, and steer toward the next one
// Leaf: ?CalculateAndMoveToPosProximity@CNavigatorAStar@@AAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N222222M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E5C0, 5437 bytes, thiscall `ret 34h`, 8-aligned ebp frame.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ASSIGN3D
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_C2DLINEF_SQUAREDDIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_CNAVIGATORMANAGER_ISACTIONPOINT
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVREGION_ISCONNECTED
#define FABLE_RETAIL_CNAVIGATORBASE
#include <math.h>
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DLineF.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavQuadTree.h"
#include "engine/CNavRegion.h"
#include "engine/C3DVector.h"
#include "engine/CNavigatorBase.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

enum ENavigatorReturn {                                // Ego_r enum; enumerator names descriptive (values from this row)
    NAVIGATOR_ARRIVED = 0,
    NAVIGATOR_FAILED = 1,
    NAVIGATOR_MOVING = 2
};

inline long __fastcall GFGetSign(float value)
{
    return value < 0.0f ? -1 : 1;
}

class CSteeringAgent;
class CNavigatorExternalInterface {                    // vtable order from CGroundNavigatorExternalInterface 0x0125EC9C
public:
    virtual bool IsMovementBlockedAt(const C3DVector& from, const C3DVector& to, bool& blocked, bool& dynamic) const;  // 0
    virtual float GetTerrainDesirabilityAt(const C3DVector& pos) const;                    // 1
    virtual float GetTerrainHeightAt(const C3DVector& pos) const;                          // 2
    virtual float GetThemeHeightAt(const C3DVector& pos) const;                            // 3
    virtual float FindPosZAt(const C3DVector& pos, bool b) const;                          // 4
    virtual float FindPosZAtIncludingThemes(const C3DVector& pos, bool b) const;           // 5
    virtual float PeekNavigatorRadius(bool physical) const;                                // 6
    virtual C2DVector PeekNavigatorOrientation() const;                                    // 7
    virtual C2DVector PeekNavigatorPosition2D() const;                                     // 8
    virtual C3DVector PeekNavigatorPosition3D() const;                                     // 9
    virtual float PeekNavigatorMaxSpeed(const C3DVector& pos) const;                       // 10
    virtual const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& GetNavigatorObstacles() const;  // 12
    virtual std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& GetNavigatorObstacles();             // 11
    virtual std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >* GetPNavigatorObstacles();            // 13
    virtual const CSteeringAgent* GetPThingToNotAvoid() const;                             // 14
    virtual float GetProximityForDirectApproach() const;                                   // 15
    virtual long GetNavigationLevelAt(const C3DVector& pos) const;                         // 16
};

class CSteeringAgent {                                 // Ego_r.pdb layout with retail's one-word STLport lists
public:
    CNavigatorExternalInterface* NavigatorInterface;          // +0x00
    bool                         DummyObstacle;               // +0x04
    float                        RepulsionMultiplier;         // +0x08
    void*                        SteeringBehaviours;          // +0x0C list<CSB*>
    void*                        SteeringBehaviourInputs;     // +0x10 list<CSBInput*>
    void*                        SteeringBehaviourConstraints;// +0x14 list<CSBConstraint*>
    float                        CurrentMaxSpeed;             // +0x18
    float                        CurrentSpeed;                // +0x1C
    C2DVector                    Goal;                        // +0x20
    C2DVector                    NextGoal;                    // +0x28
    int                          TimeToReacquireGoal;         // +0x30
    float                        DistCutoff;                  // +0x34
    C2DVector                    Steer;                       // +0x38
    float                        BestDistLeft;                // +0x40
    float                        BestDistRight;               // +0x44
    float                        BestNextDistLeft;            // +0x48
    float                        BestNextDistRight;           // +0x4C
    float                        ProximityForArrival;         // +0x50
    int                          TimeSinceBestApproach;       // +0x54
    float                        BestApproach;                // +0x58
    bool                         Wandering;                   // +0x5C
    bool                         MovingForwards;              // +0x5D
    bool                         RecalcWidth;                 // +0x5E
    bool                         AddWaypoint;                 // +0x5F
    bool                         AvoidDynamicObstacles;       // +0x60
    bool                         AvoidDynamicObstaclesUntilNextWaypoint;   // +0x61
    bool                         HasWanderedDuringThisSection;// +0x62
    bool                         FailPath;                    // +0x63
    bool                         OnDirectApproach;            // +0x64
    void SetGoal(const C2DVector& goal, const C2DVector& nextGoal);                        // 0x00A20970
    bool IsCurrentNavigabilityBetter();                                                    // 0x00A1A1A0
};

// The retail body of 0x00A1A1A0 (landed row), compiled ahead of its caller in this TU: VC7.1 then knows it leaves edx alone
// and keeps the front route node in edx across the call (0x00A1F4A8 .. 0x00A1F4BB).
#pragma auto_inline(off)
bool CSteeringAgent::IsCurrentNavigabilityBetter()
{
    return BestDistLeft > BestNextDistLeft || BestDistRight > BestNextDistRight;
}
#pragma auto_inline(on)

#pragma pack(push, 1)
class CNavWayPoint {                                   // Ego_r.pdb: C2DVector Position +0, NavLayer +8, Removable +9 (10 bytes)
public:
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
    CNavWayPoint(const C2DVector& position, unsigned char navLayer, bool removable)
        : Position(position), NavLayer(navLayer), Removable(removable) { }
};
#pragma pack(pop)

class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char           States[0xc];                     // +0x24 map<long, CNavigatorAStar::CAStarState>
    CNavBlockedQuadTreeNode BlockedStartNode;                // +0x30
    CNavBlockedQuadTreeNode BlockedEndNode;                  // +0x40
    CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;         // +0x50
    unsigned char           OpenList[0xc];                   // +0x60 CPriorityQueue<CNavigatorAStar::CAStarState*>
    const CANavQuadTreeNode* GoalNode;                       // +0x6c
    C2DVector               GoalPosition;                    // +0x70
    const CANavQuadTreeNode* NodeAskedToGetTo;               // +0x78
    C3DVector               PosAskedToGetTo;                 // +0x7c
    const CANavQuadTreeNode* LastNavigableNodeVisited;       // +0x88
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

    virtual void UpdateLastPosition(const C3DVector& position);                            // slot 13 (+0x34)
    void CalcWayPointWidths(const C3DVector& position);                                    // 0x00A1DE30
    void UpdateGoal(const C3DVector& position, C2DVector oldGoal, C2DVector newGoal);      // 0x00A1E030
private:
    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const;            // 0x00A1A170
    bool IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool same_layer) const;   // 0x00A1A260
    const CANavQuadTreeNode* GetNodeAt(const C3DVector& pos) const
    {
        return NavigationManager->GetNodeAt(C2DVector(pos), (unsigned char)PInterface->GetNavigationLevelAt(pos));
    }
    void Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* goalNode, const C2DVector& startPos,
                    const C2DVector& goalPos, const CANavQuadTreeNode* askedNode, const C3DVector& askedPos, bool b1, bool b2,
                    float timeLimit);                                                      // 0x00A1CEB0
    bool CalculateRoute(bool b);                                                           // 0x00A1D290
    void CullWayPoints(long iterations);                                                   // 0x00A1C010
    void MoveTowardNextWayPoint(const C3DVector& position, float proximity, float speed, C3DVector& movement, bool b1,
                                bool b2, bool b3, bool b4);                                // 0x00A1E110
    ENavigatorReturn CalculateAndMoveToPosProximity(const C3DVector& position, const C3DVector& destination, float proximity,
                                                    float speed, C3DVector& movement, bool lineOfSight, bool flag2, bool flag3,
                                                    bool flag4, bool flag5, bool flag6, bool flag7, float timeLimit);
};

bool CNavigatorAStar::IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const
{
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    return *a == *b;
}

bool CNavigatorAStar::IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool same_layer) const
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

ENavigatorReturn CNavigatorAStar::CalculateAndMoveToPosProximity(const C3DVector& position, const C3DVector& destination,
                                                                  float proximity, float speed, C3DVector& movement,
                                                                  bool lineOfSight, bool flag2, bool flag3, bool flag4,
                                                                  bool flag5, bool flag6, bool flag7, float timeLimit)
{
    if (Interrupted) {
        LastNavigableNodeVisited = NULL;
        UpdateLastPosition(position);
    }
    CSteeringAgent* agent = SteeringAgent;
    if (agent->FailPath) {
        agent->TimeSinceBestApproach = 0;
        agent->HasWanderedDuringThisSection = false;
        agent->AvoidDynamicObstaclesUntilNextWaypoint = true;
        agent->FailPath = false;
        Interrupted = true;
    }
    LastKnownNavPos = position;
    LastKnownNavSpeed = speed;
    float arrivalDistance = PInterface->PeekNavigatorRadius(false);
    arrivalDistance += proximity;
    movement.Initialise(0.0f, 0.0f, 0.0f);
    float arrivalDistanceSquared = arrivalDistance * arrivalDistance;
    float distanceSquared = (destination - position).GetSquaredMagnitude();
    HeadingForActionPoint = false;
    if (proximity == DELTA_NOTIONAL_ZERO) {
        HeadingForActionPoint = NavigationManager->IsActionPoint(C2DVector(destination));
        arrivalDistanceSquared = DELTA_NOTIONAL_ZERO;
    }
    if (!lineOfSight && distanceSquared <= arrivalDistanceSquared) {
        OnRoute = false;
        return NAVIGATOR_ARRIVED;
    }

    const CANavQuadTreeNode* destinationNode = GetNodeAt(destination);
    if (!destinationNode) {
        OnRoute = false;
        return NAVIGATOR_FAILED;
    }
    if (destinationNode->GetNodeType() == NAV_NODE_BLOCKED) {
        BlockedEndNode = *static_cast<const CNavBlockedQuadTreeNode*>(destinationNode);
        destinationNode = &BlockedEndNode;
    }

    bool recalculate = false;
    if (Interrupted || !OnRoute) {
        recalculate = true;
    } else if (!Route.empty() && !HeadingForActionPoint) {
        if (!IsEqual(destinationNode, NodeAskedToGetTo)) {
            if (!destinationNode->Blocked) {
                if (destinationNode->IsNeighbourOf(NodeAskedToGetTo)) {
                    CNavWayPoint goal(C2DVector(destination.X, destination.Y), destinationNode->NavLayer, true);
                    if (!IsRouteClear(Route.back(), goal, true))
                        Route.push_back(CNavWayPoint(destinationNode->Position, destinationNode->NavLayer, true));
                    Route.push_back(goal);
                    NodeAskedToGetTo = destinationNode;
                    GoalPosition = destination;
                    PosAskedToGetTo = destination;
                } else {
                    recalculate = true;
                }
            } else {
                const CANavQuadTreeNode* nearest = FindNearestNavigableNode(GoalNode, destinationNode, destination,
                                                                            GoalNode->GetSize(), NavigationManager);
                if (IsEqual(nearest, GoalNode)) {
                    Route.push_back(CNavWayPoint(C2DVector(destination.X, destination.Y), GoalNode->NavLayer, true));
                    if (destinationNode->GetNodeType() == NAV_NODE_BLOCKED) {
                        BlockedNodeAskedToGetTo = *static_cast<const CNavBlockedQuadTreeNode*>(destinationNode);
                        NodeAskedToGetTo = &BlockedNodeAskedToGetTo;
                    } else {
                        NodeAskedToGetTo = destinationNode;
                    }
                    GoalPosition = destination;
                    PosAskedToGetTo = destination;
                } else {
                    recalculate = true;
                }
            }
        } else if (!destinationNode->Blocked) {
            C2DVector newGoal(destination);
            CNavWayPoint& last = Route.back();
            if (!(last.Position == newGoal)) {
                C2DVector oldGoal = last.Position;
                last.Position = newGoal;
                GoalPosition = newGoal;
                if (Route.size() == 1)
                    UpdateGoal(position, oldGoal, newGoal);
            }
        }
    }
    if (!recalculate)
        recalculate = !(IsEqual(destinationNode, NodeAskedToGetTo) &&
                        (GoalPosition - C2DVector(destination.X, destination.Y)).GetSquaredMagnitude() <= arrivalDistanceSquared);

    const CANavQuadTreeNode* startNode = GetNodeAt(position);
    if (!startNode) {
        OnRoute = false;
        return NAVIGATOR_FAILED;
    }
    if (startNode->GetNodeType() == NAV_NODE_BLOCKED) {
        BlockedStartNode = *static_cast<const CNavBlockedQuadTreeNode*>(startNode);
        startNode = &BlockedStartNode;
    }
    if (lineOfSight && !destinationNode->Blocked && proximity * proximity >= distanceSquared) {
        if (IsRouteClear(CNavWayPoint(C2DVector(position), startNode->NavLayer, true),
                         CNavWayPoint(C2DVector(destination), destinationNode->NavLayer, true), false)) {
            OnRoute = false;
            return NAVIGATOR_ARRIVED;
        }
    }

    if (!recalculate && !Route.empty()) {
        const CNavWayPoint& first = Route.front();
        recalculate = NavigationManager->GetNodeAt(first.Position, first.NavLayer) == NULL;
    }
    if (recalculate) {
        NodeAskedToGetTo = NULL;
        Interrupted = false;
        const CANavQuadTreeNode* fromNode = startNode;
        const CANavQuadTreeNode* toNode = destinationNode;
        C2DVector goalPosition(destination);
        if (flag3) {
            if (destinationNode->Blocked) {
                if (!HeadingForActionPoint && proximity <= DELTA_NOTIONAL_ZERO) {
                    OnRoute = false;
                    return NAVIGATOR_FAILED;
                }
                const CANavQuadTreeNode* nearest = FindNearestNavigableNode(startNode, destinationNode, destination, proximity,
                                                                            NavigationManager);
                if (!nearest) {
                    OnRoute = false;
                    return NAVIGATOR_FAILED;
                }
                if (HeadingForActionPoint) {
                    toNode = nearest;
                } else {
                    if (!IsAnyPartOfNodeWithinDistOfPos(nearest, PInterface->PeekNavigatorRadius(false) + proximity, destination,
                                                        goalPosition)) {
                        OnRoute = false;
                        return NAVIGATOR_FAILED;
                    }
                    toNode = nearest;
                }
            }
            if (startNode->Blocked) {
                if (!LastNavigableNodeVisited) {
                    OnRoute = false;
                    return NAVIGATOR_FAILED;
                }
                fromNode = LastNavigableNodeVisited;
            }
            if (!fromNode->GetRegion()->IsConnectedToRegion(*toNode->GetRegion())) {
                OnRoute = false;
                return NAVIGATOR_FAILED;
            }
        }
        Initialise(fromNode, toNode, C2DVector(position), goalPosition, destinationNode, destination, flag3, flag6, timeLimit);
    }

    if (PartWayThroughAStar) {
        PartWayThroughAStar = false;
        CalculateRoute(flag7);
        if (PartWayThroughAStar)
            return NAVIGATOR_MOVING;
        if (!GotRoute) {
            OnRoute = false;
            return NAVIGATOR_FAILED;
        }
        Route.push_front(CNavWayPoint(C2DVector(position.X, position.Y), Route.front().NavLayer, true));
        Route.push_back(CNavWayPoint(GoalPosition, destinationNode->NavLayer, true));
        CullFront = Route.begin();
        CullBack = Route.end();
        --CullBack;
    } else if (!GotRoute) {
        OnRoute = false;
        return NAVIGATOR_FAILED;
    }

    if (!Culled) {
        CullWayPoints(300);
        if (!Culled)
            return NAVIGATOR_MOVING;
        SteeringAgent->SetGoal(Route.front().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }
    while (Route.size() > 1) {
        C2DLineF leg(Route.front().Position, (*++Route.begin()).Position);
        if (SteeringAgent->ProximityForArrival == 0.0f)
            SteeringAgent->ProximityForArrival = speed;
        C2DVector here(position);
        float arrival = SteeringAgent->ProximityForArrival;
        if (leg.GetSquaredDist(here) > arrival * arrival)
            break;
        LastWayPointVisited = Route.front().Position;
        Route.erase(Route.begin());
        CalcWayPointWidths(position);
        SteeringAgent->SetGoal(Route.front().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }

    if (Route.empty() || (!lineOfSight && Route.size() == 1 &&
        (Route.front().Position - C2DVector(position.X, position.Y)).GetSquaredMagnitude() <= DELTA_NOTIONAL_ZERO)) {
        OnRoute = false;
        return NAVIGATOR_ARRIVED;
    }
    if (Route.size() > 1 && Route.front().Removable && !SteeringAgent->IsCurrentNavigabilityBetter()) {
        std::list<CNavWayPoint>::iterator it = Route.begin();
        std::list<CNavWayPoint>::iterator next = it; ++next;
        if (IsRouteClear(CNavWayPoint(C2DVector(position.X, position.Y), it->NavLayer, true), *next, true)) {
            Route.pop_front();
            LastWayPointVisited = position;
            CalcWayPointWidths(position);
            SteeringAgent->SetGoal(Route.front().Position,
                                   Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
        }
    }
    MoveTowardNextWayPoint(position, proximity, speed, movement, lineOfSight, flag2, flag3, flag4);
    if (SteeringAgent->FailPath && flag5)
        return NAVIGATOR_FAILED;
    return NAVIGATOR_MOVING;
}

