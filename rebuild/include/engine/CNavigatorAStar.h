#ifndef FABLE_ENGINE_CNAVIGATORASTAR_H
#define FABLE_ENGINE_CNAVIGATORASTAR_H
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
 class CAStarState {
 private:
    CNavigatorAStar::CAStarState * ParentState;
    const CANavQuadTreeNode * Node;
    float CostToThisState;
    float TotalCost;
    bool OnOpenList;
    bool OnClosedList;
 };
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
 bool IsEqual(const CANavQuadTreeNode *, const CANavQuadTreeNode *) const;
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
 void MoveTowardNextWayPoint(const C3DVector &, float, float, C3DVector &, bool, bool, bool, bool);
public:
 virtual void UpdateLastPosition(const C3DVector &);
public:
 void UpdateGoal(const C3DVector &, C2DVector, C2DVector);
public:
 void CalculateNavigableWidthOfLine(const C3DVector&);
};
FABLE_STATIC_ASSERT(sizeof(CNavigatorAStar)==0xc4);
#endif
