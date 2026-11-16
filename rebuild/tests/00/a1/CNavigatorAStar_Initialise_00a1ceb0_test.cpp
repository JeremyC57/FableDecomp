// Actual shared classes and target body; test doubles replace only external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_CTOR_PASS23
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define private public
#define protected public
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

#undef private
#undef protected

long NumberOfPeopleCalculatingRoute=0;
long CNavigatorAStar::CAStarState::NumberOfAStarStates=0;
long CNavigatorAStar::CAStarState::NumberOfClosedStates=0;
long CNavigatorAStar::CAStarState::NumberOfOpenStates=0;
long CNavigatorAStar::CAStarState::MaximumNumberOfStates=0;
long CNavigatorAStar::CAStarState::NumberOfClosedStatesDuringMaximumNumberOfStates=0;
long CNavigatorAStar::CAStarState::NumberOfOpenStatesDuringMaximumNumberOfStates=0;
namespace std {
template<class T,class A>template<class F>void list<T,A>::remove_if(F predicate){iterator i=begin();while(i!=end()){iterator current=i++;if(predicate(*current))erase(current);}}
template<class D>void _Rb_global<D>::_Rebalance(_Rb_tree_node_base* node,_Rb_tree_node_base*&root){assert(node==root);assert(!node->_M_left&&!node->_M_right);node->_M_color=true;}
}
class FixtureInterface:public CNavigatorExternalInterface{public:
bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const{abort();}
float GetTerrainDesirabilityAt(const C3DVector&) const{abort();}
float GetTerrainHeightAt(const C3DVector&) const{abort();}
float GetThemeHeightAt(const C3DVector&) const{abort();}
float FindPosZAt(const C3DVector&,bool) const{abort();}
float FindPosZAtIncludingThemes(const C3DVector&,bool) const{abort();}
float PeekNavigatorRadius(bool) const{abort();}
C2DVector PeekNavigatorOrientation() const{abort();}
C2DVector PeekNavigatorPosition2D() const{abort();}
C3DVector PeekNavigatorPosition3D() const{abort();}
float PeekNavigatorMaxSpeed(const C3DVector&) const{abort();}
const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const{abort();}
std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles(){abort();}
std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles(){abort();}
const CSteeringAgent* GetPThingToNotAvoid() const{abort();}
float GetProximityForDirectApproach() const{abort();}
long GetNavigationLevelAt(const C3DVector&) const{abort();}
long GetConstantFPS() const{abort();}
float GetMinPathCost() const{abort();}
float GetMaxPathCost() const{abort();}
bool IsPlayer() const{abort();}
bool IsDead() const{abort();}
float GetForwardNavigationWidthExtension() const{abort();}
CNavigatorExternalInterface* Clone() const{abort();}
};
class FixtureNavigator:public CNavigatorAStar{public:FixtureNavigator(CNavigatorManager&m,const CNavigatorExternalInterface&i):CNavigatorAStar(m,i){}
ENavigatorReturn MoveToPos(const C3DVector&, const C3DVector&, float, C3DVector&, bool, bool, bool, bool, bool, bool, float){abort();}
ENavigatorReturn MoveToPosProximity(const C3DVector&, const C3DVector&, float, float, C3DVector&, bool, bool, bool, bool, bool, bool, float){abort();}
ENavigatorReturn MoveToNavLineOfSightProximity(const C3DVector&, const C3DVector&, float, float, C3DVector&, bool, bool, bool, bool, bool, bool, float){abort();}
C3DVector GetRandomNavigablePos(const C3DVector&, float, bool) const{abort();}
C3DVector GetRandomNavigablePos(const C3DVector&, float, const C2DVector&, float) const{abort();}
bool GetNearestNavigablePosTo(const C3DVector&, float, C3DVector&) const{abort();}
bool CanNavigateToPos(const C3DVector&, const C3DVector&, float, bool) const{abort();}
bool IsNavLineOfSightClear(const C3DVector&, const C3DVector&) const{abort();}
bool IsNewDestinationGoingToCauseRestart(const C3DVector&, float) const{abort();}
const C3DVector& GetCurrentDestination() const{abort();}
void UpdateLastPosition(const C3DVector&){abort();}
void CalculateRouteFromTo(const C3DVector&, const C3DVector&, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >&){abort();}
void InitialiseFromPreCalculatedRoute(const C3DVector&, const std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >&, const C3DVector&){abort();}
};
CANavQuadTreeNode::~CANavQuadTreeNode(){}
const CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() const{abort();}
CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours(){abort();}
bool CANavQuadTreeNode::HasNeighbours() const{abort();}
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const{abort();}
long CANavQuadTreeNode::GetRegionID() const{abort();}
// Original abstract base is never instantiated; real blocked override remains constant2.
ENavQuadTreeNodeType CNavBlockedQuadTreeNode::GetNodeType()const{return NAV_QUAD_TREE_BLOCKED_NODE;}
class FixtureNode:public CANavQuadTreeNode {public:ENavQuadTreeNodeType GetNodeType()const{return NAV_QUAD_TREE_NAVIGABLE_NODE;}};
const CNavRegion* CANavQuadTreeNode::GetRegion() const{abort();}
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const{abort();}
bool CANavQuadTreeNode::IsSwitchable() const{abort();}
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const{abort();}
const CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const{abort();}
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&){abort();}
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&){abort();}
bool CANavQuadTreeNode::RemoveRegionZero(){abort();}
unsigned long CANavQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const{abort();}
CNavigatorExternalInterface::~CNavigatorExternalInterface(){}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject(){}
CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{abort();}
void CClass::Save(CCharString&){abort();}
CNavigatorBase::CNavigatorBase(CNavigatorManager& m,unsigned long,const CNavigatorExternalInterface& i):PInterface(const_cast<CNavigatorExternalInterface*>(&i)),OnRoute(false),Interrupted(false),NavigationManager(m),SteeringAgent(0){}
CNavigatorBase::~CNavigatorBase(){}
const CANavQuadTreeNode* CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode&,const CANavQuadTreeNode*,const C2DVector&){abort();}
void CNavigatorBase::CreateLineList(std::vector<C3DVector>&){abort();}
void CNavigatorBase::Interrupt(){abort();}
void CNavigatorBase::CalculateNavigableWidthOfLine(const C3DVector&,const C3DVector&,float&,float&){abort();}
CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(unsigned char level,unsigned char layer){Position.X=Position.Y=0;QuadTreeLevel=level;NavLayer=layer;LeafNode=true;Blocked=true;}
CNavigatorAStar::CNavigatorAStar(CNavigatorManager& m,const CNavigatorExternalInterface& i):CNavigatorBase(m,0,i),BlockedStartNode(0,0),BlockedEndNode(0,0),BlockedNodeAskedToGetTo(0,0){}
void CNavigatorAStar::UpdateLastPosition(const C3DVector&){abort();}

