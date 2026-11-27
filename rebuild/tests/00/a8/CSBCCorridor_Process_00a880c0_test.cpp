// 00a880c0 CSBCCorridor::Process
// Leaf: ?Process@CSBCCorridor@@UAE_NPBVCSteeringAgent@@@Z
// Flags: /Gr
#define FABLE_RETAIL_NAVIGATORASTAR_PASS21
#define FABLE_RETAIL_SB_CORRIDOR_PASS29
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_KINEMATIC_ASSIGN_PASS28
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT
#define FABLE_RETAIL_C2DLINEF_POINT_ON_LINE_PASS24
#define FABLE_RETAIL_C2DLINEF_SQUAREDDIST
#define FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
#define FABLE_RETAIL_CSBIAGENTS_PROCESS_PASS28
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_DECL_PASS26
// Test-only access exposes original private state for cache and boundary assertions.
#define private public
#define protected public
#include "engine/CSteeringAgent.h"
#include "engine/CSBCCorridor.h"
#include "engine/C2DLineF.h"
#undef private
#undef protected
#include <stdio.h>
#include <stdlib.h>
static bool* debuggerSwitch;
bool CSBCCorridor::Process(const CSteeringAgent* agent)
{
    static bool dont_use = false;
    debuggerSwitch = &dont_use;
    if (dont_use) {
        Output.SetVector(Input.GetVector());
        return true;
    }
    MoveBeforeOrientation = true;
    if (Walls.size() == 0 || CurrentEndPoint != agent->GetGoal() ||
        BestLeft != agent->PeekBestDistLeft() || BestRight != agent->PeekBestDistRight()) {
        CurrentEndPoint = agent->GetGoal();
        BestLeft = agent->PeekBestDistLeft();
        BestRight = agent->PeekBestDistRight();
        C2DVector to_end = CurrentEndPoint - agent->GetPosition();
        to_end.Normalise();
        C2DVector right(to_end.Y, -to_end.X);
        float radius = agent->GetRadius(false);
        C2DVector agent_pos = agent->GetPosition() - to_end * radius * 0.1f;
        static float error_dist = agent->GetForwardNavigationWidthExtension();
        C2DVector end_pos = CurrentEndPoint + to_end * radius * error_dist;
        float right_dist = agent->PeekBestDistRight();
        float left_dist = agent->PeekBestDistLeft();
        if (right_dist < radius * 1.1f)
            right_dist = radius * 1.1f;
        if (left_dist < radius * 1.1f)
            left_dist = radius * 1.1f;
        Walls.clear();
        Walls.push_back(CWall(agent_pos + right * right_dist, end_pos + right * right_dist, -right));
        Walls.push_back(CWall(agent_pos - right * left_dist, end_pos - right * left_dist, right));
        Walls.push_back(CWall(agent_pos - right * left_dist, agent_pos + right * right_dist, to_end));
        Walls.push_back(CWall(end_pos - right * left_dist, end_pos + right * right_dist, -to_end));
    }

    C2DVector velocity = Input.GetVector();
    for (std::list<CWall>::iterator wall = Walls.begin(); wall != Walls.end(); ++wall) {
        C2DVector point_on_line;
        C2DLineF line(wall->Point1, wall->Point2);
        point_on_line = line.GetPointOnInfiniteLine(agent->GetPosition());
        C2DVector to_wall = point_on_line - agent->GetPosition();
        float dist = to_wall.GetMagnitude() - agent->GetRadius(false);
        bool outside_wall = to_wall.Dot(wall->ToInternal) > 0.0f;
        if (outside_wall) {
            Output.SetVector(C2DVector(0.0f, 0.0f));
            Reset();
            return false;
        }
        if (agent->GetRadius(false) * 2.0f > dist) {
            if (agent->GetOrientation().Dot(to_wall) > 0.0f)
                MoveBeforeOrientation = false;
            if (!(velocity.Dot(wall->ToInternal) > 0.0f)) {
                MoveBeforeOrientation = false;
                C2DVector wall_vector = wall->Point2 - wall->Point1;
                wall_vector.Normalise();
                C2DVector up_wall = wall_vector * velocity.Dot(wall_vector);
                C2DVector towards_wall(0.0f, 0.0f);
                if (dist > 0.0f) {
                    towards_wall = to_wall;
                    towards_wall.Normalise();
                    towards_wall = towards_wall * velocity.Dot(towards_wall);
                    if (towards_wall.GetMagnitude() > dist * 0.5f) {
                        towards_wall.Normalise();
                        towards_wall *= dist * 0.5f;
                    }
                }
                velocity = up_wall + towards_wall;
            }
        }
    }
    Output = velocity;
    return true;
}

extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
CSB::~CSB() {}
void CSB::Reset() {}
void CSB::CreateLineList(std::vector<C3DVector>&) {}
CSB::ESB_TYPE CSBCCorridor::GetType() { return ESBC_CORRIDOR; }
void CSBCCorridor::CreateLineList(std::vector<C3DVector>&) {}
static unsigned resets=0;
void CSBCCorridor::Reset() { ++resets; Walls.clear(); }
CNavigatorExternalInterface::~CNavigatorExternalInterface() {}
CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface* navigator, bool dummy)
    : NavigatorInterface(navigator), DummyObstacle(dummy) {}
C2DVector& C2DVector::Normalise() {
    float length=GetMagnitude();
    if(length!=0.0f) { X/=length; Y/=length; }
    return *this;
}
C2DVector C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const {
    C2DVector direction=Point2-Point1;
    return Point1+direction*((point-Point1).Dot(direction)/direction.Dot(direction));
}
namespace std {
template<class T,class A> void list<T,A>::push_back(const T& x) { insert(end(),x); }
template<class T,class A> void _List_base<T,A>::clear() {
    _List_node<T>* cur=(_List_node<T>*)_M_node._M_data->_M_next;
    while(cur!=_M_node._M_data) {
        _List_node<T>* old=cur;
        cur=(_List_node<T>*)cur->_M_next;
        old->_M_data.~T();
        _M_node.deallocate(old,1);
    }
    _M_node._M_data->_M_next=_M_node._M_data;
    _M_node._M_data->_M_prev=_M_node._M_data;
}
}
class TestNavigator : public CNavigatorExternalInterface {
public:
    C2DVector pos, facing;
    mutable unsigned widthQueries;
    std::vector<const CSteeringAgent*> obstacles;
    TestNavigator():pos(0,0),facing(1,0),widthQueries(0) {}
    bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const { return 0; }
    float GetTerrainDesirabilityAt(const C3DVector&) const { return 0; }
    float GetTerrainHeightAt(const C3DVector&) const { return 0; }
    float GetThemeHeightAt(const C3DVector&) const { return 0; }
    float FindPosZAt(const C3DVector&,bool) const { return 0; }
    float FindPosZAtIncludingThemes(const C3DVector&,bool) const { return 0; }
    float PeekNavigatorRadius(bool) const { return 1.0f; }
    C2DVector PeekNavigatorOrientation() const { return facing; }
    C2DVector PeekNavigatorPosition2D() const { return pos; }
    C3DVector PeekNavigatorPosition3D() const { return C3DVector(0,0,0); }
    float PeekNavigatorMaxSpeed(const C3DVector&) const { return 0; }
    const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const { return obstacles; }
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() { return obstacles; }
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles() { return &obstacles; }
    const CSteeringAgent* GetPThingToNotAvoid() const { return 0; }
    float GetProximityForDirectApproach() const { return 0; }
    long GetNavigationLevelAt(const C3DVector&) const { return 0; }
    long GetConstantFPS() const { return 0; }
    float GetMinPathCost() const { return 0; }
    float GetMaxPathCost() const { return 0; }
    bool IsPlayer() const { return 0; }
    bool IsDead() const { return 0; }
    float GetForwardNavigationWidthExtension() const { ++widthQueries; return 0.5f; }
    CNavigatorExternalInterface* Clone() const { return 0; }
};
static void check(bool passed,int line) {
    if(!passed) { printf("failed at %d\n",line);exit(1); }
}
#define CHECK(x) check((x),__LINE__)
int main() {
    TestNavigator nav;
    CSteeringAgent agent(&nav,false);
    agent.Goal=C2DVector(10,0);
    agent.BestDistLeft=0; agent.BestDistRight=2;
    CSBCCorridor build;
    build.CurrentEndPoint=C2DVector(0,0);
    build.BestLeft=build.BestRight=0;
    build.SetInput(CKinematic(C2DVector(1,0)));
    CHECK(build.Process(&agent) && build.Walls.size()==4 && nav.widthQueries==1);
    std::list<CWall>::iterator wall=build.Walls.begin();
    CHECK(!(wall->Point1!=C2DVector(-0.1f,-2)) && !(wall->Point2!=C2DVector(10.5f,-2)) && !(wall->ToInternal!=C2DVector(0,1)));
    ++wall;
    CHECK(!(wall->Point1!=C2DVector(-0.1f,1.1f)) && !(wall->Point2!=C2DVector(10.5f,1.1f)));
    ++wall; CHECK(!(wall->ToInternal!=C2DVector(1,0)));
    ++wall; CHECK(!(wall->ToInternal!=C2DVector(-1,0)));
    for(int changed=0;changed<4;++changed) {
        if(changed==1) agent.Goal.X+=2;
        if(changed==2) agent.BestDistLeft+=3;
        if(changed==3) agent.BestDistRight+=1;
        CHECK(build.Process(&agent) && build.Walls.size()==4);
        CHECK(!(build.CurrentEndPoint!=agent.Goal) && build.BestLeft==agent.BestDistLeft && build.BestRight==agent.BestDistRight && nav.widthQueries==1);
    }
    *debuggerSwitch=true;
    build.SetInput(CKinematic(C2DVector(7,3)));
    CHECK(build.Process(&agent) && !(build.GetOutput()!=C2DVector(7,3)));
    *debuggerSwitch=false;
    for(int side=-1;side<=1;side+=2)
    for(int step=-2;step<=12;++step)
    for(int vx=-3;vx<=3;++vx)
    for(int vy=-2;vy<=2;++vy)
    for(int facing=-1;facing<=1;++facing) {
        CSBCCorridor c;
        c.CurrentEndPoint=agent.Goal; c.BestLeft=agent.BestDistLeft; c.BestRight=agent.BestDistRight;
        c.SetInput(CKinematic(C2DVector((float)vx,(float)vy)));
        nav.pos=C2DVector(side*step*0.25f,7); nav.facing=C2DVector((float)facing,0);
        c.Walls.push_back(CWall(C2DVector(0,0),C2DVector(0,1),C2DVector((float)side,0)));
        resets=0;
        bool active=c.Process(&agent);
        float distance=step*0.25f, gap=distance-1, x=(float)vx, y=(float)vy;
        bool moveFirst=true;
        if(step<0) {x=y=0;}
        else if(gap<2) {
            if(distance>0 && facing*side<0) moveFirst=false;
            if(vx*side<=0) {
                moveFirst=false;
                float along=(float)(vx*side);
                if(gap<=0) along=0;
                else if(fabs(along)>gap*0.5f) along=along<0?-gap*0.5f:gap*0.5f;
                x=along*side;
            }
        }
        CHECK(active==(step>=0) && resets==(step<0?1U:0U));
        CHECK(!(c.GetOutput()!=C2DVector(x,y)) && c.GetMoveBeforeOrientation()==moveFirst);
    }
    puts("00a880c0_TEST PASS");
    return 0;
}