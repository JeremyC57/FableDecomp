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
static float MinCost,MaxCost;
// 00a1d290 CNavigatorAStar::CalculateRoute
// Leaf: ?CalculateRoute@CNavigatorAStar@@AAE_N_N@Z
// Flags: none
// Inline-budget: native calls Push and tree find; correct visible bodies give890B plain,812/833/890/1010 at T30/50/100/150. Existing narrow call controls retain817B.
// Supplementary context: original lookup reference; native heap wrappers/ABI.
// Fidelity: distinct inserted-state pointer lifetime restores native stack homes.
// Fidelity: grouping the heuristic call result restores native floating-result store order.
#define FABLE_RETAIL_NAV_NODETYPE_ORIGINAL_POLYMORPHISM_PASS23
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_ASTAR_PUSHHEAP_PASS23
#define FABLE_RETAIL_STLP_ASTAR_REAL_PUSH_HEAP_PASS23
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_QUEUE_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_POP_HELPER_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_API_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_PUSH_RANGE_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_ELEMENT_FIRST_PASS23
#define FABLE_RETAIL_CPRIORITYQUEUE_PUSH_CALL_PASS23
#define FABLE_RETAIL_STLP_TREE_ASTAR_FIND_CALL_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_LOOKUP_REFERENCE_PASS23
#include "engine/CNavigatorAStar.h"
// Original TU-local S_LDATA32 in both lib_navigator_a_star modules.
extern long NumberOfPeopleCalculatingRoute;
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_ITERATORS_PASS23
#include "engine/CNavNavigableLeafNode.h"
extern long NumberOfPeopleCalculatingRouteDuringMaximumNumberOfStates;
extern long CallsLeftBeforeDisplayingCounters;
// Genuine same-TU helper; independently151/151 relocation-masked exact at already-landed00a1a0a0.
float CNavigatorAStar::GetCostFromNodeToNode(const CANavQuadTreeNode* from,const CANavQuadTreeNode* to,bool ignore_path_preferability) const
{
 float distance=GFGet2DDistance(from->GetPosition(),to->GetPosition());
 float factor=1.0f;
 if(!ignore_path_preferability){switch(to->GetPreferabilityCost()){
 case 0:factor=PInterface->GetMinPathCost();break;
 case 64:factor=(PInterface->GetMaxPathCost()+PInterface->GetMinPathCost())*0.5f;break;
 case 128:factor=PInterface->GetMaxPathCost();break;
 default:factor=PInterface->GetMaxPathCost();break;
 }}
 return factor*distance;
}

bool CNavigatorAStar::CalculateRoute(bool ignore_path_preferability)
{
 if(CAStarState::NumberOfAStarStates>CAStarState::MaximumNumberOfStates){
  CAStarState::MaximumNumberOfStates=CAStarState::NumberOfAStarStates;
  CAStarState::NumberOfClosedStatesDuringMaximumNumberOfStates=CAStarState::NumberOfClosedStates;
  CAStarState::NumberOfOpenStatesDuringMaximumNumberOfStates=CAStarState::NumberOfOpenStates;
  NumberOfPeopleCalculatingRouteDuringMaximumNumberOfStates=NumberOfPeopleCalculatingRoute;
 }
 if(--CallsLeftBeforeDisplayingCounters<=0)CallsLeftBeforeDisplayingCounters=50;
 if(!NavigationManager.GetNavigatorQueueManager().IsNavigatorAllowedToCalculateRoute(*this)){
  PartWayThroughAStar=true;return false;
 }
 while(!OpenList.IsEmpty()){
  CAStarState* best_state=OpenList.Pop(CAStarStateTotalGreaterThan());
  if(IsEqual(best_state->GetNode(),GoalNode)){
   ConstructFinalRoute(best_state);GotRoute=true;return true;
  }
  const CSmallVector<const CANavQuadTreeNode*,8>& neighbours=*best_state->GetNode()->GetNeighbours();
  for(CSmallVector<const CANavQuadTreeNode*,8>::const_iterator it=neighbours.begin();it!=neighbours.end();++it){
   --NavigationManager.NumberOfNodeExaminationsLeftThisFrame;
   const CANavQuadTreeNode* current_node=*it;
   if(!current_node->IsNavigable())continue;
   CAStarState* existing_state=GetStateFromOpenOrClosedList(current_node);
   if(existing_state){
    if(best_state->GetParentState()==existing_state||existing_state->IsOnClosed())continue;
    float new_cost=GetCostFromNodeToNode(best_state->GetNode(),existing_state->GetNode(),ignore_path_preferability);
    new_cost+=best_state->GetCostToState();
    if(new_cost<existing_state->GetCostToState()){
     existing_state->SetCostToThisState(new_cost);
     existing_state->SetTotalCost((GetHeuristicCost(existing_state->GetNode(),GoalNode))+new_cost);
     existing_state->SetParentState(best_state);
     OpenList.UpdateElement(existing_state,CAStarStateTotalGreaterThan());
    }
   }else{
    CAStarState new_state(current_node,best_state);
    float edge_cost=GetCostFromNodeToNode(best_state->GetNode(),current_node,ignore_path_preferability);
    new_state.SetCostToThisState(edge_cost+best_state->GetCostToState());
    float heuristic=GetHeuristicCost(current_node,GoalNode);
    new_state.SetTotalCost(heuristic+new_state.GetCostToState());
    new_state.SetOnOpen();
    std::pair<std::map<long,CAStarState>::iterator,bool> state_it=States.insert(std::make_pair((long)current_node,new_state));
    CAStarState* inserted_state=&state_it.first->second;
    OpenList.Push(inserted_state,CAStarStateTotalGreaterThan());
   }
  }
  best_state->SetOnClosed();
  if(NavigationManager.NumberOfNodeExaminationsLeftThisFrame<=0){PartWayThroughAStar=true;return false;}
 }
 return false;
}