static const CNavigatorBase* ExpectedNavigator;
static CNavigatorQueueManager* ExpectedQueue;
static CSteeringAgent* ExpectedSteering;
static const C2DVector* ExpectedStart;
static const C2DVector* ExpectedEnd;
static int AddCalls,DistanceCalls,GoalCalls,EqualCalls,AssignCalls;
static bool ExpectedPriority;static float ExpectedDistance;
void CNavigatorQueueManager::AddNavigatorToQueue(const CNavigatorBase&n,bool priority,float distance){assert(this==ExpectedQueue&&&n==ExpectedNavigator);assert(priority==ExpectedPriority&&distance==ExpectedDistance);++AddCalls;Navigators.push_back(std::pair<long,const CNavigatorBase*>(3,&n));}
float __fastcall GFGet2DDistance(const C2DVector&a,const C2DVector&b){assert(&a==ExpectedStart&&&b==ExpectedEnd);++DistanceCalls;return 4.0f;}
void CSteeringAgent::SetGoal(const C2DVector&a,const C2DVector&b){assert(this==ExpectedSteering);++GoalCalls;Goal=a;NextGoal=b;}
bool CANavQuadTreeNode::operator==(const CANavQuadTreeNode&b)const{++EqualCalls;return Position.X==b.Position.X&&Position.Y==b.Position.Y&&NavLayer==b.NavLayer;}
const CNavBlockedQuadTreeNode&CNavBlockedQuadTreeNode::operator=(const CNavBlockedQuadTreeNode&b){++AssignCalls;Position=b.Position;QuadTreeLevel=b.QuadTreeLevel;NavLayer=b.NavLayer;LeafNode=b.LeafNode;Blocked=b.Blocked;return *this;}
static void InitNode(CANavQuadTreeNode&n,float x,float y,unsigned char layer,bool blocked){n.Position.X=x;n.Position.Y=y;n.QuadTreeLevel=2;n.NavLayer=layer;n.LeafNode=true;n.Blocked=blocked;}
static void Point(const C2DVector&a,const C2DVector&b){assert(a.X==b.X&&a.Y==b.Y);}
int main(){unsigned int cases=0;
 for(unsigned int kind=0;kind<3;++kind)for(unsigned int search=0;search<2;++search)for(unsigned int blocked=0;blocked<2;++blocked)
 for(unsigned int previous=0;previous<3;++previous)for(unsigned int priority=0;priority<2;++priority)for(unsigned int layer=0;layer<8;++layer){
  {CNavigatorManager manager;FixtureInterface external;FixtureNavigator nav(manager,external);CSteeringAgent steering;nav.SteeringAgent=&steering;
   FixtureNode a,b,unblocked_req;CNavBlockedQuadTreeNode req(0,0);InitNode(a,1.25f,-3.5f,(unsigned char)layer,false);InitNode(b,kind==2?1.25f:7.5f,kind==2?-3.5f:4.25f,(unsigned char)layer,false);InitNode(req,-8,12,(unsigned char)(layer+1),true);InitNode(unblocked_req,-8,12,(unsigned char)(layer+1),false);const CANavQuadTreeNode* requested_node=blocked?static_cast<const CANavQuadTreeNode*>(&req):&unblocked_req;
   const CANavQuadTreeNode* goal=kind==1?&a:&b;bool needsSearch=search&&kind==0;
   C2DVector start(10.25f,-2.5f),end(15.5f,9.25f);C3DVector requested(-12.5f,4.75f,8.0f);
   nav.NoOfClockwiseTurns=7;nav.NoOfAntiClockwiseTurns=9;nav.Interrupted=true;nav.OnRoute=false;nav.Initialised=false;
   nav.Route.push_back(CNavWayPoint(C2DVector(99,98),99,false));nav.CullFront=nav.Route.begin();nav.CullBack=nav.Route.end();
   if(previous){CNavigatorAStar::CAStarState state(&b,0);if(previous==1)state.SetOnOpen();else state.SetOnClosed();nav.States.insert(std::make_pair((long)&b,state));if(previous==1)nav.OpenList.Push(&nav.States.begin()->second,CAStarStateTotalGreaterThan());}
   NumberOfPeopleCalculatingRoute=10;manager.QueueManager.Navigators.push_back(std::pair<long,const CNavigatorBase*>(1,&nav));manager.QueueManager.Navigators.push_back(std::pair<long,const CNavigatorBase*>(2,0));manager.QueueManager.Navigators.push_back(std::pair<long,const CNavigatorBase*>(4,&nav));
   ExpectedNavigator=&nav;ExpectedQueue=&manager.QueueManager;ExpectedSteering=&steering;ExpectedStart=&a.Position;ExpectedEnd=&goal->Position;ExpectedPriority=priority!=0;ExpectedDistance=(float)layer*.25f;
   AddCalls=DistanceCalls=GoalCalls=EqualCalls=AssignCalls=0;
   nav.Initialise(&a,goal,start,end,requested_node,requested,search!=0,ExpectedPriority,ExpectedDistance);
   assert(AddCalls==(needsSearch?1:0));assert(DistanceCalls==(needsSearch?1:0));assert(GoalCalls==(needsSearch?0:1));assert(AssignCalls==(blocked?1:0));
   assert(EqualCalls==(search&&kind!=1?1:0));assert(NumberOfPeopleCalculatingRoute==10-(previous?1:0)+(needsSearch?1:0));
   assert(nav.GotRoute==!needsSearch&&nav.Culled==!needsSearch&&nav.PartWayThroughAStar==needsSearch);
   assert(nav.Initialised&&nav.OnRoute&&!nav.Interrupted&&nav.SlowedDownForLastPoint);assert(!nav.NoOfClockwiseTurns&&!nav.NoOfAntiClockwiseTurns);
   Point(nav.StringLeadPosition,start);Point(nav.LastWayPointVisited,start);Point(nav.GoalPosition,end);assert(nav.GoalNode==goal);
   assert(nav.PosAskedToGetTo.X==requested.X&&nav.PosAskedToGetTo.Y==requested.Y&&nav.PosAskedToGetTo.Z==requested.Z);
   assert(nav.NodeAskedToGetTo==(blocked?static_cast<const CANavQuadTreeNode*>(&nav.BlockedNodeAskedToGetTo):requested_node));
   if(blocked){Point(nav.BlockedNodeAskedToGetTo.Position,req.Position);assert(nav.BlockedNodeAskedToGetTo.NavLayer==req.NavLayer);}
   assert(manager.QueueManager.Navigators.size()==(needsSearch?2:1));assert(manager.QueueManager.Navigators.front().second==0);
   if(needsSearch){assert(nav.Route.empty()&&nav.States.size()==1&&nav.OpenList.Size()==1);const CNavigatorAStar::CAStarState&s=nav.States.begin()->second;assert(s.GetNode()==&a&&!s.GetParentState()&&s.GetCostToState()==0&&s.GetTotalCost()==8&&s.IsOnOpen()&&!s.IsOnClosed());assert(nav.OpenList.Heap.front()==&s);}
   else{assert(nav.States.size()==0&&nav.OpenList.IsEmpty()&&nav.Route.size()==2);Point(nav.Route.front().Position,start);Point(nav.Route.back().Position,end);assert(nav.Route.front().NavLayer==layer&&nav.Route.back().NavLayer==layer&&nav.Route.front().Removable&&nav.Route.back().Removable);Point(steering.Goal,end);Point(steering.NextGoal,end);}
   // Both cull iterators were captured while Route was empty, before direct points were inserted.
   assert(nav.CullFront==nav.Route.end()&&nav.CullBack==nav.Route.end());++cases;
  }
  assert(CNavigatorAStar::CAStarState::NumberOfAStarStates==0&&CNavigatorAStar::CAStarState::NumberOfClosedStates==0&&CNavigatorAStar::CAStarState::NumberOfOpenStates==0);
 }
 assert(cases==576);puts("00a1ceb0_TEST PASS");return 0;}