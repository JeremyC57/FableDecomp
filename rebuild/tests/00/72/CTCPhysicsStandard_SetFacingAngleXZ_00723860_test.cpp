#include <cstdio>
// Behaviour model of CTCPhysicsStandard::SetFacingAngleXZ (0x00723860): replace Euler angle 2, keep the other two, rebuild the set, mark moving
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Angles; CRightHandedSet() {} CRightHandedSet(const C3DVector& a) : Angles(a) {} };
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet; bool Moving;
    void UpdateOldRHSet() { OldRHSet = RHSet; }
    float GetFacingAngleXY() { return RHSet.Angles.X; } float GetFacingAngleYZ() { return RHSet.Angles.Y; } float GetFacingAngleXZ() { return RHSet.Angles.Z; }
    void SetAsMoving(bool m) { Moving = m; }
    void SetFacingAngleXZ(float angle) {
        UpdateOldRHSet();
        C3DVector a;
        a.X = 2 == 0 ? angle : GetFacingAngleXY();
        a.Y = 2 == 1 ? angle : GetFacingAngleYZ();
        a.Z = 2 == 2 ? angle : GetFacingAngleXZ();
        RHSet = CRightHandedSet(a);
        SetAsMoving(true);
    } };
int main()
{
    CTCPhysicsStandard s; s.RHSet.Angles.X = 1; s.RHSet.Angles.Y = 2; s.RHSet.Angles.Z = 3; s.Moving = false;
    s.SetFacingAngleXZ(9);
    float e[3] = { 1, 2, 3 }; e[2] = 9;
    if (s.RHSet.Angles.X != e[0] || s.RHSet.Angles.Y != e[1] || s.RHSet.Angles.Z != e[2] || s.OldRHSet.Angles.Z != 3 || !s.Moving)
    { std::printf("00723860_TEST FAIL\n"); return 1; }
    std::printf("00723860_TEST PASS\n");
    return 0;
}