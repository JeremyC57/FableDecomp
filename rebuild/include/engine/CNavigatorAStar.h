#ifndef FABLE_ENGINE_CNAVIGATORASTAR_H
#define FABLE_ENGINE_CNAVIGATORASTAR_H
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
float FABLE_FASTCALL GFGet2DDistance(const C2DVector&,const C2DVector&);
#endif
#include "engine/ENavigatorReturn.h"
#include "engine/CNavigatorBase.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/CNavWayPoint.h"
#include "engine/CSteeringAgent.h"
#include "engine/CPriorityQueue.h"
#include "stlport/fable_map.h"
// Layout: both original PDB types; native states24,three blocked nodes30/40/50,
// OpenList60,Route8c,flagsbc..c3. Real containers replace inherited opaque arrays.
class CNavigatorAStar:public CNavigatorBase {
private:
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_CTOR_PASS23
 // Both PDBs: Ego_r00b88fc0 and FableWin031a7370; original private constructor.
 CNavigatorAStar(CNavigatorManager&,const CNavigatorExternalInterface&);
#endif
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_INTERSECTIONS_PASS23
 // Both original PDB publics: Ego_r00b85d40, FableWin031a8110; native caller00a1cd30.
 std::pair<C2DVector,C2DVector> GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode&,const CANavQuadTreeNode&,float) const;
#endif
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
public:
#endif
 class CAStarState {
 private:
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
public:
 CAStarState():ParentState(0),Node(0),CostToThisState(0),TotalCost(0),OnOpenList(false),OnClosedList(false){++NumberOfAStarStates;}
 CAStarState(const CANavQuadTreeNode* node,CAStarState* parent):ParentState(parent),Node(node),CostToThisState(0),TotalCost(0),OnOpenList(false),OnClosedList(false){++NumberOfAStarStates;}
 CAStarState(const CAStarState& other):ParentState(other.ParentState),Node(other.Node),CostToThisState(other.CostToThisState),TotalCost(other.TotalCost),OnOpenList(other.OnOpenList),OnClosedList(other.OnClosedList){++NumberOfAStarStates;if(IsOnOpen())++NumberOfOpenStates;if(IsOnClosed())++NumberOfClosedStates;}
 ~CAStarState(){--NumberOfAStarStates;if(IsOnOpen())--NumberOfOpenStates;if(IsOnClosed())--NumberOfClosedStates;}
 bool IsOnOpen() const{return OnOpenList;}
 bool IsOnClosed() const{return OnClosedList;}
 void SetOnOpen(){if(IsOnClosed())--NumberOfClosedStates;OnOpenList=true;OnClosedList=false;++NumberOfOpenStates;}
 void SetOnClosed(){if(IsOnOpen())--NumberOfOpenStates;OnClosedList=true;OnOpenList=false;++NumberOfClosedStates;}
 const C2DVector& GetPosition() const{return Node->GetPosition();}
 const CANavQuadTreeNode* GetNode() const{return Node;}
 const CAStarState* GetParentState() const{return ParentState;}
 float GetTotalCost() const{return TotalCost;}
 float GetCostToState() const{return CostToThisState;}
 void SetCostToThisState(float cost){CostToThisState=cost;}
 void SetTotalCost(float cost){TotalCost=cost;}
 void SetParentState(CAStarState* state){ParentState=state;}
 static long NumberOfAStarStates,NumberOfClosedStates,NumberOfOpenStates;
 static long MaximumNumberOfStates,NumberOfClosedStatesDuringMaximumNumberOfStates,NumberOfOpenStatesDuringMaximumNumberOfStates;
private:
#endif
    CNavigatorAStar::CAStarState * ParentState;
    const CANavQuadTreeNode * Node;
    float CostToThisState;
    float TotalCost;
    bool OnOpenList;
    bool OnClosedList;
 };
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
private:
#endif
 std::map<long,CNavigatorAStar::CAStarState,std::less<long>,std::allocator<std::pair<long const ,CNavigatorAStar::CAStarState> > > States;
 CNavBlockedQuadTreeNode BlockedStartNode;
 CNavBlockedQuadTreeNode BlockedEndNode;
 CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;
 CPriorityQueue<CNavigatorAStar::CAStarState *> OpenList;
 const CANavQuadTreeNode * GoalNode;
 C2DVector GoalPosition;
 const CANavQuadTreeNode * NodeAskedToGetTo;
 C3DVector PosAskedToGetTo;
 const CANavQuadTreeNode * LastNavigableNodeVisited;
 std::list<CNavWayPoint,std::allocator<CNavWayPoint> > Route;
#ifdef FABLE_RETAIL_CREATURE_NAV_ROUTE_PASS29
public:
 const std::list<CNavWayPoint>* GetRoute() {return &Route;}
