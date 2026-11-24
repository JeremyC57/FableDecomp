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

bool CSBIAgents::Process(const CSteeringAgent* agent)
{
    static bool dont_use;
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