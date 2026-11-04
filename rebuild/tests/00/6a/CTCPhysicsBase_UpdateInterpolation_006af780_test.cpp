// Behaviour test for CTCPhysicsBase::UpdateInterpolation 0x006AF780: the base physics component's interpolation update is just the
// (virtual) UpdateOldPos, so the derived component's UpdateOldPos runs.
#include <cstdio>
struct CTCPhysicsBase {
    int OldPosUpdates;
    CTCPhysicsBase() : OldPosUpdates(0) {}
    virtual ~CTCPhysicsBase() {}
    virtual void UpdateOldPos() { ++OldPosUpdates; }
    virtual void UpdateInterpolation() { UpdateOldPos(); }
};
struct CTCPhysicsStandard : CTCPhysicsBase { void UpdateOldPos() { OldPosUpdates += 10; } };
int main()
{
    CTCPhysicsBase b; CTCPhysicsStandard s;
    CTCPhysicsBase* p = &s;
    b.UpdateInterpolation(); p->UpdateInterpolation();
    if (b.OldPosUpdates != 1 || s.OldPosUpdates != 10) { std::printf("006af780_TEST FAIL\n"); return 1; }
    std::printf("006af780_TEST PASS\n");
    return 0;
}