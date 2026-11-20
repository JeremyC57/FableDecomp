// CNavigatorAStar::CalculateAndMoveToPosProximity(const C3DVector& current_pos, const C3DVector& dest_pos, float proximity, float current_speed, C3DVector& resulting_vector, bool stop_within_nav_line_of_sight, bool avoid_dynamic_obstacles, bool avoid_static_obstacles, bool reduce_speed_when_near, bool fail_on_dynamic_blockage, bool high_priority, bool ignore_path_preferability, float distance_to_lod_centre) -- one navigation step toward dest_pos within proximity: reset on interruption or a failed steering path, record the last known position / speed, decide whether the existing A* route can be patched (neighbouring end node: append the destination, with the node centre first when the line is blocked; non-navigable end node: snap to the nearest navigable node of the goal; same node: move the last way point) or navigation must be reset (re-run Initialise with the nearest navigable / last visited nodes, failing when the regions are not connected), finish a part-way A* search, cull the way points, drop the way points already within the steering agent's arrival proximity, short-cut the first removable way point when the straight line is clear, and move toward the next way point
// Inline-budget: header-contract inline devices only (no flag, no pragma; README ablation) -- shared-header opt-ins FABLE_RETAIL_C2DVECTOR_ASSIGN3D (without it DIFFER(5465v5437)), FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE (DIFFER(5381v5437)), FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE (DIFFER(5463v5437)), FABLE_RETAIL_C3DVECTOR (DIFFER(5437v5437), 16 lines); the other FABLE_RETAIL_* defines select header class layouts / bodies the TU needs to compile (each SRC_FAIL without it); row-local inline GFGetSign (FABLE_FASTCALL; plain inline DIFFER(5437v5437), 18 lines), GFGet2DDistanceSquared, CNavigatorAStar::GetNodeAt (without it DIFFER(5314v5437)), IsEqual (DIFFER(5425v5437)) and IsRouteClear (DIFFER(2529v5437)) bodies in the TU, and the same-TU strong definition of 0x00A1A1A0 below (declaration only: DIFFER(5445v5437)). The draft's `#pragma auto_inline(off)` around it changes no byte and is dropped, as are FABLE_RETAIL_NAVIGATORASTAR_PASS21 / _C2DVECTOR_SQUAREDMAGNITUDE_YX / _C3DVECTOR_SQUAREDMAGNITUDE_XYZ.
// Leaf: ?CalculateAndMoveToPosProximity@CNavigatorAStar@@AAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N222222M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E5C0, 5437 bytes, private thiscall.
// Identity: Ego_r.pdb / FableWin.pdb ?CalculateAndMoveToPosProximity@CNavigatorAStar@@AAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N222222M@Z
//   (labels_* 0x00A1E5C0, Codex pass 21); the catalog's `CNavigatorAStar::MoveTowardGoal` is a forge autoname (NAME_FIXES.tsv).
// Source shape: Ego_d.pdb's locals -- function scope prox_and_rad_sq, end_node, reset_navigation, start_node, initial_distance_apart_sq;
//   blocks {final_waypoint}, {near_node}, {new_dest_pos, {old_dest_pos}}, {init_end_node, init_start_node, init_dest_pos, {near_node}},
//   {one_past_begin, next_section} (the route loop) and {nav_layer, it} (the short-cut block). IsRouteClear (expanded at its first and last
//   call sites) has Ego_d's IsRouteClear locals (normal, start1/end1/start2/end2, start_pos, x_sign/y_sign, c1, vert_edge/hori_edge,
//   new_point, lambda, potential_point, old_node).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ASSIGN3D
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE
#define FABLE_RETAIL_C2DLINEF_SQUAREDDIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_CNAVIGATORMANAGER_ISACTIONPOINT
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVREGION_ISCONNECTED
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVIGATORBASE_RADIUS
#include "engine/CNavigatorExternalInterface.h"
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

#include "engine/CNavigatorAStar.h"
#define min(a, b) (((a) < (b)) ? (a) : (b))

inline long FABLE_FASTCALL GFGetSign(float value)
{
    return value < 0.0f ? -1 : 1;
}

