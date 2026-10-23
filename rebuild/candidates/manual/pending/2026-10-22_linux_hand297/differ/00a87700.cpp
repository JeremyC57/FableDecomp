// CSBCAgents::Process(const CSteeringAgent*) -- the agent-avoidance steering constraint: pass the input through unless a nearby agent blocks it, in which case slide along the blocking agent (input projected on the perpendicular plus a push-away scaled by the overlap) and stop at the first one
// Grade: DIFFER(603v603) -- register colouring only: retail keeps this in edi and the agent iterator in esi, the lane swaps them (plus eax/edx temp naming and the Input copy interleave that follow); every instruction otherwise matches
// Leaf: Process
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A87700, length 601 bytes, ret 4, virtual (?Process@CSBCAgents@@UAE_NPBVCSteeringAgent@@@Z).
// Callees: CSB::ShouldIgnoreAgent 0x00A87260, CSB::GetDistanceBetweenAgents 0x00A872F0, C2DVector::Normalise 0x00A14540,
// C2DVector rotate-by-revolutions 0x00A141D0 (fpatan * 1/2pi + turns, then sin/cos); CNavigatorExternalInterface slots
// 6 (+0x18, float radius), 8 (+0x20, GetPosition by hidden pointer), 14 (+0x38), 20 (+0x50, bool). Globals: the
// constraints-disabled flag byte 0x013D2D56 (the landed CSBGoal::Process reads 0x013D2D55), floats 0.0 0x0122DEDC and 0.1 0x0122EE58.
// Layout: Ego_r.pdb CSBCAgents (28 bytes) = CSBConstraint {vptr, CSB::Output +4, input +0x0c, MoveBeforeOrientation +0x14},
// Agents +0x18 (vector<const CSteeringAgent*>*). CSteeringAgent: retail STLport lists are 4 bytes (PDB 12), so Wandering
// PDB +0x74 / AvoidDynamicObstacles PDB +0x78 sit at retail +0x5c / +0x60.
#include "engine/CSBCAgents.h"

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector& Normalise();                       // 0x00A14540
    void RotateByRevolutions(float turns);        // 0x00A141D0
};
struct CKinematic {
    C2DVector Vector;
    CKinematic() {}
    CKinematic(const C2DVector& v) : Vector(v) {}
};
struct CSteeringAgent;
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05();
    virtual float PeekNavigatorRadius(bool a) const;        // slot 6 (+0x18)
    virtual void v07();
    virtual C2DVector PeekNavigatorPosition2D() const;        // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual const CSteeringAgent* GetPThingToNotAvoid() const;         // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool IsPlayer() const;                // slot 20 (+0x50)
};
struct CSteeringAgent {
    CNavigatorExternalInterface* NavigatorInterface;   // +0x00
    unsigned char _pad04[0x5c - 0x04];
    bool Wandering;                                    // +0x5c (PDB +0x74)
    unsigned char _pad5d[0x60 - 0x5d];
    bool AvoidDynamicObstacles;                        // +0x60 (PDB +0x78)
};
struct CAgentVector {
    const CSteeringAgent** _M_start;
    const CSteeringAgent** _M_finish;
    const CSteeringAgent** _M_end_of_storage;
};
extern bool g_SteeringConstraintsDisabled;        // 0x013D2D56

struct CSBCAgentsImpl : CSBCAgents {
    CKinematic& Output() { return *(CKinematic*)_pad_0x04; }        // CSB::Output, +0x04
    CKinematic& Input() { return *(CKinematic*)_pad_0x0c; }         // CSBConstraint input, +0x0c
    bool& MoveBeforeOrientation() { return *(bool*)_pad_0x14; }     // +0x14
    CAgentVector* AgentList() const { return (CAgentVector*)Agents; }
    bool ShouldIgnoreAgent(const CSteeringAgent* agent, const CSteeringAgent* other, const CSteeringAgent* owner);   // 0x00A87260
    float GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool a);   // 0x00A872F0
    static C2DVector GetOffset(const CSteeringAgent* const& from, const CSteeringAgent* const& to)
    {
        C2DVector fromPos = from->NavigatorInterface->PeekNavigatorPosition2D();
        C2DVector toPos = to->NavigatorInterface->PeekNavigatorPosition2D();
        return toPos - fromPos;
    }
    bool Process(const CSteeringAgent* agent);
};

bool CSBCAgentsImpl::Process(const CSteeringAgent* agent)
{
    if (g_SteeringConstraintsDisabled) {
        Output() = Input();
        return true;
    }
    MoveBeforeOrientation() = true;
    const CSteeringAgent* owner = agent->NavigatorInterface->GetPThingToNotAvoid();
    for (const CSteeringAgent** it = AgentList()->_M_start; it != AgentList()->_M_finish; ++it) {
        if (ShouldIgnoreAgent(agent, *it, owner) && (!agent->AvoidDynamicObstacles || !(*it)->NavigatorInterface->IsPlayer()))
            continue;
        C2DVector away = GetOffset(*it, agent);
        if (GetDistanceBetweenAgents(agent, *it, false) <= 0.0f && !(*it)->NavigatorInterface->IsPlayer() && !agent->Wandering)
            continue;
        C2DVector input = Input().Vector;
        away = GetOffset(agent, *it);
        float dist = GetDistanceBetweenAgents(agent, *it, false);
        away.Normalise();
        float ahead = dist - (away.X * input.X + away.Y * input.Y);
        if (ahead < agent->NavigatorInterface->PeekNavigatorRadius(false) * 0.1f && ahead < dist) {
            dist -= 0.1f;
            if (dist < 0.0f)
                dist = 0.0f;
            C2DVector side = away;
            side.RotateByRevolutions(0.25f);
            C2DVector push = away * dist;
            float along = input.X * side.X + input.Y * side.Y;
            Output() = CKinematic(side * along + push);
            return true;
        }
    }
    Output() = Input();
    return true;
}
