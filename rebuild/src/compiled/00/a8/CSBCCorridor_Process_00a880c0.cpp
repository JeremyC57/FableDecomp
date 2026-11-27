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
#include "engine/CSteeringAgent.h"
#include "engine/CSBCCorridor.h"
#include "engine/C2DLineF.h"
bool CSBCCorridor::Process(const CSteeringAgent* agent)
{
    static bool dont_use = false;
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