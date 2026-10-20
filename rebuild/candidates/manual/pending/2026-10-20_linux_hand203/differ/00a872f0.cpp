// CSB::GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative) -- the gap between two steering agents' navigation discs, optionally in units of the first agent's radius
// Leaf: GetDistanceBetweenAgents
// Grade: DIFFER(196v196) -- scheduling only: retail loads `relative` (mov al,[esp+30h] ; test al,al) before the `fadd` of the
// agent radius, ours after it; a separate bool local did not move it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A872F0, length 196 bytes (`ret 0Ch`; `this` is unused).
// PDB: ?GetDistanceBetweenAgents@CSB@@QAEMPBVCSteeringAgent@@0_N@Z.
// Callees (through CSteeringAgent::NavigatorInterface, vtable of CGroundNavigatorExternalInterface 0x0125EC9C):
// slot 8 (+0x20) PeekNavigatorPosition2D() const (C2DVector by hidden pointer), slot 20 (+0x50) IsPlayer() const,
// slot 6 (+0x18) PeekNavigatorRadius(bool) const.
// Shape: both positions are fetched (other first) into frame locals, the delta agent - other into a third; the
// radius flag is `agent is the player || other is the player`, homed in the dead `other` argument slot and pushed as
// a whole dword; distance = |delta| - (r(agent) + r(other)) with the radius calls bracketing the fsqrt, and the
// agent radius is a named local taken before the length (that puts the fsqrt between the two radius calls); the
// relative form divides by the agent's radius once more.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};

class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual float PeekNavigatorRadius(bool isPlayerRelated) const;   // slot 6 (+0x18)
    virtual void v07();
    virtual C2DVector PeekNavigatorPosition2D() const;               // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool IsPlayer() const;                                   // slot 20 (+0x50)
};

class CSteeringAgent {
public:
    CNavigatorExternalInterface* NavigatorInterface;                 // +0x00
};

class CSB {
public:
    void*         __vftable;                                         // +0x00
    unsigned char Output[0x8];                                       // +0x04 CKinematic
    float GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative);
};

float CSB::GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative)
{
    C2DVector otherPosition = other->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector position = agent->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector delta(position.X - otherPosition.X, position.Y - otherPosition.Y);
    bool player = agent->NavigatorInterface->IsPlayer() || other->NavigatorInterface->IsPlayer();
    float agentRadius = agent->NavigatorInterface->PeekNavigatorRadius(player);
    float distance = delta.GetLength() - (agentRadius + other->NavigatorInterface->PeekNavigatorRadius(player));
    if (relative)
        return distance / agent->NavigatorInterface->PeekNavigatorRadius(player);
    return distance;
}
