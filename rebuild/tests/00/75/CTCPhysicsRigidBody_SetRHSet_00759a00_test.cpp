#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCPhysicsRigidBody {
    bool m_moving;
    CRightHandedSet RHSet, OldRHSet;
    void SetAsMoving(bool moving) { m_moving = moving; }
    void vfunc_75(const CRightHandedSet& set)
    {
        RHSet = set;
        SetAsMoving(true);
    }
};
int main()
{
    CTCPhysicsRigidBody b; b.m_moving = false; b.OldRHSet.Up.X = 3.0f;
    CRightHandedSet s; s.Up.X = 0.0f; s.Up.Y = 0.0f; s.Up.Z = 1.0f; s.Forward.X = 0.0f; s.Forward.Y = 1.0f; s.Forward.Z = 0.0f;
    b.vfunc_75(s);
    if (!b.m_moving || b.RHSet.Up.Z != 1.0f || b.RHSet.Forward.Y != 1.0f || b.OldRHSet.Up.X != 3.0f) { std::printf("00759a00_TEST FAIL\n"); return 1; }
    std::printf("00759a00_TEST PASS\n");
    return 0;
}