#undef private
#undef protected
static const CNavigatorExternalInterface* ActiveInterface;static int MinCalls,MaxCalls;
long NumberOfPeopleCalculatingRoute=0;
long CNavigatorAStar::CAStarState::NumberOfAStarStates=0;
long CNavigatorAStar::CAStarState::NumberOfClosedStates=0;
long CNavigatorAStar::CAStarState::NumberOfOpenStates=0;
long CNavigatorAStar::CAStarState::MaximumNumberOfStates=0;
long CNavigatorAStar::CAStarState::NumberOfClosedStatesDuringMaximumNumberOfStates=0;
long CNavigatorAStar::CAStarState::NumberOfOpenStatesDuringMaximumNumberOfStates=0;
namespace std {
template<class T,class A>template<class F>void list<T,A>::remove_if(F predicate){iterator i=begin();while(i!=end()){iterator current=i++;if(predicate(*current))erase(current);}}
template<class D>void _Rb_global<D>::_Rebalance(_Rb_tree_node_base* node,_Rb_tree_node_base*&root){assert(root&&node&&!node->_M_left&&!node->_M_right);root->_M_color=true;node->_M_color=true;}
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
float GetMinPathCost() const{assert(this==ActiveInterface);++MinCalls;return MinCost;}
float GetMaxPathCost() const{assert(this==ActiveInterface);++MaxCalls;return MaxCost;}
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
// Original base slot is pure; concrete blocked node always reports its native type.
ENavQuadTreeNodeType CNavBlockedQuadTreeNode::GetNodeType() const{return NAV_QUAD_TREE_BLOCKED_NODE;}
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


long NumberOfPeopleCalculatingRouteDuringMaximumNumberOfStates=0;
long CallsLeftBeforeDisplayingCounters=50;
namespace std {
template<class I,class D,class T,class C>void __fastcall __adjust_heap(I first,D hole,D len,T value,C compare){
 D top=hole,child=2*hole+2;
 while(child<len){if(compare(first[child],first[child-1]))--child;first[hole]=first[child];hole=child;child=2*(child+1);}
 if(child==len){first[hole]=first[child-1];hole=child-1;}
 __push_heap(first,hole,top,value,compare);
}
}
template<class T,int N>CSmallVector<T,N>::CSmallVector():Start(0),Size(0){}
template<class T,int N>CSmallVector<T,N>::~CSmallVector(){assert(Size>=0&&Size<=N);}
struct FixtureNode:CANavQuadTreeNode {
 // CalculateRoute tests the original Blocked field; this slot is never called here.
 ENavQuadTreeNodeType GetNodeType()const{abort();}
 CSmallVector<const CANavQuadTreeNode*,8> Adjacent;
 const CANavQuadTreeNode* Links[8];unsigned char Preference;
 FixtureNode(){Adjacent.Start=Links;Adjacent.Size=0;Position.X=Position.Y=0;QuadTreeLevel=0;NavLayer=0;LeafNode=true;Blocked=false;Preference=0;}
 const CSmallVector<const CANavQuadTreeNode*,8>* GetNeighbours() const{return &Adjacent;}
 unsigned char GetPreferabilityCost() const{return Preference;}
 void Link(const CANavQuadTreeNode&n){assert(Adjacent.Size<8);Links[(unsigned char)Adjacent.Size++]=&n;}
};
static CNavigatorAStar* Active;
static bool Allowed;static int AllowedCalls,FinalCalls;
static FixtureNode* Nodes;static int CapturedPath[8],CapturedLength;static float CapturedCost;
static int NodeID(const CANavQuadTreeNode*n){if(!n)return -1;for(int i=0;i<6;++i)if(n==&Nodes[i])return i;abort();}
float __fastcall GFGet2DDistance(const C2DVector&a,const C2DVector&b){bool aa=false,bb=false;for(int i=0;i<6;++i){aa=aa||&a==&Nodes[i].Position;bb=bb||&b==&Nodes[i].Position;}assert(aa&&bb);assert(a.Y==0&&b.Y==0);return fabsf(a.X-b.X);}
bool CANavQuadTreeNode::operator==(const CANavQuadTreeNode&b)const{return Position.X==b.Position.X&&Position.Y==b.Position.Y&&NavLayer==b.NavLayer;}
bool CNavigatorQueueManager::IsNavigatorAllowedToCalculateRoute(const CNavigatorBase&n)const{assert(this==&Active->NavigationManager.QueueManager&&&n==Active);assert(Active->PInterface==ActiveInterface);++AllowedCalls;return Allowed;}
void CNavigatorAStar::ConstructFinalRoute(const CAStarState*s){assert(this==Active&&!GotRoute);assert(s->GetNode()==GoalNode||*s->GetNode()==*GoalNode);++FinalCalls;CapturedCost=s->GetCostToState();CapturedLength=0;while(s){assert(CapturedLength<8);CapturedPath[CapturedLength++]=NodeID(s->GetNode());s=s->GetParentState();}}
struct Oracle {
 bool Have[6],Open[6],Closed[6],Queued[6],Found,PartWay;
 float Cost[6],Total[6];int Parent[6],Goal,Budget,Path[8],Length;bool Ignore;
 Oracle(bool ignore,int goal):Found(false),PartWay(false),Goal(goal),Ignore(ignore){for(int i=0;i<6;++i){Have[i]=Open[i]=Closed[i]=Queued[i]=false;Cost[i]=Total[i]=0;Parent[i]=-1;}Have[0]=Open[0]=Queued[0]=true;}
 float Edge(int from,int to){float factor=Ignore?1:Nodes[to].Preference==0?MinCost:Nodes[to].Preference==64?(MinCost+MaxCost)*.5f:MaxCost;return fabsf(Nodes[from].Position.X-Nodes[to].Position.X)*factor;}
 bool Frame(int budget){Budget=budget;
  for(;;){int id=-1;for(int i=0;i<6;++i)if(Queued[i]&&(id<0||Total[i]<Total[id]))id=i;
   if(id<0)return false;Queued[id]=false;
   if(id==Goal){Found=true;Length=0;for(int p=id;p>=0;p=Parent[p]){assert(Length<8);Path[Length++]=p;}return true;}
   for(int k=0;k<Nodes[id].Adjacent.Size;++k){int n=NodeID(Nodes[id].Links[k]);--Budget;if(Nodes[n].Blocked)continue;
    if(Have[n]&&(Parent[id]==n||Closed[n]))continue;
    float c=Cost[id]+Edge(id,n);
    if(!Have[n]||c<Cost[n]){Have[n]=true;Open[n]=Queued[n]=true;Closed[n]=false;Cost[n]=c;Total[n]=c+2*fabsf(Nodes[n].Position.X-Nodes[Goal].Position.X);Parent[n]=id;}
   }
   Closed[id]=true;Open[id]=false;
   if(Budget<=0){PartWay=true;return false;}
  }
 }
};
static void Compare(CNavigatorAStar&nav,const Oracle&o){
 int states=0,open=0,closed=0,queued=0;
 for(int i=0;i<6;++i){std::map<long,CNavigatorAStar::CAStarState>::iterator it=nav.States.find((long)&Nodes[i]);assert((it!=nav.States.end())==o.Have[i]);if(!o.Have[i])continue;++states;
  const CNavigatorAStar::CAStarState&s=it->second;assert(s.GetNode()==&Nodes[i]);assert(s.GetCostToState()==o.Cost[i]);if(i)assert(s.GetTotalCost()==o.Total[i]);assert(s.IsOnOpen()==o.Open[i]&&s.IsOnClosed()==o.Closed[i]);assert((s.GetParentState()?NodeID(s.GetParentState()->GetNode()):-1)==o.Parent[i]);open+=o.Open[i];closed+=o.Closed[i];queued+=o.Queued[i];
 }
 assert(nav.States.size()==(unsigned long)states&&nav.OpenList.Size()==(unsigned long)queued);assert(CNavigatorAStar::CAStarState::NumberOfAStarStates==states&&CNavigatorAStar::CAStarState::NumberOfOpenStates==open&&CNavigatorAStar::CAStarState::NumberOfClosedStates==closed);
 assert(nav.GotRoute==o.Found&&nav.PartWayThroughAStar==o.PartWay);assert(nav.NavigationManager.NumberOfNodeExaminationsLeftThisFrame==o.Budget);
 if(o.Found){assert(FinalCalls==1&&CapturedLength==o.Length);assert(CapturedCost==o.Cost[o.Goal]);for(int i=0;i<o.Length;++i)assert(CapturedPath[i]==o.Path[i]);}
}
int main(){unsigned int cases=0,frames=0;
 const int budgets[]={-2,0,1,3,8,100};const long countdowns[]={-3,0,1,2,50};
 for(int ignore=0;ignore<2;++ignore)for(int scale=1;scale<=4;++scale)for(int bi=0;bi<6;++bi)for(int ci=0;ci<5;++ci)for(int reachable=0;reachable<2;++reachable)for(int preference=0;preference<2;++preference){
  {CNavigatorManager manager;FixtureInterface external;FixtureNavigator nav(manager,external);FixtureNode nodes[6];Nodes=nodes;
   const float x[]={0,2,5,11,50,70};for(int i=0;i<6;++i)nodes[i].Position.X=x[i];nodes[1].Preference=0;nodes[2].Preference=64;nodes[3].Preference=(unsigned char)(preference?255:128);nodes[4].Blocked=true;
   nodes[0].Link(nodes[1]);nodes[0].Link(nodes[2]);nodes[0].Link(nodes[4]);nodes[1].Link(nodes[0]);nodes[1].Link(nodes[3]);nodes[2].Link(nodes[1]);nodes[2].Link(nodes[3]);
   MinCost=2.0f*scale;MaxCost=4.0f*scale;int goal=reachable?3:5;nav.GoalNode=&nodes[goal];nav.GotRoute=false;nav.PartWayThroughAStar=false;
   {CNavigatorAStar::CAStarState start(&nodes[0],0);start.SetOnOpen();start.SetCostToThisState(0);start.SetTotalCost(0);nav.States.insert(std::make_pair((long)&nodes[0],start));}nav.OpenList.Push(&nav.States.begin()->second,CAStarStateTotalGreaterThan());
   Active=&nav;ActiveInterface=&external;Allowed=false;AllowedCalls=FinalCalls=MinCalls=MaxCalls=0;NumberOfPeopleCalculatingRoute=7;CallsLeftBeforeDisplayingCounters=countdowns[ci];CNavigatorAStar::CAStarState::MaximumNumberOfStates=0;
   long oldCount=CallsLeftBeforeDisplayingCounters;assert(!nav.CalculateRoute(ignore!=0));assert(AllowedCalls==1&&FinalCalls==0&&nav.PartWayThroughAStar&&nav.OpenList.Size()==1);assert(CallsLeftBeforeDisplayingCounters==(oldCount-1<=0?50:oldCount-1));
   assert(CNavigatorAStar::CAStarState::MaximumNumberOfStates==1&&CNavigatorAStar::CAStarState::NumberOfClosedStatesDuringMaximumNumberOfStates==0&&CNavigatorAStar::CAStarState::NumberOfOpenStatesDuringMaximumNumberOfStates==1&&NumberOfPeopleCalculatingRouteDuringMaximumNumberOfStates==7);
   nav.PartWayThroughAStar=false;Oracle oracle(ignore!=0,goal);Allowed=true;
   for(int frame=0;frame<8;++frame){int budget=budgets[bi];manager.NumberOfNodeExaminationsLeftThisFrame=budget;long old=CallsLeftBeforeDisplayingCounters;
    bool expected=oracle.Frame(budget),actual=nav.CalculateRoute(ignore!=0);assert(actual==expected);assert(CallsLeftBeforeDisplayingCounters==(old-1<=0?50:old-1));Compare(nav,oracle);++frames;
    if(actual||nav.OpenList.IsEmpty())break;assert(frame!=7);
   }
   if(reachable){assert(nav.GotRoute&&FinalCalls==1);assert(CapturedCost==(ignore?11.0f:39.0f*scale));}else assert(!nav.GotRoute&&FinalCalls==0);
   assert(ignore?(MinCalls==0&&MaxCalls==0):(MinCalls>0&&MaxCalls>0));++cases;
  }
  assert(CNavigatorAStar::CAStarState::NumberOfAStarStates==0&&CNavigatorAStar::CAStarState::NumberOfClosedStates==0&&CNavigatorAStar::CAStarState::NumberOfOpenStates==0);
 }
 assert(cases==960&&frames>960);puts("00a1d290_TEST PASS");return 0;}