// Behaviour test for 0x00856D30 CCombatActionBase::UpdateActionMoveToPos.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CCreature { C3DVector pos; float movement; const C3DVector& GetPosition() const { return pos; } float GetAnimMovementLength(long) { return movement; } };
struct CCombatActionBase {
    CCreature* creature; bool usingActionMovement; bool PreferredFinalPosSet; C3DVector next; C3DVector moveTo;
    long GetAnimationIndex() { return 1; }
    C3DVector GetNextPos() { return next; }
    bool IsUsingActionMovement() { return usingActionMovement; }
    void SetMoveToPos(const C3DVector& p) { moveTo = p; }
    void UpdateActionMoveToPos()
    {
        C3DVector pos = creature->GetPosition();
        CCreature* c = creature;
        if (c->GetAnimMovementLength(GetAnimationIndex()) > 0.0001f || IsUsingActionMovement() || PreferredFinalPosSet)
            pos = GetNextPos();
        SetMoveToPos(pos);
    }
};
int main()
{
    CCreature cr; cr.pos.X = 1; cr.pos.Y = 2; cr.pos.Z = 3; cr.movement = 0.0f;
    CCombatActionBase a; a.creature = &cr; a.usingActionMovement = false; a.PreferredFinalPosSet = false;
    a.next.X = 9; a.next.Y = 8; a.next.Z = 7;
    a.UpdateActionMoveToPos();
    if (a.moveTo.X != 1 || a.moveTo.Z != 3) { std::printf("00856d30_TEST FAIL\n"); return 1; }
    cr.movement = 1.0f; a.UpdateActionMoveToPos();
    if (a.moveTo.X != 9) { std::printf("00856d30_TEST FAIL\n"); return 1; }
    cr.movement = 0.0f; a.moveTo.X = 0; a.PreferredFinalPosSet = true; a.UpdateActionMoveToPos();
    if (a.moveTo.X != 9 || a.moveTo.Z != 7) { std::printf("00856d30_TEST FAIL\n"); return 1; }
    std::printf("00856d30_TEST PASS\n");
    return 0;
}