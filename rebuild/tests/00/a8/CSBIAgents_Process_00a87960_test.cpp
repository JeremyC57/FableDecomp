// 00a87960 CSBIAgents::Process
// Flags:
// Leaf: ?Process@CSBIAgents@@UAE_NPBVCSteeringAgent@@@Z
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DLINEF_SQUAREDDIST
#define FABLE_RETAIL_C2DLINEF_POINT_ON_LINE_PASS24
#define FABLE_RETAIL_KINEMATIC_ASSIGN_PASS28
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
#define FABLE_RETAIL_CSBIAGENTS_PROCESS_PASS28
#include "engine/CSBIAgents.h"
#include "engine/C2DLineF.h"

// The test assigns the original function-local debug switch, which has no public setter.
static bool test_disable;
bool CSBIAgents::Process(const CSteeringAgent* agent)
{
    static bool dont_use;
    dont_use = test_disable;
    if (dont_use) {
        Output.SetVector(Input.GetVector());
        return true;
    }
    C2DVector point_on_max_dist(Input.GetVector());
    point_on_max_dist.Normalise();
    C2DVector right(point_on_max_dist);
    right.Rotate(0.25f);
    point_on_max_dist = agent->GetPosition() + point_on_max_dist * agent->GetDistCutoff();
    float best_to_agent = 0.0f;
    const CSteeringAgent* best_agent = 0;
    const CSteeringAgent* target = agent->GetTarget();
    for (std::vector<const CSteeringAgent*>::const_iterator agent_it = Agents->begin();
         agent_it != Agents->end(); ++agent_it) {
        if ((*agent_it)->IsDead()) continue;
        if (ShouldIgnoreAgent(agent, *agent_it, target)) continue;
        C2DLineF line(agent->GetPosition(), point_on_max_dist);
        C2DVector pos_on_line = line.GetPointOnInfiniteLine((*agent_it)->GetPosition());
        float to_agent = (pos_on_line - agent->GetPosition()).GetMagnitude();
        float to_point_on_max_dist = (pos_on_line - point_on_max_dist).GetMagnitude();
        C2DVector to_line = pos_on_line - (*agent_it)->GetPosition();
        if (to_agent != 0 && to_agent < agent->GetDistCutoff() &&
            to_point_on_max_dist < agent->GetDistCutoff() &&
            to_line.GetMagnitude() < agent->GetRadius(false) * 3.0f) {
            if (best_to_agent == 0 || best_to_agent > to_agent) {
                best_to_agent = to_agent;
                best_agent = *agent_it;
            }
        }
    }
    if (!best_agent) {
        Output = Input;
        return true;
    }
    if ((agent->GetPosition() - best_agent->GetPosition()).GetMagnitude() -
        (agent->GetRadius(false) + best_agent->GetRadius(false)) < 0.0f) {
        Output = Input;
        return true;
    }
    C2DLineF line(agent->GetPosition(), point_on_max_dist);
    C2DVector pos_on_line = line.GetPointOnInfiniteLine(best_agent->GetPosition());
    float to_agent = (pos_on_line - agent->GetPosition()).GetMagnitude();
    float to_point_on_max_dist = (pos_on_line - point_on_max_dist).GetMagnitude();
    C2DVector to_line = pos_on_line - best_agent->GetPosition();
    C2DVector new_point;
    C2DVector augmentation;
    bool right_turn;
    if (to_line.Dot(right) > 0.0f) {
        augmentation = right * agent->GetRadius(false) * 4.0f;
        right_turn = true;
    } else {
        augmentation = -right * agent->GetRadius(false) * 4.0f;
        right_turn = false;
    }
    new_point = best_agent->GetPosition() + augmentation;
    C2DVector to_goal = agent->GetGoal() - agent->GetPosition();
    C2DVector right_of_goal(to_goal);
    right_of_goal.Rotate(0.25f);
    C2DVector from_line_to_new_pos = new_point - pos_on_line;
    if (from_line_to_new_pos.Dot(right) > 0.0f) {
        if (agent->PeekBestDistRight() < agent->GetRadius(false) * 5.0f &&
            agent->PeekBestDistLeft() > agent->PeekBestDistRight()) {
            augmentation = -augmentation;
            new_point = best_agent->GetPosition() + augmentation;
            right_turn = !right_turn;
        }
    } else {
        if (agent->PeekBestDistLeft() < agent->GetRadius(false) * 5.0f &&
            agent->PeekBestDistRight() > agent->PeekBestDistLeft()) {
            augmentation = -augmentation;
            new_point = best_agent->GetPosition() + augmentation;
            right_turn = !right_turn;
        }
    }
    C2DVector from_agent(augmentation);
    if (right_turn) from_agent.Rotate(0.25f);
    else from_agent.Rotate(-0.25f);
    from_agent.Normalise();
    C2DLineF perp_line(best_agent->GetPosition(), new_point);
    C2DVector point_on_line = perp_line.GetPointOnInfiniteLine(agent->GetPosition());
    float dist_from_augmentation_line = (point_on_line - agent->GetPosition()).GetMagnitude();
    float mult = dist_from_augmentation_line > agent->GetRadius(false) * 4.0f ?
        agent->GetRadius(false) * 4.0f : dist_from_augmentation_line;
    new_point += from_agent * mult;
    C2DVector to_point = new_point - agent->GetPosition();
    to_point.Normalise();
    Output.SetVector(Input.GetVector() + to_point);
    return true;
}

