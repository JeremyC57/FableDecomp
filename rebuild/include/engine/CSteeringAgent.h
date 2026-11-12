#ifndef FABLE_ENGINE_CSTEERINGAGENT_H
#define FABLE_ENGINE_CSTEERINGAGENT_H
#include "engine/CNavigatorExternalInterface.h"
class CSB;class CSBInput;class CSBConstraint;
// Layout: original Ego_r private fields, adjusted only for retail4-byte STLport lists.
// Original donor031A9B40/031AC5D0/031AC5F0 confirms Reset and both getters.
class CSteeringAgent {
private:
    const CNavigatorExternalInterface * NavigatorInterface;
    bool DummyObstacle;
    float RepulsionMultiplier;
    std::list<CSB *,std::allocator<CSB *> > SteeringBehaviours;
    std::list<CSBInput *,std::allocator<CSBInput *> > SteeringBehaviourInputs;
    std::list<CSBConstraint *,std::allocator<CSBConstraint *> > SteeringBehaviourConstraints;
    float CurrentMaxSpeed;
    float CurrentSpeed;
    C2DVector Goal;
    C2DVector NextGoal;
    int TimeToReacquireGoal;
    float DistCutoff;
    C2DVector Steer;
    float BestDistLeft;
    float BestDistRight;
    float BestNextDistLeft;
    float BestNextDistRight;
    float ProximityForArrival;
    int TimeSinceBestApproach;
    float BestApproach;
    bool Wandering;
    bool MovingForwards;
    bool RecalcWidth;
    bool AddWaypoint;
    bool AvoidDynamicObstacles;
    bool AvoidDynamicObstaclesUntilNextWaypoint;
    bool HasWanderedDuringThisSection;
    bool FailPath;
    bool OnDirectApproach;
public:
 bool HasFailedPath() {return FailPath;}
 void Reset() {TimeSinceBestApproach=0;HasWanderedDuringThisSection=false;AvoidDynamicObstaclesUntilNextWaypoint=true;FailPath=false;}
 float GetProximityForArrival() {return ProximityForArrival;}
 void SetProximityForArrival(float proximity) {ProximityForArrival=proximity;}
 void SetGoal(const C2DVector &, const C2DVector &);
 bool IsCurrentNavigabilityBetter();
#ifdef FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
 // Retail opt-in (chunk Q17, row 0x00A87700 CSBCAgents::Process): the FableWin.pdb inline accessors (no Ego_r.pdb or Fable.exe copy),
 // the bodies the landed CSBAgents::Process 0x00A873E0 models: ?GetTarget@CSteeringAgent@@QBEPBV1@XZ (slot 14 GetPThingToNotAvoid),
 // ?IsPlayer@CSteeringAgent@@QBE_NXZ (slot 20), ?GetPosition@CSteeringAgent@@QBE?AVC2DVector@@XZ (slot 8 PeekNavigatorPosition2D),
 // ?GetRadius@CSteeringAgent@@QBEM_N@Z (slot 6 PeekNavigatorRadius), ?IsWandering@CSteeringAgent@@QBE_NXZ (+0x5C) and
 // ?GetAvoidDynamicObstacles@CSteeringAgent@@QBE_NXZ (+0x60). No layout change.
 const CSteeringAgent* GetTarget() const {return NavigatorInterface->GetPThingToNotAvoid();}
 bool IsPlayer() const {return NavigatorInterface->IsPlayer();}
 C2DVector GetPosition() const {return NavigatorInterface->PeekNavigatorPosition2D();}
 float GetRadius(bool physical) const {return NavigatorInterface->PeekNavigatorRadius(physical);}
 bool IsWandering() const {return Wandering;}
 bool GetAvoidDynamicObstacles() const {return AvoidDynamicObstacles;}
#endif
};
FABLE_STATIC_ASSERT(sizeof(CSteeringAgent)==0x68);
#endif