private:
#endif
 std::list<CNavWayPoint>::iterator CullFront;
 std::list<CNavWayPoint>::iterator CullBack;
 C2DVector LastWayPointVisited;
 C2DVector StringLeadPosition;
 C3DVector LastKnownNavPos;
 float LastKnownNavSpeed;
 long LastKnownNavLayer;
 bool HeadingForActionPoint;
 bool Initialised;
 bool GotRoute;
 bool SlowedDownForLastPoint;
 bool Culled;
 bool PartWayThroughAStar;
 unsigned char NoOfClockwiseTurns;
 unsigned char NoOfAntiClockwiseTurns;
private:
 ENavigatorReturn CalculateAndMoveToPosProximity(const C3DVector &, const C3DVector &, float, float, C3DVector &, bool, bool, bool, bool, bool, bool, bool, float);
private:
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
 // Original FableWin031A7C80 heuristic and pointer/null/structural equality donor.
 float GetHeuristicCost(const CANavQuadTreeNode* from,const CANavQuadTreeNode* to) const {return GFGet2DDistance(from->GetPosition(),to->GetPosition())*2.0f;}
 bool IsEqual(const CANavQuadTreeNode* a,const CANavQuadTreeNode* b) const {if(a==b)return true;if(!a||!b)return false;return *a==*b;}
 float GetCostFromNodeToNode(const CANavQuadTreeNode*,const CANavQuadTreeNode*,bool) const;
#if defined(FABLE_RETAIL_CNAVIGATORASTAR_LOOKUP_REFERENCE_PASS23)
 // Original FableWin S_BPREL32 local state is CAStarState&, not a pointer.
 CAStarState* GetStateFromOpenOrClosedList(const CANavQuadTreeNode* node) {std::map<long,CAStarState>::iterator i=States.find((long)node);if(i!=States.end()){CAStarState& state=i->second;return &state;}return 0;}
#else
 CAStarState* GetStateFromOpenOrClosedList(const CANavQuadTreeNode* node) {std::map<long,CAStarState>::iterator i=States.find((long)node);if(i!=States.end()){CAStarState* state=&i->second;return state;}return 0;}
#endif
 void ConstructFinalRoute(const CAStarState*);
#else
 bool IsEqual(const CANavQuadTreeNode *, const CANavQuadTreeNode *) const;
#endif
private:
 bool IsRouteClear(const CNavWayPoint &, const CNavWayPoint &, bool) const;
private:
 const CANavQuadTreeNode * GetNodeAt(const C3DVector &) const;
 // Both PDBs and original FableWin031B0BA0 forwarding body.
 const CANavQuadTreeNode* GetMyNodeAt(const C3DVector& pos) const {return GetNodeAt(pos);}
private:
 void Initialise(const CANavQuadTreeNode *, const CANavQuadTreeNode *, const C2DVector &, const C2DVector &, const CANavQuadTreeNode *, const C3DVector &, bool, bool, float);
private:
 bool CalculateRoute(bool);
private:
 void CullWayPoints(long);
private:
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_MOVETOWARD_PASS26
 float GetSpeedReducedForStartAndEnd(float speed,float proximity,const C2DVector& start_pos);
#endif
 void MoveTowardNextWayPoint(const C3DVector &, float, float, C3DVector &, bool, bool, bool, bool);
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_RANDOMPOS
private:
    C3DVector GetPosInDirection(const C3DVector&, const C2DVector&, float) const;
public:
    // Retail opt-in (Codex pass 24): both PDBs and the native virtual dispatch.
    virtual C3DVector GetRandomNavigablePos(const C3DVector&, float, bool) const;
    virtual C3DVector GetRandomNavigablePos(const C3DVector&, float, const C2DVector&, float) const;
#endif
public:
 virtual void UpdateLastPosition(const C3DVector &);
public:
 void UpdateGoal(const C3DVector &, C2DVector, C2DVector);
public:
 void CalculateNavigableWidthOfLine(const C3DVector&);
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_MOVETOWARD_PASS26
 void CalculateNavigableWidthOfLine(const CNavWayPoint&,const CNavWayPoint&,float&,float&,bool);
#endif
};
#ifdef FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23
class CAStarStateTotalGreaterThan:public std::binary_function<const CNavigatorAStar::CAStarState*,const CNavigatorAStar::CAStarState*,bool> {
public:bool operator()(const CNavigatorAStar::CAStarState* a,const CNavigatorAStar::CAStarState* b) const{return a->GetTotalCost()>b->GetTotalCost();}
};
#endif
FABLE_STATIC_ASSERT_AT(75, sizeof(CNavigatorAStar)==0xc4);
#endif
