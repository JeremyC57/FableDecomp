// 00a1aff0 CSteeringAgent::Process
// Leaf: ?Process@CSteeringAgent@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_STEER_KINEMATIC_CONSTRUCTOR_PASS29
#define FABLE_RETAIL_KINEMATIC_ASSIGN_PASS28
#define FABLE_RETAIL_STEERING_PROCESS_PASS29
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DVECTOR_EQUAL
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
#define FABLE_RETAIL_CSTEERINGAGENT_MOVETOWARD_PASS26
#define FABLE_RETAIL_CSBIAGENTS_PROCESS_PASS28
#include <stdlib.h>
#include "engine/CSteeringAgent.h"
#include "engine/CSBConstraint.h"
void CSteeringAgent::Process()
{
    static bool stop_if_slow = true;
    static float mult = 0.02f;
    if (NavigatorInterface == 0 || DummyObstacle)
        return;
    RecalcWidth = false;
    AddWaypoint = false;
    OnDirectApproach = false;
    C2DVector to_goal = GetGoal() - GetPosition();
    float dist_to_goal = to_goal.GetMagnitude();
    if (dist_to_goal < BestApproach) {
        BestApproach = dist_to_goal;
        TimeSinceBestApproach = 0;
    } else
        ++TimeSinceBestApproach;
    if (dist_to_goal < GetCurrentMaxSpeed()) {
        Steer = to_goal;
        for (std::list<CSBConstraint*>::iterator SB = SteeringBehaviourConstraints.begin(); SB != SteeringBehaviourConstraints.end(); ++SB) {
            (*SB)->SetInput(Steer);
            if (!(*SB)->Process(this)) {
                SetPathFailed();
                return;
            }
            Steer = (*SB)->GetOutput();
        }
        if (Steer == to_goal) {
            CurrentSpeed = Steer.GetMagnitude();
            return;
        }
    } else if (GetProximityForDirectApproach() > dist_to_goal) {
        OnDirectApproach = true;
        Steer = to_goal;
        if (dist_to_goal > GetCurrentMaxSpeed()) {
            Steer *= GetCurrentMaxSpeed() / dist_to_goal;
        }
        for (std::list<CSBConstraint*>::iterator SB = SteeringBehaviourConstraints.begin(); SB != SteeringBehaviourConstraints.end(); ++SB) {
            (*SB)->SetInput(Steer);
            if (!(*SB)->Process(this)) {
                SetPathFailed();
                return;
            }
            Steer = (*SB)->GetOutput();
        }
        CurrentSpeed = Steer.GetMagnitude();
    } else {
        if (to_goal.GetMagnitude() < 5.0f)
            DistCutoff = to_goal.GetMagnitude();
        else
            DistCutoff = 5.0f;
    }
    Steer.Initialise(0.0f,0.0f);
    for (std::list<CSB*>::iterator SB = SteeringBehaviours.begin(); SB != SteeringBehaviours.end(); ++SB) {
        if (IsWandering() && (*SB)->GetType() == CSB::ESB_GOAL)
            continue;
        if (!(*SB)->Process(this)) {
            SetPathFailed();
            return;
        }
        Steer += (*SB)->GetOutput();
    }
    for (std::list<CSBInput*>::iterator SBI = SteeringBehaviourInputs.begin(); SBI != SteeringBehaviourInputs.end(); ++SBI) {
        (*SBI)->SetInput(Steer);
        if (!(*SBI)->Process(this)) {
            SetPathFailed();
            return;
        }
        Steer = (*SBI)->GetOutput();
    }
    if (stop_if_slow && Steer.GetMagnitude() < GetCurrentMaxSpeed() * 0.1f) {
        Steer.Y=0.0f;
        Steer.X=0.0f;
    }
    if (Steer.GetMagnitude() > GetCurrentMaxSpeed()) {
        Steer.Normalise();
        Steer *= GetCurrentMaxSpeed();
    }
    bool MoveBeforeOrientation = true;
    for (std::list<CSBConstraint*>::iterator SB = SteeringBehaviourConstraints.begin(); SB != SteeringBehaviourConstraints.end(); ++SB) {
        (*SB)->SetInput(Steer);
        if (!(*SB)->Process(this)) {
            SetPathFailed();
            return;
        }
        Steer = (*SB)->GetOutput();
        if (!(*SB)->GetMoveBeforeOrientation()) MoveBeforeOrientation = false;
    }
    if (stop_if_slow && Steer.GetMagnitude() < mult * GetCurrentMaxSpeed()) {
        Steer.Y=0.0f;
        Steer.X=0.0f;
    }
    to_goal.Normalise();
    if (TimeToReacquireGoal > 0)
        --TimeToReacquireGoal;
    if (TimeToReacquireGoal == 0 && Wandering == true) {
        Wandering = false;
        BestApproach = (GetGoal()-GetPosition()).GetMagnitude();
        TimeSinceBestApproach = 0;
    }
    if (TimeSinceBestApproach != 0 && TimeSinceBestApproach % (NavigatorInterface->GetConstantFPS() * 3) == 0) {
        if (!AvoidDynamicObstaclesUntilNextWaypoint)
            SetPathFailed();
        else if (HasWanderedDuringThisSection) {
            AvoidDynamicObstaclesUntilNextWaypoint = false;
            HasWanderedDuringThisSection = false;
        } else {
            Wandering = true;
            TimeToReacquireGoal = NavigatorInterface->GetConstantFPS() * (rand() % 3 + 3);
            DistCutoff = 5.0f;
            HasWanderedDuringThisSection = true;
        }
    } else if (TimeSinceBestApproach == NavigatorInterface->GetConstantFPS() * 2)
        AddWaypoint = true;
    else if (TimeSinceBestApproach == NavigatorInterface->GetConstantFPS())
        RecalcWidth = true;
    CurrentSpeed = Steer.GetMagnitude();
}