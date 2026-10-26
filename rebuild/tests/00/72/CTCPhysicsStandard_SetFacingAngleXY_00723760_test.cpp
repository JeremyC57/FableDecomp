#include <cstdio>
// Behaviour model of CTCPhysicsStandard::SetFacingAngleXY (0x00723760): replace Euler angle 0, keep the other two, rebuild the set, mark moving
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Angles; CRightHandedSet() {} CRightHandedSet(const C3DVector& a) : Angles(a) {} };
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet; bool Moving;
    void UpdateOldRHSet() { OldRHSet = RHSet; }
    float GetFacingAngleXY() { return RHSet.Angles.X; } float GetFacingAngleYZ() { return RHSet.Angles.Y; } float GetFacingAngleXZ() { return RHSet.Angles.Z; }
    void SetAsMoving(bool m) { Moving = m; }
    void SetFacingAngleXY(float angle) {
        UpdateOldRHSet();
        C3DVector a;
        a.X = 0 == 0 ? angle : GetFacingAngleXY();
        a.Y = 0 == 1 ? angle : GetFacingAngleYZ();
        a.Z = 0 == 2 ? angle : GetFacingAngleXZ();
        RHSet = CRightHandedSet(a);
        SetAsMoving(true);
    } };
int main()
{
    CTCPhysicsStandard s; s.RHSet.Angles.X = 1; s.RHSet.Angles.Y = 2; s.RHSet.Angles.Z = 3; s.Moving = false;
    s.SetFacingAngleXY(9);
    float e[3] = { 1, 2, 3 }; e[0] = 9;
    if (s.RHSet.Angles.X != e[0] || s.RHSet.Angles.Y != e[1] || s.RHSet.Angles.Z != e[2] || s.OldRHSet.Angles.Z != 3 || !s.Moving)
    { std::printf("00723760_TEST FAIL\n"); return 1; }
    std::printf("00723760_TEST PASS\n");
    return 0;
}