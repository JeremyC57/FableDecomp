// CSB::GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative) -- the gap between two steering agents' navigation discs, optionally in units of the first agent's radius
// Leaf: ?GetDistanceBetweenAgents@CSB@@QAEMPBVCSteeringAgent@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A872F0, length 196 bytes (`ret 0Ch`; `this` is unused).
// PDB: ?GetDistanceBetweenAgents@CSB@@QAEMPBVCSteeringAgent@@0_N@Z.
// Callees (through CSteeringAgent::NavigatorInterface, vtable of CGroundNavigatorExternalInterface 0x0125EC9C):
// slot 8 (+0x20) PeekNavigatorPosition2D() const (C2DVector by hidden pointer), slot 20 (+0x50) IsPlayer() const,
// slot 6 (+0x18) PeekNavigatorRadius(bool) const.
// Shape: both positions are fetched (other first) into frame locals, the delta agent - other into a third; the
// radius flag is `agent is the player || other is the player`, homed in the dead `other` argument slot and pushed as
// a whole dword; distance = |delta| - (r(agent) + r(other)) with the radius calls bracketing the fsqrt, and the
// agent radius is a named local taken before the length (that puts the fsqrt between the two radius calls); the
// relative form divides by the agent's radius once more. The length and the other agent's radius are NAMED float locals
// (`float length = delta.GetLength(); float otherRadius = ...; distance = length - (agentRadius + otherRadius)`): that puts
// retail's `mov al,[relative] ; test al,al` between the radius call and the `fadd`; the one-expression form schedules the
// byte load after the fadd (196v196).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};

class C3DVector;
class CSteeringAgent;
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
typedef std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> > CSteeringAgentList;
// Slot order: vtable 0x0125EC9C of CGroundNavigatorExternalInterface (rtti-vtables.tsv slots 0..20, names and signatures from
// Ego_r.pdb CGroundNavigatorExternalInterface overrides; slots 11/12 are the two GetNavigatorObstacles overloads -- which one
// sits in which slot is not proven and does not reach these bytes).
class CNavigatorExternalInterface {
public:
    virtual bool IsMovementBlockedAt(const C3DVector& from, const C3DVector& to, bool& a, bool& b) const = 0;   // slot 0
    virtual float GetTerrainDesirabilityAt(const C3DVector& pos) const = 0;                                    // slot 1
    virtual float GetTerrainHeightAt(const C3DVector& pos) const = 0;                                          // slot 2
    virtual float GetThemeHeightAt(const C3DVector& pos) const = 0;                                            // slot 3
    virtual float FindPosZAt(const C3DVector& pos, bool flag) const = 0;                                       // slot 4
    virtual float FindPosZAtIncludingThemes(const C3DVector& pos, bool flag) const = 0;                        // slot 5
    virtual float PeekNavigatorRadius(bool isPlayerRelated) const = 0;                                         // slot 6 (+0x18)
    virtual C2DVector PeekNavigatorOrientation() const = 0;                                                    // slot 7
    virtual C2DVector PeekNavigatorPosition2D() const = 0;                                                     // slot 8 (+0x20)
    virtual C3DVector PeekNavigatorPosition3D() const = 0;                                                     // slot 9
    virtual float PeekNavigatorMaxSpeed(const C3DVector& pos) const = 0;                                       // slot 10
    virtual const CSteeringAgentList& GetNavigatorObstacles() const = 0;                                       // slot 11
    virtual CSteeringAgentList& GetNavigatorObstacles() = 0;                                                   // slot 12
    virtual CSteeringAgentList* GetPNavigatorObstacles() = 0;                                                  // slot 13
    virtual const CSteeringAgent* GetPThingToNotAvoid() const = 0;                                             // slot 14
    virtual float GetProximityForDirectApproach() const = 0;                                                   // slot 15
    virtual long GetNavigationLevelAt(const C3DVector& pos) const = 0;                                         // slot 16
    virtual long GetConstantFPS() const = 0;                                                                   // slot 17
    virtual float GetMinPathCost() const = 0;                                                                  // slot 18
    virtual float GetMaxPathCost() const = 0;                                                                  // slot 19
    virtual bool IsPlayer() const = 0;                                                                         // slot 20 (+0x50)
};

class CSteeringAgent {                                               // Ego_r.pdb, 0x80 bytes; only the interface is used here
public:
    CNavigatorExternalInterface* NavigatorInterface;                 // +0x00
};

#define CSB CSBLayout
#include "engine/CSB.h"
#undef CSB
class CSB : public CSBLayout {                                       // vptr +0x00, Output (CKinematic) +0x04
public:
    float GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative);
};

float CSB::GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative)
{
    C2DVector otherPosition = other->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector position = agent->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector delta(position.X - otherPosition.X, position.Y - otherPosition.Y);
    bool player = agent->NavigatorInterface->IsPlayer() || other->NavigatorInterface->IsPlayer();
    float agentRadius = agent->NavigatorInterface->PeekNavigatorRadius(player);
    float length = delta.GetLength();
    float otherRadius = other->NavigatorInterface->PeekNavigatorRadius(player);
    float distance = length - (agentRadius + otherRadius);
    if (relative)
        return distance / agent->NavigatorInterface->PeekNavigatorRadius(player);
    return distance;
}