// CNavigatorAStar::Initialise(...) -- reset the navigator and either start an A* search or route straight to the goal
// Leaf: ?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_STLP_LIST_MEMBERS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/CNavigatorBase.h"
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#include "engine/CNavBlockedQuadTreeNode.h"
#define FABLE_RETAIL_CNAVIGATORMANAGER
#include "engine/CNavigatorManager.h"

float __fastcall GFGet2DDistance(const C2DVector& a, const C2DVector& b);   // 0x009E2460

class CNavWayPoint {                                   // Ego_r.pdb: Position +0, NavLayer +8, Removable +9 (10 bytes)
public:
    CNavWayPoint(const C2DVector& position, unsigned char navLayer) : Position(position), NavLayer(navLayer), Removable(true) { }
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
};
class CSteeringAgent {
public:
    void SetGoal(const C2DVector& goal, const C2DVector& nextGoal);          // 0x00A20970
};
template <class T> class CPriorityQueue {
public:
    template <class C> void Push(const T& element, const C& compare);       // 0x00A22460 for CAStarStateTotalGreaterThan
    void Clear() { Heap.clear(); }
    std::vector<T> Heap;                                                    // +0x00
};
class CAStarStateTotalGreaterThan;

class CNavigatorAStar : public CNavigatorBase {
public:
    class CAStarState {                                // Ego_r.pdb, 0x14 bytes
    public:
        CAStarState(const CANavQuadTreeNode* node, CAStarState* parent)
            : ParentState(parent), Node((CANavQuadTreeNode*)node), CostToThisState(0.0f), TotalCost(0.0f), OnOpenList(false), OnClosedList(false)
        {
            ++NoOfStates;
        }
        CAStarState(const CAStarState& other)
            : ParentState(other.ParentState), Node(other.Node), CostToThisState(other.CostToThisState),
              TotalCost(other.TotalCost), OnOpenList(other.OnOpenList), OnClosedList(other.OnClosedList)
        {
            ++NoOfStates;
            if (OnOpenList)
                ++NoOfStatesOnOpenList;
            if (OnClosedList)
                ++NoOfStatesOnClosedList;
        }
        ~CAStarState()
        {
            --NoOfStates;
            if (OnOpenList)
                --NoOfStatesOnOpenList;
            if (OnClosedList)
                --NoOfStatesOnClosedList;
        }
        void SetOnOpen()
        {
            OnOpenList = true;
            ++NoOfStatesOnOpenList;
        }
        void SetCostToThisState(float cost) { CostToThisState = cost; }
        void SetTotalCost(float cost) { TotalCost = cost; }
        CAStarState*       ParentState;                 // +0x00
        CANavQuadTreeNode* Node;                        // +0x04
        float              CostToThisState;             // +0x08
        float              TotalCost;                   // +0x0C
        bool               OnOpenList;                  // +0x10
        bool               OnClosedList;                // +0x11
        static long NoOfStates;                         // 0x013D27B8
        static long NoOfStatesOnOpenList;               // 0x013D27BC
        static long NoOfStatesOnClosedList;             // 0x013D27C0
    };
    std::map<long, CAStarState>        States;                     // +0x24
    CNavBlockedQuadTreeNode            BlockedStartNode;           // +0x30
    CNavBlockedQuadTreeNode            BlockedEndNode;             // +0x40
    CNavBlockedQuadTreeNode            BlockedNodeAskedToGetTo;    // +0x50
    CPriorityQueue<CAStarState*>       OpenList;                   // +0x60
    CANavQuadTreeNode*                 GoalNode;                   // +0x6C
    C2DVector                          GoalPosition;               // +0x70
    CANavQuadTreeNode*                 NodeAskedToGetTo;           // +0x78
    C3DVector                          PosAskedToGetTo;            // +0x7C
    CANavQuadTreeNode*                 LastNavigableNodeVisited;   // +0x88
    std::list<CNavWayPoint>            Route;                      // +0x8C
    std::list<CNavWayPoint>::iterator  CullFront;                  // +0x90
    std::list<CNavWayPoint>::iterator  CullBack;                   // +0x94
    C2DVector                          LastWayPointVisited;        // +0x98
    C2DVector                          StringLeadPosition;         // +0xA0
    C3DVector                          LastKnownNavPos;            // +0xA8
    float                              LastKnownNavSpeed;          // +0xB4
    long                               LastKnownNavLayer;          // +0xB8
    bool                               HeadingForActionPoint;      // +0xBC
    bool                               Initialised;                // +0xBD
    bool                               GotRoute;                   // +0xBE
    bool                               SlowedDownForLastPoint;     // +0xBF
    bool                               Culled;                     // +0xC0
    bool                               PartWayThroughAStar;        // +0xC1
    unsigned char                      NoOfClockwiseTurns;         // +0xC2
    unsigned char                      NoOfAntiClockwiseTurns;     // +0xC3
private:
    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const
    {
        if (a == b)
            return true;
        if (!a || !b)
            return false;
        return *a == *b;
    }
    float GetHeuristicCost(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to) const
    {
        return GFGet2DDistance(from->Position, to->Position) * 2.0f;
    }
    void Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* endNode, const C2DVector& startPos,
                    const C2DVector& endPos, const CANavQuadTreeNode* nodeAskedToGetTo, const C3DVector& posAskedToGetTo,
                    bool calculateRoute, bool immediately, float priority);
};
class CAStarStateTotalGreaterThan : public std::binary_function<const CNavigatorAStar::CAStarState*, const CNavigatorAStar::CAStarState*, bool> {
public:
    bool operator()(const CNavigatorAStar::CAStarState* a, const CNavigatorAStar::CAStarState* b) const;
};
extern long g_AStarSearchesInProgress;             // 0x013D27B0