// Fidelity: same-TU strong definition of 0x00A1A1A0 (retail TU-mate)
// (Ego_r.pdb lists ?IsCurrentNavigabilityBetter@CSteeringAgent@@QAE_NXZ in .\Release\lib_navigator_a_star.obj, this row's object; the landed
// row's body expression verbatim. With the body visible VC7.1 knows the call leaves edx alone and keeps the front route node in edx across
// it, 0x00A1F4A8 .. 0x00A1F4BB.)
bool CSteeringAgent::IsCurrentNavigabilityBetter()
{
    return BestDistLeft > BestNextDistLeft || BestDistRight > BestNextDistRight;
}

inline const CANavQuadTreeNode* CNavigatorAStar::GetNodeAt(const C3DVector& pos) const {return NavigationManager.GetNodeAt(C2DVector(pos),(unsigned char)PInterface->GetNavigationLevelAt(pos));}
bool CNavigatorAStar::IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) const
{
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    return *a == *b;
}

// Original FableWin PDB symbol and01B4F090 donor body; original locals pos1,pos2,dx,dy.
inline float FABLE_FASTCALL GFGet2DDistanceSquared(const C2DVector& pos1,const C2DVector& pos2)
{
    float dx=pos2.X-pos1.X;
    float dy=pos2.Y-pos1.Y;
    dy *= dy;
    dy += dx*dx;
    return dy;
}

bool CNavigatorAStar::IsRouteClear(const CNavWayPoint& from, const CNavWayPoint& to, bool test_layers) const
{
    if (test_layers && from.NavLayer != to.NavLayer)
        return false;
    const CANavQuadTreeNode* from_node = NavigationManager.GetNodeAt(from.Position, from.NavLayer);
    const CANavQuadTreeNode* to_node = NavigationManager.GetNodeAt(to.Position, to.NavLayer);
    if (!from_node || !to_node)
        return false;
    if (!(from_node->IsNavigable() && to_node->IsNavigable()))
        return false;
    int best_cost = min(from_node->GetPreferabilityCost(), to_node->GetPreferabilityCost());
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
    C2DVector normal(direction.Y, -direction.X);
    normal *= CNavQuadTree::GetSmallestNodeSize() * 0.45f;
    C2DVector start1 = from.Position + normal;
    C2DVector end1 = to.Position + normal;
    C2DVector start2 = from.Position - normal;
    C2DVector end2 = to.Position - normal;
    C2DVector start_pos;
    const CANavQuadTreeNode* current_node = NULL;
    long x_sign = GFGetSign(direction.X);
    long y_sign = GFGetSign(direction.Y);
    for (unsigned long c1 = 1; c1 <= 2; ++c1) {
        if (c1 == 0) {
            current_node = NavigationManager.GetNodeAt(from.Position, from.NavLayer);
            to_node = NavigationManager.GetNodeAt(to.Position, from.NavLayer);
            if (!current_node || !to_node || !current_node->IsNavigable() || !to_node->IsNavigable() ||
                current_node->GetPreferabilityCost() > best_cost || to_node->GetPreferabilityCost() > best_cost)
                return false;
            start_pos = from.Position;
        } else if (c1 == 1) {
            current_node = NavigationManager.GetNodeAt(start1, from.NavLayer);
            to_node = NavigationManager.GetNodeAt(end1, from.NavLayer);
            if (!current_node || !to_node || !current_node->IsNavigable() || !to_node->IsNavigable() ||
                current_node->GetPreferabilityCost() > best_cost || to_node->GetPreferabilityCost() > best_cost)
                return false;
            start_pos = start1;
        } else if (c1 == 2) {
            current_node = NavigationManager.GetNodeAt(start2, from.NavLayer);
            to_node = NavigationManager.GetNodeAt(end2, from.NavLayer);
            if (!current_node || !to_node || !current_node->IsNavigable() || !to_node->IsNavigable() ||
                current_node->GetPreferabilityCost() > best_cost || to_node->GetPreferabilityCost() > best_cost)
                return false;
            start_pos = start2;
        }
        while (!IsEqual(current_node, to_node)) {
            if (!current_node || !current_node->IsNavigable() || current_node->GetPreferabilityCost() > best_cost)
                return false;
            float vert_edge = current_node->GetSize() * x_sign * 0.5f + current_node->GetPosition().X;
            float hori_edge = current_node->GetSize() * y_sign * 0.5f + current_node->GetPosition().Y;
            C2DVector new_point;
            if (!vertical) {
                float lambda = (vert_edge - start_pos.X) / direction.X;
                float y = lambda * direction.Y + start_pos.Y;
                new_point.Initialise(vert_edge, y);
            }
            if (!horizontal) {
                float lambda = (hori_edge - start_pos.Y) / direction.Y;
                float x = lambda * direction.X + start_pos.X;
                C2DVector potential_point(x, hori_edge);
                if (vertical || GFGet2DDistanceSquared(start_pos,potential_point) < GFGet2DDistanceSquared(start_pos,new_point))
                    new_point = potential_point;
            }
            new_point += direction * CNavQuadTree::GetSmallestNodeSize() * 0.125f;
            const CANavQuadTreeNode* old_node = current_node;
            current_node = NavigationManager.GetNodeAt(new_point, from.NavLayer);
            if (IsEqual(current_node, old_node))
                return false;
        }
    }
    return true;
}

