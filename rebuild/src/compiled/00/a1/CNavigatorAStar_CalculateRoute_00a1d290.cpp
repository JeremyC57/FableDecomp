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