#include <stdio.h>
#include <math.h>
typedef std::vector<const CSteeringAgent*> AgentVector;
CNavigatorExternalInterface::~CNavigatorExternalInterface() {}
CSBIAgents::CSBIAgents(const AgentVector& agents) : Agents(&agents) {}
static float config_left=20.0f, config_right=20.0f;
CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface* nav, bool dummy)
: NavigatorInterface(nav), DummyObstacle(dummy), Goal(10.0f,0.0f), DistCutoff(10.0f),
  BestDistLeft(config_left), BestDistRight(config_right) {}
bool CSB::ShouldIgnoreAgent(const CSteeringAgent* agent, const CSteeringAgent* other,
                            const CSteeringAgent* target) { return other==target || agent==other; }
void C2DVector::Rotate(float turns) {
    float x=X;
    if(turns==0.25f){X=-Y;Y=x;}
    else if(turns==-0.25f){X=Y;Y=-x;}
}
C2DVector& C2DVector::Normalise(){
    float length=GetMagnitude();
    if(length!=0.0f){X/=length;Y/=length;}
    return *this;
}
C2DVector C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const {
    C2DVector axis=Point2-Point1;
    float d=axis.Dot(axis);
    return d==0?Point1:Point1+axis*((point-Point1).Dot(axis)/d);
}
class NavigatorFixture : public CNavigatorExternalInterface {
public:
    NavigatorFixture(float x,float y): position(x,y),dead(false),ignore(0),reads(0){}
    C2DVector position;
    bool dead;
    const CSteeringAgent* ignore;
    mutable int reads;
    AgentVector obstacles;
    bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const { return 0; }
    float GetTerrainDesirabilityAt(const C3DVector&) const { return 0; }
    float GetTerrainHeightAt(const C3DVector&) const { return 0; }
    float GetThemeHeightAt(const C3DVector&) const { return 0; }
    float FindPosZAt(const C3DVector&,bool) const { return 0; }
    float FindPosZAtIncludingThemes(const C3DVector&,bool) const { return 0; }
    float PeekNavigatorRadius(bool) const { return 1.0f; }
    C2DVector PeekNavigatorOrientation() const { return C2DVector(0,0); }
    C2DVector PeekNavigatorPosition2D() const { ++reads; return position; }
    C3DVector PeekNavigatorPosition3D() const { return C3DVector(); }
    float PeekNavigatorMaxSpeed(const C3DVector&) const { return 0; }
    const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const { return obstacles; }
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() { return obstacles; }
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles() { return &obstacles; }
    const CSteeringAgent* GetPThingToNotAvoid() const { return ignore; }
    float GetProximityForDirectApproach() const { return 0; }
    long GetNavigationLevelAt(const C3DVector&) const { return 0; }
    long GetConstantFPS() const { return 0; }
    float GetMinPathCost() const { return 0; }
    float GetMaxPathCost() const { return 0; }
    bool IsPlayer() const { return 0; }
    bool IsDead() const { return dead; }
    float GetForwardNavigationWidthExtension() const { return 0; }
    CNavigatorExternalInterface* Clone() const { return 0; }

};
static bool nearf(float a,float b){return fabs(a-b)<0.0001f;}
static bool scenario(float left,float right,float x,float y,bool dead,bool ignore,
                     bool disabled,bool add_far,float expected_direction) {
    config_left=left; config_right=right;
    NavigatorFixture self(0,0), other(x,y), far_nav(x+1,y);
    CSteeringAgent agent(&self,false), obstacle(&other,false), far_agent(&far_nav,false);
    self.ignore=ignore?&obstacle:0;
    other.dead=dead;
    AgentVector candidates;
    if(add_far) candidates.push_back(&far_agent);
    candidates.push_back(&obstacle);
    CSBIAgents behaviour(candidates);
    behaviour.SetInput(CKinematic(C2DVector(1,0)));
    test_disable=disabled;
    if(!behaviour.Process(&agent)) return false;
    C2DVector output=behaviour.GetOutput();
    if(disabled && (self.reads || other.reads))return false;
    if(expected_direction==0) return nearf(output.X,1) && nearf(output.Y,0);
    return output.X>=1 && output.X<=2 && expected_direction*output.Y>0 &&
           nearf((output-C2DVector(1,0)).GetMagnitude(),1.0f);
}
int main(){
    if(!scenario(20,20,5,1,false,false,true,false,0)) return 1;
    if(!scenario(20,20,5,1,true,false,false,false,0)) return 2;
    if(!scenario(20,20,5,1,false,true,false,false,0)) return 3;
    if(!scenario(20,20,0,1,false,false,false,false,0)) return 4;
    if(!scenario(20,20,11,1,false,false,false,false,0)) return 5;
    if(!scenario(20,20,-1,1,false,false,false,false,0)) return 6;
    if(!scenario(20,20,5,3,false,false,false,false,0)) return 7;
    if(!scenario(20,20,1,1,false,false,false,false,0)) return 8;
    if(!scenario(20,20,6,1,false,false,false,true,-1)) return 9;
    if(!scenario(20,20,3,-1,false,false,false,false,1)) return 10;
    if(!scenario(1,3,6,1,false,false,false,false,1)) return 11;
    if(!scenario(3,1,6,-1,false,false,false,false,-1)) return 12;
    if(!scenario(2,2,6,1,false,false,false,false,-1)) return 13;
    if(!scenario(2,2,6,-1,false,false,false,false,1)) return 14;
    puts("00a87960_TEST PASS");return 0;
}