ENavigatorReturn CNavigatorAStar::CalculateAndMoveToPosProximity(const C3DVector& current_pos, const C3DVector& dest_pos,
                                                                  float proximity, float current_speed, C3DVector& resulting_vector,
                                                                  bool stop_within_nav_line_of_sight, bool avoid_dynamic_obstacles, bool avoid_static_obstacles, bool reduce_speed_when_near,
                                                                  bool fail_on_dynamic_blockage, bool high_priority, bool ignore_path_preferability, float distance_to_lod_centre)
{
    if (Interrupted) {
        LastNavigableNodeVisited = NULL;
        UpdateLastPosition(current_pos);
    }
    if (SteeringAgent->HasFailedPath()) {
        SteeringAgent->Reset();
        Interrupted = true;
    }
    LastKnownNavPos = current_pos;
    LastKnownNavSpeed = current_speed;
    float prox_and_rad_sq = PeekNavigatorRadius();
    prox_and_rad_sq += proximity;
    resulting_vector.Initialise(0.0f, 0.0f, 0.0f);
    prox_and_rad_sq = GFSqr(prox_and_rad_sq);
    float initial_distance_apart_sq = (dest_pos - current_pos).GetSquaredMagnitude();
    HeadingForActionPoint = false;
    if (proximity == DELTA_NOTIONAL_ZERO) {
        HeadingForActionPoint = NavigationManager.IsActionPoint(C2DVector(dest_pos));
        prox_and_rad_sq = DELTA_NOTIONAL_ZERO;
    }
    if (!stop_within_nav_line_of_sight && initial_distance_apart_sq <= prox_and_rad_sq) {
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }

    const CANavQuadTreeNode* end_node = GetNodeAt(dest_pos);
    if (!end_node) {
        OnRoute = false;
        return NAV_CANNOT_REACH_TARGET;
    }
    if (end_node->GetNodeType() == NAV_QUAD_TREE_BLOCKED_NODE) {
        BlockedEndNode = *static_cast<const CNavBlockedQuadTreeNode*>(end_node);
        end_node = &BlockedEndNode;
    }

    bool reset_navigation = false;
    if (Interrupted || !OnRoute) {
        reset_navigation = true;
    } else if (!Route.empty() && !HeadingForActionPoint) {
        if (!IsEqual(end_node, NodeAskedToGetTo)) {
            if (end_node->IsNavigable()) {
                if (end_node->IsNeighbourOf(NodeAskedToGetTo)) {
                    CNavWayPoint final_waypoint(C2DVector(dest_pos.X, dest_pos.Y), end_node->GetNavLayer(), true);
                    if (!IsRouteClear(Route.back(), final_waypoint, true))
                        Route.push_back(CNavWayPoint(end_node->GetPosition(), end_node->GetNavLayer(), true));
                    Route.push_back(final_waypoint);
                    NodeAskedToGetTo = end_node;
                    GoalPosition = dest_pos;
                    PosAskedToGetTo = dest_pos;
                } else {
                    reset_navigation = true;
                }
            } else {
                const CANavQuadTreeNode* near_node = FindNearestNavigableNode(GoalNode, end_node, dest_pos,
                                                                            GoalNode->GetSize(), &NavigationManager);
                if (IsEqual(near_node, GoalNode)) {
                    Route.push_back(CNavWayPoint(C2DVector(dest_pos.X, dest_pos.Y), GoalNode->GetNavLayer(), true));
                    if (end_node->GetNodeType() == NAV_QUAD_TREE_BLOCKED_NODE) {
                        BlockedNodeAskedToGetTo = *static_cast<const CNavBlockedQuadTreeNode*>(end_node);
                        NodeAskedToGetTo = &BlockedNodeAskedToGetTo;
                    } else {
                        NodeAskedToGetTo = end_node;
                    }
                    GoalPosition = dest_pos;
                    PosAskedToGetTo = dest_pos;
                } else {
                    reset_navigation = true;
                }
            }
        } else if (end_node->IsNavigable()) {
            C2DVector new_dest_pos(dest_pos);
            // Inferred: retail-only reference local, not in Ego_d (a later revision whose /Od line sizes 586/588/589 = 39/26/28 fit three
            //   Route.back() calls); descriptive name (the draft's). The bytes prove one evaluation of Route.back() held in a named local, not
            //   its name or form (pointer / C2DVector& / list iterator forms are byte-equal). Without it (three Route.back() calls)
            //   DIFFER(5437v5437): 0x00A1E81B (register swap) and the x87 multiply order at 0x00A1F65A / 0x00A1F9D1; a dead scalar in its
            //   place fixes 0x00A1E81B only.
            CNavWayPoint& last = Route.back();
            if (!(last.Position == new_dest_pos)) {
                C2DVector old_dest_pos = last.Position;
                last.Position = new_dest_pos;
                GoalPosition = new_dest_pos;
                if (Route.size() == 1)
                    UpdateGoal(current_pos, old_dest_pos, new_dest_pos);
            }
        }
    }
    reset_navigation = !(!reset_navigation && IsEqual(end_node, NodeAskedToGetTo) &&
                        GFGet2DDistanceSquared(dest_pos, GoalPosition) <= prox_and_rad_sq);

    const CANavQuadTreeNode* start_node = GetMyNodeAt(current_pos);
    if (!start_node) {
        OnRoute = false;
        return NAV_CANNOT_REACH_TARGET;
    }
    if (start_node->GetNodeType() == NAV_QUAD_TREE_BLOCKED_NODE) {
        BlockedStartNode = *static_cast<const CNavBlockedQuadTreeNode*>(start_node);
        start_node = &BlockedStartNode;
    }
    if (stop_within_nav_line_of_sight && end_node->IsNavigable() && GFSqr(proximity) >= initial_distance_apart_sq) {
        if (IsRouteClear(CNavWayPoint(current_pos, start_node->GetNavLayer(), true),
                         CNavWayPoint(dest_pos, end_node->GetNavLayer(), true), false)) {
            OnRoute = false;
            return NAV_REACHED_TARGET;
        }
    }

    if (!reset_navigation && !Route.empty()) {
        reset_navigation = NavigationManager.GetNodeAt(Route.front().Position, Route.front().NavLayer) == NULL;
    }
    if (reset_navigation) {
        NodeAskedToGetTo = NULL;
        Interrupted = false;
        const CANavQuadTreeNode* init_start_node = start_node;
        const CANavQuadTreeNode* init_end_node = end_node;
        C2DVector init_dest_pos(dest_pos);
        if (avoid_static_obstacles) {
            if (!end_node->IsNavigable()) {
                if (!HeadingForActionPoint && proximity <= DELTA_NOTIONAL_ZERO) {
                    OnRoute = false;
                    return NAV_CANNOT_REACH_TARGET;
                }
                const CANavQuadTreeNode* near_node = FindNearestNavigableNode(start_node, end_node, dest_pos, proximity,
                                                                            &NavigationManager);
                if (!near_node) {
                    OnRoute = false;
                    return NAV_CANNOT_REACH_TARGET;
                }
                if (HeadingForActionPoint) {
                    init_end_node = near_node;
                } else {
                    if (!IsAnyPartOfNodeWithinDistOfPos(near_node, (PeekNavigatorRadius()) + proximity, dest_pos,
                                                        init_dest_pos)) {
                        OnRoute = false;
                        return NAV_CANNOT_REACH_TARGET;
                    }
                    init_end_node = near_node;
                }
            }
            if (!start_node->IsNavigable()) {
                if (!LastNavigableNodeVisited) {
                    OnRoute = false;
                    return NAV_CANNOT_REACH_TARGET;
                }
                init_start_node = LastNavigableNodeVisited;
            }
            if (!init_start_node->GetRegion()->IsConnectedToRegion(*init_end_node->GetRegion())) {
                OnRoute = false;
                return NAV_CANNOT_REACH_TARGET;
            }
        }
        Initialise(init_start_node, init_end_node, C2DVector(current_pos), init_dest_pos, end_node, dest_pos, avoid_static_obstacles, high_priority, distance_to_lod_centre);
    }

    if (PartWayThroughAStar) {
        PartWayThroughAStar = false;
        CalculateRoute(ignore_path_preferability);
        if (PartWayThroughAStar)
            return NAV_MOVING_TO_TARGET;
        if (!GotRoute) {
            OnRoute = false;
            return NAV_CANNOT_REACH_TARGET;
        }
        Route.push_front(CNavWayPoint(C2DVector(current_pos.X, current_pos.Y), Route.front().NavLayer, true));
        Route.push_back(CNavWayPoint(GoalPosition, end_node->GetNavLayer(), true));
        CullFront = Route.begin();
        CullBack = Route.end();
        --CullBack;
    } else if (!GotRoute) {
        OnRoute = false;
        return NAV_CANNOT_REACH_TARGET;
    }

    if (!Culled) {
        CullWayPoints(300);
        if (!Culled)
            return NAV_MOVING_TO_TARGET;
        SteeringAgent->SetGoal(Route.front().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }
    while (Route.size() > 1) {
        std::list<CNavWayPoint>::iterator one_past_begin = Route.begin();
        ++one_past_begin;
        C2DLineF next_section(Route.front().Position, one_past_begin->Position);
        if (SteeringAgent->GetProximityForArrival() == 0.0f)
            SteeringAgent->SetProximityForArrival(current_speed);
        if (next_section.GetSquaredDist(C2DVector(current_pos)) > GFSqr(SteeringAgent->GetProximityForArrival()))
            break;
        LastWayPointVisited = Route.front().Position;
        Route.erase(Route.begin());
        CalculateNavigableWidthOfLine(current_pos);
        SteeringAgent->SetGoal(Route.front().Position, Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
    }

    if (Route.empty() || (!stop_within_nav_line_of_sight && Route.size() == 1 &&
        GFGet2DDistanceSquared(current_pos, Route.front().Position) <= DELTA_NOTIONAL_ZERO)) {
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }
    if (Route.size() > 1 && Route.front().Removable && !SteeringAgent->IsCurrentNavigabilityBetter()) {
        std::list<CNavWayPoint>::iterator it = Route.begin();
        unsigned char nav_layer = it->NavLayer;
        ++it;
        if (IsRouteClear(CNavWayPoint(C2DVector(current_pos.X, current_pos.Y), nav_layer, true), *it, true)) {
            Route.pop_front();
            LastWayPointVisited = current_pos;
            CalculateNavigableWidthOfLine(current_pos);
            SteeringAgent->SetGoal(Route.front().Position,
                                   Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position);
        }
    }
    MoveTowardNextWayPoint(current_pos, current_speed, proximity, resulting_vector, stop_within_nav_line_of_sight, avoid_dynamic_obstacles, avoid_static_obstacles, reduce_speed_when_near);
    if (SteeringAgent->HasFailedPath() && fail_on_dynamic_blockage)
        return NAV_CANNOT_REACH_TARGET;
    return NAV_MOVING_TO_TARGET;
}
