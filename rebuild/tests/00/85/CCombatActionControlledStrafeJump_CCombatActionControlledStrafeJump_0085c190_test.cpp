// Behaviour test for CCombatAction_ControlledStrafeJump's constructor (0x0085C190): both facings are flattened
// and normalised, or replaced by the owner's facing when (nearly) zero; code 150, roll mode off.
#include <cstdio>
#include <cmath>

class CThing { public: int id; };
static float g_Epsilon = 1e-4f;
class C3DVector { public:
    float X, Y, Z;
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CSoundPair { public: long m_first, m_second; CSoundPair() : m_first(0), m_second(0) {} };
struct CPhysicsBody { C3DVector Facing; C3DVector GetFacing() { return Facing; } };
class CThingCreatureBase : public CThing { public: CPhysicsBody* PhysicsBody; };
class CCombatActionBase { public:
    long Code;
    CCombatActionBase(CThingCreatureBase*, CThing*, long code, const CSoundPair&) : Code(code) {}
};
class CCombatAction_ControlledStrafeJump : public CCombatActionBase { public:
    C3DVector RequiredFacing, OriginalFacing; bool AddedRollMode;
    CCombatAction_ControlledStrafeJump(CThingCreatureBase* owner, CThing* target, const C3DVector& requiredFacing, const C3DVector& originalFacing);
};

CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(CThingCreatureBase* owner, CThing* target,
                                                                       const C3DVector& requiredFacing, const C3DVector& originalFacing)
    : CCombatActionBase(owner, target, 150, CSoundPair())
{
    AddedRollMode = false;
    RequiredFacing = requiredFacing;
    OriginalFacing = originalFacing;
    OriginalFacing.Z = 0.0f;
    RequiredFacing.Z = 0.0f;
    if (RequiredFacing.GetLengthSquared() > g_Epsilon * g_Epsilon)
        RequiredFacing.Normalise();
    else
        RequiredFacing = owner->PhysicsBody->GetFacing();
    if (OriginalFacing.GetLengthSquared() > g_Epsilon * g_Epsilon)
        OriginalFacing.Normalise();
    else
        OriginalFacing = owner->PhysicsBody->GetFacing();
}

int main()
{
    CPhysicsBody body; body.Facing.X = 0.0f; body.Facing.Y = 1.0f; body.Facing.Z = 0.0f;
    CThingCreatureBase owner; owner.PhysicsBody = &body; CThing target;
    C3DVector req = { 3.0f, 4.0f, 9.0f }, orig = { 0.0f, 0.0f, 5.0f };   // the second flattens to zero
    CCombatAction_ControlledStrafeJump a(&owner, &target, req, orig);
    bool ok = a.Code == 150 && !a.AddedRollMode
        && std::fabs(a.RequiredFacing.X - 0.6f) < 1e-5f && std::fabs(a.RequiredFacing.Y - 0.8f) < 1e-5f && a.RequiredFacing.Z == 0.0f
        && a.OriginalFacing.X == 0.0f && a.OriginalFacing.Y == 1.0f && a.OriginalFacing.Z == 0.0f;
    if (!ok) { std::printf("0085c190_TEST FAIL\n"); return 1; }
    std::printf("0085c190_TEST PASS\n");
    return 0;
}