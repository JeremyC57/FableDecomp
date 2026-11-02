// Behaviour test for 0x00759970 CTCPhysicsRigidBody::UpdateInterpolation(): the current right-handed set is copied into
// OldRHSet (both axes, RHSet unchanged), then the base class's UpdateInterpolation runs once.
#include <cstdio>

static int g_baseCalls = 0;

class C3DVector { public: float X; float Y; float Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CTCPhysicsBase {
public:
    virtual ~CTCPhysicsBase() { }
protected:
    virtual void UpdateInterpolation() { ++g_baseCalls; }
};

class CTCPhysicsRigidBody : public CTCPhysicsBase {
public:
    virtual void UpdateInterpolation();
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
};

void CTCPhysicsRigidBody::UpdateInterpolation()
{
    OldRHSet = RHSet;
    CTCPhysicsBase::UpdateInterpolation();
}

int main()
{
    CTCPhysicsRigidBody b;
    b.RHSet.Up.X = 1.0f; b.RHSet.Up.Y = 2.0f; b.RHSet.Up.Z = 3.0f;
    b.RHSet.Forward.X = 4.0f; b.RHSet.Forward.Y = 5.0f; b.RHSet.Forward.Z = 6.0f;
    b.OldRHSet.Up.X = b.OldRHSet.Up.Y = b.OldRHSet.Up.Z = -1.0f;
    b.OldRHSet.Forward.X = b.OldRHSet.Forward.Y = b.OldRHSet.Forward.Z = -1.0f;
    b.UpdateInterpolation();
    const CRightHandedSet& o = b.OldRHSet;
    if (o.Up.X != 1.0f || o.Up.Y != 2.0f || o.Up.Z != 3.0f || o.Forward.X != 4.0f || o.Forward.Y != 5.0f || o.Forward.Z != 6.0f) {
        std::printf("00759970_TEST FAIL\n"); return 1;
    }
    if (b.RHSet.Forward.Z != 6.0f || g_baseCalls != 1) { std::printf("00759970_TEST FAIL\n"); return 1; }
    std::printf("00759970_TEST PASS\n");
    return 0;
}