void CNavigatorAStar::Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* endNode, const C2DVector& startPos,
                                 const C2DVector& endPos, const CANavQuadTreeNode* nodeAskedToGetTo, const C3DVector& posAskedToGetTo,
                                 bool calculateRoute, bool immediately, float priority)
{
    if (States.size() != 0)
        --g_AStarSearchesInProgress;
    NavigationManager->GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this);
    States.clear();
    Route.clear();
    OpenList.Clear();
    CullFront = Route.begin();
    CullBack = Route.end();
    if (calculateRoute && !IsEqual(startNode, endNode)) {
        CAStarState state(startNode, 0);
        state.SetOnOpen();
        state.SetCostToThisState(0.0f);
        float heuristic = GetHeuristicCost(startNode, endNode);
        state.SetTotalCost(heuristic);
        std::pair<std::map<long, CAStarState>::iterator, bool> inserted = States.insert(std::make_pair((long)startNode, state));
        OpenList.Push(&inserted.first->second, CAStarStateTotalGreaterThan());
        GotRoute = false;
        Culled = false;
        PartWayThroughAStar = true;
        ++g_AStarSearchesInProgress;
        NavigationManager->GetNavigatorQueueManager().AddNavigatorToQueue(*this, immediately, priority);
    } else {
        GotRoute = true;
        Culled = true;
        PartWayThroughAStar = false;
        Route.push_front(CNavWayPoint(startPos, startNode->NavLayer));
        Route.push_back(CNavWayPoint(endPos, endNode->NavLayer));
        SteeringAgent->SetGoal(Route.back().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }
    StringLeadPosition = startPos;
    SlowedDownForLastPoint = true;
    LastWayPointVisited = startPos;
    GoalNode = (CANavQuadTreeNode*)endNode;
    GoalPosition = endPos;
    if (nodeAskedToGetTo->GetNodeType() != NAV_NODE_BLOCKED)
        NodeAskedToGetTo = (CANavQuadTreeNode*)nodeAskedToGetTo;
    else {
        BlockedNodeAskedToGetTo = *(const CNavBlockedQuadTreeNode*)nodeAskedToGetTo;
        NodeAskedToGetTo = &BlockedNodeAskedToGetTo;
    }
    PosAskedToGetTo = posAskedToGetTo;
    Initialised = true;
    OnRoute = true;
    Interrupted = false;
    NoOfClockwiseTurns = 0;
    NoOfAntiClockwiseTurns = 0;
}
