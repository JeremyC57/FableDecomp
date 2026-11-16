// 00a1ceb0 CNavigatorAStar::Initialise
// Flags: none
// CNavigatorAStar::Initialise(...) -- reset search start_state and begin either AStar or a direct two-point route.
// Leaf: ?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z
// Retail: Fable.exe0x00A1CEB0,992bytes,thiscall ret24h.
// Shared original navigation classes; start_state/helper bodies follow both original PDB donors.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NAV_NODETYPE_ORIGINAL_POLYMORPHISM_PASS23
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_QUEUE_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_API_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
#include "engine/CNavigatorAStar.h"
// Original TU-local S_LDATA32 in both lib_navigator_a_star modules.
extern long NumberOfPeopleCalculatingRoute;
void CNavigatorAStar::Initialise(const CANavQuadTreeNode* start_node, const CANavQuadTreeNode* goal_node, const C2DVector& start_pos,
                                 const C2DVector& goal_pos, const CANavQuadTreeNode* requested_node, const C3DVector& requested_pos,
                                 bool avoid_static_obstacles, bool high_priority, float distance_to_lod_centre)
{
    if (States.size() != 0)
        --NumberOfPeopleCalculatingRoute;
    NavigationManager.GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this);
    States.clear();
    Route.clear();
    OpenList.Clear();
    CullFront = Route.begin();
    CullBack = Route.end();
    if (avoid_static_obstacles && !IsEqual(start_node, goal_node)) {
        CAStarState start_state(start_node, 0);
        start_state.SetOnOpen();
        start_state.SetCostToThisState(0.0f);
        // Fidelity: retain scalar call-result parentheses for the native fstp/counter schedule.
        start_state.SetTotalCost((GetHeuristicCost(start_node, goal_node)));
        std::pair<std::map<long, CAStarState>::iterator, bool> state_it = States.insert(std::make_pair((long)start_node, start_state));
        OpenList.Push(&state_it.first->second, CAStarStateTotalGreaterThan());
        GotRoute = false;
        Culled = false;
        PartWayThroughAStar = true;
        ++NumberOfPeopleCalculatingRoute;
        NavigationManager.GetNavigatorQueueManager().AddNavigatorToQueue(*this, high_priority, distance_to_lod_centre);
    } else {
        GotRoute = true;
        Culled = true;
        PartWayThroughAStar = false;
        Route.push_front(CNavWayPoint(start_pos,(unsigned char)start_node->GetNavLayer(),true));
        Route.push_back(CNavWayPoint(goal_pos,(unsigned char)goal_node->GetNavLayer(),true));
        SteeringAgent->SetGoal(Route.back().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }
    StringLeadPosition = start_pos;
    SlowedDownForLastPoint = true;
    LastWayPointVisited = start_pos;
    GoalNode = goal_node;
    GoalPosition = goal_pos;
    if (requested_node->GetNodeType() != NAV_QUAD_TREE_BLOCKED_NODE)
        NodeAskedToGetTo = requested_node;
    else {
        BlockedNodeAskedToGetTo = *(const CNavBlockedQuadTreeNode*)requested_node;
        NodeAskedToGetTo = &BlockedNodeAskedToGetTo;
    }
    PosAskedToGetTo = requested_pos;
    Initialised = true;
    OnRoute = true;
    Interrupted = false;
    NoOfClockwiseTurns = 0;
    NoOfAntiClockwiseTurns = 0;
}