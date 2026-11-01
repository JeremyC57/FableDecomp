// Behaviour test for 007599c0 CTCPhysicsRigidBody::SetFacingAnglesFromRHSet: copies the set into RHSet, leaves OldRHSet,
// then calls SetAsMoving(true).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
static int g_calls = 0;
static bool g_moving = false;
class CTCPhysicsBase {
public:
    virtual ~CTCPhysicsBase() { }
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet&) { }
    void SetAsMoving(bool moving) { ++g_calls; g_moving = moving; }
};
class CTCPhysicsRigidBody : public CTCPhysicsBase {
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set);
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
};
void CTCPhysicsRigidBody::SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set)
{
    RHSet = rh_set;
    SetAsMoving(true);
}
int main()
{
    CTCPhysicsRigidBody body;
    CRightHandedSet zero = { { 0, 0, 0 }, { 0, 0, 0 } };
    body.RHSet = zero; body.OldRHSet = zero;
    CRightHandedSet s = { { 1, 2, 3 }, { 4, 5, 6 } };
    CTCPhysicsBase* base = &body;
    base->SetFacingAnglesFromRHSet(s);
    if (body.RHSet.Up.X != 1 || body.RHSet.Up.Z != 3 || body.RHSet.Forward.X != 4 || body.RHSet.Forward.Z != 6 ||
        body.OldRHSet.Up.X != 0 || g_calls != 1 || !g_moving) { std::printf("007599c0_TEST FAIL\n"); return 1; }
    std::printf("007599c0_TEST PASS\n");
    return 0;
}