// CSB::ShouldIgnoreAgent(const CSteeringAgent* self, const CSteeringAgent* other, const CSteeringAgent* ignore) -- decide whether a steering behaviour should ignore another agent (itself, the explicit ignore agent, a non-obstacle, or one at a different height)
// Grade: DIFFER(133v137) -- all but one test: retail re-tests the already-loaded +0x60 flag (`mov cl,[esi+61h] ; test cl,cl ; jne ; test al,al ; je`) before the IsAvoidanceObstacle call; ours folds the second test away (inline helper, one expression and a local bool all fold)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A87260, length 137 bytes (+7 int3 padding).
// PDB: ?ShouldIgnoreAgent@CSB@@QAE_NPBVCSteeringAgent@@00@Z (`this` unused, three agent pointers, `ret 0Ch`).
// CSteeringAgent: +0x00 CNavigatorExternalInterface* NavigatorInterface (virtual slot 9 +0x24 returns the agent
// position by value, slot 20 +0x50 a bool "takes part in avoidance" test); the three flag bytes at +0x60/+0x61/+0x64
// are the agent's avoidance switches (retail offsets; the PDB layout differs here, so they get descriptive names).
// 0x013986C4 is the float height tolerance.
// Shape: early `return true` tests; the +0x60 flag is tested twice because the second test sits in an inline
// helper; the height test is `fabs(dz) > tolerance` (fcomp + `test ah,41h ; je`).
#include <math.h>

struct C3DVector { float x, y, z; };

struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual C3DVector GetPosition() const;                         // slot 9 (+0x24)
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3c(); virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual bool IsAvoidanceObstacle() const;                      // slot 20 (+0x50)
};

struct CSteeringAgent {
    CNavigatorExternalInterface* NavigatorInterface;              // +0x00
    unsigned char _pad04[0x60 - 0x04];
    bool          AvoidAgents;                                     // +0x60
    bool          AvoidAllAgents;                                  // +0x61
    unsigned char _pad62[2];
    bool          AvoidOnlyObstacles;                              // +0x64

    __forceinline bool WantsToAvoid(const CSteeringAgent* other) const
    {
        if (!AvoidAllAgents)
            return AvoidAgents && other->NavigatorInterface->IsAvoidanceObstacle();
        return true;
    }
};

extern const float g_SteeringHeightTolerance;                      // 0x013986C4

struct CSB {
    virtual ~CSB();
    bool ShouldIgnoreAgent(const CSteeringAgent* self, const CSteeringAgent* other, const CSteeringAgent* ignore);
};

bool CSB::ShouldIgnoreAgent(const CSteeringAgent* self, const CSteeringAgent* other, const CSteeringAgent* ignore)
{
    if (other == self || other == ignore)
        return true;
    if (self->AvoidOnlyObstacles && !other->NavigatorInterface->IsAvoidanceObstacle())
        return true;
    if (!self->AvoidAgents || !self->WantsToAvoid(other))
        return true;
    C3DVector selfPos = self->NavigatorInterface->GetPosition();
    C3DVector otherPos = other->NavigatorInterface->GetPosition();
    if (fabsf(selfPos.z - otherPos.z) > g_SteeringHeightTolerance)
        return true;
    return false;
}
