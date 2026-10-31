// Behaviour test for 0x00692AA0 CCreatureActionBase::UpdateActionMovement: (UseMovementInActions || IsAlwaysUseAnimation-
// Movement()) && (IsUsingAnimationMovement() || IsUsingForcedMovement()) -> SetAsMoving(true), UpdateActionMoveToPos(),
// SetForcedToMovePos(NextMovementPos, IsMovementUsingPhysics()).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct Physics {
    int movingCalls; bool moving; int forcedCalls; C3DVector forcedPos; bool forcedFlag;
    void SetAsMoving(bool m) { ++movingCalls; moving = m; }
    virtual void SetForcedToMovePos(const C3DVector& p, bool f) { ++forcedCalls; forcedPos = p; forcedFlag = f; }
    virtual ~Physics() { }
};
struct Creature { Physics* PhysicsTC; bool UseMovementInActions; Physics* GetPhysicsTC() { return PhysicsTC; } };
struct Action {
    Creature* PCreature; C3DVector NextMovementPos;
    bool alwaysAnim, usingAnim, usingForced, usingPhysics; int moveToPosCalls;
    virtual bool IsAlwaysUseAnimationMovement() const { return alwaysAnim; }
    virtual bool IsUsingAnimationMovement() const { return usingAnim; }
    virtual bool IsUsingForcedMovement() const { return usingForced; }
    virtual bool IsMovementUsingPhysics() const { return usingPhysics; }
    virtual void UpdateActionMoveToPos() { ++moveToPosCalls; NextMovementPos.X += 1.0f; }
    virtual ~Action() { }
    void UpdateActionMovement()
    {
        if ((PCreature->UseMovementInActions || IsAlwaysUseAnimationMovement())
            && (IsUsingAnimationMovement() || IsUsingForcedMovement())) {
            PCreature->GetPhysicsTC()->SetAsMoving(true);
            UpdateActionMoveToPos();
            PCreature->GetPhysicsTC()->SetForcedToMovePos(NextMovementPos, IsMovementUsingPhysics());
        }
    }
};
static bool run(bool use, bool always, bool anim, bool forced, bool phys, Physics& p, Action& a)
{
    Creature c; c.PhysicsTC = &p; c.UseMovementInActions = use;
    p.movingCalls = 0; p.moving = false; p.forcedCalls = 0; p.forcedFlag = !phys;
    a.PCreature = &c; a.NextMovementPos.X = 1.0f; a.NextMovementPos.Y = 2.0f; a.NextMovementPos.Z = 3.0f;
    a.alwaysAnim = always; a.usingAnim = anim; a.usingForced = forced; a.usingPhysics = phys; a.moveToPosCalls = 0;
    a.UpdateActionMovement();
    return p.forcedCalls == 1;
}
int main()
{
    Physics p; Action a;
    if (!run(true, false, true, false, true, p, a) || !p.moving || a.moveToPosCalls != 1 || p.forcedPos.X != 2.0f || !p.forcedFlag)
        { std::printf("00692aa0_TEST FAIL\n"); return 1; }
    if (!run(false, true, false, true, false, p, a) || p.forcedFlag) { std::printf("00692aa0_TEST FAIL\n"); return 1; }
    if (run(false, false, true, true, true, p, a) || p.movingCalls != 0) { std::printf("00692aa0_TEST FAIL\n"); return 1; }
    if (run(true, true, false, false, true, p, a) || a.moveToPosCalls != 0) { std::printf("00692aa0_TEST FAIL\n"); return 1; }
    std::printf("00692aa0_TEST PASS\n");
    return 0;
}