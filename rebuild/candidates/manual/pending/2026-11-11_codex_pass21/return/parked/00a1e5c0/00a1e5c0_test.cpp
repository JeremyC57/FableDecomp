// Unchanged selected navigation and route-clear bodies below; deterministic
// dependency doubles isolate the row's decisions, forwarding and state changes.
#include <cassert>
#include <cmath>
#include <cstdio>
#include <list>
#include <vector>
#define FABLE_FASTCALL
const float DELTA_NOTIONAL_ZERO=0.0001f;
inline float GFSqr(float x){return x*x;}
inline long GFGetSign(float x){return x<0.0f?-1:1;}
class C3DVector {
public:
 float X,Y,Z;
 C3DVector():X(0),Y(0),Z(0){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}
};
class C2DVector {
public:
 float X,Y;
 C2DVector():X(0),Y(0){}
 C2DVector(float x,float y):X(x),Y(y){}
 C2DVector(const C3DVector& v):X(v.X),Y(v.Y){}
 void Initialise(float x,float y){X=x;Y=y;}
 void operator=(const C3DVector& v){X=v.X;Y=v.Y;}
 C2DVector operator+(const C2DVector& v)const{return C2DVector(X+v.X,Y+v.Y);}
 C2DVector operator-(const C2DVector& v)const{return C2DVector(X-v.X,Y-v.Y);}
 const C2DVector operator*(float s)const{return C2DVector(X*s,Y*s);}
 const C2DVector& operator*=(float s){X*=s;Y*=s;return *this;}
 const C2DVector& operator+=(const C2DVector& v){X+=v.X;Y+=v.Y;return *this;}
 bool operator==(const C2DVector& v)const{return fabs(X-v.X)<=DELTA_NOTIONAL_ZERO&&fabs(Y-v.Y)<=DELTA_NOTIONAL_ZERO;}
 bool operator!=(const C2DVector& v)const{return !(*this==v);}
 bool IsNonZero()const{return Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 C2DVector& Normalise(){float m=(float)sqrt(X*X+Y*Y);X/=m;Y/=m;return *this;}
};
class C2DLineF {
public:
 C2DVector A,B;
 static float TestSquaredDistance;
 C2DLineF(const C2DVector& a,const C2DVector& b):A(a),B(b){}
 float GetSquaredDist(const C2DVector&){return TestSquaredDistance;}
};
float C2DLineF::TestSquaredDistance=1024.0f;
class CNavRegion {public:bool Connected;CNavRegion():Connected(true){}bool IsConnectedToRegion(const CNavRegion& other)const{return Connected&&other.Connected;}};
enum ENavQuadTreeNodeType {NAV_QUAD_TREE_NON_LEAF_NODE=0,NAV_QUAD_TREE_NAVIGABLE_NODE=1,NAV_QUAD_TREE_BLOCKED_NODE=2,NAV_QUAD_TREE_SWITCHABLE_NODE=3};
class CANavQuadTreeNode {
public:
 C2DVector Position;unsigned char NavLayer;bool Blocked,Neighbour;unsigned char Cost;float Size;CNavRegion Region;ENavQuadTreeNodeType Type;
 CANavQuadTreeNode():NavLayer(0),Blocked(false),Neighbour(false),Cost(1),Size(8),Type(NAV_QUAD_TREE_NAVIGABLE_NODE){}
 virtual ~CANavQuadTreeNode(){}
 virtual bool IsNeighbourOf(const CANavQuadTreeNode*)const{return Neighbour;}
 virtual ENavQuadTreeNodeType GetNodeType()const{return Type;}
 virtual const CNavRegion* GetRegion()const{return &Region;}
 virtual unsigned char GetPreferabilityCost()const{return Cost;}
 bool IsNavigable()const{return !Blocked;}
 long GetNavLayer()const{return NavLayer;}
 const C2DVector& GetPosition()const{return Position;}
 float GetSize()const{return Size;}
 bool operator==(const CANavQuadTreeNode& n)const{return Position==n.Position&&NavLayer==n.NavLayer;}
};
class CNavBlockedQuadTreeNode:public CANavQuadTreeNode {};
class CNavWayPoint {public:C2DVector Position;unsigned char NavLayer;bool Removable;CNavWayPoint(const C2DVector& p,unsigned char n,bool r):Position(p),NavLayer(n),Removable(r){}};
class CNavQuadTree {public:static float GetSmallestNodeSize(){return 0.5f;}};
class CNavigatorExternalInterface {public:float Radius;CNavigatorExternalInterface():Radius(0){}float PeekNavigatorRadius(bool)const{return Radius;}long GetNavigationLevelAt(const C3DVector&)const{return 0;}};
class CNavigatorManager {
public:
 CNavBlockedQuadTreeNode Start,End;bool MissingStart,MissingEnd,SameNode,ActionPoint;mutable unsigned Calls;unsigned NullOnCall;
 CNavigatorManager():MissingStart(false),MissingEnd(false),SameNode(false),ActionPoint(false),Calls(0),NullOnCall(0){Start.Position=C2DVector(0,0);End.Position=C2DVector(16,0);}
 const CANavQuadTreeNode* GetNodeAt(const C2DVector& p,unsigned char)const{
  ++Calls;if(NullOnCall&&Calls==NullOnCall)return 0;
  if(SameNode)return &Start;
  return p.X<8?(MissingStart?0:&Start):(MissingEnd?0:&End);
 }
 bool IsActionPoint(const C2DVector&)const{return ActionPoint;}
};
class CSteeringAgent {
public:
 bool FailPath,HasWanderedDuringThisSection,AvoidDynamicObstaclesUntilNextWaypoint;
 float TimeSinceBestApproach,ProximityForArrival,BestDistLeft,BestNextDistLeft,BestDistRight,BestNextDistRight;
 unsigned ResetCalls,GoalCalls;
 CSteeringAgent():FailPath(false),HasWanderedDuringThisSection(false),AvoidDynamicObstaclesUntilNextWaypoint(false),TimeSinceBestApproach(0),ProximityForArrival(1),BestDistLeft(0),BestNextDistLeft(1),BestDistRight(0),BestNextDistRight(1),ResetCalls(0),GoalCalls(0){}
 bool HasFailedPath(){return FailPath;}
 void Reset(){++ResetCalls;TimeSinceBestApproach=0;HasWanderedDuringThisSection=false;AvoidDynamicObstaclesUntilNextWaypoint=true;FailPath=false;}
 float GetProximityForArrival(){return ProximityForArrival;}
 void SetProximityForArrival(float x){ProximityForArrival=x;}
 void SetGoal(const C2DVector&,const C2DVector&){++GoalCalls;}
 bool IsCurrentNavigabilityBetter(){return BestDistLeft>BestNextDistLeft||BestDistRight>BestNextDistRight;}
};
enum ENavigatorReturn {NAV_REACHED_TARGET=0,NAV_CANNOT_REACH_TARGET=1,NAV_MOVING_TO_TARGET=2};
class CNavigatorAStar {
public:
 CNavigatorExternalInterface* PInterface;CNavigatorManager& NavigationManager;CSteeringAgent* SteeringAgent;
 bool OnRoute,Interrupted,HeadingForActionPoint,PartWayThroughAStar,GotRoute,Culled;
 const CANavQuadTreeNode *LastNavigableNodeVisited,*NodeAskedToGetTo,*GoalNode;
 CNavBlockedQuadTreeNode BlockedStartNode,BlockedEndNode,BlockedNodeAskedToGetTo;
 C2DVector GoalPosition,LastWayPointVisited;C3DVector PosAskedToGetTo,LastKnownNavPos;float LastKnownNavSpeed;
 std::list<CNavWayPoint> Route;std::list<CNavWayPoint>::iterator CullFront,CullBack;
 // Explicit test controls and observations; none occur in the selected body.
 const CANavQuadTreeNode* TestNearest;bool TestWithin,TestCalculatePending,TestCalculateGot,TestCullDone,TestFailMovement;
 unsigned TestUpdateCalls,TestInitialiseCalls,TestCalculateCalls,TestCullCalls,TestMoveCalls,TestWidthCalls,TestGoalUpdateCalls;
 float TestSpeed,TestProximity,TestLod;bool TestMoveFlags[4];bool TestIgnorePreference;
 CNavigatorAStar(CNavigatorExternalInterface& i,CNavigatorManager& m,CSteeringAgent& a):PInterface(&i),NavigationManager(m),SteeringAgent(&a),OnRoute(true),Interrupted(false),HeadingForActionPoint(false),PartWayThroughAStar(false),GotRoute(true),Culled(true),LastNavigableNodeVisited(&m.Start),NodeAskedToGetTo(&m.End),GoalNode(&m.End),GoalPosition(16,0),LastKnownNavSpeed(0),TestNearest(&m.Start),TestWithin(true),TestCalculatePending(false),TestCalculateGot(true),TestCullDone(true),TestFailMovement(false),TestUpdateCalls(0),TestInitialiseCalls(0),TestCalculateCalls(0),TestCullCalls(0),TestMoveCalls(0),TestWidthCalls(0),TestGoalUpdateCalls(0),TestSpeed(0),TestProximity(0),TestLod(0),TestIgnorePreference(false){Route.push_back(CNavWayPoint(GoalPosition,0,true));for(int k=0;k<4;++k)TestMoveFlags[k]=false;}
 float PeekNavigatorRadius()const{return PInterface->PeekNavigatorRadius(false);}
 const CANavQuadTreeNode* GetNodeAt(const C3DVector&)const;
 const CANavQuadTreeNode* GetMyNodeAt(const C3DVector& p)const{return GetNodeAt(p);}
 bool IsEqual(const CANavQuadTreeNode*,const CANavQuadTreeNode*)const;
 bool IsRouteClear(const CNavWayPoint&,const CNavWayPoint&,bool)const;
 void UpdateLastPosition(const C3DVector&){++TestUpdateCalls;}
 const CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode*,const CANavQuadTreeNode*,const C3DVector&,float,const CNavigatorManager*)const{return TestNearest;}
 bool IsAnyPartOfNodeWithinDistOfPos(const CANavQuadTreeNode* n,float,const C3DVector&,C2DVector& out)const{out=n->Position;return TestWithin;}
 void UpdateGoal(const C3DVector&,C2DVector,C2DVector){++TestGoalUpdateCalls;}
 void Initialise(const CANavQuadTreeNode*,const CANavQuadTreeNode*,const C2DVector&,const C2DVector&,const CANavQuadTreeNode*,const C3DVector&,bool,bool,float lod){++TestInitialiseCalls;TestLod=lod;}
 bool CalculateRoute(bool ignore){++TestCalculateCalls;TestIgnorePreference=ignore;PartWayThroughAStar=TestCalculatePending;GotRoute=TestCalculateGot;return GotRoute;}
 void CullWayPoints(long n){assert(n==300);++TestCullCalls;Culled=TestCullDone;}
 void CalculateNavigableWidthOfLine(const C3DVector&){++TestWidthCalls;}
 void MoveTowardNextWayPoint(const C3DVector&,float speed,float proximity,C3DVector& out,bool a,bool b,bool c,bool d){++TestMoveCalls;TestSpeed=speed;TestProximity=proximity;TestMoveFlags[0]=a;TestMoveFlags[1]=b;TestMoveFlags[2]=c;TestMoveFlags[3]=d;out=C3DVector(speed,0,0);if(TestFailMovement)SteeringAgent->FailPath=true;}
 ENavigatorReturn CalculateAndMoveToPosProximity(const C3DVector&,const C3DVector&,float,float,C3DVector&,bool,bool,bool,bool,bool,bool,bool,float);
};
#define min(a,b) (((a)<(b))?(a):(b))

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
        C2DLineF next_section(Route.front().Position, (*++Route.begin()).Position);
        if (SteeringAgent->GetProximityForArrival() == 0.0f)
            SteeringAgent->SetProximityForArrival(current_speed);
        C2DVector here(current_pos);
        float arrival = SteeringAgent->GetProximityForArrival();
        if (next_section.GetSquaredDist(here) > arrival * arrival)
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
        std::list<CNavWayPoint>::iterator next = it; ++next;
        if (IsRouteClear(CNavWayPoint(C2DVector(current_pos.X, current_pos.Y), it->NavLayer, true), *next, true)) {
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



class NavigationScenario {
public:
 CNavigatorExternalInterface Interface;CNavigatorManager Manager;CSteeringAgent Agent;CNavigatorAStar Nav;C3DVector Current,Destination,Result;
 NavigationScenario():Nav(Interface,Manager,Agent),Current(0,0,0),Destination(16,0,0),Result(99,99,99){C2DLineF::TestSquaredDistance=1024;}
 ENavigatorReturn Run(float proximity=3,float speed=2,bool sight=false,bool dynamic=true,bool statics=false,bool slow=true,bool fail=true){return Nav.CalculateAndMoveToPosProximity(Current,Destination,proximity,speed,Result,sight,dynamic,statics,slow,fail,true,true,32);}
};
int main(){
 {NavigationScenario s;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestMoveCalls==1);assert(s.Nav.TestSpeed==2&&s.Nav.TestProximity==3);assert(s.Result.X==2&&s.Result.Y==0&&s.Result.Z==0);assert(!s.Nav.TestMoveFlags[0]&&s.Nav.TestMoveFlags[1]&&!s.Nav.TestMoveFlags[2]&&s.Nav.TestMoveFlags[3]);assert(s.Nav.LastKnownNavSpeed==2);}
 {NavigationScenario s;s.Nav.TestFailMovement=true;assert(s.Run()==NAV_CANNOT_REACH_TARGET);assert(s.Nav.TestMoveCalls==1);}
 {NavigationScenario s;s.Nav.TestFailMovement=true;assert(s.Run(3,2,false,false,false,false,false)==NAV_MOVING_TO_TARGET);assert(!s.Nav.TestMoveFlags[1]&&!s.Nav.TestMoveFlags[3]);}
 {NavigationScenario s;s.Manager.MissingEnd=true;assert(s.Run()==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestMoveCalls==0);assert(s.Result.X==0&&s.Result.Y==0&&s.Result.Z==0);}
 {NavigationScenario s;s.Manager.MissingStart=true;assert(s.Run()==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Destination=C3DVector(2,0,0);assert(s.Run(2)==NAV_REACHED_TARGET);assert(!s.Nav.OnRoute&&s.Manager.Calls==0);}
 {NavigationScenario s;s.Destination=C3DVector(3,0,0);s.Interface.Radius=1;assert(s.Run(2)==NAV_REACHED_TARGET);}
 {NavigationScenario s;s.Nav.Interrupted=true;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestUpdateCalls==1&&s.Nav.LastNavigableNodeVisited==0&&!s.Nav.Interrupted&&s.Nav.TestInitialiseCalls==1&&s.Nav.TestLod==32);}
 {NavigationScenario s;s.Agent.FailPath=true;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Agent.ResetCalls==1&&!s.Agent.FailPath&&s.Agent.AvoidDynamicObstaclesUntilNextWaypoint&&s.Nav.TestInitialiseCalls==1);}
 {NavigationScenario s;s.Manager.ActionPoint=true;assert(s.Run(DELTA_NOTIONAL_ZERO)==NAV_MOVING_TO_TARGET);assert(s.Nav.HeadingForActionPoint);}
 {NavigationScenario s;s.Manager.SameNode=true;assert(s.Run(20,2,true)==NAV_REACHED_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.End.Blocked=true;s.Manager.End.Type=NAV_QUAD_TREE_BLOCKED_NODE;s.Nav.TestNearest=0;assert(s.Run(3,2,false,true,true)==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestInitialiseCalls==0);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.End.Blocked=true;s.Manager.End.Type=NAV_QUAD_TREE_BLOCKED_NODE;assert(s.Run(0,2,false,true,true)==NAV_CANNOT_REACH_TARGET);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.End.Blocked=true;s.Manager.End.Type=NAV_QUAD_TREE_BLOCKED_NODE;s.Nav.TestWithin=false;assert(s.Run(3,2,false,true,true)==NAV_CANNOT_REACH_TARGET);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.End.Blocked=true;s.Manager.End.Type=NAV_QUAD_TREE_BLOCKED_NODE;assert(s.Run(3,2,false,true,true)==NAV_MOVING_TO_TARGET);assert(s.Nav.TestInitialiseCalls==1&&s.Nav.TestMoveFlags[2]);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.End.Region.Connected=false;assert(s.Run(3,2,false,true,true)==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Manager.Start.Blocked=true;s.Manager.Start.Type=NAV_QUAD_TREE_BLOCKED_NODE;s.Nav.LastNavigableNodeVisited=0;assert(s.Run(3,2,false,true,true)==NAV_CANNOT_REACH_TARGET);}
 {NavigationScenario s;s.Manager.NullOnCall=3;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestInitialiseCalls==1);}
 {NavigationScenario s;s.Nav.PartWayThroughAStar=true;s.Nav.TestCalculatePending=true;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestCalculateCalls==1&&s.Nav.TestIgnorePreference&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Nav.PartWayThroughAStar=true;s.Nav.TestCalculateGot=false;assert(s.Run()==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Nav.GotRoute=false;assert(s.Run()==NAV_CANNOT_REACH_TARGET);assert(!s.Nav.OnRoute);}
 {NavigationScenario s;s.Nav.Culled=false;s.Nav.TestCullDone=false;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestCullCalls==1&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Nav.Culled=false;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestCullCalls==1&&s.Agent.GoalCalls==1);}
 {NavigationScenario s;s.Nav.Route.clear();assert(s.Run()==NAV_REACHED_TARGET);assert(!s.Nav.OnRoute&&s.Nav.TestMoveCalls==0);}
 {NavigationScenario s;s.Nav.Route.push_front(CNavWayPoint(C2DVector(0,0),0,true));C2DLineF::TestSquaredDistance=0;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.Route.size()==1&&s.Nav.TestWidthCalls==1&&s.Agent.GoalCalls==1);assert(s.Nav.LastWayPointVisited.X==0);}
 {NavigationScenario s;s.Nav.Route.push_front(CNavWayPoint(C2DVector(0,0),0,false));s.Agent.ProximityForArrival=0;assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Agent.ProximityForArrival==2&&s.Nav.Route.size()==2&&s.Nav.TestWidthCalls==0);}
 {NavigationScenario s;s.Nav.Route.front().Position=C2DVector(15,0);assert(s.Run()==NAV_MOVING_TO_TARGET);assert(s.Nav.TestGoalUpdateCalls==1&&s.Nav.Route.front().Position.X==16);}
 {NavigationScenario s;s.Nav.NodeAskedToGetTo=0;s.Nav.Route.front().Position=C2DVector(0,0);assert(s.Run()==NAV_REACHED_TARGET);assert(s.Nav.TestInitialiseCalls==1&&s.Nav.TestMoveCalls==0);}
 // Direct helper decisions use the very same selected IsRouteClear and IsEqual bodies.
 {NavigationScenario s;CNavWayPoint a(C2DVector(0,0),0,true),b(C2DVector(0,0),1,true);assert(!s.Nav.IsRouteClear(a,b,true));assert(s.Manager.Calls==0);}
 {NavigationScenario s;CNavWayPoint a(C2DVector(0,0),0,true);assert(s.Nav.IsRouteClear(a,a,true));assert(s.Nav.IsEqual(0,0));assert(!s.Nav.IsEqual(&s.Manager.Start,0));assert(!s.Nav.IsEqual(0,&s.Manager.Start));}
 {NavigationScenario s;s.Manager.MissingStart=true;CNavWayPoint a(C2DVector(0,0),0,true);assert(!s.Nav.IsRouteClear(a,a,false));}
 {NavigationScenario s;s.Manager.Start.Blocked=true;CNavWayPoint a(C2DVector(0,0),0,true);assert(!s.Nav.IsRouteClear(a,a,false));}
 {NavigationScenario s;CNavWayPoint a(C2DVector(0,0),0,true),b(C2DVector(16,0),0,true);assert(!s.Nav.IsRouteClear(a,b,false));assert(s.Manager.Calls>2);}
 std::puts("00a1e5c0_TEST PASS");return 0;
}
