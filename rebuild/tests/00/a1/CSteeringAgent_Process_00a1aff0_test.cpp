#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DVECTOR_EQUAL
#include "engine/C2DVector.h"
const float DELTA_NOTIONAL_ZERO=0.0001f;
C2DVector& C2DVector::Normalise(){
    const float n=GetMagnitude();if(n!=0){X/=n;Y/=n;}return *this;
}
static void check(bool ok){if(!ok) exit(2);}
static bool near(float a,float b){return fabs(double(a)-double(b))<0.00002;}
static bool damping;
static int failCall, calls, recordId[12];static C2DVector recordInput[12];
struct CKinematic{C2DVector v;CKinematic(const C2DVector& a):v(a){}};
struct CSteeringAgent;
struct CSB {
    enum ESB_TYPE { ESB_AGENTS,ESB_GOAL };
    int id;ESB_TYPE kind; C2DVector input,output;
    CSB(int n,ESB_TYPE k):id(n),kind(k),input(0,0),output(0,0){}
    bool Process(const CSteeringAgent*){recordId[calls]=id;recordInput[calls]=input;return ++calls!=failCall;}
    ESB_TYPE GetType()const{return kind;}
    const C2DVector& GetOutput()const{return output;}
};
struct CSBInput:CSB {
    CSBInput(int n):CSB(n,ESB_AGENTS){}
    void SetInput(CKinematic a){input=a.v;}
};
struct CSBConstraint:CSBInput{
    bool before,identity;CSBConstraint(int n):CSBInput(n),before(true),identity(false){}
    bool GetMoveBeforeOrientation()const{return before;}
    void SetInput(CKinematic a){input=a.v;if(identity)output=input;}
};
struct Interface {
    C2DVector position;long GetConstantFPS()const{return 2;}
};
struct CSteeringAgent {
    Interface* NavigatorInterface;
    bool DummyObstacle,RecalcWidth,AddWaypoint,OnDirectApproach,FailPath,Wandering,AvoidDynamicObstaclesUntilNextWaypoint,HasWanderedDuringThisSection;
    float CurrentSpeed,maxSpeed,direct,DistCutoff,BestApproach;
    int TimeSinceBestApproach,TimeToReacquireGoal;
    C2DVector Steer,Goal;
    std::list<CSBConstraint*> SteeringBehaviourConstraints;std::list<CSBInput*> SteeringBehaviourInputs;std::list<CSB*> SteeringBehaviours;
    C2DVector GetPosition()const{return NavigatorInterface->position;}
    const C2DVector& GetGoal()const{return Goal;}
    float GetCurrentMaxSpeed()const{return maxSpeed;}
    float GetProximityForDirectApproach()const{return direct;}
    bool IsWandering()const{return Wandering;}
    void SetPathFailed(){FailPath=true;}
    void Process();
};
void CSteeringAgent::Process()
{
    static bool stop_if_slow = true;
    static float mult = 0.02f;
    // Test access to the original writable tuning switch.
    stop_if_slow = damping;
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
struct Expected {
    bool failed,waypoint,width,direct,wander,avoid,tried;
    float steer,speed,best,cutoff;int frames,timer,n,ids[12];float inputs[12];
    bool call(int id,float value){
        ids[n]=id;inputs[n++]=value;if(n!=failCall)return true;failed=true;return false;
    }
};
static void predict(Expected& e,float distance,float speed,float proximity,int numC,int numF,int numI,float inputOut,bool identity,int randomFirst){
    e.width=e.waypoint=e.direct=false;
    if(distance<e.best){e.best=distance;e.frames=0;}else ++e.frames;
    if(distance<speed||distance<proximity){
        bool close=distance<speed;
        e.direct=!close;e.steer=!close&&distance>speed?speed:distance;
        for(int i=0;i<numC;++i){if(!e.call(10+i,e.steer))return;if(!identity)e.steer=1.0f+i;}
        if(close && e.steer==distance){e.speed=(float)fabs(e.steer);return;}
        if(!close)e.speed=(float)fabs(e.steer);
    }else e.cutoff=distance<5?distance:5;
    e.steer=0;
    for(int j=0;j<numF;++j){
        if(e.wander&&j==0)continue;
        if(!e.call(20+j,0))return;e.steer+=(j==0?0.125f:8.0f);
    }
    for(int k=0;k<numI;++k){if(!e.call(30+k,e.steer))return;e.steer=inputOut;}
    if(damping && fabs(e.steer)<speed*0.1f)e.steer=0;
    if(fabs(e.steer)>speed)e.steer=e.steer<0?-speed:speed;
    for(int h=0;h<numC;++h){if(!e.call(10+h,e.steer))return;if(!identity)e.steer=1.0f+h;}
    if(damping&&fabs(e.steer)<speed*0.02f)e.steer=0;
    if(e.timer>0)--e.timer;
    if(e.timer==0&&e.wander){e.wander=false;e.best=distance;e.frames=0;}
    if(e.frames && e.frames%6==0){
        if(!e.avoid)e.failed=true;
        else if(e.tried){e.avoid=false;e.tried=false;}
        else{e.wander=e.tried=true;e.timer=2*(randomFirst%3+3);e.cutoff=5;}
    }else if(e.frames==4)e.waypoint=true;
    else if(e.frames==2)e.width=true;
    e.speed=(float)fabs(e.steer);
}
int main(){
    unsigned long seed=7;
    for(unsigned step=0;step<32768;++step){
        seed=1664525UL*seed+1013904223UL;unsigned long key=seed;
        Interface owner;owner.position=C2DVector(0,0);
        CSteeringAgent c;c.NavigatorInterface=step%67==0?0:&owner;c.DummyObstacle=step%53==0;
        const float ds[]={0,1,2,3,5,6,9};float distance=ds[step%7];c.Goal=C2DVector(distance,0);
        c.maxSpeed=(key&1)?2.0f:4.0f;c.direct=(key&2)?6.0f:3.0f;c.CurrentSpeed=7;c.Steer=C2DVector(-7,0);
        c.RecalcWidth=c.AddWaypoint=c.OnDirectApproach=true;c.FailPath=false;
        c.Wandering=(key&4)!=0;c.AvoidDynamicObstaclesUntilNextWaypoint=(key&8)!=0;
        c.HasWanderedDuringThisSection=(key&16)!=0;damping=(key&32)!=0;
        c.TimeToReacquireGoal=(key>>6)%4;c.TimeSinceBestApproach=(key>>8)%7;
        c.BestApproach=(key&512)?0.0f:10.0f;c.DistCutoff=15;
        int nc=(key>>12)%3,nf=(key>>15)%3,ni=(key>>18)%2;bool identity=(key&0x100000)!=0;
        float inputOut=(key&0x200000)?-8.0f:0.03125f;
        CSBConstraint c0(10),c1(11);c0.identity=c1.identity=identity;c0.before=(key&256)!=0;c1.before=!c0.before;
        c0.output=C2DVector(1,0);c1.output=C2DVector(2,0);
        CSB f0(20,CSB::ESB_GOAL),f1(21,CSB::ESB_AGENTS);f0.output=C2DVector(0.125f,0);f1.output=C2DVector(8,0);
        CSBInput input(30);input.output=C2DVector(inputOut,0);
        if(nc)c.SteeringBehaviourConstraints.push_back(&c0);if(nc==2)c.SteeringBehaviourConstraints.push_back(&c1);
        if(nf)c.SteeringBehaviours.push_back(&f0);if(nf==2)c.SteeringBehaviours.push_back(&f1);if(ni)c.SteeringBehaviourInputs.push_back(&input);
        Expected e;e.failed=c.FailPath;e.width=c.RecalcWidth;e.waypoint=c.AddWaypoint;e.direct=c.OnDirectApproach;
        e.wander=c.Wandering;e.avoid=c.AvoidDynamicObstaclesUntilNextWaypoint;e.tried=c.HasWanderedDuringThisSection;
        e.steer=c.Steer.X;e.speed=c.CurrentSpeed;e.best=c.BestApproach;e.cutoff=c.DistCutoff;e.frames=c.TimeSinceBestApproach;e.timer=c.TimeToReacquireGoal;
        e.n=calls=0;failCall=(key>>24)%9;srand(42);int randomFirst=rand();srand(42);
        if(c.NavigatorInterface&&!c.DummyObstacle)predict(e,distance,c.maxSpeed,c.direct,nc,nf,ni,inputOut,identity,randomFirst);
        c.Process();check(calls==e.n);for(int i=0;i<calls;++i){check(recordId[i]==e.ids[i]&&near(recordInput[i].X,e.inputs[i])&&recordInput[i].Y==0);}
        check(c.FailPath==e.failed&&c.RecalcWidth==e.width&&c.AddWaypoint==e.waypoint&&c.OnDirectApproach==e.direct);
        check(c.Wandering==e.wander&&c.AvoidDynamicObstaclesUntilNextWaypoint==e.avoid&&c.HasWanderedDuringThisSection==e.tried);
        check(c.TimeToReacquireGoal==e.timer&&c.TimeSinceBestApproach==e.frames);
        check(near(c.Steer.X,e.steer)&&c.Steer.Y==0&&near(c.CurrentSpeed,e.speed)&&near(c.BestApproach,e.best)&&near(c.DistCutoff,e.cutoff));
    }
    puts("00a1aff0_TEST PASS");return 0;
}