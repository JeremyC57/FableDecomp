// CSBCAgents::Process(const CSteeringAgent* agent) -- the agent-avoidance steering constraint: pass the input through unless a nearby agent blocks it, in which case slide along the blocking agent (the input projected on the perpendicular plus a push-away scaled by the overlap) and stop at the first one
// Leaf: ?Process@CSBCAgents@@UAE_NPBVCSteeringAgent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A87700, 601 bytes, `ret 4`, virtual (Ego_r.pdb name above); 8-byte aligned frame.
// Callees: CSB::ShouldIgnoreAgent 0x00A87260, CSB::GetDistanceBetweenAgents 0x00A872F0 (both landed), C2DVector::Normalise 0x00A14540,
//   C2DVector::Rotate 0x00A141D0, and through the CSteeringAgent accessors the CNavigatorExternalInterface slots 6 (+0x18,
//   PeekNavigatorRadius), 8 (+0x20, PeekNavigatorPosition2D), 14 (+0x38, GetPThingToNotAvoid) and 20 (+0x50, IsPlayer).
//   Constants 0.0f 0x0122DEDC, 0.1f 0x0122EE58.
// Shape: on the shared headers (CSB / CSBInput / CSBConstraint / CSBCAgents retail classes, CKinematic, the CSteeringAgent FableWin
//   accessors): the port alone gives retail's this-in-edi / iterator-in-esi colouring (the earlier local model swapped them). The
//   first offset is computed and dropped as in retail (0x00A877A2..0x00A877B6 store it). The blocked result is stored with
//   CKinematic::SetVector (FableWin.pdb); `Output = CKinematic(...)` builds a copy temporary and reorders the final x87 sums.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorExternalInterface.h"
#include "engine/CSteeringAgent.h"
#include "engine/CKinematic.h"
#include "engine/CSB.h"
#include "engine/CSBConstraint.h"
#include "engine/CSBCAgents.h"

extern bool g_SBCAgentsDisabled;                  // 0x013D2D56 (descriptive: an unnamed .bss bool, read only here; no PDB data name)

bool CSBCAgents::Process(const CSteeringAgent* agent)
{
    if (g_SBCAgentsDisabled) {
        Output = Input;
        return true;
    }
    MoveBeforeOrientation = true;
    const CSteeringAgent* target = agent->GetTarget();
    for (std::vector<const CSteeringAgent*>::iterator it = Agents->begin(); it != Agents->end(); ++it) {
        if (ShouldIgnoreAgent(agent, *it, target) && (!agent->GetAvoidDynamicObstacles() || !(*it)->IsPlayer()))
            continue;
        C2DVector away = agent->GetPosition() - (*it)->GetPosition();
        if (GetDistanceBetweenAgents(agent, *it, false) <= 0.0f && !(*it)->IsPlayer() && !agent->IsWandering())
            continue;
        C2DVector input = Input.GetVector();
        away = (*it)->GetPosition() - agent->GetPosition();
        float dist = GetDistanceBetweenAgents(agent, *it, false);
        away.Normalise();
        float ahead = dist - (away.X * input.X + away.Y * input.Y);
        if (ahead < agent->GetRadius(false) * 0.1f && ahead < dist) {
            dist -= 0.1f;
            if (dist < 0.0f)
                dist = 0.0f;
            C2DVector side = away;
            side.Rotate(0.25f);
            C2DVector push = away * dist;
            float along = input.X * side.X + input.Y * side.Y;
            Output.SetVector(side * along + push);
            return true;
        }
    }
    Output = Input;
    return true;
}